#ifndef COLORCODEDCLOCK_H_
#define COLORCODEDCLOCK_H_

// Clock LED layout (6 LEDs total) - HH:MM:SS
// LED 62: Hour tens digit
// LED 63: Hour units digit
// LED 64: Minute tens digit
// LED 65: Minute units digit
// LED 66: Second tens digit
// LED 67: Second units digit
#define CLOCK_LED_H1 62
#define CLOCK_LED_H2 63
#define CLOCK_LED_M1 64
#define CLOCK_LED_M2 65
#define CLOCK_LED_S1 66
#define CLOCK_LED_S2 67

// Track last update for non-blocking refresh
unsigned long lastClockUpdate = 0;
byte lastDisplayedSecond = 255;

void timeDisplay(byte h, byte m, byte s, bool showNow = false) {
	// Hour digits
	leds[CLOCK_LED_H1] = rainbow_colors[h / 10];
	leds[CLOCK_LED_H2] = rainbow_colors[h % 10];

	// Minute digits
	leds[CLOCK_LED_M1] = rainbow_colors[m / 10];
	leds[CLOCK_LED_M2] = rainbow_colors[m % 10];

	// Second digits
	leds[CLOCK_LED_S1] = rainbow_colors[s / 10];
	leds[CLOCK_LED_S2] = rainbow_colors[s % 10];

	lastDisplayedSecond = s;
	lastClockUpdate = millis();

	if (showNow) {
		FastLED.show();
	}
}

// Non-blocking clock update - call frequently in loop
// Returns true if clock was updated (for external refresh tracking)
bool updateClockIfNeeded(byte h, byte m, byte s) {
	if (s != lastDisplayedSecond || (millis() - lastClockUpdate > 1000)) {
		timeDisplay(h, m, s, true);  // Show immediately
		return true;
	}
	return false;
}

// Force clock refresh (call after calendar display updates)
void refreshClock(byte h, byte m, byte s) {
	timeDisplay(h, m, s);
}

#endif /* COLORCODEDCLOCK_H_ */
