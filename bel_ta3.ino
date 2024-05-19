
#include <Arduino.h>
#include "Libraries.h"
#include "Config.h"
#include "models/Structs.h"
#include "models/Variables.h"
#include "controllers/Monitors.h"
#include "controllers/AudioManager.h"
#include "controllers/FirebaseDataFetch.h"
#include "controllers/BluetoothSerialControl.h"
#include "setups/WifiSetup.h"
#include "setups/FirebaseSetup.h"
#include "setups/DFPlayerSetup.h"
#include "utils/CustomTimeUtils.h"

void setup()
{
  // Serial Setup
  Serial.begin(115200);
  delay(100); // DELAY SETUP 1

  // LCD Setup
  LCD.init();
  LCD.backlight(); // open the backlight
  LCD.home();
  delay(100); // DELAY SETUP 2

  // DFPlayer Setup
  setupDFPlayer();
  delay(100); // DELAY SETUP 4

  // WiFi Setup -> NTPSetup Setup && Firebase Setup
  wifiSetup();
  delay(100); // DELAY SETUP 5

  // Bluetooth Setup
  startBluetooth(); // DELAY SETUP 6 (ada delay 1 menit didalemnya)

  // IO Setup
  pinMode(BUTTON1PIN, INPUT_PULLUP);
  pinMode(BUTTON2PIN, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);       // Lampu biru bawaan ESP32
  pinMode(LED1REDPIN, OUTPUT);   // Lampu merah ESP32
  pinMode(LED2GREENPIN, OUTPUT); // Lampu hijau ESP32
}

void loop()
{
  unsigned long currentMillis = millis();
  // startHotspot();
  serialBTMonitor();

  // Update tiap detik
  if (currentMillis - previousMillisA >= interval)
  {
    previousMillisA = currentMillis;
    timeClient.update();
    currentTime = getCurrentTime(timeClient);

    Serial.println(" ");
    // volumeControl();

    // ----------------------- DILUAR -----------------------

    if (WiFi.status() != WL_CONNECTED)
    {
      lcdMonitor(0, 1);
      if (currentMillis - previousMillisResetWifi >= (interval * 10))
      {
        previousMillisResetWifi = currentMillis;
        Serial.println("Anda belum terhubung ke WiFi.");
        wifiSetup();
      }
      return;
    }

    if (!signupOK)
    {
      ntpSetup();
      delay(100); // DELAY LOOP 1
      firebaseSetup();
      delay(100); // DELAY LOOP 2
      return;
    }

    if (Firebase.isTokenExpired())
    {
      Firebase.refreshToken(&config);
      Serial.println("Refresh token");
      delay(100); // DELAY LOOP 3
      return;
    }

    // ----------------------- DILUAR -----------------------

    if (!Firebase.ready())
    {
      lcdMonitor(0, 3);
      Serial.println(fbdo.errorReason());
      return;
    }

    cekPemutaranManualLebih1x(); // ERROR MULAI DARI BAWAH SINI
    Serial.print("Info Pilihan Putar: ");
    Serial.println(infoPilihanPutar);
    Serial.print("Info Putar Manual: ");
    Serial.println(infoPlay ? "true" : "false");
    // Serial.println(cekBelPilihan()); // INI MUNCUL
    // putarBelManual(cekBelManual(), cekBelPilihan()); // PUTAR MANUAL DARI ONLINE ?? INI STUCK
    delay(100);                                      // DELAY LOOP 4 ?? Ada kemungkinan karena delay di dalam kode nonblocking ini

    if (dataIsAvailable)
    {
      // putarBelOtomatis(json);
      delay(100); // DELAY LOOP 5
    }
    else
    {
      if (hariLibur)
      {
        Serial.println("Hari ini adalah hari libur, Tidak ada jadwal bel yang aktif");
      }
      else
      {
        getJsonData();
        delay(100); // DELAY LOOP 6
      }
    }

    serialMonitor();
    lcdMonitor(1);
  }

  // Update tiap 10 detik
  if (currentMillis - previousMillisB >= (interval * 10))
  {
    if(!hariLibur) {
      previousMillisB = currentMillis;
      displayTime = !displayTime;
      Serial.println(" ");

      // To pause stream
      // stream.pauseFirebase(true);
      // stream.clear(); // close session and release memory
      getJsonData();
      delay(100); // DELAY LOOP 7
      
      // To resume stream with callback
      // stream.pauseFirebase(false);
      // Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
    }
  }
}