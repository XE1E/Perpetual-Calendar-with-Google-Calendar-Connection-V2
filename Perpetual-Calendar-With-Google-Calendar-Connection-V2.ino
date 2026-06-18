// Version funcional con configuracion web
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "HTTPSRedirect.h"

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define LED_PIN 2
#define NUM_LEDS 75
CRGB leds[NUM_LEDS];

#include "global.h"
#include "NTP.h"

byte calendar_leds[] = {68, 69, 70, 71, 72, 73, 74,
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
byte calendar_months[] = {19, 20, 21, 22, 7, 6, 5, 4, 3, 2, 1, 0};

CHSV weekday_color = CHSV(96, 255, 192);
CHSV actualday_color = CHSV(160, 255, 192);
CHSV weekend_color = CHSV(0, 255, 128);
CRGB month_color = CRGB(192, 192, 192);
CHSV holidays_color = CHSV(192, 255, 255);
CHSV anniversaries_color = CHSV(128, 255, 255);
CHSV todos_color = CHSV(32, 255, 192);

int Days[31];
int Holidays[20];
int Anniversaries[20];
int Todos[20];
bool isMondayFirst = false;
bool events_loaded = false;
int temp_hour = -1;

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

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><title>Calendario</title></head><body>";
  html += "<h1>Calendario Perpetuo</h1>";
  html += "<p>WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Conectado" : "No conectado") + "</p>";
  html += "<p>NTP: " + String(ntp_response_ok ? "OK" : "Esperando") + "</p>";
  html += "<p>Fecha: " + String(DateTime.day) + "/" + String(DateTime.month) + "/" + String(DateTime.year) + "</p>";
  html += "<p>Hora: " + String(DateTime.hour) + ":" + String(DateTime.minute) + ":" + String(DateTime.second) + "</p>";
  html += "<hr><h2>Configurar Primer Dia Semana</h2>";
  html += "<form action='/setday' method='get'>";
  html += "<select name='day'><option value='0'>Domingo primero</option><option value='1'>Lunes primero</option></select>";
  html += "<input type='submit' value='Guardar'></form>";
  html += "<p>Actual: " + String(isMondayFirst ? "Lunes" : "Domingo") + " primero</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSetDay() {
  if (server.hasArg("day")) {
    isMondayFirst = (server.arg("day") == "1");
    config.FirstWeekDay = server.arg("day");
    WriteConfig();
    server.send(200, "text/html", "<html><body><h1>Guardado!</h1><a href='/'>Volver</a></body></html>");
  } else {
    server.send(400, "text/plain", "Error");
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

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== TEST 10 + Config Simple ===");

  EEPROM.begin(512);

  if (!ReadConfig()) {
    Serial.println("Config not found");
  } else {
    Serial.println("Config loaded");
    Serial.print("SSID: ");
    Serial.println(config.ssid);
  }

  isMondayFirst = (config.FirstWeekDay == "1");

  // FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(150);
  Serial.println("FastLED initialized");

  // WiFi
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Calendario", "12345678");
  Serial.println("AP: Calendario");

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
    }
  }

  server.on("/", handleRoot);
  server.on("/setday", handleSetDay);
  server.begin();

  // mDNS
  if (MDNS.begin("perpetualcalendar")) {
    Serial.println("mDNS: perpetualcalendar.local");
  }

  tkSecond.attach(1, ISRsecondTick);
  customWatchdog = millis();

  Serial.println("=== SETUP COMPLETE ===");
}

void pride() {
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
  MDNS.update();
  server.handleClient();

  if (!ntp_response_ok) {
    if (cNTP_Update >= 5) {
      cNTP_Update = 0;
      getNTPtime();
    }
    pride();
  } else {
    // Load events once per hour or on first run
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

      // Re-init FastLED after Google
      FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
      FastLED.setBrightness(150);

      Serial.println("Events loaded!");
    }

    CalendarDisplay(DateTime.year, DateTime.month, DateTime.day);
    EventsDisplay();
    FastLED.show();
  }

  delay(10);
}
