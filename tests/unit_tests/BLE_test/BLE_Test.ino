/*
 * BLE Debug Test
 * Board: Seeed XIAO ESP32-C6
 */

#include <NimBLEDevice.h>

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== BLE Debug Test ===");

    Serial.println("Step 1: Initializing NimBLE...");
    NimBLEDevice::init("DirtBike_001");
    Serial.println("Step 1: DONE");

    Serial.println("Step 2: Setting power...");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    Serial.println("Step 2: DONE");

    Serial.println("Step 3: Getting advertising object...");
    NimBLEAdvertising *pAdv = NimBLEDevice::getAdvertising();
    Serial.println("Step 3: DONE");

    Serial.println("Step 4: Starting advertising...");
    bool started = NimBLEDevice::startAdvertising();
    Serial.printf("Step 4: %s\n", started ? "SUCCESS" : "FAILED");

    if (started) {
        Serial.println(">>> DirtBike_001 is now visible <<<");
        Serial.println("Scan with nRF Connect NOW");
    } else {
        Serial.println(">>> ADVERTISING FAILED — BLE hardware issue <<<");
    }
}

void loop() {
    delay(3000);
    Serial.printf("Advertising: %s\n",
        NimBLEDevice::getAdvertising()->isAdvertising() ? "YES" : "NO");
}
