# Troubleshooting

[![English](https://img.shields.io/badge/lang-English-blue.svg)](TROUBLESHOOTING.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](TROUBLESHOOTING.md)

Single reference of common problems and their solutions, organized by category. For the technical detail of each subsystem, see the [Technical Manual](MANUAL.en.md) and the [Google Calendar Guide](GOOGLE_CALENDAR_SETUP.en.md).

## Index

- [Hardware and LEDs](#hardware-and-leds)
- [Boot and restarts](#boot-and-restarts)
- [WiFi and network](#wifi-and-network)
- [Time and NTP](#time-and-ntp)
- [Google Calendar](#google-calendar)
- [Color clock](#color-clock)
- [mDNS and OTA](#mdns-and-ota)
- [Brightness and colors](#brightness-and-colors)

---

## Hardware and LEDs

### LEDs don't light up
1. Check the data pin connection (**GPIO2 / D4**) to the strip's **DIN**.
2. Check the **5V** power and that the supply delivers enough current (3A min).
3. Check the **3 kΩ resistor** between GPIO2 and VCC.
4. Verify the **arrow** direction on the strip (data flowing the right way).
5. Use the **"Test LEDs"** button on `/led.html`.

### Some LEDs don't respond
- Check the zigzag solder joints and that each row runs in the alternating direction. See [Assembly](ASSEMBLY.en.md#step-2--mount-the-leds-on-the-holder).

### Erratic colors or brightness
- Ensure a **common GND** between supply, strip and ESP.
- Shorten the data wire to DIN.

---

## Boot and restarts

### The ESP won't boot or restarts at power-on
**Cause:** missing 3 kΩ resistor or insufficient supply.
1. Add the **3 kΩ resistor** between GPIO2 and VCC.
2. Use a **5V / 3A** supply minimum.
3. Tie all **GNDs** together.

### Constant restart every ~30 seconds
**Cause:** the watchdog detects the loop is blocked.
1. The Google connection may be taking too long — check Internet.
2. Verify WiFi stability.
3. Review the logs over Serial (115200 baud).

---

## WiFi and network

### No WiFi connection
1. Verify SSID and password in `/config.html`.
2. Verify the router is on and move the device closer.
3. Configure a **backup network**.
4. Restart the ESP8266.

### WiFi disconnects frequently
1. Configure a backup network in `/config.html`.
2. Move the device closer to the router.
3. Check for interference on the WiFi channel.
4. Review the logs over Serial to see the disconnection pattern.

> The device retries every 30 s and, after 3 failures, waits 5 minutes.

---

## Time and NTP

### Continuous rainbow animation
**Cause:** no response from the NTP server.
1. Verify the Internet connection.
2. Try another server (`time.cloudflare.com`, `time.google.com`).
3. Verify that **UDP port 123** is not blocked.

### Incorrect time
1. Verify the **time zone** in `/ntp.html`.
2. Toggle **daylight saving time** according to your region.
3. Use manual time configuration temporarily (`/time.html`).

---

## Google Calendar

### Events don't show
1. Verify the scripts' **deployment IDs**.
2. Make sure the scripts are published as **"Anyone"**.
3. The event must have a date within the **current month**.
4. Test the script in the browser: `https://script.google.com/macros/s/YOUR_ID/exec`.

### "Error: Calendar not found"
- The calendar ID is wrong. Copy it again from *Calendar settings → Integrate calendar* and republish the script.

### Error 401 / "Unauthorized"
- Delete the current deployment and create a new one with **"Anyone"** access. Copy the new ID to the device.

### The script works in the browser but not on the ESP8266
- Paste **only the deployment ID** (starts with `AKfycb`), **not** the full URL or `/exec`.

More detail in the [Google Calendar Guide](GOOGLE_CALENDAR_SETUP.en.md#troubleshooting).

---

## Color clock

### The clock doesn't update or flickers
1. Verify you installed the **"Color Coded Clock"** version (not Standard).
2. Verify NTP works (correct time).
3. The clock LEDs (**62-67**) must not be damaged.
4. Restart the device.

---

## mDNS and OTA

### `http://perpetualcalendar.local` doesn't work
1. Verify the device is connected to WiFi.
2. On **Windows:** install Bonjour (comes with iTunes).
3. On **Android:** use the direct IP or a Bonjour Browser app.
4. Wait 30 s after restart and try another browser.

### OTA doesn't appear in Arduino IDE
1. The IDE and the ESP8266 must be on the **same network**.
2. Temporarily disable the firewall.
3. Restart the Arduino IDE.
4. Verify that mDNS works (try web access).

> The first upload must be over USB; subsequent ones can be OTA (port 8266).

---

## Brightness and colors

### Brightness is not saved
1. Use the **"Save"** button, not just the slider.
2. Restart the device to verify.

### LEDs look dim at night
- That's **night mode**: check `/autobrightness.html`. Night brightness of `0` turns the LEDs off completely.

---

*Troubleshooting guide for the Perpetual Calendar with Google Calendar Connection v2.2 project*
