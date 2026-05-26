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
- Auto brightness based on time of day
- WiFi configuration via web interface
- OTA (Over-The-Air) updates
- Optional color-coded clock display

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
