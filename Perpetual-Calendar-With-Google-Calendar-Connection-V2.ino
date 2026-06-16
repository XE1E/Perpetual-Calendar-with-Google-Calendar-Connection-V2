/*
 * * ESP8266 template with phone config web page
 * based on ESP 8266 Arduino IDE WebConfig by John Lassen and
 * BVB_WebConfig_OTA_V7 from Andreas Spiess https://github.com/SensorsIot/Internet-of-Things-with-ESP8266
 *
 */
#ifndef FASTLED_INTERRUPT_RETRY_COUNT
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#endif
#include <FastLED.h>
#include "HTTPSRedirect.h"
// COLOR_CODED_CLOCK can be enabled via build flags or uncomment below
//#define COLOR_CODED_CLOCK

//Calendar connection Settings
const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const int httpsPort = 443;

HTTPSRedirect* client = nullptr;
String calendarData = "";
String escriptData = "";
bool events_updated = false;
int Days[31];
int Holidays[20];
int Anniversaries[20];
int Todos[20];

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN 2
#define NUM_LEDS  75
CRGB leds[NUM_LEDS];
uint8 BRIGHTNESS = 100;

byte calendar_leds[] = {68, 69, 70, 71, 72, 73, 74,
		61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
		36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
		35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
byte calendar_months[] = {19, 20, 21, 22, 7, 6, 5, 4, 3, 2, 1, 0};
// Clock uses LEDs 62-67 for HH:MM:SS when COLOR_CODED_CLOCK is enabled
// WiFi warning: clock LEDs blink red when no connection

// https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors - Color map: "Rainbow" vs "Spectrum"
//also dark grey and light grey for ten color values as in instructable - https://www.instructables.com/Color-Coded-Clock-Colors-Show-the-Time/
CHSV rainbow_colors[] = { CHSV(0, 0, 64), CHSV(0, 255, 192), CHSV(32, 255, 255), CHSV(64, 255, 255), CHSV(96, 255, 255), CHSV(138, 255, 255), CHSV(160, 255, 255), CHSV(180, 255, 255), CHSV(214, 255, 255), CHSV(0, 0, 192)};
//colors used and some alternatives
//CHSV weekday_color = CHSV(96, 255, 192);
CHSV weekday_color = rainbow_colors[4];
//CHSV actualday_color = CHSV(160, 255, 192);
CHSV actualday_color = rainbow_colors[6];
//CHSV weekend_color = CHSV(0, 255, 128);
CHSV weekend_color = rainbow_colors[1];
//CRGB month_color = CRGB(128, 128, 128);
CRGB month_color = rainbow_colors[9];
//CHSV holidays_color = CHSV(192, 255,255);
CHSV holidays_color = rainbow_colors[7];
//CHSV anniversaries_color = CHSV(128, 255, 255);
CHSV anniversaries_color = rainbow_colors[5];
//CHSV todos_color = CHSV(32, 255, 192);
CHSV todos_color = rainbow_colors[2];

int temp_hour = -1;
int temp_minute = -1;

#ifdef COLOR_CODED_CLOCK
#include "ColorCodedClock.h"
int temp_second = -1;
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "global.h"
#include "NTP.h"

// Include STYLE and Script "Pages"
#include "Page_Script.js.h"
#include "Page_Style.css.h"

// Include HTML "Pages"
#include "Page_Admin.h"
#include "Page_AppsScriptSettings.h"
#include "Page_NTPSettings.h"
#include "Page_Information.h"
#include "Page_NetworkConfiguration.h"
#include "Page_SetTime.h"
#include "Page_LEDSettings.h"
#include "Page_OTA.h"
#include "AutoBrightness.h"
#include "Page_AutoBrightness.h"
#include "Page_ColorSettings.h"
#include "WiFiManager.h"

//Connect to Google script
void connectToGoogle() {
	// Use HTTPSRedirect class to create a new TLS connection
	client = new HTTPSRedirect(httpsPort);
	client->setInsecure();
	client->setPrintResponseBody(true);
	client->setContentTypeHeader("application/json");

	Serial.print("Connecting to ");
	Serial.println(host);

	// Try to connect for a maximum of 5 times
	bool flag = false;
	for (int i = 0; i < 3; i++) {
		int retval = client->connect(host, httpsPort);
		if (retval == 1) {
			flag = true;
			Serial.println("Connected to Google");
			break;
		}
		else
			Serial.println("Connection failed. Retrying...");
	}

	if (!flag) {
		Serial.print("Could not connect to server: ");
		Serial.println(host);
		Serial.println("Exiting...");
		//ESP.reset();
	}
	//Serial.println("Connected to Google");
}

void getCalendar(String GScriptId) {
	//  Serial.println("Start Request");
	// HTTPSRedirect client(httpsPort);

	// Try to connect for a maximum of 5 times
	bool flag = false;
	for (int i = 0; i < 3; i++) {
		int retval = client->connect(host, httpsPort);
		if (retval == 1) {
			flag = true;
			break;
		}
		else
			Serial.println("Connection failed. Retrying...");
		yield();
		server.handleClient();
	}
	if (!flag) {
		Serial.print("Could not connect to server: ");
		Serial.println(host);
		Serial.println("Exiting...");
	}
	//Fetch Google Calendar events
	Serial.print("Calendar Data---> ");
	String url = String("/macros/s/") + GScriptId + "/exec";
	client->GET(url, host);
	calendarData = client->getResponseBody();
	yield();
	server.handleClient();
}

void initDatesArray(int (&Dates)[20], String calendarString) {
	int index;
	for (index = 0; index < 20; index++) {
		Dates[index] = 0;
	}
	index = 0;
	while(calendarString.indexOf("-")!=-1){
		Dates[index] = calendarString.substring(0,calendarString.indexOf("-")).toInt();
		index++;
		calendarString = calendarString.substring(calendarString.indexOf("-")+1);
	}
}

void CalendarDisplay(int y, byte m, byte d) {
	//erase all leds
	fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
	byte mdays = daysInMonth(y, m); // number of days from the current month
	int i = 0;
	for (int k = 1; k <= mdays; k++) {
		int z = DayOfTheWeek(y, m, k); // sunday-0 saturday-6
		//Serial.print(z);Serial.print("--");Serial.print(k);Serial.print("--");Serial.print(i);Serial.print("--");Serial.println(calendar_leds[i]);
		if (z == 6) {
			if (d == k) {
				leds[calendar_leds[i]] = actualday_color;
				Days[k - 1] = calendar_leds[i];
			}
			else {
				leds[calendar_leds[i]] = weekend_color;
				Days[k - 1] = calendar_leds[i];
			}
			if (config.FirstWeekDay == "Monday") {
				i = i + 1 ;
			}
			else {
				i = i + 7; //because saturday is the last day in the week
			}
		}
		else if (z == 0) {
			if (d == k) {
				leds[calendar_leds[i]] = actualday_color;
				Days[k - 1] = calendar_leds[i];
			}
			else {
				leds[calendar_leds[i]] = weekend_color;
				Days[k - 1] = calendar_leds[i];
			}
			if (config.FirstWeekDay == "Monday") {
				i = i + 7 ; //because sunday is the last day in the week
			}
			else {
				i = i + 1;
			}
		}
		else {
			if (d == k) {
				leds[calendar_leds[i]] = actualday_color;
				Days[k - 1] = calendar_leds[i];
			}
			else {
				leds[calendar_leds[i]] = weekday_color;
				Days[k - 1] = calendar_leds[i];
			}
			i = i + 1;
		}
		if (i == d) {
			leds[calendar_leds[i]] = actualday_color;
			Days[k - 1] = calendar_leds[i];
		}
	}
	leds[calendar_months[m - 1]] = month_color;
}

void EventsDisplay() {
	// display days from calendars
	for (int i = 1; i <= 31; i++) {
		for (int j = 0; j < 20; j++) {
			if (Holidays[j] == i) { leds[Days[i - 1]] = holidays_color; }
		}
		for (int j = 0; j < 20; j++) {
			if (Anniversaries[j] == i) { leds[Days[i - 1]] = anniversaries_color; }
		}
		for (int j = 0; j < 20; j++) {
			if (Todos[j] == i) { leds[Days[i - 1]] = todos_color; }
		}
	}
}

void pride()
{
	static uint16_t sPseudotime = 0;
	static uint16_t sLastMillis = 0;
	static uint16_t sHue16 = 0;

	uint8_t sat8 = beatsin88( 87, 220, 250);
	uint8_t brightdepth = beatsin88( 341, 96, 224);
	uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
	uint8_t msmultiplier = beatsin88(147, 23, 60);

	uint16_t hue16 = sHue16;//gHue * 256;
	uint16_t hueinc16 = beatsin88(113, 1, 3000);

	uint16_t ms = millis();
	uint16_t deltams = ms - sLastMillis ;
	sLastMillis  = ms;
	sPseudotime += deltams * msmultiplier;
	sHue16 += deltams * beatsin88( 400, 5,9);
	uint16_t brightnesstheta16 = sPseudotime;

	for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
		hue16 += hueinc16;
		uint8_t hue8 = hue16 / 256;

		brightnesstheta16  += brightnessthetainc16;
		uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

		uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
		uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
		bri8 += (255 - brightdepth);

		CRGB newcolor = CHSV( hue8, sat8, bri8);

		uint16_t pixelnumber = i;
		pixelnumber = (NUM_LEDS-1) - pixelnumber;

		nblend( leds[pixelnumber], newcolor, 64);
	}
}

void setup() {
	Serial.begin(115200);
	//**** Network Config load
	EEPROM.begin(640); // define an EEPROM space of 640 Bytes to store data (increased for backup WiFi network)
	CFG_saved = ReadConfig();

	//  Connect to WiFi acess point or start as Acess point
	if (CFG_saved)  //if no configuration yet saved, load defaults
	{
		// Connect the ESP8266 to local WIFI network in Station mode
		Serial.println("Booting");
		//printConfig();
		WiFi.mode(WIFI_STA);
		WiFi.begin(config.ssid.c_str(), config.password.c_str());
		WIFI_connected = WiFi.waitForConnectResult();
		if (WIFI_connected != WL_CONNECTED)
			Serial.println("Connection Failed! activating the AP mode...");

		Serial.print("Wifi ip:");
		Serial.println(WiFi.localIP());
	}

	if ((WIFI_connected != WL_CONNECTED) or !CFG_saved) {
		// DEFAULT CONFIG
		Serial.println("Setting AP mode default parameters");
		config.ssid = "PerpetualCalendar-" + String(ESP.getChipId(), HEX); // SSID of access point
		config.password = "";
		config.dhcp = true;
		config.IP[0] = 192;
		config.IP[1] = 168;
		config.IP[2] = 1;
		config.IP[3] = 100;
		config.Netmask[0] = 255;
		config.Netmask[1] = 255;
		config.Netmask[2] = 255;
		config.Netmask[3] = 0;
		config.Gateway[0] = 192;
		config.Gateway[1] = 168;
		config.Gateway[2] = 1;
		config.Gateway[3] = 254;
		config.DeviceName = "Perpetual Calendar";
		config.ntpServerName = "0.europe.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
		config.Update_Time_Via_NTP_Every = 3;
		config.timeZone = 20;
		config.isDayLightSaving = true;
		config.FirstWeekDay = "Sunday";
		//WriteConfig();
		WiFi.mode(WIFI_AP);
		WiFi.softAP(config.ssid.c_str(),"admin1234");
		Serial.print("Wifi ip:");
		Serial.println(WiFi.softAPIP());
	}

	// Start HTTP Server for configuration
	server.on("/", []() {
		Serial.println("admin.html");
		server.send_P ( 200, "text/html", PAGE_AdminMainPage); // const char top of page
	});

	server.on("/favicon.ico", []() {
		Serial.println("favicon.ico");
		server.send( 200, "text/html", "" );
	});
	// Network config
	server.on("/config.html", send_network_configuration_html);
	// Info Page
	server.on("/info.html", []() {
		Serial.println("info.html");
		server.send_P ( 200, "text/html", PAGE_Information );
	});
	server.on("/ntp.html", send_NTP_configuration_html);
	server.on("/appsscript.html", send_Apps_Script_Settings_html);
	server.on("/time.html", send_Time_Set_html);
	server.on("/style.css", []() {
		Serial.println("style.css");
		server.send_P ( 200, "text/plain", PAGE_Style_css );
	});
	server.on("/microajax.js", []() {
		Serial.println("microajax.js");
		server.send_P ( 200, "text/plain", PAGE_microajax_js );
	});
	server.on("/admin/values", send_network_configuration_values_html);
	server.on("/admin/connectionstate", send_connection_state_values_html);
	server.on("/admin/infovalues", send_information_values_html);
	server.on("/admin/ntpvalues", send_NTP_configuration_values_html);
	server.on("/admin/appsscript", send_Apps_Script_Settings_values_html);
	server.on("/admin/timevalues", send_Time_Set_values_html);
	// LED Settings
	server.on("/led.html", send_LED_Settings_html);
	server.on("/admin/ledvalues", send_LED_Settings_values_html);
	server.on("/admin/setbrightness", handle_set_brightness);
	server.on("/admin/savebrightness", handle_save_brightness);
	server.on("/admin/testleds", handle_test_leds);
	server.on("/admin/refreshcalendar", handle_refresh_calendar);
	// OTA Info
	server.on("/ota.html", send_OTA_html);
	server.on("/admin/otavalues", send_OTA_values_html);
	// Auto Brightness
	server.on("/autobrightness.html", send_AutoBrightness_html);
	server.on("/admin/autobrightvalues", send_AutoBrightness_values_html);
	server.on("/admin/saveautobrightness", handle_save_autobrightness);
	// Color Settings
	server.on("/colors.html", send_Color_Settings_html);
	server.on("/admin/colorvalues", send_Color_Settings_values_html);
	server.on("/admin/previewcolors", handle_preview_colors);
	server.on("/admin/savecolors", handle_save_colors);
	server.on("/admin/resetcolors", handle_reset_colors);
	server.onNotFound([]() {
		Serial.println("Page Not Found");
		server.send ( 400, "text/html", "Page not Found" );
	});
	server.begin();
	Serial.println("HTTP server started");

	// Setup mDNS responder
	if (WIFI_connected == WL_CONNECTED) {
		String mdnsName = config.DeviceName;
		mdnsName.replace(" ", "");
		mdnsName.toLowerCase();
		if (MDNS.begin(mdnsName.c_str())) {
			Serial.println("mDNS responder started: http://" + mdnsName + ".local");
			MDNS.addService("http", "tcp", 80);
		}

		// Setup OTA Updates
		ArduinoOTA.setHostname(mdnsName.c_str());
		ArduinoOTA.onStart([]() {
			Serial.println("OTA Update Starting...");
			fill_solid(leds, NUM_LEDS, CRGB::Blue);
			FastLED.show();
		});
		ArduinoOTA.onEnd([]() {
			Serial.println("OTA Update Complete!");
			fill_solid(leds, NUM_LEDS, CRGB::Green);
			FastLED.show();
		});
		ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
			int percent = (progress / (total / 100));
			int ledsToLight = map(percent, 0, 100, 0, NUM_LEDS);
			fill_solid(leds, NUM_LEDS, CRGB::Black);
			fill_solid(leds, ledsToLight, CRGB::Purple);
			FastLED.show();
		});
		ArduinoOTA.onError([](ota_error_t error) {
			Serial.printf("OTA Error[%u]: ", error);
			fill_solid(leds, NUM_LEDS, CRGB::Red);
			FastLED.show();
		});
		ArduinoOTA.begin();
		Serial.println("OTA ready on port 8266");
	}

	// Read brightness from EEPROM
	byte savedBrightness = EEPROM.read(488);
	if (savedBrightness >= 10 && savedBrightness <= 255) {
		BRIGHTNESS = savedBrightness;
	}

	// Load auto brightness settings
	loadAutoBrightnessConfig();

	// Load custom colors
	loadCustomColors();

	// Initialize WiFi manager
	initWiFiManager();

	printConfig();

	// start internal time update ISR
	tkSecond.attach(1, ISRsecondTick);

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.setBrightness(BRIGHTNESS);
	Serial.println("FastLed Setup done");

	connectToGoogle();

	// start internal time update ISR
	tkSecond.attach(1, ISRsecondTick);
}

// the loop function runs over and over again forever
void loop() {
	ArduinoOTA.handle();
	MDNS.update();
	server.handleClient();

	// Check WiFi connection and reconnect if needed
	checkWiFiConnection();
	if (config.Update_Time_Via_NTP_Every > 0) {
		if (cNTP_Update > 5 && firstStart) {
			getNTPtime();
			delay(1500); //wait for DateTime
			cNTP_Update = 0;
			firstStart = false;
		}
		else if (cNTP_Update > (config.Update_Time_Via_NTP_Every * 60)) {
			getNTPtime();
			cNTP_Update = 0;
		}
	}
	//  feed de DOG :)
	customWatchdog = millis();

	//============================
	if (WIFI_connected != WL_CONNECTED and manual_time_set == false) {
		config.Update_Time_Via_NTP_Every = 0;
#ifdef COLOR_CODED_CLOCK
		// Blink clock LEDs red when no WiFi
		static unsigned long lastWifiBlink = 0;
		static bool wifiBlinkState = false;
		if (millis() - lastWifiBlink > 500) {
			lastWifiBlink = millis();
			wifiBlinkState = !wifiBlinkState;
			CRGB wifiColor = wifiBlinkState ? CRGB(255, 0, 0) : CRGB(0, 0, 0);
			for (int i = 62; i <= 67; i++) {
				leds[i] = wifiColor;
			}
		}
#else
		// Show red on first calendar LED when no WiFi
		leds[calendar_leds[0]] = CRGB(255, 0, 0);
#endif
		FastLED.show();
	} else if (ntp_response_ok == false and manual_time_set == false) {
		config.Update_Time_Via_NTP_Every = 1;
		//display_animation_no_ntp
		pride();
		FastLED.show();
	} else if (ntp_response_ok == true or manual_time_set == true) {
		bool needsRefresh = false;

		// Update auto brightness based on current hour
		if (updateAutoBrightness(DateTime.hour)) {
			needsRefresh = true;
		}

#ifdef COLOR_CODED_CLOCK
		// Non-blocking clock update
		if (updateClockIfNeeded(DateTime.hour, DateTime.minute, DateTime.second)) {
			needsRefresh = true;
		}
#endif

		if (temp_minute != DateTime.minute) {
			temp_minute = DateTime.minute;
			getCalendar(config.TodosScriptID);
			yield();
#ifdef COLOR_CODED_CLOCK
			updateClockIfNeeded(DateTime.hour, DateTime.minute, DateTime.second);
#endif
			if (escriptData != calendarData) {
				initDatesArray(Todos, calendarData);
				escriptData = calendarData;
				events_updated = true;
			}
		}

		if ((temp_hour != DateTime.hour) or events_updated == true) {
			temp_hour = DateTime.hour;
			events_updated = false;

			getCalendar(config.HolidaysScriptID);
			yield();
#ifdef COLOR_CODED_CLOCK
			updateClockIfNeeded(DateTime.hour, DateTime.minute, DateTime.second);
#endif
			initDatesArray(Holidays, calendarData);

			getCalendar(config.AnniversariesScriptID);
			yield();
#ifdef COLOR_CODED_CLOCK
			updateClockIfNeeded(DateTime.hour, DateTime.minute, DateTime.second);
#endif
			initDatesArray(Anniversaries, calendarData);

			CalendarDisplay(DateTime.year, DateTime.month, DateTime.day);
			EventsDisplay();

#ifdef COLOR_CODED_CLOCK
			// Refresh clock after calendar update to prevent overwrite
			refreshClock(DateTime.hour, DateTime.minute, DateTime.second);
#endif
			needsRefresh = true;
		}

		if (needsRefresh) {
			FastLED.show();
		}
	}
}



