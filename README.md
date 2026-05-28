# 🚀 Model Rocket — D12-5 Flight Computer

A single-stage model rocket designed, simulated, and built from scratch. Includes a custom flight computer for data logging and mechanical parachute ejection.

---

## 📋 Project Overview

| Parameter | Value |
|-----------|-------|
| Motor | Estes D12-5 |
| Length | 39 cm |
| Diameter | 35 mm |
| Mass (no motor) | 190 g |
| Mass (with motor) | 232 g |
| Stability | 1.15 cal |

---

## 📊 Simulation Results (OpenRocket)

| Metric | Value |
|--------|-------|
| Apogee | **184 m** |
| Max Velocity | **56.6 m/s** (Mach 0.167) |
| Max Acceleration | **120 m/s²** |
| Flight Duration | ~30 s |
| CG | 22.3 cm |
| CP | 26.3 cm |

---

## 🔧 Flight Computer

**Hardware:**
- Arduino Uno
- BMP280 — barometric altimeter (altitude + pressure)
- MPU6050 — IMU (acceleration + gyroscope)

**Features:**
- Real-time altitude and acceleration logging
- Mechanical parachute ejection at apogee detection
- Data stored onboard for post-flight analysis

---

## 🖨️ Structure

- **Nose cone** — 3D printed in PETG (heat resistant)
- **Body tube** — 3D printed in PETG
- **Fins** — 3D printed in PETG
- Recovery system: mechanical parachute ejection

PETG selected over PLA for thermal resistance near the motor mount.

---

## 📁 Repository Structure

```
rocket-project/
├── simulation/        # OpenRocket files (.ork)
├── firmware/          # Arduino Nano code
├── cad/               # 3D models (STL files)
├── docs/              # Photos, flight data, analysis
└── README.md
```

---

## 🎯 Goals

- [x] OpenRocket simulation complete
- [x] Flight computer designed
- [ ] PETG components printed
- [ ] Full assembly
- [ ] Launch — school showcase, October 2026
- [ ] Post-launch data analysis

---

## 📸 Build Process

*Photos coming as construction progresses.*

---

## 🛠️ Tools & Software

- [OpenRocket](https://openrocket.info/) — flight simulation
- Arduino IDE — firmware
- Fusion 360 / FreeCAD — 3D modeling

---

## 👤 Author

**Rocco V. Degano**  
Electronics technician student — Instituto Tecnológico San Bonifacio, Lomas de Zamora  
Interested in aerospace propulsion and flight systems.

---

*Part of a personal aerospace engineering portfolio.*
