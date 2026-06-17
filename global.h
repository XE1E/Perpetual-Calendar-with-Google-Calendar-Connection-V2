#ifndef GLOBAL_H
#define GLOBAL_H


ESP8266WebServer server(80);							// The Webserver
bool firstStart = true;								// On firststart = true, NTP will try to get a valid time
bool ntp_response_ok = false;
bool manual_time_set = false;
String chipID;
uint8_t WIFI_connected = WL_DISCONNECTED;
bool CFG_saved = false;
uint16_t AdminTimeOutCounter = 0;							// Counter for Disabling the AdminMode
WiFiUDP UDPNTPClient;											// NTP Client
volatile unsigned long UnixTimestamp = 0;	// GLOBALTIME  ( Will be set by NTP)
int cNTP_Update = 0;											// Counter for Updating the time via NTP
Ticker tkSecond;												  // Second - Timer for Updating Datetime Structure
//custom declarations
long absoluteActualTime;
long  customWatchdog;                     // WatchDog to detect main loop blocking. There is a builtin WatchDog to the chip firmare not related to this one

struct strConfig {
  boolean dhcp;                         // 1 Byte - EEPROM 16
  boolean isDayLightSaving;             // 1 Byte - EEPROM 17
  long Update_Time_Via_NTP_Every;       // 4 Byte - EEPROM 18
  long timeZone;                        // 4 Byte - EEPROM 22
  byte  IP[4];                          // 4 Byte - EEPROM 32
  byte  Netmask[4];                     // 4 Byte - EEPROM 36
  byte  Gateway[4];                     // 4 Byte - EEPROM 40
  String ssid;                          // up to 32 Byte - EEPROM 64
  String password;                      // up to 32 Byte - EEPROM 96
  String ntpServerName;                 // up to 32 Byte - EEPROM 128
  String DeviceName;                    // up to 32 Byte - EEPROM 160
  // Application Settings here... from EEPROM 192 up to 639
  String FirstWeekDay;
  String HolidaysScriptID;
  String AnniversariesScriptID;
  String TodosScriptID;
  // Backup WiFi network - EEPROM 520-583
  String ssid2;                         // up to 32 Byte - EEPROM 520
  String password2;                     // up to 32 Byte - EEPROM 552
} config;


//  Auxiliar function to handle EEPROM

void EEPROMWritelong(int address, long value){
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address){
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

// Check the Values is between 0-255
boolean checkRange(String Value){
  if (Value.toInt() < 0 || Value.toInt() > 255)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void WriteStringToEEPROM(int beginaddress, String string){
  char  charBuf[string.length() + 1];
  string.toCharArray(charBuf, string.length() + 1);
  for (size_t t = 0; t < sizeof(charBuf); t++)
  {
    EEPROM.write(beginaddress + t, charBuf[t]);
  }
}

String ReadStringFromEEPROM(int beginaddress) {
  char buffer[33];
  byte counter = 0;
  while (counter < 32) {
    char c = EEPROM.read(beginaddress + counter);
    if (c == 0) break;
    buffer[counter++] = c;
  }
  buffer[counter] = '\0';
  return String(buffer);
}

String ReadLongStringFromEEPROM(int beginaddress) {
  char buffer[97];
  byte counter = 0;
  while (counter < 96) {
    char c = EEPROM.read(beginaddress + counter);
    if (c == 0) break;
    buffer[counter++] = c;
  }
  buffer[counter] = '\0';
  return String(buffer);
}
/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi(){
  Serial.println("Configuring Wifi");
  WiFi.begin (config.ssid.c_str(), config.password.c_str());

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 40) {
    Serial.println("WiFi not connected");
    delay(500);
    timeout++;
  }
  if (timeout >= 40) {
    Serial.println("WiFi connection timeout - continuing anyway");
  }
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ));
  }
}


void WriteConfig(){

  Serial.println("Writing Config");
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');

  EEPROM.write(16, config.dhcp);
  EEPROM.write(17, config.isDayLightSaving);

  EEPROMWritelong(18, config.Update_Time_Via_NTP_Every); // 4 Byte
  EEPROMWritelong(22, config.timeZone); // 4 Byte

  EEPROM.write(32, config.IP[0]);
  EEPROM.write(33, config.IP[1]);
  EEPROM.write(34, config.IP[2]);
  EEPROM.write(35, config.IP[3]);

  EEPROM.write(36, config.Netmask[0]);
  EEPROM.write(37, config.Netmask[1]);
  EEPROM.write(38, config.Netmask[2]);
  EEPROM.write(39, config.Netmask[3]);

  EEPROM.write(40, config.Gateway[0]);
  EEPROM.write(41, config.Gateway[1]);
  EEPROM.write(42, config.Gateway[2]);
  EEPROM.write(43, config.Gateway[3]);

  WriteStringToEEPROM(64, config.ssid);
  WriteStringToEEPROM(96, config.password);
  WriteStringToEEPROM(128, config.ntpServerName);
  WriteStringToEEPROM(160, config.DeviceName);
  // Application Settings here... from EEPROM 192 up to 511 (0 - 511)
  WriteStringToEEPROM(192, config.FirstWeekDay);
  WriteStringToEEPROM(200, config.HolidaysScriptID);
  WriteStringToEEPROM(296, config.AnniversariesScriptID);
  WriteStringToEEPROM(392, config.TodosScriptID);
  // Backup WiFi network
  WriteStringToEEPROM(520, config.ssid2);
  WriteStringToEEPROM(552, config.password2);
  EEPROM.commit();
}

boolean ReadConfig(){
  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    Serial.println("Configuration Found!");
    config.dhcp = 	EEPROM.read(16);
    config.isDayLightSaving = EEPROM.read(17);
    config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte
    config.timeZone = EEPROMReadlong(22); // 4 Byte
    config.IP[0] = EEPROM.read(32);
    config.IP[1] = EEPROM.read(33);
    config.IP[2] = EEPROM.read(34);
    config.IP[3] = EEPROM.read(35);
    config.Netmask[0] = EEPROM.read(36);
    config.Netmask[1] = EEPROM.read(37);
    config.Netmask[2] = EEPROM.read(38);
    config.Netmask[3] = EEPROM.read(39);
    config.Gateway[0] = EEPROM.read(40);
    config.Gateway[1] = EEPROM.read(41);
    config.Gateway[2] = EEPROM.read(42);
    config.Gateway[3] = EEPROM.read(43);
    config.ssid = ReadStringFromEEPROM(64);
    config.password = ReadStringFromEEPROM(96);
    config.ntpServerName = ReadStringFromEEPROM(128);
    config.DeviceName = ReadStringFromEEPROM(160);
    // Application parameters here ... from EEPROM 192 to 511
    config.FirstWeekDay = ReadStringFromEEPROM(192);
    config.HolidaysScriptID = ReadLongStringFromEEPROM(200);
    config.AnniversariesScriptID = ReadLongStringFromEEPROM(296);
    config.TodosScriptID = ReadLongStringFromEEPROM(392);
    // Backup WiFi network
    config.ssid2 = ReadStringFromEEPROM(520);
    config.password2 = ReadStringFromEEPROM(552);
    return true;
  }
  else
  {
    Serial.println("Configuration NOT FOUND!!!!");
    return false;
  }
}

void printConfig(){

  Serial.println("Printing Config");

  Serial.printf("DHCP:%d\n", config.dhcp);
  Serial.printf("DayLight:%d\n", config.isDayLightSaving);

  Serial.printf("NTP update every %ld sec\n", config.Update_Time_Via_NTP_Every); // 4 Byte
  Serial.printf("Timezone %ld\n", config.timeZone); // 4 Byte

  Serial.printf("IP:%d.%d.%d.%d\n", config.IP[0],config.IP[1],config.IP[2],config.IP[3]);
  Serial.printf("Mask:%d.%d.%d.%d\n", config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]);
  Serial.printf("Gateway:%d.%d.%d.%d\n", config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3]);

  Serial.printf("SSID:%s\n", config.ssid.c_str());
  Serial.printf("PWD:%s\n", config.password.c_str());
  Serial.printf("ntp ServerName:%s\n", config.ntpServerName.c_str());
  Serial.printf("Device Name:%s\n", config.DeviceName.c_str());
  Serial.printf("First Day of the Week:%s\n", config.FirstWeekDay.c_str());
  Serial.printf("Holidays Script ID:%s\n", config.HolidaysScriptID.c_str());
  Serial.printf("Anniversaries Script ID:%s\n", config.AnniversariesScriptID.c_str());
  Serial.printf("To-dos Script ID:%s\n", config.TodosScriptID.c_str());
    // Application Settings here... from EEPROM 192 up to 511 (0 - 511)

}

String GetMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.macAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

String GetAPMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.softAPmacAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c){
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

String urldecode(String input) {
  size_t len = input.length();
  char* buffer = new char[len + 1];
  size_t j = 0;
  for (size_t t = 0; t < len; t++) {
    char c = input[t];
    if (c == '+') c = ' ';
    else if (c == '%' && t + 2 < len) {
      c = (h2int(input[t + 1]) << 4) | h2int(input[t + 2]);
      t += 2;
    }
    buffer[j++] = c;
  }
  buffer[j] = '\0';
  String ret(buffer);
  delete[] buffer;
  return ret;
}

#endif
