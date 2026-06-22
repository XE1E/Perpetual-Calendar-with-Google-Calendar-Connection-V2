# Complete Guide: Google Calendar Setup

[![English](https://img.shields.io/badge/lang-English-blue.svg)](GOOGLE_CALENDAR_SETUP.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](GOOGLE_CALENDAR_SETUP.md)

## Table of Contents

0. [QUICK GUIDE (10 minutes)](#quick-guide-10-minutes) - **Start here**
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Integration Architecture](#integration-architecture)
4. [Part 1: Create the Calendars in Google](#part-1-create-the-calendars-in-google)
5. [Part 2: Create the Google Apps Scripts](#part-2-create-the-google-apps-scripts)
6. [Part 3: Publish the Scripts](#part-3-publish-the-scripts)
7. [Part 4: Configure the Device](#part-4-configure-the-device)
8. [Verification and Testing](#verification-and-testing)
9. [Troubleshooting](#troubleshooting)

---

## QUICK GUIDE (10 minutes)

> **For users who want to set everything up quickly.** If you run into problems, check the detailed sections below.

> 📷 **Note about images:** This guide includes screenshot markers (lines starting with 📷). There are no images yet; each marker indicates what to capture and the suggested file name inside `docs/images/gcal/`. To add an image, save it under that name and change the line to `![description](docs/images/gcal/name.png)`.

### What you need

- A Google account (Gmail)
- Your Perpetual Calendar connected to WiFi

### Step 1: Create 3 calendars in Google Calendar

1. Go to **https://calendar.google.com**
2. In the left panel, click the **"+" next to "Other calendars"**
3. Select **"Create new calendar"**
4. Create these 3 calendars:
   - `Holidays`
   - `Anniversaries`
   - `Todos`

📷 *Screenshot: Google Calendar "Create new calendar" dialog* — `docs/images/gcal/01-crear-calendario.png`

### Step 2: Get the IDs of your calendars

For **each** calendar you created:

1. Click the **3 dots** next to the calendar name
2. Select **"Settings and sharing"**
3. Scroll down to **"Integrate calendar"**
4. Copy the **"Calendar ID"** (something like `abc123@group.calendar.google.com`)

📷 *Screenshot: "Integrate calendar" section showing the Calendar ID* — `docs/images/gcal/02-id-calendario.png`

**Save the 3 IDs in a notepad.**

### Step 3: Create the first script (Holidays)

1. Go to **https://script.google.com**
2. Click **"New project"**
3. **Delete all** the code and paste this:

```javascript
function doGet() {
  var calendarId = 'PASTE_YOUR_HOLIDAYS_ID_HERE';
  
  var calendar = CalendarApp.getCalendarById(calendarId);
  var now = new Date();
  var start = new Date(now.getFullYear(), now.getMonth(), 1);
  var end = new Date(now.getFullYear(), now.getMonth() + 1, 0);
  var events = calendar.getEvents(start, end);
  
  var days = "";
  var added = {};
  for (var i = 0; i < events.length; i++) {
    var d = events[i].getStartTime().getDate();
    if (!added[d]) { days += d + "-"; added[d] = true; }
  }
  return ContentService.createTextOutput(days);
}
```

4. **Replace** `PASTE_YOUR_HOLIDAYS_ID_HERE` with the actual ID of your Holidays calendar
5. Press **Ctrl+S** to save
6. Name the project: `Holidays`

📷 *Screenshot: Apps Script editor with the code pasted* — `docs/images/gcal/03-editor-script.png`

### Step 4: Publish the script

1. Click **"Deploy"** (blue button at the top)
2. Select **"New deployment"**
3. Click the **gear** > **"Web app"**
4. Configure:
   - Execute as: **Me**
   - Who has access: **Anyone**

📷 *Screenshot: "New deployment" dialog with type "Web app" and permissions* — `docs/images/gcal/04-nueva-implementacion.png`

5. Click **"Deploy"**
6. Click **"Authorize access"**
7. Select your Google account
8. Click **"Advanced"** > **"Go to Holidays (unsafe)"** > **"Allow"**

📷 *Screenshot: "Google hasn't verified this app" warning screen with the "Advanced" link* — `docs/images/gcal/05-autorizar.png`

9. **COPY the "Deployment ID"** (starts with `AKfycb...`)

📷 *Screenshot: final dialog showing the "Deployment ID"* — `docs/images/gcal/06-id-implementacion.png`

**Save this ID. It is the first of 3.**

### Step 5: Repeat for Anniversaries and Todos

Repeat steps 3 and 4 two more times:

**For Anniversaries:**
1. New project at script.google.com
2. Paste the same code
3. Change the ID to your Anniversaries calendar ID
4. Deploy and copy the deployment ID

**For Todos:**
1. New project at script.google.com
2. Paste the same code
3. Change the ID to your Todos calendar ID
4. Deploy and copy the deployment ID

**Now you have 3 deployment IDs.**

### Step 6: Configure the Perpetual Calendar

1. Open your browser
2. Go to **http://perpetualcalendar.local/appsscript.html**
   (or use your device's IP: `http://192.168.X.X/appsscript.html`)
3. Paste the IDs:
   - **Holidays:** ID of the Holidays script
   - **Anniversaries:** ID of the Anniversaries script
   - **To-dos:** ID of the Todos script
4. Click **"Save"**

📷 *Screenshot: the device's `appsscript.html` page with the 3 IDs pasted* — `docs/images/gcal/07-config-dispositivo.png`

### Step 7: Test that it works

1. Go to Google Calendar
2. Add an event for **today** in the "Todos" calendar
3. Wait 1-2 minutes
4. Today's LED should change to **orange**

### Done!

Your Perpetual Calendar now shows:
- **Purple:** Days with holidays
- **Cyan:** Days with anniversaries
- **Orange:** Days with to-dos

> **Problems?** Check the [Troubleshooting](#troubleshooting) section below.

---

## Introduction

The Perpetual Calendar connects to Google Calendar through **Google Apps Script**, a free Google service that lets you build simple web applications. The integration works like this:

```
ESP8266 --> Google Apps Script --> Google Calendar API --> Your Calendars
```

The device queries 3 different calendars:
- **Holidays:** Public holidays, vacations
- **Anniversaries:** Birthdays, special dates
- **To-dos:** Reminders, day-to-day events

Each event type is shown in a different color on the physical calendar.

---

## Prerequisites

Before you begin, make sure you have:

- [ ] A Google account (Gmail)
- [ ] Access to Google Calendar (calendar.google.com)
- [ ] Access to Google Apps Script (script.google.com)
- [ ] The Perpetual Calendar connected to your WiFi network
- [ ] The device's IP, or access via `http://perpetualcalendar.local`

---

## Integration Architecture

### How it works

```
                    INTERNET
                       |
    +------------------+------------------+
    |                  |                  |
    v                  v                  v
+--------+        +--------+        +--------+
|Script 1|        |Script 2|        |Script 3|
|Holidays|        |Anniv.  |        | Todos  |
+---+----+        +---+----+        +---+----+
    |                 |                  |
    v                 v                  v
+--------+        +--------+        +--------+
|Calendar|        |Calendar|        |Calendar|
|Holidays|        |Birthd. |        |Todos   |
+--------+        +--------+        +--------+
```

### Data flow

1. The ESP8266 makes an HTTPS request to each Google Apps Script
2. The Script queries the corresponding calendar via the Google Calendar API
3. The Script returns a list of days with events: `"5-12-25-"`
4. The ESP8266 parses the days and lights the corresponding LEDs

### Update frequency

| Calendar | Frequency | Reason |
|----------|-----------|--------|
| Todos | Every minute | To-dos can change frequently |
| Holidays | Every hour | Holidays are relatively static |
| Anniversaries | Every hour | Anniversaries are static |

---

## Part 1: Create the Calendars in Google

### Step 1.1: Access Google Calendar

1. Open your browser
2. Go to: **https://calendar.google.com**
3. Sign in with your Google account

### Step 1.2: Create the Holidays Calendar

1. In the left panel, find **"Other calendars"**
2. Click the **+** (Add other calendars)
3. Select **"Create new calendar"**
4. Fill in the details:
   - **Name:** `Holidays` (or whatever you prefer)
   - **Description:** Public holidays and vacations
   - **Time zone:** Your time zone
5. Click **"Create calendar"**

### Step 1.3: Create the Anniversaries Calendar

Repeat the previous process with:
- **Name:** `Anniversaries`
- **Description:** Birthdays and special dates

### Step 1.4: Create the Todos Calendar

Repeat the previous process with:
- **Name:** `Todos`
- **Description:** Reminders and daily events

> **Tip:** You can also use existing calendars or your primary calendar. You only need the calendar ID.

### Step 1.5: Get the Calendar IDs

For each calendar created:

1. Click the **three dots** next to the calendar name
2. Select **"Settings and sharing"**
3. Scroll down to **"Integrate calendar"**
4. Copy the **"Calendar ID"**

The ID has this format:
```
abc123xyz@group.calendar.google.com
```

Or for the primary calendar:
```
your-email@gmail.com
```

**Save these 3 IDs, you will need them later.**

### Step 1.6: Add test events

To verify everything works, add a few events:

1. In the **Holidays** calendar, add an event for today or tomorrow
2. In the **Anniversaries** calendar, add a birthday this month
3. In the **Todos** calendar, add a task for today

---

## Part 2: Create the Google Apps Scripts

### Step 2.1: Access Google Apps Script

1. Open your browser
2. Go to: **https://script.google.com**
3. Sign in with the same Google account

### Step 2.2: Create the Script for Holidays

1. Click **"New project"**
2. The editor will open with a `Code.gs` file
3. **Delete all the content** and paste the following code:

```javascript
function doGet() {
  // IMPORTANT: Replace with your Holidays calendar ID
  var calendarId = 'YOUR_HOLIDAYS_CALENDAR_ID@group.calendar.google.com';
  
  try {
    var calendar = CalendarApp.getCalendarById(calendarId);
    
    if (!calendar) {
      return ContentService.createTextOutput('Error: Calendar not found');
    }
    
    // Get the first and last day of the current month
    var now = new Date();
    var firstDay = new Date(now.getFullYear(), now.getMonth(), 1);
    var lastDay = new Date(now.getFullYear(), now.getMonth() + 1, 0);
    
    // Get all events of the month
    var events = calendar.getEvents(firstDay, lastDay);
    
    // Build a string with the days that have events
    var days = "";
    var addedDays = {}; // To avoid duplicates
    
    for (var i = 0; i < events.length; i++) {
      var eventDate = events[i].getStartTime();
      var day = eventDate.getDate();
      
      // Avoid duplicates of the same day
      if (!addedDays[day]) {
        days += day + "-";
        addedDays[day] = true;
      }
    }
    
    return ContentService.createTextOutput(days);
    
  } catch (e) {
    return ContentService.createTextOutput('Error: ' + e.toString());
  }
}
```

4. **IMPORTANT:** Replace `YOUR_HOLIDAYS_CALENDAR_ID@group.calendar.google.com` with the actual ID of your Holidays calendar (the one you copied in Step 1.5)

5. Click the disk icon or press `Ctrl+S` to save

6. Change the project name:
   - Click **"Untitled project"** in the top left
   - Type: `Holidays Calendar`
   - Press Enter

### Step 2.3: Create the Script for Anniversaries

1. Go to **https://script.google.com** again
2. Click **"New project"**
3. Paste the same code from Step 2.2
4. Change the `calendarId` to your Anniversaries calendar ID
5. Save and rename the project to: `Anniversaries Calendar`

### Step 2.4: Create the Script for Todos

1. Go to **https://script.google.com** again
2. Click **"New project"**
3. Paste the same code from Step 2.2
4. Change the `calendarId` to your Todos calendar ID
5. Save and rename the project to: `Todos Calendar`

---

## Part 3: Publish the Scripts

Each script must be published as a "web app" so the ESP8266 can access it.

### Step 3.1: Publish the Holidays Script

1. Open the **"Holidays Calendar"** project at script.google.com
2. Click **"Deploy"** (blue button in the top right)
3. Select **"New deployment"**
4. Click the gear next to "Select type"
5. Select **"Web app"**
6. Configure:
   - **Description:** `Holidays v1`
   - **Execute as:** `Me (your-email@gmail.com)`
   - **Who has access:** `Anyone`
7. Click **"Deploy"**

### Step 3.2: Authorize the Script

The first time you publish, Google will ask for authorization:

1. Click **"Authorize access"**
2. Select your Google account
3. A warning will appear: "Google hasn't verified this app"
4. Click **"Advanced"** or **"Advanced settings"**
5. Click **"Go to Holidays Calendar (unsafe)"**
6. Review the permissions and click **"Allow"**

### Step 3.3: Copy the Deployment ID

After authorizing, you will see a screen with:
- **Deployment ID:** A long code like `AKfycbx...`
- **Web app URL:** `https://script.google.com/macros/s/AKfycbx.../exec`

**IMPORTANT:** Copy the **Deployment ID** (NOT the full URL, just the ID).

The ID has this format:
```
AKfycbxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

**Save this ID. You will need it to configure the device.**

### Step 3.4: Publish the Other Scripts

Repeat steps 3.1 to 3.3 for:
- **Anniversaries Calendar**
- **Todos Calendar**

In the end you will have **3 deployment IDs**, one for each script.

### Step 3.5: Verify the Scripts

You can test each script in your browser:

1. Copy the **web app URL** (the one that ends in `/exec`)
2. Paste it in a new browser tab
3. It should show something like: `5-12-25-` (the days with events)
4. If there are no events this month, it will show an empty line

---

## Part 4: Configure the Device

### Step 4.1: Access the Web Interface

1. Make sure the Perpetual Calendar is powered on and connected to WiFi
2. Open your browser
3. Go to: `http://perpetualcalendar.local` or use the device's IP

### Step 4.2: Go to the Apps Script Page

1. On the main page, find the **"Apps Script"** or **"Script Settings"** link
2. Or go directly to: `http://perpetualcalendar.local/appsscript.html`

### Step 4.3: Enter the IDs

You will see 3 text fields:

```
+------------------------------------------+
|    Apps Script Deployment IDs            |
+------------------------------------------+
| Holidays:     [_________________________]|
| Anniversaries:[_________________________]|
| To-dos:       [_________________________]|
|                                          |
|              [ Save ]                    |
+------------------------------------------+
```

1. In **Holidays**, paste the deployment ID of the Holidays script
2. In **Anniversaries**, paste the deployment ID of the Anniversaries script
3. In **To-dos**, paste the deployment ID of the Todos script
4. Click **Save**

### Step 4.4: Restart the Device (Optional)

For the changes to take effect immediately:

1. Disconnect the ESP8266 power
2. Wait 5 seconds
3. Reconnect

Or wait about 1 minute for it to update automatically.

---

## Verification and Testing

### Verify that it works

1. Add an event to one of your Google calendars for **today**
2. Wait 1-2 minutes
3. The LED for today should change color:
   - **Orange** if it is a task (To-do)
   - **Purple** if it is a holiday (Holiday)
   - **Cyan** if it is an anniversary (Anniversary)

### Verify via the Serial Monitor

If you have the ESP8266 connected over USB:

1. Open the Arduino IDE
2. Go to `Tools > Serial Monitor`
3. Set it to **115200 baud**
4. Look for messages like:
   ```
   Calendar Data---> 5-12-25-
   ```
   This indicates the script is responding correctly.

### Force a manual update

1. Go to `http://perpetualcalendar.local/led.html`
2. Click **"Refresh Calendar"**
3. The events will update immediately

---

## Troubleshooting

### Problem: "Error: Calendar not found"

**Cause:** The calendar ID is incorrect.

**Solution:**
1. Go to Google Calendar
2. Calendar settings > Integrate calendar
3. Copy the correct ID (ends in `@group.calendar.google.com` or `@gmail.com`)
4. Update the script and republish

### Problem: Events do not appear

**Checks:**
1. Is the event in the correct calendar?
2. Does the event have a date within the current month?
3. Is the deployment ID correct?
4. Did you test the script directly in the browser?

**Solution:**
1. Open the script URL in the browser
2. If it shows an error, review the script code
3. If it shows data, verify the ID on the device

### Problem: Error 401 or "Unauthorized"

**Cause:** The script does not have the correct permissions.

**Solution:**
1. Go to the project at script.google.com
2. Deploy > Manage deployments
3. Delete the current deployment
4. Create a new deployment
5. Make sure to select "Anyone" for permissions
6. Copy the new ID and update it on the device

### Problem: HTTPS connection error

**Cause:** The ESP8266 cannot connect to Google.

**Checks:**
1. Does the device have an Internet connection?
2. Did the rainbow animation stop? (indicates NTP responded)
3. Can you access the device's web interface?

**Solution:**
1. Verify the WiFi connection
2. Restart the device
3. Check that the router has Internet access

### Problem: "Could not connect to server"

**Cause:** Google is rejecting the connection.

**Solution:**
1. Google scripts use HTTPS — verify that the device's time is correct
2. Go to `/ntp.html` and verify the NTP configuration
3. The ESP8266 SSL certificate expires; update the firmware if it is very old

### Problem: Script works in the browser but not on the ESP8266

**Check:**
1. Make sure to copy only the deployment ID, not the full URL
2. The ID must start with `AKfycb`
3. It must not include `https://` or `/exec`

**Correct format:**
```
AKfycbx1234567890abcdefghijklmnopqrstuvwxyz123456
```

**Incorrect format:**
```
https://script.google.com/macros/s/AKfycbx.../exec  <-- Do NOT use the URL
```

---

## Script Code (Complete Reference)

This is the complete code with explanatory comments:

```javascript
/**
 * Google Apps Script for the Perpetual Calendar
 * This script returns the days of the current month that have events
 * 
 * Response format: "day1-day2-day3-"
 * Example: "5-12-25-" = events on days 5, 12, and 25
 */

function doGet() {
  // ============================================================
  // CONFIGURATION - Modify this line with your calendar ID
  // ============================================================
  var calendarId = 'YOUR_CALENDAR_ID@group.calendar.google.com';
  
  // To use your primary calendar, use your email:
  // var calendarId = 'your-email@gmail.com';
  
  try {
    // Get a reference to the calendar
    var calendar = CalendarApp.getCalendarById(calendarId);
    
    // Verify that the calendar exists
    if (!calendar) {
      return ContentService.createTextOutput('Error: Calendar not found. Check the ID.');
    }
    
    // Calculate the date range for the current month
    var now = new Date();
    var year = now.getFullYear();
    var month = now.getMonth(); // 0-11
    
    var firstDay = new Date(year, month, 1);                    // First day of the month
    var lastDay = new Date(year, month + 1, 0, 23, 59, 59);    // Last day of the month
    
    // Get all events in the range
    var events = calendar.getEvents(firstDay, lastDay);
    
    // Build the response
    var days = "";
    var addedDays = {}; // Object to track days already added
    
    for (var i = 0; i < events.length; i++) {
      var event = events[i];
      var startDate = event.getStartTime();
      var day = startDate.getDate();
      
      // Only add each day once (avoid duplicates)
      if (!addedDays[day]) {
        days += day + "-";
        addedDays[day] = true;
      }
      
      // For multi-day events, add all the days of the event
      var endDate = event.getEndTime();
      if (endDate.getDate() !== day) {
        // It is a multi-day event
        var currentDate = new Date(startDate);
        while (currentDate < endDate && currentDate.getMonth() === month) {
          var currentDay = currentDate.getDate();
          if (!addedDays[currentDay]) {
            days += currentDay + "-";
            addedDays[currentDay] = true;
          }
          currentDate.setDate(currentDate.getDate() + 1);
        }
      }
    }
    
    // Return the days as plain text
    return ContentService.createTextOutput(days);
    
  } catch (e) {
    // On error, return a descriptive message
    return ContentService.createTextOutput('Error: ' + e.toString());
  }
}

/**
 * Test function - Run from the editor to verify
 */
function testScript() {
  var result = doGet();
  Logger.log(result.getContent());
}
```

---

## How the Code Works on the ESP8266

### HTTPS Connection

The ESP8266 uses the `HTTPSRedirect` library to connect to Google:

```cpp
// File: Perpetual-Calendar-With-Google-Calendar-Connection.ino

const char* host = "script.google.com";
const int httpsPort = 443;

void getCalendar(String GScriptId) {
    // Build URL: /macros/s/[ID]/exec
    String url = String("/macros/s/") + GScriptId + "/exec";
    
    // Make an HTTPS GET request
    client->GET(url, host);
    
    // Get the response (e.g.: "5-12-25-")
    calendarData = client->getResponseBody();
}
```

### Response Processing

The days are extracted from the string and stored in arrays:

```cpp
void initDatesArray(int (&Dates)[20], String calendarString) {
    // Example: "5-12-25-" -> Dates = [5, 12, 25, 0, 0, ...]
    
    while(calendarString.indexOf("-") != -1) {
        // Extract the number before the hyphen
        Dates[index] = calendarString.substring(0, 
                         calendarString.indexOf("-")).toInt();
        // Advance to the next number
        calendarString = calendarString.substring(
                         calendarString.indexOf("-") + 1);
    }
}
```

### LED Display

Events are shown with specific colors:

```cpp
void EventsDisplay() {
    for (int i = 1; i <= 31; i++) {
        // Holidays: Purple
        for (int j = 0; j < 20; j++) {
            if (Holidays[j] == i) 
                leds[Days[i-1]] = holidays_color;  // Purple
        }
        // Anniversaries: Cyan
        for (int j = 0; j < 20; j++) {
            if (Anniversaries[j] == i) 
                leds[Days[i-1]] = anniversaries_color;  // Cyan
        }
        // To-dos: Orange (highest visual priority)
        for (int j = 0; j < 20; j++) {
            if (Todos[j] == i) 
                leds[Days[i-1]] = todos_color;  // Orange
        }
    }
}
```

### Color Hierarchy

If a day has multiple event types, the last one overrides:

```
1. Base color (green weekday / red weekend)
2. Current day (blue)
3. Holidays (purple)
4. Anniversaries (cyan)
5. To-dos (orange) <-- Highest priority
```

---

## Additional Tips

### Use existing calendars

You do not need to create new calendars. You can use:
- Your primary Gmail calendar
- Your country's holiday calendars (Google offers these for free)
- Shared calendars

### Multiple events on the same day

The script only reports the **day**, not the number of events. If you have 5 meetings on the 15th, the script returns `15-` only once.

### Recurring events

Recurring events (annual birthdays, weekly meetings) work automatically. The script detects all occurrences within the month.

### Privacy

- The scripts only read the **days** with events, not the titles or details
- The ESP8266 does not store your calendar data
- All traffic is HTTPS (encrypted)

### Google limits

- Google Apps Script has free execution limits
- For personal use, you should not reach these limits
- Approximate limit: 90 minutes of execution per day

---

*Guide created for the Perpetual Calendar with Google Calendar Connection v2.2 project*
