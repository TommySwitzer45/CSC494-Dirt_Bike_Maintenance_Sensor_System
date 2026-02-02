---
marp: true
theme: gaia
paginate: true
backgroundColor: #f0f4f8
style: |
  section {
    font-size: 0.85em;   /* Try 0.8em–0.9em to start */
  }
  h1 { font-size: 8em; }
  h2 { font-size: 4.5em; }
  p, ul, ol { font-size: 2.5em; }
---

# DirtBike Maintence System 
IoT Predictive Maintenance for Dirt Bikes

CSC 494-001  
Tommy Switzer

---

## 1. Problem & Importance

**Problem**  
Dirt bikes face dust, high RPMs, crashes → neglected maintenance causes 80–90% of engine "blow-ups"  
- Low oil → seizure, scored bearings  
- Low coolant → overheating, warped heads  
- Clogged air filter → power loss, abrasive wear  

**Why important?** 
Prevents costly rebuilds, mid-ride failures, enhances safety on KY trails

---

## 2. Project Topic (Team)

**Software focus**  
Build IoT app on ESP32 hardware:  
- Real-time sensor data processing  
- Secure cloud transmission  
- Mobile dashboard + alerts  

**Hardware used**  
- Sensors: ultrasonic (levels), differential pressure (air box)  
- ESP32 + WiFi comms  
- No actuators/motors in MVP  

Sensor + communication + monitoring project

---

## 3. Two Topics to Learn with AI

**Software**  
Secure IoT comms & cloud integration  
- MQTT/TLS, API auth  
- Blynk/ThingSpeak dashboard  
- Alert logic + buffering  

**Hardware**  
ESP32 sensor interfacing & calibration  
- Ultrasonic & differential pressure  
- Vibration/noise filtering  
- 12V bike power regulation  

Use AI (Grok/Copilot) to accelerate both

---

## 4. Time Usage – Two Iterations

**Iteration 1 (Sprint 1)**  
Learn → build MVP  
- Sensor reading/calibration  
- WiFi → cloud data send  
- Basic dashboard + thresholds  

**Iteration 2 (Sprint 2)**  
Full features  
- Alerts (push/email)  
- Secure TLS/auth  
- Trends, local buffering  
- Vibration/dust testing  

---

## 5. Final Goal (End of Semester)

- **Working prototype** mounted & demonstrated  
- **Portfolio piece**: GitHub repo, code, schematics, demo video   
- **Future extension** potential: vibration sensor, edge ML, LoRa

