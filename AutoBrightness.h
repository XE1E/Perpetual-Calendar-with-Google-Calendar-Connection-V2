#ifndef AUTOBRIGHTNESS_H
#define AUTOBRIGHTNESS_H

// Auto-brightness settings (stored in EEPROM)
// EEPROM addresses: 489-493
#define EEPROM_AUTO_BRIGHTNESS_ENABLED 489
#define EEPROM_DAY_BRIGHTNESS 490
#define EEPROM_NIGHT_BRIGHTNESS 491
#define EEPROM_DAY_START_HOUR 492
#define EEPROM_NIGHT_START_HOUR 493

struct AutoBrightnessConfig {
    bool enabled;
    uint8_t dayBrightness;
    uint8_t nightBrightness;
    uint8_t dayStartHour;    // Hour when day mode starts (e.g., 7)
    uint8_t nightStartHour;  // Hour when night mode starts (e.g., 22)
} autoBrightness;

void loadAutoBrightnessConfig() {
    autoBrightness.enabled = EEPROM.read(EEPROM_AUTO_BRIGHTNESS_ENABLED);
    autoBrightness.dayBrightness = EEPROM.read(EEPROM_DAY_BRIGHTNESS);
    autoBrightness.nightBrightness = EEPROM.read(EEPROM_NIGHT_BRIGHTNESS);
    autoBrightness.dayStartHour = EEPROM.read(EEPROM_DAY_START_HOUR);
    autoBrightness.nightStartHour = EEPROM.read(EEPROM_NIGHT_START_HOUR);

    // Set defaults if not configured (0xFF = uninitialized EEPROM)
    if (autoBrightness.dayBrightness == 0 || autoBrightness.dayBrightness == 255) {
        autoBrightness.enabled = false;
        autoBrightness.dayBrightness = 150;
        autoBrightness.nightBrightness = 30;
        autoBrightness.dayStartHour = 7;
        autoBrightness.nightStartHour = 22;
    }
}

void saveAutoBrightnessConfig() {
    EEPROM.write(EEPROM_AUTO_BRIGHTNESS_ENABLED, autoBrightness.enabled ? 1 : 0);
    EEPROM.write(EEPROM_DAY_BRIGHTNESS, autoBrightness.dayBrightness);
    EEPROM.write(EEPROM_NIGHT_BRIGHTNESS, autoBrightness.nightBrightness);
    EEPROM.write(EEPROM_DAY_START_HOUR, autoBrightness.dayStartHour);
    EEPROM.write(EEPROM_NIGHT_START_HOUR, autoBrightness.nightStartHour);
    EEPROM.commit();
}

// Returns true if brightness was changed
bool updateAutoBrightness(uint8_t currentHour) {
    if (!autoBrightness.enabled) {
        return false;
    }

    uint8_t targetBrightness;
    bool isDayTime;

    // Determine if it's day or night
    if (autoBrightness.dayStartHour < autoBrightness.nightStartHour) {
        // Normal case: day is between dayStart and nightStart
        isDayTime = (currentHour >= autoBrightness.dayStartHour &&
                     currentHour < autoBrightness.nightStartHour);
    } else {
        // Inverted case: night spans midnight
        isDayTime = (currentHour >= autoBrightness.dayStartHour ||
                     currentHour < autoBrightness.nightStartHour);
    }

    targetBrightness = isDayTime ? autoBrightness.dayBrightness : autoBrightness.nightBrightness;

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
