# Perpetual Calendar with Google Calendar Connection

[![English](https://img.shields.io/badge/lang-English-blue.svg)](README.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](README.md)

![Perpetual Calendar](docs/perpetual_calendar.jpg)

A physical LED perpetual calendar powered by ESP8266 that syncs with Google Calendar to display holidays, anniversaries, and to-dos using color-coded LEDs.

## Quick Install (Web Flasher)

**No software installation required!** Flash the firmware directly from your browser:

1. Connect your ESP8266 (D1 Mini) via USB
2. Visit the [Web Flasher](https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/)
3. Click "Install" and select your device
4. Done!

## Features

- 75 WS2811 addressable LEDs
- Google Calendar integration (holidays, anniversaries, to-dos)
- **Color-coded clock** displaying HH:MM:SS (6 LEDs)
- **Auto brightness** with night mode (can turn off LEDs completely)
- **Customizable colors** for weekdays, weekends, today, current month, holidays, anniversaries, and to-dos
- **Bilingual interface** (Spanish default, English available via toggle)
- WiFi configuration via web interface
- OTA (Over-The-Air) updates
- mDNS support (access via `http://perpetualcalendar.local`)

### Color Coded Clock

When enabled, 6 LEDs display the time using color-coded digits:

| Digit | Color |
|-------|-------|
| 0 | Dark gray |
| 1 | Red |
| 2 | Orange |
| 3 | Yellow |
| 4 | Green |
| 5 | Cyan |
| 6 | Blue |
| 7 | Purple |
| 8 | Magenta |
| 9 | Light gray |

### Night Mode

Configure auto-brightness to automatically dim or turn off LEDs at night:
- Day brightness: 5-255
- Night brightness: 0-255 (0 = completely off)

## Build from Source

### Using Arduino IDE
See the [Instructables guide](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)

### Using PlatformIO
```bash
# Standard version
pio run -e standard

# With color coded clock
pio run -e color_coded_clock

# Upload
pio run -e standard -t upload
```

## Documentation

- [Instructables Guide](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)
- [Google Calendar Setup](GOOGLE_CALENDAR_SETUP.md)
- [Manual](MANUAL.md)

## License

This project is licensed under [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).

[![CC BY-NC-SA 4.0](https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

Based on the original project from [Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/).
