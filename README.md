# CSC494 — Dirt Bike Maintenance Sensor System

## Overview

This IoT system is designed to protect dirt bike engines during aggressive off-road riding. It continuously monitors two critical conditions using sensors connected to a Seeed Studio XIAO ESP32-C6 microcontroller: oil temperature and air box differential pressure. Real-time data is transmitted via Bluetooth Low Energy to a mobile web dashboard, delivering notifications so riders know when their engine is at operating temperature, when it is running too hot, and when the air filter is becoming restricted — before minor issues escalate into expensive damage like overheating or seizure.

---

## Problem

Dirt bikes — both 2-stroke and 4-stroke — face extreme conditions: dust-filled trails, sustained high RPMs, crashes, and frequent tipping. Neglecting basic maintenance is the leading cause of catastrophic engine failures:

- **Clogged air filters** — restricted airflow, rich running, power loss, and abrasive dirt ingestion that accelerates internal wear
- **Overheating** — riding a cold engine too hard or pushing a hot engine beyond its limits leads to piston expansion, seizure, and scored components

These preventable issues account for the vast majority of engine failures in dirt bikes, as manual checks are often skipped mid-ride or between sessions. A single engine rebuild on a YZ450F costs **$1,000–$3,000**.

---

## Solution

The system provides real-time visibility of:

- **Oil temperature** via a DS18B20 waterproof probe clamped to the engine case — alerting the rider when the engine is still cold, in the normal operating range, or critically overheating
- **Air box differential pressure** via an MPXV7002DP sensor with a tube routed through the air box lid seam — detecting filter restriction with a 0–100% clog severity reading

Threshold-based alerts are delivered via BLE notifications directly to the rider's phone — for example "Air filter restricted — clean filter soon" or "Oil temperature high — ease off throttle."

The system is powered via the SAE trickle charger connector already on the bike — zero permanent wiring required.

---

## Hardware

| Component | Purpose |
|---|---|
| Seeed Studio XIAO ESP32-C6 | Main microcontroller + BLE 5.0 transmission |
| Arduino Nano | DS18B20 temperature acquisition via 1-Wire |
| MPXV7002DP breakout | Air box differential pressure sensor |
| DS18B20 waterproof probe | Oil pan surface temperature sensor |
| DS18B20 pluggable terminal board | Breakout board with built-in pull-up resistor |
| SAE to USB adapter | Power from bike trickle charger connector |

---

## Wiring

### MPXV7002DP → XIAO ESP32-C6
| Sensor Pin | XIAO Pin |
|---|---|
| GND | GND (blue rail) |
| +5V | 3V3 (red rail) |
| Analog OUT | A1 |

### DS18B20 probe → pluggable terminal board
| Probe wire | Terminal |
|---|---|
| Yellow | DAT |
| Red | VCC |
| Black | GND |

### Pluggable terminal board → Arduino Nano
| Terminal | Nano Pin |
|---|---|
| DAT | D3 |
| VCC | 5V |
| GND | GND |

### Arduino Nano → XIAO ESP32-C6
| Nano Pin | XIAO Pin |
|---|---|
| TX | D7 |
| GND | GND |

---

## Software

### Libraries Required
| Library | Board | Install via |
|---|---|---|
| ArduinoJson by Benoit Blanchon | XIAO | Library Manager |
| OneWire by Jim Studt | Nano | Library Manager |
| DallasTemperature by Miles Burton | Nano | Library Manager |
| Standard ESP32 BLE | XIAO | Built into ESP32 board package |

> **Note:** The project originally used NimBLE-Arduino but switched to the standard ESP32 BLE library for iOS compatibility. NimBLE is not required.

### Firmware Files
| File | Upload to |
|---|---|
| `src/lib/firmware/main/main.ino` | Seeed Studio XIAO ESP32-C6 |
| `src/lib/firmware/Nano_TempSensor/Nano_TempSensor.ino` | Arduino Nano |

---

## Alert Thresholds

### Air Filter (Differential Pressure)
| Status | Threshold | Action |
|---|---|---|
| CLEAN | > −0.50 kPa | No action needed |
| WATCH | ≤ −0.50 kPa | Monitor closely |
| WARNING | ≤ −0.75 kPa | Clean filter soon |
| CRITICAL | ≤ −1.00 kPa | Stop and clean immediately |

### Oil Temperature
| Status | Range | Action |
|---|---|---|
| COLD | < 60°C | Warm up gently before hard riding |
| NORMAL | 60–110°C | Safe operating range |
| WARNING | > 110°C | Ease off throttle |
| CRITICAL | > 130°C | Stop engine immediately |

---

## BLE Output

The XIAO advertises as `DirtBike_001` and sends a JSON telemetry packet every 500ms:

```json
{
  "id": "DirtBike_001",
  "ts": 5000,
  "pRaw": -0.9087,
  "pAdj": -0.0012,
  "clogPct": 0,
  "filterStatus": "CLEAN",
  "oilTempC": 28.62,
  "oilStatus": "COLD"
}
```

### BLE UUIDs
| Characteristic | UUID |
|---|---|
| Service | `12345678-1234-1234-1234-123456789abc` |
| Telemetry | `12345678-1234-1234-1234-123456789abd` |
| Alerts | `12345678-1234-1234-1234-123456789abe` |

---

## Dashboard

The web dashboard is built with HTML, CSS, JavaScript, and the Web Bluetooth API. It is hosted on GitHub Pages and opens directly in Bluefy on iPhone — no app install required.

**Live Dashboard:**
[https://TommySwitzer45.github.io/CSC494-Dirt_Bike_Maintenance_Sensor_System](https://TommySwitzer45.github.io/CSC494-Dirt_Bike_Maintenance_Sensor_System)

> **Note:** Web Bluetooth requires HTTPS and is not supported in Safari on iPhone. Use **Bluefy** (free on App Store) to connect from iPhone.

**Dashboard features:**
- Live air filter clog % gauge with color coded status
- Live oil temperature gauge with zone colors
- Real-time alert banner for threshold notifications
- History chart — toggle between temperature and clog % over last 60 readings
- Auto-reconnect on BLE disconnect

---

## Demo Video

Warm Bike: [https://youtube.com/shorts/1X038i7_y94?feature=share](https://youtube.com/shorts/QOzN9UsvcXs?feature=share)

Cold Bike: [https://youtube.com/shorts/FnaW9bqta3Q?feature=share](https://youtube.com/shorts/u4xNnDTEOb8?feature=share)

---

## Tech Stack

- **Seeed Studio XIAO ESP32-C6** — ESP32-C6 RISC-V, BLE 5.0
- **Arduino Nano** — AVR ATmega328P, handles DS18B20 1-Wire communication
- **Standard ESP32 BLE library** — BLE GATT server with notify characteristics
- **ArduinoJson** — JSON telemetry serialization
- **HTML + CSS + JavaScript + Web Bluetooth API** — mobile web dashboard
- **GitHub Pages** — free HTTPS hosting
- **Arduino C++** — firmware for both boards

---

## Key Technical Challenges

- **ESP32-C6 RISC-V incompatibility** — bit-bang 1-Wire timing fails on the RISC-V core. DS18B20 temperature reading was offloaded to an Arduino Nano which sends data to the XIAO via serial on D7
- **BLE iOS visibility** — NimBLE library was invisible to iOS devices. Switched to standard ESP32 BLE library which resolved the issue
- **ADC pin** — A0 was not reading correctly. Pressure sensor moved to A1 with `analogSetPinAttenuation()` configured for full 0-3.3V range

---

## Bike

**2018 Yamaha YZ450F** — reversed cylinder engine, air box mounted on top, SAE trickle charger connector used for power
