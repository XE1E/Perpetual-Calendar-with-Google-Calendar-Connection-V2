#ifndef COLORCODEDCLOCK_H_
#define COLORCODEDCLOCK_H_

// Clock LED layout (5 LEDs total):
// LED 63: Hour tens digit
// LED 64: Hour units digit
// LED 65: Separator (blinks every second)
// LED 66: Minute tens digit
// LED 67: Minute units digit
#define CLOCK_LED_H1 63
#define CLOCK_LED_H2 64
#define CLOCK_LED_SEP 65
#define CLOCK_LED_M1 66
#define CLOCK_LED_M2 67

// Track last update for non-blocking refresh
unsigned long lastClockUpdate = 0;
byte lastDisplayedSecond = 255;

// Separator color (white when on)
CRGB separatorColor = CRGB(128, 128, 128);

void timeDisplay(byte h, byte m, byte s) {
	// Hour digits
	leds[CLOCK_LED_H1] = rainbow_colors[h / 10];
	leds[CLOCK_LED_H2] = rainbow_colors[h % 10];

	// Separator blinks with seconds (on for even, off for odd)
	if (s % 2 == 0) {
		leds[CLOCK_LED_SEP] = separatorColor;
	} else {
		leds[CLOCK_LED_SEP] = CRGB(0, 0, 0);
	}

	// Minute digits
	leds[CLOCK_LED_M1] = rainbow_colors[m / 10];
	leds[CLOCK_LED_M2] = rainbow_colors[m % 10];

	lastDisplayedSecond = s;
	lastClockUpdate = millis();
}

// Non-blocking clock update - call frequently in loop
bool updateClockIfNeeded(byte h, byte m, byte s) {
	// Update if second changed OR if more than 1 second passed without update
	if (s != lastDisplayedSecond || (millis() - lastClockUpdate > 1000)) {
		timeDisplay(h, m, s);
		return true;
	}
	return false;
}

// Force clock refresh (call after calendar display updates)
void refreshClock(byte h, byte m, byte s) {
	timeDisplay(h, m, s);
}

#endif /* COLORCODEDCLOCK_H_ */
