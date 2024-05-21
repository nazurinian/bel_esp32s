#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Arduino.h>
#include <FirebaseESP32.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "NTPSetup.h"
#include "FirebaseSetup.h"
#include "FS.h"
#include "AudioManager.h"
#include "Variables.h"

#include <AsyncTCP.h>
#include <ESP32Ping.h>
#include <ESPAsyncWebServer.h>

void spiffConfig();
void saveCredentials(const String &newSSID, const String &newPassword);
void wifiSetup();
void putarBelKelasWiFi(int pilihan);
void startConfigServer();
void startHotspot(long currentMillis);

#endif
