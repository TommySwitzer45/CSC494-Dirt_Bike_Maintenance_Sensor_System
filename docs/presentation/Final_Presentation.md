---
marp: true
paginate: true
style: |
  section {
    background-color: #080C14;
    color: #C8D8E8;
    font-family: 'Trebuchet MS', sans-serif;
    padding: 50px 70px;
  }
  h1 {
    font-size: 2.4em;
    color: #E94560;
    letter-spacing: 2px;
  }
  h2 {
    font-size: 1.8em;
    color: #E94560;
    border-bottom: 3px solid #E94560;
    padding-bottom: 12px;
  }
  h3 {
    font-size: 1.35em;
    color: #00FF88;
  }
  p, ul, li {
    font-size: 0.95em;
    line-height: 1.65;
  }
  table {
    font-size: 0.88em;
    width: 100%;
    border-collapse: collapse;
  }
  th {
    background: #1A2840;
    color: #E94560;
    padding: 8px 10px;
  }
  td {
    padding: 8px 10px;
    border-bottom: 1px solid #1A2840;
  }
  code {
    background: #1A2840;
    color: #00FF88;
    padding: 3px 7px;
    border-radius: 4px;
  }
---

# Dirt Bike Maintenance Sensor System

**Final Presentation**  
CSC494 — Tommy Switzer  
2018 Yamaha YZ450F

---

## The Problem

Dirt bikes face extreme conditions:
- Dust & mud
- High RPMs
- Crashes & tipping

**80–90% of engine failures** are caused by neglected maintenance.

**Main causes:**
- Clogged air filter
- Low oil
- Overheating

Riders usually don’t notice until damage is already done.

---

## Problem Domain — In My Own Words

I ride a 2018 Yamaha YZ450F. Checking the air filter or oil temperature mid-ride just doesn’t happen — you’re focused on riding.

The air filter clogs with mud and dust, and the oil gets too hot, but there’s no warning until the engine loses power or seizes.

**The problem I solved:**  
Give riders real-time alerts on their phone so they can prevent expensive damage before it happens.

Engine rebuilds cost **$1,000–$3,000**. This system helps avoid that.

---

## The Solution

A real-time IoT sensor system that monitors:
- **Air filter restriction** using a differential pressure sensor
- **Oil temperature** using a waterproof probe

**Key Features:**
- Zero permanent modifications to the bike
- Bluetooth Low Energy data sent to phone
- Live gauges and threshold alerts on a web dashboard
- Powered from the bike’s existing SAE trickle charger plug

---

## Solution Domain — In My Own Words

I built a sensor system that mounts without drilling or permanent changes.

- Pressure sensor tube routes through the air box lid seam
- Temperature probe clamps to the engine case near the oil fill cap

Data is processed by a small microcontroller and broadcast via Bluetooth.  
A web dashboard (no app install) shows live gauges, color-coded alerts, and history charts. It opens in Bluefy on iPhone.

The entire system plugs into the bike’s SAE connector — truly plug-and-play.

---

## Original Plan vs Reality

| What I Planned                  | What Happened                          |
|---------------------------------|----------------------------------------|
| Ultrasonic oil level sensor     | Dropped — too invasive                 |
| Coolant level sensor            | Dropped — no non-invasive option       |
| WiFi cloud sync                 | Not needed — BLE was enough            |
| DC-DC converter from 12V        | Replaced with SAE to USB adapter       |
| Pressure sensor + dashboard     | ✅ Fully implemented                    |
| Push alerts                     | ✅ Implemented via BLE                  |

**Lesson:** Keep it simple and non-invasive for the prototype.

---

## Technology Stack

- **XIAO ESP32-C6** — Main MCU + Bluetooth 5.0
- **Arduino Nano** — Reads DS18B20 1-Wire protocol
- **MPXV7002DP** — Air filter differential pressure
- **DS18B20** — Waterproof oil temperature probe
- **Standard ESP32 BLE** — Better iOS compatibility
- **ArduinoJson** — Lightweight JSON telemetry
- **HTML + CSS + JS + Web Bluetooth** — Mobile dashboard
- **GitHub Pages** — Free HTTPS hosting
- **SAE to USB adapter** — Bike power source

---

## How AI Helped

**AI was very helpful for:**
- Fast debugging using serial monitor output
- Generating exact wiring tables with pin numbers
- Rewriting specific firmware functions
- Quickly identifying hardware issues

**Where AI was wrong:**
- Suggested `analogSetAttenuation()` (doesn’t exist on XIAO ESP32-C6)
- Outdated NimBLE callback signatures
- Didn’t know ESP32-C6 RISC-V core can’t bit-bang 1-Wire

**Lesson:** AI is a powerful tool, but always test and verify.

---

## Learning with AI – Topic 1: Sensor Interfacing

**What I set out to learn:**  
How to connect, calibrate, and mount sensors on a real dirt bike.

**What I learned:**
- Real hardware is very different from tutorials
- Pressure sensor failed on A0 → moved to A1
- ESP32-C6 RISC-V cannot reliably bit-bang 1-Wire for DS18B20 → offloaded to Arduino Nano
- Mounting on a vibrating dirt bike (zip ties, foil, air box seam) is much harder than breadboard work

Real-world vibration, dust, and “no drilling” rule made everything more challenging.

---

## Learning with AI – Topic 2: The Dashboard

**What I set out to learn:**  
How to build a wireless mobile dashboard with live data and alerts.

**What I learned:**
- Web Bluetooth does **not** work in Safari on iPhone
- NimBLE library was invisible to iOS → switched to standard ESP32 BLE library
- Chrome on Windows blocked it (no Bluetooth adapter)
- Final solution: GitHub Pages (HTTPS) + Bluefy browser on iPhone

Browser security, platform limits, and hosting requirements made it far more complex than expected.

---

## Weekly Progress Summary

**Stage 1 (Weeks 1–3):** Research, requirements, and GitHub setup  
**Sprint 1 (Weeks 4–7):** Hardware, firmware, BLE, and first presentation  
**Sprint 2 (Weeks 10–15):** Dashboard development, bike testing, and final deliverables

---

## Demonstration

**Watch the video:**

> 📹 **[INSERT YOUR VIDEO LINK HERE]**  
> *(Upload to YouTube as Unlisted and paste the link)*

**What you’ll see:**
- Live dashboard connected via Bluetooth in Bluefy
- Pressure sensor responding to throttle input
- Temperature rising during engine warm-up
- Alerts triggering at the correct thresholds

---

## Final Results

**System works successfully on the 2018 Yamaha YZ450F!**

- ✅ Pressure sensor responds to throttle and restriction
- ✅ Temperature sensor tracks warm-up from ~20°C
- ✅ Bluetooth device visible as `DirtBike_001`
- ✅ Live web dashboard with alerts in Bluefy
- ✅ Powered directly from bike’s SAE connector
- ✅ All code and docs on GitHub

---

## Future Improvements

- Improved mounting (hose clamps, thermal paste, barbed fittings)
- Weatherproof enclosure
- Solder or heat shrink tubing for better connection
- Custom oil fill cap for direct oil temperature
- Machine learning for predictive alerts
- Support for bikes without batteries

---
