---

marp: true
paginate: true
style: |
section {
padding: 40px 60px;
}
h1 {
font-size: 2.2em;
margin-bottom: 0.2em;
}
h2 {
font-size: 1.5em;
padding-bottom: 0.2em;
margin-bottom: 0.5em;
}
p, ul, ol {
font-size: 0.85em;
line-height: 1.6;
}
ul li {
margin-bottom: 0.3em;
}
strong {
}
.columns {
display: grid;
grid-template-columns: 1fr 1fr;
gap: 2em;
}

---

# CSC494
## Dirt Bike Maintenance Sensor System

---

## Challenges

Integrating sensors into a real dirt bike revealed challenges not apparent during initial design.
- The core problem — avoiding invasive modifications:

- Preserving the structural integrity of engine components
- Preventing potential leaks or contamination
- Maintaining warranty and resale value
- Minimizing risk of mechanical failure during aggressive off-road use

---

## Alterations in Plans

**Original design included:**

- Ultrasonic oil level sensor inside the oil pan
Coolant level sensor in the radiator/overflow reservoir

**Why these were dropped:**

- Oil level sensor requires access inside the oil pan — a custom oil fill cap was considered but requires fabrication tools not currently available
- Coolant sensor has the same constraints — no way to access the reservoir without drilling or permanent modification

These mounting challenges are the primary reason the project scope was changed.

---

## New Plan

**Refocused on two sensors that can be mounted right now:**

- MPXV7002DP air pressure sensor — two small silicone tubes, zero permanent modification to the bike
- DS18B20 temperature probe — clamped or zip-tied to the outside of the oil pan to read surface temperature

**Prototype goal:**

- Prove the concept works with the bike stationary — show that pressure changes measurably when the air filter is restricted or dirtied, and that temperature tracks engine warm-up correctly. This validates the core sensing logic before tackling permanent mounting.

---

## Progress

- Connected MPXV7002DP differential pressure sensor to Seeed Studio XIAO ESP32-C6
- Successfully reading live air box pressure with automatic zero calibration at boot
- Connected DS18B20 waterproof temperature probe via Arduino Nano to XIAO over serial
- Nano reads oil temperature and transmits to XIAO every second
- Live JSON telemetry payload transmitting pressure and temperature every 500ms
- Threshold alerts implemented for filter status (CLEAN / WATCH / WARNING / CRITICAL) and oil temperature (COLD / NORMAL / WARNING / CRITICAL)
- Wiring was harder than expected — voltage levels, pull-up resistors, and pin compatibility required significant troubleshooting

---

## Known Unknowns

- Building a mobile dashboard to display live BLE telemetry visually
- Mounting the sensors on an actual dirt bike in a waterproof enclosure
- Power source independent of a laptop — bike battery or small dedicated battery
- Validating pressure thresholds against a real dirty vs clean air filter

---

## Unknown Unknowns

- Whether MPXV7002DP readings will remain stable under engine vibration
- How heat from the engine bay will affect sensor accuracy over time
- How accurate and generalizable the readings will be across different bike models

---

## Learning with AI
**Making stuff up:**

- AI sometimes suggests incorrect pin numbers or library functions that do not exist

**Hardware compatibility gaps:**

- DS18B20 could not be read directly from the XIAO — required offloading to the Arduino Nano

---

## What AI was good at:

- Diagnosing serial monitor output and identifying root causes quickly
- Rewriting specific functions without breaking the rest of the code
- Explaining wiring step by step with clear tables

---

## Sprint 2

- Build a mobile dashboard to display live - pressure and temperature readings
- Test the system on a real dirt bike engine
- Validate pressure thresholds using a real dirty air filter