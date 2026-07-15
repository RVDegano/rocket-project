# 🚀 Frontera 1 — D12-5 Flight Computer

A single-stage model rocket designed, simulated, and built from scratch. Includes a custom flight computer for onboard data logging during flight.

## 📋 Project Overview

| Parameter | Value |
|---|---|
| Motor | Estes D12-5 |
| Length | 42.5 cm |
| Max Diameter | 33 mm (body tube, external) |
| Mass (no motor) | 175 g |
| Mass (with motor) | 218 g |
| Stability Margin | 1.51 cal |

## 📊 Simulation Results (OpenRocket)

| Metric | Value |
|---|---|
| Apogee | 204 m |
| Max Velocity | 61.1 m/s (Mach 0.180) |
| Max Acceleration | 129 m/s² |
| CG | 24.2 cm |
| CP | 29.3 cm |

*Note: the design went through an earlier iteration with a heavier body tube that dropped stability to 1.15 cal and apogee to 180 m. This was corrected by adding a mass component near the nose, restoring the margin to 1.51 cal.*

## 🔧 Flight Computer

**Hardware:**
- Arduino Nano V3.0 (ATmega328)
- BMP280 — barometric altimeter (altitude + pressure + temperature)
- MPU6050 — IMU (acceleration + gyroscope, 6-axis)
- microSD module + card — onboard data storage

**Features:**
- Continuous data logging from power-on (20 Hz sampling rate)
- CSV output: timestamp, altitude, temperature, acceleration (x/y/z), gyroscope (x/y/z)
- No liftoff/apogee detection — logging starts as soon as the board is powered, before the rocket is loaded on the launch rail

**Recovery / ejection:**
Parachute deployment is **mechanical**, not electronically triggered. The D12-5 motor's built-in ejection charge (5-second delay after burnout) pushes the nose cone off by friction, releasing the parachute. The flight computer only logs data — it does not control ejection.

## 🖨️ Structure

| Component | Material |
|---|---|
| Body tube | Cardboard, 31.5 mm internal / 33 mm external diameter (0.75 mm wall) |
| Nose cone (ojiva) | PVC, 13 cm long, 1 mm wall |
| Fins | PETG, 3D printed, trapezoidal |

PETG was selected for the fins for its heat resistance near the motor mount / ejection charge area.

## 📁 Repository Structure

```
rocket-project/
├── simulation/        # OpenRocket files (.ork)
├── firmware/           # Arduino Nano code
├── cad/                 # 3D models (STL files, fins)
├── docs/                # Photos, flight data, analysis
└── README.md
```

## 🎯 Goals

- [x] OpenRocket simulation complete
- [x] Motor selection finalized (D12-5, chosen over Quest D16-4 for higher apogee — D16-4 reserved for a future test flight)
- [x] Flight computer designed
- [ ] Fins printed and assembled
- [ ] Full assembly
- [ ] Launch — school showcase, San Bonifacio, October 2026 (launch site: Finca Cinco Cumbres, Mendoza)
- [ ] Post-launch data analysis

## 📸 Build Process

Photos coming as construction progresses.

## 🛠️ Tools & Software

- OpenRocket — flight simulation
- Arduino IDE — firmware
- Tinkercad — 3D modeling (fins)

## 👤 Author

**Rocco V. Degano**
Electronics technician student — Instituto Tecnológico San Bonifacio, Lomas de Zamora
Interested in aerospace propulsion and flight systems.
Part of a personal aerospace engineering portfolio.
