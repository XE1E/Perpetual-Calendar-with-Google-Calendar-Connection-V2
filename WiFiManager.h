#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFiMulti.h>

// WiFi Multi instance for multiple network support
ESP8266WiFiMulti wifiMulti;

// WiFi reconnection settings
#define WIFI_RECONNECT_INTERVAL 30000  // Try reconnect every 30 seconds
#define WIFI_MAX_RETRIES 3             // Max retries before giving up temporarily
#define WIFI_CONNECT_TIMEOUT 10000     // Timeout for WiFiMulti connection

unsigned long lastWifiCheck = 0;
int wifiRetryCount = 0;
bool wifiWasConnected = false;
bool wifiMultiInitialized = false;

// Initialize WiFiMulti with configured networks
void setupWiFiMulti() {
    // Add primary network
    if (config.ssid.length() > 0) {
        wifiMulti.addAP(config.ssid.c_str(), config.password.c_str());
        Serial.print("WiFiMulti: Added primary network: ");
        Serial.println(config.ssid);
    }

    // Add backup network if configured
    if (config.ssid2.length() > 0) {
        wifiMulti.addAP(config.ssid2.c_str(), config.password2.c_str());
        Serial.print("WiFiMulti: Added backup network: ");
        Serial.println(config.ssid2);
    }

    wifiMultiInitialized = true;
}

// Connect using WiFiMulti (tries all configured networks)
bool connectWiFiMulti() {
    if (!wifiMultiInitialized) {
        setupWiFiMulti();
    }

    Serial.println("WiFiMulti: Connecting to best available network...");

    if (wifiMulti.run(WIFI_CONNECT_TIMEOUT) == WL_CONNECTED) {
        Serial.print("WiFiMulti: Connected to ");
        Serial.print(WiFi.SSID());
        Serial.print(" IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }

    Serial.println("WiFiMulti: Connection failed");
    return false;
}

void checkWiFiConnection() {
    unsigned long currentMillis = millis();

    // Only check periodically
    if (currentMillis - lastWifiCheck < WIFI_RECONNECT_INTERVAL) {
        return;
    }
    lastWifiCheck = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
        // WiFi is connected
        if (!wifiWasConnected) {
            Serial.print("WiFi connected to: ");
            Serial.println(WiFi.SSID());
            wifiWasConnected = true;
            WIFI_connected = WL_CONNECTED;

            // Restart mDNS after reconnection
            String mdnsName = config.DeviceName;
            mdnsName.replace(" ", "");
            mdnsName.toLowerCase();
            MDNS.begin(mdnsName.c_str());
        }
        wifiRetryCount = 0;
    } else {
        // WiFi is disconnected
        if (wifiWasConnected) {
            Serial.println("WiFi connection lost!");
            wifiWasConnected = false;
            WIFI_connected = WL_DISCONNECTED;
        }

        // Try to reconnect using WiFiMulti
        if (wifiRetryCount < WIFI_MAX_RETRIES) {
            Serial.print("Attempting WiFi reconnection (");
            Serial.print(wifiRetryCount + 1);
            Serial.print("/");
            Serial.print(WIFI_MAX_RETRIES);
            Serial.println(")...");

            if (connectWiFiMulti()) {
                wifiWasConnected = true;
                WIFI_connected = WL_CONNECTED;
                wifiRetryCount = 0;
            } else {
                wifiRetryCount++;
            }
        } else {
            // Reset retry counter after a longer period
            static unsigned long lastRetryReset = 0;
            if (currentMillis - lastRetryReset > 300000) { // 5 minutes
                wifiRetryCount = 0;
                lastRetryReset = currentMillis;
                Serial.println("Resetting WiFi retry counter");
            }
        }
    }
}

void initWiFiManager() {
    setupWiFiMulti();
    if (WiFi.status() == WL_CONNECTED) {
        wifiWasConnected = true;
    }
}

// Get current connected network name
String getConnectedSSID() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.SSID();
    }
    return "Not connected";
}

#endif
