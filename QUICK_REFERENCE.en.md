# Quick Reference

[![English](https://img.shields.io/badge/lang-English-blue.svg)](QUICK_REFERENCE.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](QUICK_REFERENCE.md)

At-a-glance cheat sheet. For the detail, see the [Technical Manual](MANUAL.en.md).

## Wiring (Pinout)

| ESP8266 | LED strip | Notes |
|---------|-----------|-------|
| GPIO2 (D4) | DIN | Through a **3 kΩ** resistor to VCC |
| GND | GND | Common GND with the supply |
| 5V (external supply) | VCC | 5V / **3A** supply min. |

## Access and defaults

| Parameter | Value |
|-----------|-------|
| Setup network (AP) | SSID `Calendario` · pass `12345678` |
| Configuration portal | `http://192.168.4.1` |
| Normal access (mDNS) | `http://perpetualcalendar.local` |
| Web port | 80 |
| OTA port | 8266 |
| NTP port (client) | 2390 |
| Serial | 115200 baud |
| Suggested NTP server | `time.cloudflare.com` |

## Web pages

| URL | Function |
|-----|----------|
| `/` | Main menu |
| `/config.html` | WiFi (primary + backup) |
| `/ntp.html` | NTP and time zone |
| `/appsscript.html` | Google Apps Script IDs |
| `/time.html` | Manual time |
| `/info.html` | System info, restart, clear WiFi |
| `/led.html` | Brightness, Test LEDs, Refresh |
| `/autobrightness.html` | Day/night mode |
| `/colors.html` | Colors + gamma correction |
| `/ota.html` | OTA info |

## LED mapping (zigzag)

```
Row 1: LEDs 68-74  (left→right)
Row 2: LEDs 61-49  (right→left)
Row 3: LEDs 36-48  (left→right)
Row 4: LEDs 35-23  (right→left)
Row 5: LEDs 10-22  (left→right)
Row 6: LEDs  9-0   (right→left)
Clock: LEDs 62-67  (HH:MM:SS, Color Coded Clock only)
```

## Calendar colors (defaults)

| Day type | Color | HSV |
|----------|-------|-----|
| Weekday | Green | (96, 255, 192) |
| Current day | Blue | (160, 255, 192) |
| Weekend | Red | (0, 255, 128) |
| Month | Gray | (0, 0, 192) |
| Holidays | Purple | (192, 255, 255) |
| Anniversaries | Cyan | (128, 255, 255) |
| To-dos | Orange | (32, 255, 192) |

**Priority (last one overrides):** base → current day → holidays → anniversaries → to-dos.

## Clock colors by digit

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| Black | Red | Orange | Yellow | Green | Cyan | Blue | Purple | Pink | White |

## API endpoints (GET)

| Endpoint | Action |
|----------|--------|
| `/admin/values` · `/admin/connectionstate` | Network config · WiFi state |
| `/admin/infovalues` | System info |
| `/admin/ntpvalues` · `/admin/appsscript` | NTP · script IDs |
| `/admin/timevalues` | Current time |
| `/admin/ledvalues` · `/admin/setbrightness` · `/admin/savebrightness` | Brightness |
| `/admin/autobrightvalues` · `/admin/saveautobrightness` | Auto-brightness |
| `/admin/colorvalues` · `/admin/previewcolors` · `/admin/savecolors` · `/admin/resetcolors` | Colors |
| `/admin/gammavalues` · `/admin/previewgamma` · `/admin/savegamma` | Gamma |
| `/admin/otavalues` | OTA info |
| `/admin/testleds` · `/admin/refreshcalendar` | Test LEDs · refresh |
| `/admin/restart` · `/admin/clearwifi` | Restart · clear WiFi |

## Key EEPROM addresses

| Address | Content |
|---------|---------|
| 0-2 | "CFG" signature |
| 64-127 | SSID + pass (primary) |
| 128-159 | NTP server |
| 200-487 | Script IDs (Holidays / Anniversaries / Todos) |
| 488 | Manual brightness |
| 489-495 | Auto-brightness (day/night, hour:min) |
| 520-583 | SSID2 + pass2 (backup) |
| 584-607 | Custom colors (HSV) + gamma |

> Total EEPROM: 640 bytes. Full map in the [Manual](MANUAL.en.md#eeprom-storage).

## Build (PlatformIO)

```bash
pio run -e standard            # standard
pio run -e color_coded_clock   # with clock
pio run -e standard -t upload  # upload
```

---

*Quick reference for the Perpetual Calendar with Google Calendar Connection v2.2 project*
