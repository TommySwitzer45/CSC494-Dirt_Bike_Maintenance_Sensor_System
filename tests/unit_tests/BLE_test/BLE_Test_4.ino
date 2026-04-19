/*
 * Simple BLE Advertising Test
 * Board: Seeed XIAO ESP32-C6
 * Just advertises "DirtBike_001" — nothing else
 */

#include <NimBLEDevice.h>

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== BLE Test ===");

    NimBLEDevice::init("DirtBike_001");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    NimBLEDevice::startAdvertising();

    Serial.println("Advertising as DirtBike_001");
    Serial.println("Open Bluefy and scan now!");
}

void loop() {
    Serial.println("Still advertising...");
    delay(2000);
}
