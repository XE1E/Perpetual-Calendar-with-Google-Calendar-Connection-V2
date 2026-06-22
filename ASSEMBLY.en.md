# Assembly & Wiring Guide

[![English](https://img.shields.io/badge/lang-English-blue.svg)](ASSEMBLY.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](ASSEMBLY.md)

Steps to assemble the Perpetual Calendar hardware, from cutting the LED strips to the final assembly.

> Before you start, check the [Bill of Materials](BILL_OF_MATERIALS.en.md). For the full guide with all images, see the [Complete Guide](https://xe1e.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/GUIA_COMPLETA.html) (Spanish).

---

## Step 1 — Cut the LED Strips

Cut the WS2811/WS2812B LED strip (60 LEDs/m) into **6 pieces**:

- **5 pieces** of 13 LEDs each
- **1 piece** of 10 LEDs

Total: 75 LEDs.

![Cut LED strips](docs/images/tiras_led_cortadas.jpg)

> ⚠️ **Data direction:** Watch the arrows printed on the strip. The pieces connect in a **zigzag**, alternating direction on each row.

---

## Step 2 — Mount the LEDs on the Holder

1. Place the LED strips on the 3D-printed holder, between the ridges.
2. Press firmly with a plastic stick for good adhesion.
3. The ridges bring the LEDs to **~12 mm** apart (effective density similar to 83 LEDs/m).
4. Respect the arrow direction: each row runs opposite to the previous one (zigzag).

![LEDs mounted on the holder](docs/images/leds_montados.jpg)

### LED logical order (mapping)

The firmware expects the 75 LEDs wired in this zigzag order:

```
Row 1 (LEDs 68-74): left  → right
Row 2 (LEDs 61-49): right → left
Row 3 (LEDs 36-48): left  → right
Row 4 (LEDs 35-23): right → left
Row 5 (LEDs 10-22): left  → right
Row 6 (LEDs  9-0 ): right → left

LEDs 62-67: HH:MM:SS clock (Color Coded Clock version only)
```

> The full mapping detail is in the [Technical Manual](MANUAL.en.md#led-mapping).

---

## Step 3 — Wiring & Connections

![Wiring schematic](docs/images/esquema_electronico.png)

### Main connections

| ESP8266 | WS2811 LED strip | Notes |
|---------|------------------|-------|
| GPIO2 (D4) | DIN (Data In) | Through the **3 kΩ resistor** to VCC |
| GND | GND | Common GND with the supply |
| — | VCC (+5V) | From the **external** 5V/3A supply |
| VIN / 5V | — | ESP power from the 5V rail (or regulator) |

### Wiring rules

```
        5V / 3A supply
         │        │
       (+5V)    (GND)
         │        │
         ├────────┼──────────► LED strip (VCC / GND)
         │        │
         ▼        ▼
       ESP8266   ESP8266
       (5V/VIN)  (GND)  ◄── COMMON GND required

   GPIO2 (D4) ──[ 3 kΩ ]── VCC
       │
       └──────────► LED strip DIN
```

> 🔴 **3 kΩ resistor (required):** Goes between GPIO2 and VCC. Without it, the ESP8266 may fail to boot with the strip connected.

> 🔴 **Don't power the LEDs from the ESP.** 75 LEDs can draw several amps. Use the external 5V/3A supply and tie **all GNDs together** (supply + LEDs + ESP).

---

## Step 4 — Prepare the Numbers Paper

Print the template [`docs/plantilla_numeros.pdf`](docs/plantilla_numeros.pdf) on A4 (white) paper. It contains the day numbers and month names. Cut along the outline.

![Numbers paper](docs/images/papel_numeros.jpg)

---

## Step 5 — Final Assembly

The unit is built up in layers, back to front:

1. **LED holder** — base with the strips mounted and soldered.
2. **ESP + regulator module** — hot-glued into the slot.
3. **3D grid** — over the holder; separates each LED's light.
4. **Printed paper** — numbers facing up.
5. **Smoked acrylic** — diffuses the light and protects.
6. **Front frame** — holds everything in place.
7. **Rear stand** — with foam adhesive tape.

![Calendar running](docs/images/calendario_funcionando.jpg)

> 💡 **Clock grid:** If you'll use the *Color Coded Clock* firmware, use the grid version **with** dividers for the 6 clock LEDs (62–67).

---

## Next Step

With the hardware built, flash the firmware and configure the device:

- 📥 [Firmware installation (Web Flasher)](README.en.md#quick-install-web-flasher)
- ⚙️ [Initial & network setup](MANUAL.en.md#installation-and-initial-setup)
- 📅 [Google Calendar setup](GOOGLE_CALENDAR_SETUP.en.md)

---

## Common Hardware Problems

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| LEDs don't light | Data/power/resistor | Check GPIO2, the 5V, the 3 kΩ resistor and the arrow direction. Use "Test LEDs" in `/led.html` |
| ESP won't boot or keeps restarting | Missing 3 kΩ resistor or weak supply | Add the resistor; use a 5V/**3A** supply minimum; tie the GNDs together |
| Erratic colors or brightness | GND not common / long data wire | Tie all GNDs; shorten the data wire to DIN |
| Some LEDs don't respond | Bad cut or reversed zigzag solder | Check the arrow direction on each row |

---

*Assembly guide for the Perpetual Calendar with Google Calendar Connection v2.2 project*
