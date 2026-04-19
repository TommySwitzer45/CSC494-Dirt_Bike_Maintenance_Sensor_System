---
marp: true
paginate: true
style: |
  section {
    background-color: #080C14;
    color: #C8D8E8;
    font-family: 'Trebuchet MS', sans-serif;
    padding: 40px 60px;
  }
  h1 {
    font-size: 2.0em;
    color: #E94560;
    margin-bottom: 0.2em;
    letter-spacing: 3px;
  }
  h2 {
    font-size: 1.4em;
    color: #E94560;
    border-bottom: 2px solid #E94560;
    padding-bottom: 0.2em;
    margin-bottom: 0.6em;
    letter-spacing: 2px;
  }
  h3 {
    font-size: 1.0em;
    color: #00FF88;
    margin-bottom: 0.3em;
    letter-spacing: 1px;
  }
  p, ul, ol {
    font-size: 0.82em;
    line-height: 1.7;
  }
  ul li {
    margin-bottom: 0.25em;
  }
  strong {
    color: #E94560;
  }
  code {
    background: #1A2840;
    color: #00FF88;
    padding: 2px 6px;
    border-radius: 4px;
    font-size: 0.82em;
  }
  table {
    font-size: 0.75em;
    width: 100%;
    border-collapse: collapse;
  }
  th {
    background: #1A2840;
    color: #E94560;
    padding: 6px 10px;
    letter-spacing: 1px;
  }
  td {
    padding: 5px 10px;
    border-bottom: 1px solid #1A2840;
  }
---

# CSC494
# Dirt Bike Maintenance
# Sensor System
## Final Presentation — TommySwitzer45

**2018 Yamaha YZ450F · XIAO ESP32-C6 · Arduino Nano · BLE Dashboard**

---

## 1.1 Problem Domain

**I clearly defined the problem I am solving.**

Dirt bikes operate in extreme conditions — dust, mud, high RPMs, and frequent tipping — making engine maintenance critical but easy to neglect mid-ride. The two leading causes of preventable engine failure are **clogged air filters** and **oil overheating**, yet most riders have no way to monitor these conditions in real time.

**I explained why this problem is important to solve.**

- Clogged air filters cause restricted airflow, rich running, power loss, and abrasive dirt ingestion that accelerates internal wear
- Running a cold engine too hard or overheating causes piston expansion, seizure, and scored components
- A single engine rebuild costs **$1,000–$3,000**
- Manual checks are skipped mid-ride or between sessions — there is no warning until damage is done

**My problem definition is accessible via GitHub README and this presentation.**

---

## 1.1 Solution Domain

**I clearly described my proposed solution.**

A real-time IoT sensor system that monitors air filter restriction and oil temperature on a dirt bike, alerting riders before engine damage occurs.

**I explained how my solution addresses the problem.**

- **MPXV7002DP** pressure sensor detects filter restriction by measuring pressure drop across the air box filter — tube routed through lid seam, zero permanent modification
- **DS18B20** waterproof probe monitors oil temperature — clamped to engine case near oil fill cap, zip tied with aluminum foil for heat transfer
- **XIAO ESP32-C6** broadcasts live JSON telemetry via BLE as `DirtBike_001`
- **Web dashboard** on GitHub Pages connects via Bluefy on iPhone — shows live gauges, alerts, and history charts
- **Powered via SAE trickle charger connector** on the YZ450F — plug and play, no wiring required

**Solution is fully documented in README.md and accessible on GitHub.**

---

## 1.2 Technology Stack

**How I solved problems with AI:**

- ESP32-C6 RISC-V core cannot bit-bang 1-Wire timing → AI identified issue and offloaded DS18B20 to Arduino Nano via serial
- NimBLE library not visible to iOS → AI switched to standard ESP32 BLE library
- `analogSetAttenuation()` not available on XIAO → AI replaced with `analogSetPinAttenuation()`
- A0 pin not reading ADC correctly → AI diagnosed floating pin and moved to A1

**The technology stack I used:**

| Component | Purpose | Why Chosen |
|---|---|---|
| XIAO ESP32-C6 | Main MCU + BLE 5.0 | Compact, BLE built-in, 3.3V ADC |
| Arduino Nano | DS18B20 1-Wire reading | AVR handles 1-Wire timing correctly |
| MPXV7002DP | Air filter pressure | Differential pressure, analog output |
| DS18B20 probe | Oil temperature | Waterproof, accurate, 1-Wire |
| Standard ESP32 BLE | BLE transmission | Better iOS compatibility than NimBLE |
| Web Bluetooth API + GitHub Pages | Mobile dashboard | No app install, HTTPS, free hosting |

---

## 1.2 Demonstration Video

**I created a demonstration video showing my project results.**

The video demonstrates:
- Live dashboard open in Bluefy on iPhone connected to `DirtBike_001`
- Pressure sensor responding to throttle input on the YZ450F
- Temperature probe tracking engine warm-up from cold
- Alert notifications triggering when thresholds are crossed
- System powered via SAE trickle charger connector on the bike

**Link to demonstration video:**

> 📹 **[INSERT YOUR VIDEO LINK HERE]**
> *(Upload to YouTube or Google Drive and paste link)*

---

## 1.2 Marp Presentation

**I created a high-quality Marp presentation communicating my project.**

This presentation covers:
- Problem and solution domain
- Technology stack with justification
- Weekly progress summary
- Demonstration video link
- All sprint deliverables

**Link to Marp presentation PDF on GitHub:**

> 📄 **https://github.com/TommySwitzer45/REPO-NAME/blob/main/docs/presentation/Final_Presentation.pdf**

*(Export this .md file to PDF using Marp CLI or VS Code Marp extension and upload to GitHub)*

---

## 1.3 Weekly Progress — Sprint 1

**Week 1 — Hardware Setup**
- Ordered and received XIAO ESP32-C6, MPXV7002DP pressure sensor, DS18B20 waterproof probe, Arduino Nano, and breadboard components
- Set up Arduino IDE with ESP32 board package
- Initial wiring of pressure sensor — discovered A0 pin not reading correctly, moved to A1

**Week 2 — Firmware Development**
- Wrote bit-bang 1-Wire DS18B20 driver — discovered ESP32-C6 RISC-V core incompatible with timing
- Offloaded DS18B20 to Arduino Nano via serial — temperature reading working on D3
- Pressure sensor calibration working with auto zero-offset at boot
- BLE advertising confirmed working — switched from NimBLE to standard ESP32 BLE library for iOS visibility

---

## 1.3 Weekly Progress — Sprint 2

**Week 3 — Dashboard + Bike Integration**
- Built web dashboard in HTML/CSS/JavaScript with Web Bluetooth API
- Deployed to GitHub Pages — accessible at `https://TommySwitzer45.github.io/REPO-NAME`
- Connected dashboard via Bluefy on iPhone — live gauges, alerts, and history chart working
- Powered system via SAE trickle charger connector on 2018 Yamaha YZ450F
- Mounted pressure sensor tube through air box lid seam — no drilling required
- Mounted DS18B20 probe on engine case near oil fill cap

**Week 4 — Testing + Final Deliverables**
- Tested pressure sensor with throttle input — responds correctly to air restriction
- Validated temperature probe tracks engine warm-up from cold to operating range
- Completed README.md, Sprint1.md progress report, and this final presentation
- All firmware and dashboard files pushed to GitHub

---

## Results

**Both sensors working on the 2018 Yamaha YZ450F:**

| Sensor | Status | Result |
|---|---|---|
| MPXV7002DP pressure | ✅ Working | Responds to throttle input and filter restriction |
| DS18B20 temperature | ✅ Working | Tracks engine warm-up, reads 20–25°C at rest |
| BLE advertising | ✅ Working | Visible as `DirtBike_001` in nRF Connect and Bluefy |
| Web dashboard | ✅ Working | Live gauges, alerts, history chart in Bluefy on iPhone |
| SAE power | ✅ Working | Powers both XIAO and Nano from bike battery |

**Dashboard live at:**
`https://TommySwitzer45.github.io/REPO-NAME`

---

## Questions?

**Repository:**
`github.com/TommySwitzer45/REPO-NAME`

**Dashboard:**
`https://TommySwitzer45.github.io/REPO-NAME`

**BLE Device:** `DirtBike_001`

**Stack:** XIAO ESP32-C6 · Arduino Nano · MPXV7002DP · DS18B20 · Web Bluetooth · GitHub Pages

**2018 Yamaha YZ450F — powered via SAE trickle charger connector**
