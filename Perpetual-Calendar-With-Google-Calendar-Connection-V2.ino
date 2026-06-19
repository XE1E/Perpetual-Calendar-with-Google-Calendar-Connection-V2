// Perpetual Calendar with Google Calendar Connection V2
// Full web interface version

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include "HTTPSRedirect.h"

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN 2
#define NUM_LEDS 75
CRGB leds[NUM_LEDS];

// Global brightness variable (required before Page includes)
uint8_t BRIGHTNESS = 150;
int temp_hour = -1;
int temp_minute = -1;

// Rainbow colors for color-coded clock and color reset defaults
CHSV rainbow_colors[10] = {
    CHSV(0, 0, 0),       // 0 - Black/off
    CHSV(0, 255, 192),   // 1 - Red
    CHSV(32, 255, 192),  // 2 - Orange
    CHSV(64, 255, 192),  // 3 - Yellow
    CHSV(96, 255, 192),  // 4 - Green
    CHSV(128, 255, 192), // 5 - Cyan
    CHSV(160, 255, 192), // 6 - Blue
    CHSV(192, 255, 192), // 7 - Purple
    CHSV(224, 255, 192), // 8 - Pink
    CHSV(0, 0, 192)      // 9 - White/Gray
};

// Calendar colors (defaults, can be overridden by custom colors)
CHSV weekday_color = CHSV(96, 255, 192);
CHSV actualday_color = CHSV(160, 255, 192);
CHSV weekend_color = CHSV(0, 255, 128);
CHSV month_color = CHSV(0, 0, 192);
CHSV holidays_color = CHSV(192, 255, 255);
CHSV anniversaries_color = CHSV(128, 255, 255);
CHSV todos_color = CHSV(32, 255, 192);

#include "global.h"
#include "NTP.h"
#include "AutoBrightness.h"
#include "ColorCodedClock.h"

// Include web pages (must come after global variables are defined)
#include "Page_Style.css.h"
#include "Page_Script.js.h"
#include "Page_Admin.h"
#include "Page_NetworkConfiguration.h"
#include "Page_Information.h"
#include "Page_NTPSettings.h"
#include "Page_AppsScriptSettings.h"
#include "Page_SetTime.h"
#include "Page_LEDSettings.h"
#include "Page_AutoBrightness.h"
#include "Page_ColorSettings.h"
#include "Page_OTA.h"

// Calendar LED mapping
byte calendar_leds[] = {68, 69, 70, 71, 72, 73, 74,
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
byte calendar_months[] = {19, 20, 21, 22, 7, 6, 5, 4, 3, 2, 1, 0};

int Days[31];
int Holidays[20];
int Anniversaries[20];
int Todos[20];
bool isMondayFirst = false;
bool events_loaded = false;

HTTPSRedirect* client = nullptr;
const char* host = "script.google.com";
const int httpsPort = 443;
String calendarData = "";

void connectToGoogle() {
  if (client != nullptr) {
    delete client;
    client = nullptr;
  }
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  Serial.print("Connecting to Google...");
  for (int i = 0; i < 3; i++) {
    if (client->connect(host, httpsPort)) {
      Serial.println(" OK");
      return;
    }
    Serial.print(".");
    yield();
  }
  Serial.println(" FAILED");
}

void getCalendar(String GScriptId) {
  if (GScriptId.length() == 0) return;
  if (client == nullptr || !client->connected()) {
    connectToGoogle();
  }
  if (client == nullptr) return;

  String url = String("/macros/s/") + GScriptId + "/exec";
  client->GET(url, host);
  calendarData = client->getResponseBody();
  Serial.println(calendarData);
  yield();
  customWatchdog = millis();
}

void initDatesArray(int (&Dates)[20], String calendarString) {
  memset(Dates, 0, sizeof(int) * 20);
  int index = 0;
  int num = 0;
  for (size_t i = 0; i <= calendarString.length() && index < 20; i++) {
    char c = (i < calendarString.length()) ? calendarString[i] : '-';
    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
    } else if (c == '-' && num > 0) {
      Dates[index++] = num;
      num = 0;
    }
  }
}

void EventsDisplay() {
  for (int j = 0; j < 20 && Holidays[j] != 0; j++) {
    int day = Holidays[j];
    if (day >= 1 && day <= 31) leds[Days[day - 1]] = holidays_color;
  }
  for (int j = 0; j < 20 && Anniversaries[j] != 0; j++) {
    int day = Anniversaries[j];
    if (day >= 1 && day <= 31) leds[Days[day - 1]] = anniversaries_color;
  }
  for (int j = 0; j < 20 && Todos[j] != 0; j++) {
    int day = Todos[j];
    if (day >= 1 && day <= 31) leds[Days[day - 1]] = todos_color;
  }
}

void CalendarDisplay(int y, byte m, byte d) {
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  byte mdays = daysInMonth(y, m);
  uint8_t i = 0;
  for (byte k = 1; k <= mdays; k++) {
    int z = DayOfTheWeek(y, m, k);
    CHSV color = (d == k) ? actualday_color : ((z == 0 || z == 6) ? weekend_color : weekday_color);
    leds[calendar_leds[i]] = color;
    Days[k - 1] = calendar_leds[i];
    if (z == 6) {
      i += isMondayFirst ? 1 : 7;
    } else if (z == 0) {
      i += isMondayFirst ? 7 : 1;
    } else {
      i++;
    }
  }
  leds[calendar_months[m - 1]] = month_color;
}

void setupWebServer() {
  // Main admin page
  server.on("/", []() { server.send_P(200, "text/html", PAGE_AdminMainPage); });

  // CSS and JS
  server.on("/style.css", []() { server.send_P(200, "text/css", PAGE_Style_css); });
  server.on("/microajax.js", []() { server.send_P(200, "application/javascript", PAGE_microajax_js); });

  // Network configuration
  server.on("/config.html", send_network_configuration_html);
  server.on("/admin/values", send_network_configuration_values_html);
  server.on("/admin/connectionstate", send_connection_state_values_html);

  // Network information
  server.on("/info.html", []() { server.send_P(200, "text/html", PAGE_Information); });
  server.on("/admin/infovalues", send_information_values_html);
  server.on("/admin/restart", handle_restart);
  server.on("/admin/clearwifi", handle_clear_wifi);

  // NTP settings
  server.on("/ntp.html", send_NTP_configuration_html);
  server.on("/admin/ntpvalues", send_NTP_configuration_values_html);

  // Apps Script settings
  server.on("/appsscript.html", send_Apps_Script_Settings_html);
  server.on("/admin/appsscript", send_Apps_Script_Settings_values_html);

  // Manual time setting
  server.on("/time.html", send_Time_Set_html);
  server.on("/admin/timevalues", send_Time_Set_values_html);

  // LED settings
  server.on("/led.html", send_LED_Settings_html);
  server.on("/admin/ledvalues", send_LED_Settings_values_html);
  server.on("/admin/setbrightness", handle_set_brightness);
  server.on("/admin/savebrightness", handle_save_brightness);
  server.on("/admin/testleds", handle_test_leds);
  server.on("/admin/refreshcalendar", handle_refresh_calendar);

  // Auto brightness
  server.on("/autobrightness.html", send_AutoBrightness_html);
  server.on("/admin/autobrightvalues", send_AutoBrightness_values_html);
  server.on("/admin/saveautobrightness", handle_save_autobrightness);

  // Color settings
  server.on("/colors.html", send_Color_Settings_html);
  server.on("/admin/colorvalues", send_Color_Settings_values_html);
  server.on("/admin/previewcolors", handle_preview_colors);
  server.on("/admin/savecolors", handle_save_colors);
  server.on("/admin/resetcolors", handle_reset_colors);
  server.on("/admin/gammavalues", send_gamma_values_html);
  server.on("/admin/previewgamma", handle_preview_gamma);
  server.on("/admin/savegamma", handle_save_gamma);

  // OTA page
  server.on("/ota.html", send_OTA_html);
  server.on("/admin/otavalues", send_OTA_values_html);

  server.begin();
  Serial.println("Web server started");
}

void setupOTA() {
  String hostname = config.DeviceName;
  hostname.replace(" ", "");
  hostname.toLowerCase();

  ArduinoOTA.setHostname(hostname.c_str());
  ArduinoOTA.setPort(8266);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA: Start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA: End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== Perpetual Calendar V2 ===");

  EEPROM.begin(700);

  if (!ReadConfig()) {
    Serial.println("Config not found, using defaults");
    config.DeviceName = "PerpetualCalendar";
    config.ntpServerName = "pool.ntp.org";
    config.timeZone = -60;
    config.Update_Time_Via_NTP_Every = 30;
  } else {
    Serial.println("Config loaded");
    printConfig();
  }

  isMondayFirst = (config.FirstWeekDay == "Monday" || config.FirstWeekDay == "1");

  // Load saved brightness from EEPROM
  uint8_t savedBrightness = EEPROM.read(488);
  if (savedBrightness >= 5 && savedBrightness <= 255) {
    BRIGHTNESS = savedBrightness;
  }

  // FastLED initialization
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("FastLED initialized");

  // Load auto brightness settings
  loadAutoBrightnessConfig();

  // Load custom colors
  loadCustomColors();
  loadColorCorrection();

  // WiFi setup
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Calendario", "12345678");
  Serial.println("AP: Calendario (password: 12345678)");

  if (config.ssid.length() > 0) {
    Serial.print("Connecting to: ");
    Serial.println(config.ssid);
    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    int timeout = 20;
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
      delay(500);
      Serial.print(".");
      timeout--;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      WIFI_connected = WL_CONNECTED;
    } else {
      Serial.println("\nWiFi connection failed, AP mode only");
    }
  }

  // Setup web server
  setupWebServer();

  // Setup mDNS
  String mdnsName = config.DeviceName;
  mdnsName.replace(" ", "");
  mdnsName.toLowerCase();
  if (MDNS.begin(mdnsName.c_str())) {
    Serial.print("mDNS: ");
    Serial.print(mdnsName);
    Serial.println(".local");
  }

  // Setup OTA
  setupOTA();

  // Start NTP ticker
  tkSecond.attach(1, ISRsecondTick);
  customWatchdog = millis();

  Serial.println("=== SETUP COMPLETE ===");
}

void pride() {
  FastLED.setBrightness(120);  // Brillo fijo para animación
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;
    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    leds[i] = CHSV(hue8, sat8, bri8);
  }
  FastLED.show();
}

void loop() {
  customWatchdog = millis();

  // Handle web requests and OTA
  MDNS.update();
  server.handleClient();
  ArduinoOTA.handle();

  // Procesar test de LEDs si fue solicitado
  processTestLeds();

  if (!ntp_response_ok) {
    // Waiting for NTP - show pride animation
    if (cNTP_Update >= 5) {
      cNTP_Update = 0;
      getNTPtime();
    }
    pride();
  } else {
    // NTP received - show calendar

    // Update auto brightness
    updateAutoBrightness(DateTime.hour, DateTime.minute);

    // Load events once per hour or when forced (temp_hour = -1)
    if (temp_hour != DateTime.hour) {
      temp_hour = DateTime.hour;

      Serial.println("Loading calendar events...");

      getCalendar(config.HolidaysScriptID);
      initDatesArray(Holidays, calendarData);

      getCalendar(config.AnniversariesScriptID);
      initDatesArray(Anniversaries, calendarData);

      getCalendar(config.TodosScriptID);
      initDatesArray(Todos, calendarData);

      // Cleanup Google client
      if (client != nullptr) {
        delete client;
        client = nullptr;
      }

      // Re-init FastLED after Google HTTPS operations
      FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
      FastLED.setBrightness(BRIGHTNESS);

      Serial.println("Events loaded!");
    }

    // Display calendar
    CalendarDisplay(DateTime.year, DateTime.month, DateTime.day);
    EventsDisplay();

    // Display clock
    refreshClock(DateTime.hour, DateTime.minute, DateTime.second);

    FastLED.show();
  }

  delay(10);
}
