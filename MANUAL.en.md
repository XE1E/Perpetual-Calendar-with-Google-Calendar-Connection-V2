# Complete Manual: Perpetual Calendar with Google Calendar Connection

[![English](https://img.shields.io/badge/lang-English-blue.svg)](MANUAL.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](MANUAL.md)

## Table of Contents

1. [Overview](#overview)
2. [Hardware Requirements](#hardware-requirements)
3. [System Architecture](#system-architecture)
4. [File Structure](#file-structure)
5. [Detailed Operation](#detailed-operation)
6. [LED Mapping](#led-mapping)
7. [Color System](#color-system)
8. [Color Coded Clock](#color-coded-clock)
9. [NTP Synchronization](#ntp-synchronization)
10. [Google Calendar Integration](#google-calendar-integration) - See also: [Complete Google Calendar Guide](GOOGLE_CALENDAR_SETUP.en.md)
11. [Web Configuration Interface](#web-configuration-interface)
12. [Advanced Features V2](#advanced-features-v2)
    - [mDNS](#mdns)
    - [OTA Updates](#ota-updates)
    - [Brightness Control](#brightness-control)
    - [Auto-Brightness by Hour](#auto-brightness-by-hour-night-mode)
    - [Customizable Colors](#customizable-colors)
    - [WiFi Multi-Network](#wifi-multi-network)
13. [EEPROM Storage](#eeprom-storage)
14. [Installation and Initial Setup](#installation-and-initial-setup)
15. [Troubleshooting](#troubleshooting)

---

## Overview

This project implements a **physical perpetual calendar** using an ESP8266 microcontroller and a strip of 75 WS2811 LEDs.
The calendar:

- Visually displays every day of the current month
- Automatically synchronizes time via NTP
- Connects to Google Calendar to display events
- Visually distinguishes weekends, holidays, anniversaries, and to-dos
- Is configured through a built-in web interface
- **Optional clock mode** with colors to show the time (HH:MM:SS)
- **Easy access** via mDNS (http://perpetualcalendar.local)
- **OTA updates** without needing a USB cable
- **Adjustable brightness** manually or automatically by time of day
- **Customizable colors** via the web interface
- **Multi-network WiFi support** with a backup network

### Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                        PERPETUAL CALENDAR                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│   ┌─────────────┐      ┌──────────────┐      ┌──────────────┐   │
│   │   ESP8266   │◀────▶│  NTP         │      │   Google     │   │
│   │             │      │  Server      │      │   Calendar   │   │
│   └──────┬──────┘      └──────────────┘      └──────┬───────┘   │
│          │                                          │           │
│          │              ┌──────────────┐            │           │
│          │              │ Google Apps  │◀───────────┘           │
│          │              │   Script     │                        │
│          │              └──────┬───────┘                        │
│          │                     │                                │
│          ▼                     ▼                                │
│   ┌─────────────┐      ┌──────────────┐                        │
│   │  75 LEDs    │      │  Web         │                        │
│   │   WS2811    │      │  Interface   │                        │
│   └─────────────┘      └──────────────┘                        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## Hardware Requirements

| Component | Specification |
|-----------|---------------|
| Microcontroller | ESP8266 (NodeMCU, Wemos D1 Mini, etc.) |
| LED strip | WS2811/WS2812B, 75 LEDs |
| LED data pin | GPIO 2 (D4 on NodeMCU) |
| Power | 5V, minimum 3A (75 LEDs × 60mA max) |
| Connection | WiFi 2.4GHz |

### Wiring Diagram

```
ESP8266          WS2811 LED Strip
────────         ────────────────
GPIO 2  ───────▶ DIN (Data In)
GND     ───────▶ GND
5V      ───────▶ VCC (from external supply)
```

> **Note:** It is recommended to power the LED strip from an external 5V supply and connect the common GND with the ESP8266.

---

## File Structure

```
Perpetual-Calendar-with-Google-Calendar-Connection/
│
├── Perpetual-Calendar-With-Google-Calendar-Connection.ino  # Main file
├── global.h                    # Global variables and EEPROM configuration
├── NTP.h                       # NTP time synchronization
├── HTTPSRedirect.h/.cpp        # HTTPS client for Google Apps Script
├── ColorCodedClock.h           # Color clock mode (optional)
├── DebugMacros.h               # Debugging macros
│
├── WiFiManager.h               # Multi-network WiFi management and auto-reconnect
├── AutoBrightness.h            # Automatic brightness control by hour
│
├── Page_Admin.h                # Main administration page
├── Page_NetworkConfiguration.h # WiFi network configuration (primary + backup)
├── Page_NTPSettings.h          # NTP server configuration
├── Page_AppsScriptSettings.h   # Google Apps Script configuration
├── Page_SetTime.h              # Manual time configuration
├── Page_Information.h          # Device information
├── Page_LEDSettings.h          # LED brightness control
├── Page_AutoBrightness.h       # Auto-brightness configuration
├── Page_ColorSettings.h        # Color customization
├── Page_OTA.h                  # OTA update information
├── Page_Style.css.h            # CSS styles for the web interface
├── Page_Script.js.h            # JavaScript for the web interface
│
├── README.md                   # Basic documentation
└── MANUAL.md                   # This manual
```

### Description of Each File

| File | Purpose |
|------|---------|
| `*.ino` | Contains `setup()`, `loop()`, LED display functions, and Google connection |
| `global.h` | Defines the configuration structure, EEPROM functions, web server |
| `NTP.h` | Gets UTC time, computes time zone, daylight saving time, day of week |
| `HTTPSRedirect.*` | Handles HTTPS connections with redirect for Google Apps Script |
| `WiFiManager.h` | Management of multiple WiFi networks with ESP8266WiFiMulti |
| `AutoBrightness.h` | Day/night automatic brightness logic |
| `ColorCodedClock.h` | Time display using colors on LEDs |
| `Page_*.h` | Embedded HTML pages for the web configuration interface |

---

## Detailed Operation

### Initialization Flow (setup)

```
1. Start Serial (115200 baud)
2. Read configuration from EEPROM
3. Attempt WiFi connection
   ├── Success: Station mode (WiFi client)
   └── Failure: AP mode (access point "Calendario")
4. Start web server on port 80
5. Configure FastLED (75 LEDs, WS2811, GPIO 2 pin)
6. Connect to Google Apps Script
7. Start 1-second timer (ISRsecondTick)
```

### Main Loop (loop)

```
loop() {
    1. Handle web requests (server.handleClient)

    2. Update NTP time when due
       └── First time: after 5 seconds
       └── Then: every X minutes (configurable)

    3. Check connection state:
       ├── No WiFi → Show warning LED (red)
       ├── No NTP  → Show "pride" animation (rainbow)
       └── Time OK → Show calendar

    4. If time is valid:
       ├── Every MINUTE: Update "Todos" calendar
       └── Every HOUR: Update "Holidays" and "Anniversaries"
           └── Redraw the entire display
}
```

### Day-of-Week Calculation Algorithm

The project uses the **modified Zeller's algorithm** to compute the day of the week:

```cpp
// File: NTP.h, lines 201-208
int DayOfTheWeek(int year, int month, int day) {
    if (month <= 2) {
        --year;
        month += 12;
    }
    return ((((day + (((month + 1) * 26) / 10) + year + (year / 4)
            + (6 * (year / 100)) + (year / 400)) % 7) + 6) % 7);
}
// Result: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
```

### Leap Year Algorithm

```cpp
// File: NTP.h, lines 167-172
bool isLeapYear(int yr) {
    if (yr % 4 == 0 && yr % 100 != 0 || yr % 400 == 0)
        return true;
    else
        return false;
}
```

A year is a leap year if:
- It is divisible by 4 **AND** not divisible by 100
- **OR** it is divisible by 400

---

## LED Mapping

The calendar uses 75 LEDs arranged in a zigzag pattern:

### Physical Layout

```
LED 68-74:  [Sun][Mon][Tue][Wed][Thu][Fri][Sat]  ← Row 1 (days 1-7)
LED 61-49:  [   ][   ][   ][   ][   ][   ][   ]  ← Row 2 (reverse zigzag)
LED 36-48:  [   ][   ][   ][   ][   ][   ][   ]  ← Row 3
LED 35-23:  [   ][   ][   ][   ][   ][   ][   ]  ← Row 4 (reverse zigzag)
LED 10-22:  [   ][   ][   ][   ][   ][   ][   ]  ← Row 5
LED 9-0:    [   ][   ][   ][   ][   ][   ][   ]  ← Row 6 (reverse zigzag)

LEDs 62-67: Used by the clock (Color Coded Clock version) or part of the calendar (Standard version)
```

### Mapping Array

```cpp
// File: *.ino, lines 33-38
byte calendar_leds[] = {
    68, 69, 70, 71, 72, 73, 74,                    // Row 1: left → right
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,  // Row 2: right → left
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,  // Row 3: left → right
    35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,  // Row 4: right → left
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,  // Row 5: left → right
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0                   // Row 6: right → left
};
```

### Month LEDs

```cpp
// File: *.ino, line 39
byte calendar_months[] = {19, 20, 21, 22, 7, 6, 5, 4, 3, 2, 1, 0};
// Index:                 Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
```

---

## Color System

### Color Palette (HSV)

| Index | Color | HSV | Use |
|-------|-------|-----|-----|
| 0 | Black/Off | (0, 0, 0) | Clock digit 0 |
| 1 | Red | (0, 255, 192) | Clock digit 1 |
| 2 | Orange | (32, 255, 192) | Clock digit 2 |
| 3 | Yellow | (64, 255, 192) | Clock digit 3 |
| 4 | Green | (96, 255, 192) | Clock digit 4 |
| 5 | Cyan | (128, 255, 192) | Clock digit 5 |
| 6 | Blue | (160, 255, 192) | Clock digit 6 |
| 7 | Purple | (192, 255, 192) | Clock digit 7 |
| 8 | Pink | (224, 255, 192) | Clock digit 8 |
| 9 | White/Gray | (0, 0, 192) | Clock digit 9 |

### Calendar Color Assignment

```cpp
// File: *.ino, lines 41-47
CHSV weekday_color      = CHSV(96, 255, 192);   // Green - weekdays
CHSV actualday_color    = CHSV(160, 255, 192);  // Blue - current day
CHSV weekend_color      = CHSV(0, 255, 128);    // Red - weekends
CHSV month_color        = CHSV(0, 0, 192);      // Gray - month indicator
CHSV holidays_color     = CHSV(192, 255, 255);  // Purple - holidays
CHSV anniversaries_color = CHSV(128, 255, 255); // Cyan - anniversaries
CHSV todos_color        = CHSV(32, 255, 192);   // Orange - to-dos
```

### Color Priority Hierarchy

When a day has multiple events, the final color follows this priority (the last one overrides):

```
1. Base color (weekday/weekend)
2. Current day
3. Holidays
4. Anniversaries
5. To-dos (Todos) ← Highest visual priority
```

### Weekend Colors

The weekend color (`weekend_color`) applies to **both Saturdays and Sundays**. Both days use the same color (red by default) to keep visual consistency.

---

## Color Coded Clock

### Activation

Clock mode comes in a **separate firmware version**. When installing via the Web Flasher, you can choose:

- **Standard** - Basic calendar without clock
- **Color Coded Clock** - Includes an HH:MM:SS clock on LEDs 62-67

If you compile manually with PlatformIO:
```bash
pio run -e standard          # Without clock
pio run -e color_coded_clock # With clock
```

### Clock Layout (6 LEDs)

```
LED 62   LED 63   LED 64   LED 65   LED 66   LED 67
 ┌──┐    ┌──┐     ┌──┐     ┌──┐     ┌──┐     ┌──┐
 │H1│    │H2│     │M1│     │M2│     │S1│     │S2│
 └──┘    └──┘     └──┘     └──┘     └──┘     └──┘
  ↓       ↓        ↓        ↓        ↓        ↓
 Tens    Units    Tens     Units    Tens     Units
 Hour    Hour    Minute   Minute   Second   Second
```

### Color Coding by Digit

| Digit | Color | HSV |
|-------|-------|-----|
| 0 | Black/Off | (0, 0, 0) |
| 1 | Red | (0, 255, 192) |
| 2 | Orange | (32, 255, 192) |
| 3 | Yellow | (64, 255, 192) |
| 4 | Green | (96, 255, 192) |
| 5 | Cyan | (128, 255, 192) |
| 6 | Blue | (160, 255, 192) |
| 7 | Purple | (192, 255, 192) |
| 8 | Pink | (224, 255, 192) |
| 9 | White/Gray | (0, 0, 192) |

### Example: Time 14:35:27

```
LED 62 = 1 → Red       (hour tens)
LED 63 = 4 → Green     (hour units)
LED 64 = 3 → Yellow    (minute tens)
LED 65 = 5 → Cyan      (minute units)
LED 66 = 2 → Orange    (second tens)
LED 67 = 7 → Purple    (second units)
```

### LED Separation

```
LEDs 62-67: HH:MM:SS clock (6 digits)
LEDs 68-74: First row of the calendar
```

> **Note:** The clock updates automatically every second and is redrawn after each calendar update to avoid being overwritten.

---

## NTP Synchronization

### Synchronization Process

```
1. Create UDP client on port 2390
2. Resolve the NTP server name to an IP
3. Send NTP packet (48 bytes)
4. Wait for response (100ms timeout)
5. Extract Unix timestamp from bytes 40-43
6. Subtract 70 years (1900-1970 difference)
7. Store in the global UnixTimestamp
```

### NTP Packet Structure

```cpp
// File: NTP.h, lines 39-47
packetBuffer[0] = 0b11100011;  // LI=3, Version=4, Mode=3 (client)
packetBuffer[1] = 0;           // Stratum
packetBuffer[2] = 6;           // Polling interval
packetBuffer[3] = 0xEC;        // Clock precision
// Bytes 12-15: Reference identifier
```

### Time Zone Adjustment

```cpp
// File: NTP.h, lines 157-165
unsigned long adjustTimeZone(unsigned long _timeStamp, int _timeZone,
        bool _isDayLightSavingSaving) {
    _timeStamp += _timeZone * 360;  // Adjust time zone
    if (_isDayLightSavingSaving && summerTime(_timeStamp))
        _timeStamp += 3600;  // Add 1 hour if daylight saving time
    return _timeStamp;
}
```

> **Note:** The time zone is stored in units of 6 minutes (360 seconds). For example, UTC+1 = timeZone 10.

### Daylight Saving Time Calculation (Central Europe)

The system automatically computes daylight saving time for Central Europe:
- **Start:** Last Sunday of March at 3:00
- **End:** Last Sunday of October at 3:00

---

## Google Calendar Integration

### Connection Architecture

```
ESP8266 ──HTTPS──▶ script.google.com ──redirect──▶ script.googleusercontent.com
                          │
                          ▼
                   Google Apps Script
                          │
                          ▼
                   Google Calendar API
```

### Google Apps Script Configuration

**3 separate scripts** are required, one for each event type:

1. **HolidaysScriptID** - Holidays calendar
2. **AnniversariesScriptID** - Anniversaries calendar
3. **TodosScriptID** - To-dos calendar

### Script Response Format

The Google Apps Script must return the days of the month with events in the format:

```
day1-day2-day3-day4-
```

Example for events on days 5, 12, and 25:
```
5-12-25-
```

### Calendar Data Processing

```cpp
// File: *.ino, lines 153-164
void initDatesArray(int (&Dates)[20], String calendarString) {
    // Initialize array to zeros
    for (index = 0; index < 20; index++) {
        Dates[index] = 0;
    }
    // Parse hyphen-separated string
    while(calendarString.indexOf("-") != -1) {
        Dates[index] = calendarString.substring(0, calendarString.indexOf("-")).toInt();
        index++;
        calendarString = calendarString.substring(calendarString.indexOf("-") + 1);
    }
}
```

### Update Frequency

| Calendar | Frequency | Reason |
|----------|-----------|--------|
| Todos | Every minute | To-dos can change frequently |
| Holidays | Every hour | Holidays are static |
| Anniversaries | Every hour | Anniversaries are static |

---

## Web Configuration Interface

### Accessing the Interface

- **Station mode (connected to WiFi):** `http://[assigned_IP]/`
- **AP mode (no WiFi configured):** `http://192.168.4.1/`
  - SSID: `Calendario`
  - Password: `12345678`

### Available Pages

| URL | Description |
|-----|-------------|
| `/` | Main administration page |
| `/config.html` | WiFi network configuration (primary + backup) |
| `/ntp.html` | NTP server configuration |
| `/appsscript.html` | Google Apps Script IDs |
| `/time.html` | Manual time configuration |
| `/info.html` | System information (network, resources, actions) |
| `/led.html` | LED brightness control |
| `/autobrightness.html` | Auto-brightness configuration |
| `/colors.html` | Color customization |
| `/ota.html` | OTA update information |

### API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/admin/values` | GET | Network configuration values |
| `/admin/connectionstate` | GET | WiFi connection state |
| `/admin/infovalues` | GET | System information |
| `/admin/ntpvalues` | GET | NTP configuration |
| `/admin/appsscript` | GET | Script IDs |
| `/admin/timevalues` | GET | Current time |
| `/admin/ledvalues` | GET | Current brightness value |
| `/admin/setbrightness` | GET | Set brightness (temporary) |
| `/admin/savebrightness` | GET | Save brightness to EEPROM |
| `/admin/autobrightvalues` | GET | Auto-brightness configuration |
| `/admin/saveautobrightness` | GET | Save auto-brightness |
| `/admin/colorvalues` | GET | Current colors |
| `/admin/previewcolors` | GET | Color preview |
| `/admin/savecolors` | GET | Save colors |
| `/admin/resetcolors` | GET | Restore default colors |
| `/admin/gammavalues` | GET | Gamma correction configuration |
| `/admin/previewgamma` | GET | Gamma correction preview |
| `/admin/savegamma` | GET | Save gamma correction |
| `/admin/otavalues` | GET | OTA information |
| `/admin/testleds` | GET | Test all LEDs |
| `/admin/refreshcalendar` | GET | Force calendar update |
| `/admin/restart` | GET | Restart the device |
| `/admin/clearwifi` | GET | Clear WiFi credentials and restart |

---

## Advanced Features V2

### System Information Page

**Page:** `/info.html`

Shows complete device information and allows maintenance actions.

**Sections:**

| Section | Content |
|---------|---------|
| **Network** | Connected SSID, IP, Mask, Gateway, MAC |
| **Resources** | Free RAM, Fragmentation, Flash, CPU, Uptime |
| **Actions** | Restart and clear WiFi buttons |

**Available actions:**

- **Restart ESP:** Restarts the device (with confirmation)
- **Clear WiFi:** Erases the saved WiFi credentials and restarts in AP mode (with confirmation)

> **Note:** When clearing WiFi, the device will again create the `Calendario` network for reconfiguration.

---

### mDNS

The device is accessible via a local domain name, without needing to know the IP.

**Access:** `http://perpetualcalendar.local`

```cpp
// The name is derived from config.DeviceName
// "Perpetual Calendar" → "perpetualcalendar.local"
```

**How it works:**
- Activates automatically when connecting to WiFi
- Restarts automatically after WiFi reconnection
- Compatible with Windows, macOS, Linux, and iOS
- Android requires an additional app (Bonjour Browser)

---

### OTA Updates

Allows updating the firmware without a USB connection, directly from the Arduino IDE.

**Port:** 8266

**LED indicators during the update:**
| State | LED Color |
|-------|-----------|
| Starting | Blue |
| Progress | Purple (progress bar) |
| Completed | Green |
| Error | Red |

**Steps to update:**
1. Open the Arduino IDE
2. Go to `Tools > Port`
3. Select the network port: `perpetualcalendar at [IP]`
4. Upload the sketch normally

> **Note:** The first upload must be done over USB. Subsequent ones can be OTA.

---

### Brightness Control

**Page:** `/led.html`

**Features:**
- Brightness slider: 5-255
- Real-time change (preview)
- Permanent save to EEPROM
- "Test LEDs" button to verify operation
- "Refresh" button to force a calendar update

**EEPROM:** Address 488

---

### Auto-Brightness by Hour (Night Mode)

**Page:** `/autobrightness.html`

Automatically adjusts the brightness based on the time of day. Allows configuring a night mode with reduced brightness or completely off LEDs.

**Configuration:**

| Parameter | Description | Range |
|-----------|-------------|-------|
| Day brightness | Brightness during daytime hours | 5-255 |
| Night brightness | Brightness during nighttime hours | 0-255 (0 = OFF) |
| Day start hour | Hour when day mode starts | 0-23 |
| Night start hour | Hour when night mode starts | 0-23 |

**Example configuration:**
```
Day brightness: 150
Night brightness: 15 (or 0 to turn off completely)
Day start: 07:00
Night start: 22:00
```

> **Tip:** Set night brightness to **0** to completely turn off the LEDs during the night.

**EEPROM:** Addresses 489-495

---

### Customizable Colors

**Page:** `/colors.html`

Allows changing the colors of each day type through visual selectors.

**Configurable colors:**

| Element | Default color | Description |
|---------|---------------|-------------|
| Weekdays | Green | Monday to Friday |
| Weekends | Red | Saturdays and Sundays |
| Current day | Blue | Today |
| Current month | Gray | LED indicator of the current month |
| Holidays | Purple | Events from the Holidays calendar |
| Anniversaries | Cyan | Events from the Anniversaries calendar |
| To-dos | Orange | Events from the Todos calendar |

**Functions:**
- **Preview:** See changes in real time without saving
- **Save:** Save colors permanently
- **Reset:** Restore original colors

**Gamma Correction:**

Allows adjusting the color profile and temperature to improve LED appearance:

| Option | Values |
|--------|--------|
| Color profile | No correction, TypicalLEDStrip (recommended), TypicalSMD5050 |
| Temperature | No adjustment, DirectSunlight (recommended), Tungsten100W, ClearBlueSky |

**EEPROM:** Addresses 584-607

**Storage format:** HSV (Hue, Saturation, Value) - 3 bytes per color, plus 2 bytes for gamma correction

---

### WiFi Multi-Network

**Page:** `/config.html`

Allows configuring two WiFi networks. The device automatically connects to the best available signal.

**Configuration:**

```
┌─────────────────────────────────────┐
│ Primary Network (required)          │
│ SSID:     [___________________]     │
│ Password: [___________________]     │
├─────────────────────────────────────┤
│ Backup Network (optional)           │
│ SSID 2:     [___________________]   │
│ Password 2: [___________________]   │
└─────────────────────────────────────┘
```

**Behavior:**
1. On startup, it tries to connect to either of the two networks
2. Automatically selects the one with the best signal
3. If the connection is lost, it tries to reconnect every 30 seconds
4. Alternates between both networks looking for a connection
5. After 3 failed attempts, it waits 5 minutes

**Connection state:**
- The page shows which network it is currently connected to
- Example: `CONNECTED to MyWiFiNetwork`

**EEPROM:** Addresses 520-583 (SSID2 + Password2)

---

### Automatic WiFi Reconnection

The system constantly monitors the WiFi connection and automatically reconnects if it is lost.

**Parameters:**

| Parameter | Value | Description |
|-----------|-------|-------------|
| Check interval | 30 sec | Time between checks |
| Maximum retries | 3 | Attempts before a long pause |
| Pause after failures | 5 min | Wait time after 3 failures |
| Connection timeout | 10 sec | Maximum time per attempt |

**Automatic actions after reconnection:**
- Restart mDNS service
- Continue NTP update
- Resume Google Calendar queries

---

## EEPROM Storage

### EEPROM Memory Map (640 bytes)

| Address | Size | Content |
|---------|------|---------|
| 0-2 | 3 bytes | Signature "CFG" (validation) |
| 16 | 1 byte | DHCP enabled |
| 17 | 1 byte | Daylight saving time |
| 18-21 | 4 bytes | NTP update interval |
| 22-25 | 4 bytes | Time zone |
| 32-35 | 4 bytes | IP address |
| 36-39 | 4 bytes | Subnet mask |
| 40-43 | 4 bytes | Gateway |
| 64-95 | 32 bytes | WiFi SSID (primary network) |
| 96-127 | 32 bytes | WiFi password (primary network) |
| 128-159 | 32 bytes | NTP server |
| 160-191 | 32 bytes | Device name |
| 192-199 | 8 bytes | First day of the week |
| 200-295 | 96 bytes | HolidaysScriptID |
| 296-391 | 96 bytes | AnniversariesScriptID |
| 392-487 | 96 bytes | TodosScriptID |
| **488** | **1 byte** | **Manual brightness** |
| **489** | **1 byte** | **Auto-brightness enabled** |
| **490** | **1 byte** | **Day brightness** |
| **491** | **1 byte** | **Night brightness** |
| **492** | **1 byte** | **Day start hour** |
| **493** | **1 byte** | **Night start hour** |
| **494** | **1 byte** | **Day start minute** |
| **495** | **1 byte** | **Night start minute** |
| **520-551** | **32 bytes** | **WiFi SSID (backup network)** |
| **552-583** | **32 bytes** | **WiFi password (backup network)** |
| **584** | **1 byte** | **Custom colors enabled** |
| **585-587** | **3 bytes** | **Weekday color (HSV)** |
| **588-590** | **3 bytes** | **Weekend color (HSV)** |
| **591-593** | **3 bytes** | **Current day color (HSV)** |
| **594-596** | **3 bytes** | **Holidays color (HSV)** |
| **597-599** | **3 bytes** | **Anniversaries color (HSV)** |
| **600-602** | **3 bytes** | **To-dos color (HSV)** |
| **603-605** | **3 bytes** | **Month color (HSV)** |
| **606** | **1 byte** | **Color correction mode** |
| **607** | **1 byte** | **Color temperature mode** |

### EEPROM Functions

```cpp
// Write configuration
void WriteConfig();

// Read configuration (returns true if it exists)
boolean ReadConfig();

// Helper functions
void EEPROMWritelong(int address, long value);
long EEPROMReadlong(long address);
void WriteStringToEEPROM(int beginaddress, String string);
String ReadStringFromEEPROM(int beginaddress);      // Max 32 chars
String ReadLongStringFromEEPROM(int beginaddress);  // Max 96 chars
```

---

## Installation and Initial Setup

### Option A: Web Flasher (Recommended)

The easiest way to install the firmware is using the **Web Flasher** directly from your browser. It does not require installing any software.

**Requirements:**
- Chrome, Edge, or Opera browser (Firefox and Safari do not support Web Serial)
- USB cable to connect the ESP8266
- No other program using the serial port (close the Arduino IDE if it is open)

**Steps:**

1. Connect the ESP8266 (D1 Mini or compatible) to the computer via USB
2. Open the Web Flasher: **https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/**
3. Select the version to install:
   - **Standard** - Basic calendar
   - **Color Coded Clock** - Includes a color clock on LEDs 62-67 (HH:MM:SS)
4. Click "Install"
5. Select the serial port when the browser requests it
6. Wait ~30 seconds for the installation to finish
7. Continue with **Step 3: Initial Setup** (below)

> **Note:** If the "Install" button does not appear or shows an error, verify that the browser is compatible and that the USB cable supports data (not just charging).

---

### Option B: Arduino IDE (Manual)

If you prefer to compile the code yourself or need to make modifications:

#### Step 1: Prepare the Arduino Environment

1. Install the Arduino IDE (1.8.x or 2.x)
2. Add ESP8266 support:
   - Go to `File > Preferences`
   - In "Additional boards manager URLs" add:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
3. Install the ESP8266 board from the boards manager
4. Install the required libraries from the library manager:
   - **FastLED** - WS2811/WS2812 LED control

**Libraries included in the ESP8266 Core (do not require installation):**
- ESP8266WiFi
- ESP8266WebServer
- ESP8266mDNS
- ESP8266WiFiMulti
- ArduinoOTA
- EEPROM
- Ticker

#### Step 2: Upload the Firmware

1. Select the board: `Tools > Board > NodeMCU 1.0`
2. Select the correct COM port
3. Upload the sketch

---

### Step 3: Initial Setup (Both options)

1. The ESP8266 will create a WiFi network: `Calendario`
2. Connect with the password: `12345678`
3. Open a browser at: `http://192.168.4.1`
4. Configure in order:
   - **Primary WiFi network** (SSID and password)
   - **Backup WiFi network** (optional, recommended)
   - **NTP server** (e.g., `time.cloudflare.com`)
   - **Time zone**
   - **Google Apps Script IDs**
5. Optional advanced configuration:
   - **LED brightness** (`/led.html`)
   - **Auto-brightness** (`/autobrightness.html`)
   - **Custom colors** (`/colors.html`)

### Step 4: Normal Access

Once configured, access it via:
- **mDNS:** `http://perpetualcalendar.local`
- **Direct IP:** `http://[assigned_IP]/`

> **Tip:** The IP is shown in the Serial Monitor (115200 baud) at startup.

### Step 5: Create Google Apps Scripts

For each calendar (Holidays, Anniversaries, Todos), create a script that:

1. Accesses the corresponding calendar
2. Gets the current month's events
3. Returns the days as a string: `"day1-day2-day3-"`

Basic script example:
```javascript
function doGet() {
  var calendar = CalendarApp.getCalendarById('calendario@gmail.com');
  var now = new Date();
  var start = new Date(now.getFullYear(), now.getMonth(), 1);
  var end = new Date(now.getFullYear(), now.getMonth() + 1, 0);
  var events = calendar.getEvents(start, end);

  var days = "";
  for (var i = 0; i < events.length; i++) {
    days += events[i].getStartTime().getDate() + "-";
  }
  return ContentService.createTextOutput(days);
}
```

---

## Troubleshooting

### No WiFi Connection

**Symptoms:** The calendar does not show events, the time does not synchronize

**Solutions:**
1. Verify SSID and password in `/config.html`
2. Verify that the router is on
3. Move the device closer to the router
4. Configure a backup network
5. Restart the ESP8266

### Continuous Rainbow Animation

**Problem:** No NTP response is received

**Solutions:**
1. Verify the Internet connection
2. Change the NTP server (try `time.cloudflare.com` or `time.google.com`)
3. Verify that UDP port 123 is not blocked

### Calendar Events Not Shown

**Problem:** Google Calendar events do not appear

**Solutions:**
1. Verify the script IDs
2. Verify that the scripts are published as a "web app"
3. Verify the script permissions ("Anyone, even anonymous")
4. Test the script directly in the browser

### Incorrect Time

**Problem:** The displayed time does not match

**Solutions:**
1. Verify the configured time zone
2. Verify the daylight saving time setting
3. Use manual time configuration temporarily

### Constant Restarts

**Problem:** The ESP8266 restarts every 30 seconds

**Cause:** The watchdog detects that the loop is blocked

**Solutions:**
1. Check the connection to Google (it may be taking too long)
2. Verify WiFi stability
3. Review logs over Serial (115200 baud)

### mDNS Not Working

**Problem:** Cannot access via `http://perpetualcalendar.local`

**Solutions:**
1. Verify that the device is connected to WiFi
2. On Windows: install Bonjour (comes with iTunes)
3. On Android: use the IP directly or install the Bonjour Browser app
4. Wait 30 seconds after restart
5. Try another browser

### OTA Does Not Appear in Arduino IDE

**Problem:** The network port does not appear in Tools > Port

**Solutions:**
1. Verify that the Arduino IDE and ESP8266 are on the same network
2. Temporarily disable the firewall
3. Restart the Arduino IDE
4. Verify that mDNS works (try web access)

### Brightness Is Not Saved

**Problem:** The brightness returns to the previous value after restart

**Solutions:**
1. Use the "Save" button instead of just moving the slider
2. Verify that the EEPROM is not full
3. Restart the device to verify

### WiFi Disconnects Frequently

**Problem:** The device loses the WiFi connection constantly

**Solutions:**
1. Configure a backup network in `/config.html`
2. Move the device closer to the router
3. Verify that there is no interference on the WiFi channel
4. Review logs over Serial to see the disconnection pattern

### Clock Does Not Update

**Problem:** The clock (Color Coded Clock) does not change or flashes incorrectly

**Solutions:**
1. Verify that the "Color Coded Clock" firmware version was installed (not the Standard version)
2. Verify that NTP is working (correct time)
3. The clock LEDs (62-67) must not be damaged
4. Restart the device

---

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| Microcontroller | ESP8266 |
| CPU frequency | 80/160 MHz |
| Flash memory | 4 MB |
| Emulated EEPROM | 640 bytes |
| Supported LEDs | 75 (WS2811/WS2812) |
| Calendar LEDs | 68 (days) + 12 (months) |
| Clock LEDs | 6 (HH:MM:SS) - Color Coded Clock version only |
| No-NTP indicator | Rainbow animation (pride) |
| Default brightness | 150/255 |
| Minimum brightness | 5/255 (0 = off in night mode) |
| Web port | 80 |
| OTA port | 8266 |
| NTP port | 2390 (client) |
| Watchdog timeout | 30 seconds |
| WiFi reconnection | 30 seconds |
| Serial speed | 115200 baud |
| mDNS | perpetualcalendar.local |
| Supported WiFi networks | 2 (primary + backup) |

---

## References

- [Web Flasher - Browser-based installation](https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/)
- [Instructables - Perpetual Calendar With Google Calendar Connection](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)
- [FastLED Library](https://github.com/FastLED/FastLED)
- [FastLED HSV Colors](https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [ESP8266 WiFiMulti](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-class.html)
- [ArduinoOTA](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)
- [mDNS/Bonjour](https://arduino-esp8266.readthedocs.io/en/latest/esp8266mdns.html)
- [Google Apps Script](https://developers.google.com/apps-script)

---

## Version History

### Version 2.2
- Added gamma correction and color temperature on the colors page
- Added a customizable color for the month indicator
- Added minutes in the auto-brightness configuration (hour:minute)
- Split the firmware into two versions: Standard and Color Coded Clock
- Improved documentation and build guide with images
- Web Flasher support with version selection

### Version 2.1
- Added a system information page with resources (RAM, Flash, CPU, Uptime)
- Added a button to restart the device from the web
- Added a button to clear WiFi credentials from the web
- Improved persistence of custom colors
- Improved the color configuration interface (bigger buttons)
- Added fixed brightness (120) for the rainbow animation
- Added translations for new elements

### Version 2.0
- Added mDNS support (access via .local)
- Added OTA updates
- Added brightness control via web
- Added auto-brightness by time of day
- Added color customization via web
- Added multi-network WiFi support (primary + backup)
- Added automatic WiFi reconnection
- Fixed the color clock display
- Optimized EEPROM memory usage (640 bytes)

### Version 1.0
- Initial version
- Calendar display with LEDs
- Google Calendar integration
- Configuration via web interface
- NTP synchronization

---

*Manual generated for the Perpetual Calendar with Google Calendar Connection v2.2 project*
