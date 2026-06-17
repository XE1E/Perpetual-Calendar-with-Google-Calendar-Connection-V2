#ifndef AUTOBRIGHTNESS_H
#define AUTOBRIGHTNESS_H

// Auto-brightness settings (stored in EEPROM)
// EEPROM addresses: 489-495
#define EEPROM_AUTO_BRIGHTNESS_ENABLED 489
#define EEPROM_DAY_BRIGHTNESS 490
#define EEPROM_NIGHT_BRIGHTNESS 491
#define EEPROM_DAY_START_HOUR 492
#define EEPROM_NIGHT_START_HOUR 493
#define EEPROM_DAY_START_MINUTE 494
#define EEPROM_NIGHT_START_MINUTE 495

struct AutoBrightnessConfig {
    bool enabled;
    uint8_t dayBrightness;
    uint8_t nightBrightness;
    uint8_t dayStartHour;
    uint8_t dayStartMinute;
    uint8_t nightStartHour;
    uint8_t nightStartMinute;
} autoBrightness;

void loadAutoBrightnessConfig() {
    autoBrightness.enabled = EEPROM.read(EEPROM_AUTO_BRIGHTNESS_ENABLED);
    autoBrightness.dayBrightness = EEPROM.read(EEPROM_DAY_BRIGHTNESS);
    autoBrightness.nightBrightness = EEPROM.read(EEPROM_NIGHT_BRIGHTNESS);
    autoBrightness.dayStartHour = EEPROM.read(EEPROM_DAY_START_HOUR);
    autoBrightness.nightStartHour = EEPROM.read(EEPROM_NIGHT_START_HOUR);
    autoBrightness.dayStartMinute = EEPROM.read(EEPROM_DAY_START_MINUTE);
    autoBrightness.nightStartMinute = EEPROM.read(EEPROM_NIGHT_START_MINUTE);

    // Set defaults if not configured (0xFF = uninitialized EEPROM)
    if (autoBrightness.dayBrightness == 0 || autoBrightness.dayBrightness == 255) {
        autoBrightness.enabled = false;
        autoBrightness.dayBrightness = 150;
        autoBrightness.nightBrightness = 30;
        autoBrightness.dayStartHour = 7;
        autoBrightness.dayStartMinute = 0;
        autoBrightness.nightStartHour = 22;
        autoBrightness.nightStartMinute = 0;
    }
    // Validate minutes (fix corrupted EEPROM)
    if (autoBrightness.dayStartMinute > 59) autoBrightness.dayStartMinute = 0;
    if (autoBrightness.nightStartMinute > 59) autoBrightness.nightStartMinute = 0;
}

void saveAutoBrightnessConfig() {
    EEPROM.write(EEPROM_AUTO_BRIGHTNESS_ENABLED, autoBrightness.enabled ? 1 : 0);
    EEPROM.write(EEPROM_DAY_BRIGHTNESS, autoBrightness.dayBrightness);
    EEPROM.write(EEPROM_NIGHT_BRIGHTNESS, autoBrightness.nightBrightness);
    EEPROM.write(EEPROM_DAY_START_HOUR, autoBrightness.dayStartHour);
    EEPROM.write(EEPROM_NIGHT_START_HOUR, autoBrightness.nightStartHour);
    EEPROM.write(EEPROM_DAY_START_MINUTE, autoBrightness.dayStartMinute);
    EEPROM.write(EEPROM_NIGHT_START_MINUTE, autoBrightness.nightStartMinute);
    EEPROM.commit();
}

// Returns true if brightness was changed
bool updateAutoBrightness(uint8_t currentHour, uint8_t currentMinute) {
    if (!autoBrightness.enabled) {
        return false;
    }

    // Convert to total minutes for comparison
    uint16_t currentTime = currentHour * 60 + currentMinute;
    uint16_t dayStart = autoBrightness.dayStartHour * 60 + autoBrightness.dayStartMinute;
    uint16_t nightStart = autoBrightness.nightStartHour * 60 + autoBrightness.nightStartMinute;

    bool isDayTime;
    if (dayStart < nightStart) {
        isDayTime = (currentTime >= dayStart && currentTime < nightStart);
    } else {
        isDayTime = (currentTime >= dayStart || currentTime < nightStart);
    }

    uint8_t targetBrightness = isDayTime ? autoBrightness.dayBrightness : autoBrightness.nightBrightness;

    if (BRIGHTNESS != targetBrightness) {
        BRIGHTNESS = targetBrightness;
        FastLED.setBrightness(BRIGHTNESS);
        Serial.print("Auto-brightness: ");
        Serial.print(isDayTime ? "Day" : "Night");
        Serial.print(" mode, brightness = ");
        Serial.println(BRIGHTNESS);
        return true;
    }

    return false;
}

#endif
