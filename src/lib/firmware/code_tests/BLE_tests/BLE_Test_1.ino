/*
 * BLE Test using standard ESP32 BLE library
 * Board: Seeed XIAO ESP32-C6
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

BLEAdvertising *pAdvertising;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== BLE Test (Standard Library) ===");

    BLEDevice::init("DirtBike_001");
    BLEDevice::setPower(ESP_PWR_LVL_P9);

    BLEServer *pServer = BLEDevice::createServer();

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Advertising as DirtBike_001");
    Serial.println("Scan with nRF Connect NOW");
}

void loop() {
    Serial.printf("Advertising: %s\n",
        pAdvertising->isAdvertising() ? "YES" : "NO");
    if (!pAdvertising->isAdvertising()) {
        BLEDevice::startAdvertising();
        Serial.println("Restarted advertising");
    }
    delay(2000);
}
