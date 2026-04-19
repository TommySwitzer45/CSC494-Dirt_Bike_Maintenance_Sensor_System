/*
 * BLE Continuous Advertising Test
 * Board: Seeed XIAO ESP32-C6
 */

#include <NimBLEDevice.h>

NimBLEAdvertising *pAdv;

void startAdv() {
    if (!pAdv->isAdvertising()) {
        NimBLEDevice::startAdvertising();
        Serial.println("Advertising restarted");
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== BLE Continuous Test ===");

    NimBLEDevice::init("DirtBike_001");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    pAdv = NimBLEDevice::getAdvertising();
    pAdv->setMinInterval(160);   // 100ms
    pAdv->setMaxInterval(320);   // 200ms

    NimBLEDevice::startAdvertising();
    Serial.println("Advertising started — scan with nRF Connect NOW");
}

void loop() {
    startAdv();  // restart if stopped
    Serial.printf("Advertising: %s\n",
        pAdv->isAdvertising() ? "YES" : "NO");
    delay(1000);
}
