/*
 * ============================================================
 *  CSC494 — Dirt Bike Maintenance Sensor System
 *  Board   : Seeed XIAO ESP32-C6
 *  Library : Standard ESP32 BLE (not NimBLE)
 * ============================================================
 *  Libraries required:
 *    - ArduinoJson by Benoit Blanchon
 *    - Built-in ESP32 BLE (no install needed)
 *
 *  Wiring:
 *    MPXV7002DP GND  → GND
 *    MPXV7002DP +5V  → 3V3
 *    MPXV7002DP OUT  → A1
 *    Nano TX         → XIAO D7
 *    Nano GND        → XIAO GND
 * ============================================================
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

// ─── Device identity ─────────────────────────────────────────
#define DEVICE_ID  "DirtBike_001"

// ─── Pin definitions ─────────────────────────────────────────
#define PRESSURE_ADC_PIN   A1
#define LED_PIN            LED_BUILTIN

// ─── MPXV7002DP constants ────────────────────────────────────
#define ADC_MAX            4095.0f
#define V_SUPPLY           3.3f
#define MPXV7002_SENS      0.057f
#define MPXV7002_OFFSET    0.5f
#define PRESSURE_SAMPLES   32

#define PRESSURE_WATCH_KPA  -0.50f
#define PRESSURE_WARN_KPA   -0.75f
#define PRESSURE_CRIT_KPA   -1.00f

// ─── Temperature thresholds ──────────────────────────────────
#define OIL_COLD_C    60.0f
#define OIL_WARN_C   110.0f
#define OIL_CRIT_C   130.0f

// ─── Timing ──────────────────────────────────────────────────
#define SAMPLE_INTERVAL_MS  500

// ─── BLE UUIDs ───────────────────────────────────────────────
#define BLE_SERVICE_UUID  "12345678-1234-1234-1234-123456789abc"
#define BLE_TELEM_UUID    "12345678-1234-1234-1234-123456789abd"
#define BLE_ALERT_UUID    "12345678-1234-1234-1234-123456789abe"

// ─── Globals ─────────────────────────────────────────────────
BLEServer        *pServer    = nullptr;
BLECharacteristic *pTelemChar = nullptr;
BLECharacteristic *pAlertChar = nullptr;
bool  bleConnected  = false;
float zeroOffsetKpa = 0.0f;
float lastOilTempC  = -127.0f;

struct SensorReading {
    float         pressureKpa;
    float         pressureAdj;
    int           filterClogPct;
    String        filterStatus;
    float         oilTempC;
    String        oilTempStatus;
    unsigned long uptimeMs;
};

// ─── BLE Callbacks ───────────────────────────────────────────
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pSvr) override {
        bleConnected = true;
        digitalWrite(LED_PIN, HIGH);
        Serial.println("[BLE] Dashboard connected");
    }
    void onDisconnect(BLEServer *pSvr) override {
        bleConnected = false;
        digitalWrite(LED_PIN, LOW);
        pSvr->startAdvertising();
        Serial.println("[BLE] Disconnected — re-advertising");
    }
};

// ─── Forward declarations ────────────────────────────────────
float  readPressureKpa();
void   calibrateZero();
float  readTempFromNano();
int    pressureToClogPct(float kpa);
String pressureToStatus(float kpa);
String tempToStatus(float c);
String buildJSON(const SensorReading &r);
void   checkAndSendAlerts(const SensorReading &r);
void   sendAlert(const char *level, const char *sensor, const char *msg);
void   setupBLE();

// ════════════════════════════════════════════════════════════
//  SETUP
// ════════════════════════════════════════════════════════════
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== Dirt Bike Sensor System ===");

    pinMode(LED_PIN, OUTPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(PRESSURE_ADC_PIN, ADC_11db);

    // Serial1 receives temperature from Arduino Nano
    Serial1.begin(9600, SERIAL_8N1, D7, -1);
    Serial.println("[SERIAL] Listening for Nano temperature on D7");

    delay(500);
    calibrateZero();
    setupBLE();

    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH); delay(150);
        digitalWrite(LED_PIN, LOW);  delay(150);
    }
    Serial.println("[BOOT] Ready\n");
}

// ════════════════════════════════════════════════════════════
//  MAIN LOOP
// ════════════════════════════════════════════════════════════
void loop() {
    if (!bleConnected) {
        digitalWrite(LED_PIN, HIGH); delay(100);
        digitalWrite(LED_PIN, LOW);  delay(100);
    }

    SensorReading r;
    r.uptimeMs      = millis();
    r.pressureKpa   = readPressureKpa();
    r.pressureAdj   = r.pressureKpa - zeroOffsetKpa;
    r.filterClogPct = pressureToClogPct(r.pressureAdj);
    r.filterStatus  = pressureToStatus(r.pressureAdj);

    float t = readTempFromNano();
    if (t > -100.0f) lastOilTempC = t;
    r.oilTempC      = lastOilTempC;
    r.oilTempStatus = tempToStatus(r.oilTempC);

    Serial.printf("[PRESSURE] Raw=%+.4f kPa  Adj=%+.4f kPa  Clog=%3d%%  [%s]\n",
                  r.pressureKpa, r.pressureAdj,
                  r.filterClogPct, r.filterStatus.c_str());
    Serial.printf("[OIL TEMP] %.2f C  [%s]\n",
                  r.oilTempC, r.oilTempStatus.c_str());

    String json = buildJSON(r);
    if (bleConnected) {
        pTelemChar->setValue(json.c_str());
        pTelemChar->notify();
    }

    checkAndSendAlerts(r);
    delay(SAMPLE_INTERVAL_MS);
}

// ════════════════════════════════════════════════════════════
//  TEMPERATURE FROM NANO
// ════════════════════════════════════════════════════════════
float readTempFromNano() {
    while (Serial1.available()) {
        String line = Serial1.readStringUntil('\n');
        line.trim();
        if (line.startsWith("TEMP:")) {
            String val = line.substring(5);
            if (val == "ERR") return -127.0f;
            return val.toFloat();
        }
    }
    return -127.0f;
}

// ════════════════════════════════════════════════════════════
//  PRESSURE SENSOR
// ════════════════════════════════════════════════════════════
float readPressureKpa() {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < PRESSURE_SAMPLES; i++) {
        sum += analogRead(PRESSURE_ADC_PIN);
        delayMicroseconds(200);
    }
    float adcAvg = (float)sum / PRESSURE_SAMPLES;
    float vout   = (adcAvg / ADC_MAX) * V_SUPPLY;
    float pKpa   = (vout / V_SUPPLY - MPXV7002_OFFSET) / MPXV7002_SENS;
    return constrain(pKpa, -2.0f, 2.0f);
}

void calibrateZero() {
    Serial.print("[CAL] Zero calibration");
    float sum = 0;
    for (int i = 0; i < 64; i++) {
        sum += readPressureKpa();
        delay(10);
        if (i % 16 == 0) Serial.print(".");
    }
    zeroOffsetKpa = sum / 64.0f;
    Serial.printf("\n[CAL] Offset = %+.4f kPa\n", zeroOffsetKpa);
}

int pressureToClogPct(float kpa) {
    if (kpa >= 0.0f)               return 0;
    if (kpa <= PRESSURE_CRIT_KPA)  return 100;
    return (int)(100.0f * kpa / PRESSURE_CRIT_KPA);
}

String pressureToStatus(float kpa) {
    if (kpa <= PRESSURE_CRIT_KPA)  return "CRITICAL";
    if (kpa <= PRESSURE_WARN_KPA)  return "WARNING";
    if (kpa <= PRESSURE_WATCH_KPA) return "WATCH";
    return "CLEAN";
}

String tempToStatus(float c) {
    if (c < -100.0f)      return "SENSOR_ERR";
    if (c >= OIL_CRIT_C)  return "CRITICAL";
    if (c >= OIL_WARN_C)  return "WARNING";
    if (c <  OIL_COLD_C)  return "COLD";
    return "NORMAL";
}

// ════════════════════════════════════════════════════════════
//  ALERTS
// ════════════════════════════════════════════════════════════
void checkAndSendAlerts(const SensorReading &r) {
    if      (r.filterStatus == "CRITICAL")
        sendAlert("CRITICAL", "AIR_FILTER", "Air filter CRITICALLY clogged — stop and clean!");
    else if (r.filterStatus == "WARNING")
        sendAlert("WARNING",  "AIR_FILTER", "Air filter restricted — clean filter soon.");
    else if (r.filterStatus == "WATCH")
        sendAlert("WATCH",    "AIR_FILTER", "Air filter starting to restrict — monitor closely.");

    if      (r.oilTempStatus == "CRITICAL")
        sendAlert("CRITICAL", "OIL_TEMP", "OIL CRITICALLY HOT — stop engine immediately!");
    else if (r.oilTempStatus == "WARNING")
        sendAlert("WARNING",  "OIL_TEMP", "Oil temperature high — ease off throttle.");
    else if (r.oilTempStatus == "COLD")
        sendAlert("INFO",     "OIL_TEMP", "Oil still cold — warm up gently before hard riding.");
}

void sendAlert(const char *level, const char *sensor, const char *msg) {
    Serial.printf("[ALERT][%s][%s] %s\n", level, sensor, msg);
    StaticJsonDocument<192> doc;
    doc["level"]  = level;
    doc["sensor"] = sensor;
    doc["msg"]    = msg;
    doc["ts"]     = millis();
    String out;
    serializeJson(doc, out);
    if (bleConnected) {
        pAlertChar->setValue(out.c_str());
        pAlertChar->notify();
    }
}

// ════════════════════════════════════════════════════════════
//  JSON TELEMETRY
// ════════════════════════════════════════════════════════════
String buildJSON(const SensorReading &r) {
    StaticJsonDocument<256> doc;
    doc["id"]           = DEVICE_ID;
    doc["ts"]           = r.uptimeMs;
    doc["pRaw"]         = serialized(String(r.pressureKpa, 4));
    doc["pAdj"]         = serialized(String(r.pressureAdj, 4));
    doc["clogPct"]      = r.filterClogPct;
    doc["filterStatus"] = r.filterStatus;
    doc["oilTempC"]     = serialized(String(r.oilTempC, 2));
    doc["oilStatus"]    = r.oilTempStatus;
    String out;
    serializeJson(doc, out);
    return out;
}

// ════════════════════════════════════════════════════════════
//  BLE SETUP
// ════════════════════════════════════════════════════════════
void setupBLE() {
    BLEDevice::init(DEVICE_ID);
    BLEDevice::setPower(ESP_PWR_LVL_P9);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService *pService = pServer->createService(BLE_SERVICE_UUID);

    pTelemChar = pService->createCharacteristic(
        BLE_TELEM_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTelemChar->addDescriptor(new BLE2902());

    pAlertChar = pService->createCharacteristic(
        BLE_ALERT_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pAlertChar->addDescriptor(new BLE2902());

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("[BLE] Advertising as \"" DEVICE_ID "\"");
    Serial.printf( "[BLE] Telem UUID : %s\n", BLE_TELEM_UUID);
    Serial.printf( "[BLE] Alert UUID : %s\n", BLE_ALERT_UUID);
}
