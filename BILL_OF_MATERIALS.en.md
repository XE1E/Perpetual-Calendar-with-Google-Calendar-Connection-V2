# Bill of Materials (BOM)

[![English](https://img.shields.io/badge/lang-English-blue.svg)](BILL_OF_MATERIALS.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](BILL_OF_MATERIALS.md)

Complete list of components, materials, 3D-printed parts and tools needed to build the Perpetual Calendar.

> For the step-by-step build steps, see the [Assembly Guide](ASSEMBLY.en.md). For the full illustrated guide, see the [Complete Guide](https://xe1e.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/GUIA_COMPLETA.html) (Spanish).

---

## Electronic Components

| Component | Qty | Specification / Notes |
|-----------|:---:|-----------------------|
| ESP8266 microcontroller | 1 | Wemos D1 Mini (recommended), NodeMCU or other compatible board |
| WS2811 / WS2812B LED strip | 75 LEDs | 60 LEDs/m, addressable, 5V. Cut into strips (see Assembly) |
| 5V power supply | 1 | **3 A** minimum (75 LEDs × 60 mA max ≈ 4.5 A at full white; 3 A is enough for normal use) |
| DC barrel jack (female) | 1 | 5.5 mm / 2.5 mm, for the power supply input |
| 5V → 3.3V regulator module | 1 | Only if your ESP8266 board lacks a regulator (the D1 Mini already has one) |
| 3 kΩ resistor | 1 | **Required** — between GPIO2 (data pin) and VCC. Prevents boot failures |
| Hookup wire | Several | For data and power connections |
| Solder | — | To join the LED strips and wires |

> ⚠️ **3 kΩ resistor:** Required so the ESP8266 boots reliably with the LED strip connected to the data pin (GPIO2 / D4). Without it the module may fail to start.

> ⚡ **Power:** Do NOT power the 75 LEDs from the ESP8266. Use an external 5V/3A supply and tie a **common GND** between the supply, the LED strip and the ESP8266.

---

## Build Materials

| Material | Specification |
|----------|---------------|
| 3D-printed parts | Front frame, LED holder, separator grid and rear stand/cover (see below) |
| Smoked acrylic | 191 mm × 96 mm × 2 mm thick (front diffuser) |
| Printed white paper | Template with day numbers and month names ([`plantilla_numeros.pdf`](docs/plantilla_numeros.pdf), print on A4) |
| Hot glue | To fix the ESP, regulator and components |
| Foam adhesive tape | 1 mm thick, for the rear stand |

---

## 3D-Printed Parts

All parts ship in a single STL file:

- 📥 [`docs/stl/3D_Printed_Perpetual_Calendar.stl`](docs/stl/3D_Printed_Perpetual_Calendar.stl) (~7.3 MB) — ready to print
- ✏️ [View / edit on Tinkercad](https://www.tinkercad.com/things/52D6lIWCsAP-3d-printed-perpetual-calendar) — to modify the design

| Part | Purpose |
|------|---------|
| LED holder | Base with ridges that position the LED strips ~12 mm apart |
| Separator grid | Isolates the light of each LED. **Two versions** exist: with and without dividers for the 6 clock LEDs |
| Front frame | Holds the assembly together and frames the acrylic diffuser |
| Rear stand / cover | Support to stand the calendar up |

> 💡 **Clock version:** If you plan to install the *Color Coded Clock* firmware, use the grid **with** dividers for LEDs 62–67. For the *Standard* version, either grid works.

---

## Tools

- Soldering iron and solder
- Hot glue gun
- Sandpaper (to round the acrylic corners)
- 3D printer (or access to a printing service)
- USB data cable (not charge-only) to program the ESP8266

---

## Quick Cost Overview *(rough guide)*

Prices vary by region and supplier. This is only an approximate reference of the main parts:

| Category | Items |
|----------|-------|
| Electronics | ESP8266, LED strip, 5V/3A supply, DC jack, resistor, wires |
| 3D printing | ~4 parts (PLA filament recommended) |
| Diffuser | 2 mm smoked acrylic |
| Consumables | Paper, hot glue, foam tape, solder |

---

*Bill of materials for the Perpetual Calendar with Google Calendar Connection v2.2 project*
