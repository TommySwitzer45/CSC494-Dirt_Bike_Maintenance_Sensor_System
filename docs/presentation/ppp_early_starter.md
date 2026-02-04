---
marp: true
paginate: true
style: |
  section {
    font-size: 0.85em;   /* Try 0.8em–0.9em to start */
  }
  h1 { font-size: 8em; }
  h2 { font-size: 4em; }
  p, ul, ol { font-size: 2m; }
---

# DirtBike Maintence System 
IoT Predictive Maintenance for Dirt Bikes
  
Tommy Switzer

---

## 1. Problem & Importance

**Problem**  
Dirt bikes face dust, high RPMs, crashes → neglected maintenance causes 80–90% of engine "blow-ups"  
- Low oil → seizure, scored bearings  
- Low coolant → overheating, warped heads  
- Clogged air filter → power loss, abrasive wear  

**Why important?** 
Prevents costly rebuilds, mid-ride failures, enhances safety on

---

## 2. Project Topic

**Software Focus**  
Build IoT application on ESP32 hardware:  
- Real-time sensor data processing & calibration    
- Mobile-friendly dashboard with gauges, trends & alerts  

**Hardware Used**  
- Core: ESP32 microcontroller (WiFi + Bluetooth)  
- Sensors: Waterproof ultrasonic (oil/coolant levels) 
- Differential pressure sensor (air-box restriction)
- Power: DC-DC converter from 12V bike battery  

**Communication**  
- WiFi: Cloud sync for historical data when in range  
- Bluetooth: Connection to smartphone app for dashboards, gauges, and alerts
  
---

## 3. Two Topics to Learn with AI

**Software**  
Build IoT app on ESP32 hardware:  
- Build dashboard  
- Alert logic + buffering  

**Hardware**  
ESP32 sensor interfacing & calibration  
- Ultrasonic & differential pressure     

---

## 4. Time Usage – Two Iterations

**Iteration 1 (Sprint 1)**  
Learn → build 
- Sensor reading/calibration  
- Basic dashboard + thresholds  

**Iteration 2 (Sprint 2)**  
Full features  
- Alerts (push/email)   
- Trends, local buffering  
- Vibration/dust testing  

---

## 5. Final Goal (End of Semester)

- **Working prototype** mounted & demonstrated  
- **Research paper**: show findings, bugs, fixes, and w
- **Portfolio piece**: GitHub repo, code, schematics, demo video   
- **Future extension** potential: vibration sensor
  
---

## Questions?



