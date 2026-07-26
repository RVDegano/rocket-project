# 🚀 Frontera 1 — D12-5 Flight Computer

A single-stage model rocket designed, simulated, and built from scratch. Includes a custom flight computer for onboard data logging during flight. First launch completed successfully at Finca Cinco Cumbres, Uspallata, Mendoza (Argentina).

## 📋 Project Overview

| Parameter | Value |
|---|---|
| Motor | Estes D12-5 |
| Length | 42.5 cm |
| Max Diameter | 33 mm (body tube, external) |
| Mass (no motor) | 175 g |
| Mass (with motor) | 218 g |
| Stability Margin | 1.51 cal |

## 📊 Simulation vs. Real Flight Data

| Metric | Simulated (OpenRocket) | Real (first launch) |
|---|---|---|
| Apogee | 204 m | **178 m** |
| Max Acceleration | ~129 m/s² (sim, peak) | **61 m/s²** (measured) |
| Max Velocity | 61.1 m/s | — |
| Flight Duration | ~30-40 s (estimated) | **50 s** |
| Temperature | — | 11°C |

The real apogee came in below simulation, which is expected — real-world drag, wind, and the exact mass with all electronics loaded are hard to match perfectly in simulation. The overall flight profile (ignition, ascent, ejection timing) matched expectations well.

**Caveat on data reliability:** during ejection, the parachute burned and the payload (electronics bay) detached from the shock cord, getting ejected along with the nose cone instead of staying tethered as designed. Because of this, the exact accuracy of the logged flight parameters above is uncertain — the payload likely experienced additional uncontrolled forces/impacts after separating that weren't part of the intended flight profile. The numbers are presented as the best available real data, not as fully validated measurements.

*Design note: the project went through an earlier iteration with a heavier body tube that dropped stability to 1.15 cal and apogee to 180 m (simulated). This was corrected by adding a mass component near the nose, restoring the margin to 1.51 cal before the tube was later shortened to 27 cm in the final build.*

## 🔧 Flight Computer

**Hardware:**
- Arduino Nano V3.0 (ATmega328)
- BMP280 — barometric altimeter (altitude + pressure + temperature)
- MPU6050 — IMU (acceleration + gyroscope, 6-axis), read directly via I2C registers (no library) for better compatibility with clone modules
- 3x AA-equivalent battery pack (~4.1V), soldered direct to the 5V pin

**Data logging — plan change:**
The original design used a microSD module for full 20Hz CSV logging. During pre-launch testing the SD module failed (confirmed via low-level diagnostics: card responds electrically but the module could not complete initialization) and could not be replaced in time given the remote launch location (Uspallata, Mendoza).

As a fallback, the flight computer was reprogrammed to log a **flight summary to the Arduino's internal EEPROM** instead of a full time-series CSV: apogee, max acceleration magnitude, max angular velocity, average temperature, and flight duration. This was validated on the bench before launch and worked correctly during the actual flight.

A future revision will fix the SD module (or replace it) to recover full 20Hz time-series data again.

**Recovery / ejection:**
Parachute deployment is **mechanical**, not electronically triggered. The D12-5 motor's built-in ejection charge (5-second delay after burnout) pushes the nose cone off by friction, releasing the parachute. The flight computer only logs data — it does not control ejection.

## 🖨️ Structure

| Component | Material |
|---|---|
| Body tube | Cardboard, 27 cm long, 31.5 mm internal / 33 mm external diameter (0.75 mm wall) |
| Nose cone (ojiva) | PVC, 13 cm long, 1 mm wall |
| Fins | PETG, 3D printed, trapezoidal, 39.8° sweep angle |

PETG was selected for the fins for its heat resistance near the motor mount / ejection charge area.

## 🪂 First Launch Results (Uspallata, Mendoza)

- ✅ Ignition and ascent: successful
- ✅ Ejection charge and nose cone separation: successful
- ⚠️ Parachute: burned during ejection
- ⚠️ Payload (electronics bay) detached from the shock cord during ejection and was launched out along with the nose cone, instead of staying tethered as designed — all hardware was still recovered intact, but this means the logged data may include forces from an uncontrolled post-separation event, not just the intended flight profile
- ✅ Flight computer: correctly logged apogee, acceleration, temperature, and duration to EEPROM through the whole flight (see data reliability caveat above)

**Lessons for next revision:**
- Fix or replace the SD module to recover full time-series telemetry (not just a flight summary)
- Redesign parachute packing/shielding to prevent heat damage during ejection
- Redesign the payload attachment to the shock cord so it cannot detach under the force of ejection
- Consider soldering sensor connections more robustly (I2C jumpers survived bench testing but this should be hardened further for repeated flights)

## 📁 Repository Structure

```
rocket-project/
├── simulation/        # OpenRocket files (.ork)
├── firmware/           # Arduino Nano code (main + EEPROM reader)
├── cad/                 # 3D models (STL files, fins)
├── docs/                # Photos, flight data, analysis
└── README.md
```

## 🎯 Goals

- [x] OpenRocket simulation complete
- [x] Motor selection finalized (D12-5, chosen over Quest D16-4 for higher apogee — D16-4 reserved for a future test flight)
- [x] Flight computer designed and built (with EEPROM fallback logging)
- [x] Fins printed and assembled
- [x] Full assembly
- [x] Launch — school showcase, San Bonifacio, October 2026 (launch site: Finca Cinco Cumbres, Mendoza)
- [x] Post-launch data analysis
- [ ] Contact Prof. Guerra-Garcia (MIT AeroAstro) with real flight data
- [ ] V2: active control (TVC or moving fins), full SD telemetry restored

## 🛠️ Tools & Software

- OpenRocket — flight simulation
- Arduino IDE — firmware
- Tinkercad — 3D modeling (fins)

## 👤 Author

**Rocco V. Degano**
Electronics technician student — Instituto Tecnológico San Bonifacio, Lomas de Zamora
Interested in aerospace propulsion and flight systems.
Part of a personal aerospace engineering portfolio.
