
#include <Arduino.h>
#include "Libraries.h"
#include "Config.h"
#include "Structs.h"
#include "Variables.h"
#include "Monitors.h"
#include "AudioManager.h"
#include "FirebaseDataFetch.h"
#include "BluetoothSerialControl.h"
#include "WifiSetup.h"
#include "FirebaseSetup.h"
#include "FirebaseInitialSetup.h"
#include "DFPlayerSetup.h"
#include "CustomTimeUtils.h"

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
  // setupBluetooth(); // DELAY SETUP 6 (ada delay 1 menit didalemnya)
  // startBluetooth();
  // stopBluetooth();

  // IO Setup
  pinMode(BUTTON_1_PIN, INPUT_PULLUP); // Bluetooth 
  pinMode(BUTTON_2_PIN, INPUT_PULLUP); // WiFi Manager
  pinMode(LED_PIN, OUTPUT);            // Lampu biru bawaan ESP32
  pinMode(LED_1_GREEN_PIN, OUTPUT);    // Lampu merah ESP32
  pinMode(LED_2_RED_PIN, OUTPUT);      // Lampu hijau ESP32
  pinMode(BUTTON_3_PIN, INPUT_PULLUP); // Tombol Stop Play
  pinMode(LED_3_COLOR_PIN, OUTPUT);    // Lampu hijau ESP32
}

void loop()
{
  unsigned long currentMillis = millis();
  int sdStatus = myDFPlayer.readState();
  startHotspot(currentMillis);
  // serialBTMonitor(currentMillis);
  stopAudioPlay(currentMillis);

  // Update tiap detik
  if (currentMillis - previousMillisA >= interval)
  {
    previousMillisA = currentMillis;
    timeClient.update();
    currentTime = getCurrentTime(timeClient);

    Serial.println(" ");
    // volumeControl();

    // if (WiFi.status() != WL_CONNECTED)
    if (!internetAvailable)
    {
      lcdMonitor(0, 1);
      // if (currentMillis - previousMillisResetWifi >= (interval * 10))
      // {
      //   previousMillisResetWifi = currentMillis;
      //   Serial.println("Anda belum terhubung ke WiFi.");
      wifiSetup();
      // }
      return;
    }

    if (!signupOK && internetAvailable)
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

    if (!Firebase.ready())
    {
      lcdMonitor(0, 3);
      Serial.println(fbdo.errorReason());
      return;
    }

    // After calling stream.keepAlive, now we can track the server connecting status
    if (!stream.httpConnected())
    {
      // Server was disconnected!
      lcdMonitor(0, 3);
      Serial.println("Gagal melakukan streaming data pemutaran di server");
      return;
    }

    if (sdStatus == 0) {
      lcdMonitor(0, 4);
      return;
    } 
    
    cekPemutaranManualLebih1x(currentMillis);
    
    Serial.print("Info Pilihan Putar: ");
    Serial.println(infoPilihanPutar);
    Serial.print("Info Putar Manual: ");
    Serial.println(infoPlay ? "true" : "false");

    putarBelManual(infoPlay, infoPilihanPutar); // PUTAR MANUAL DARI ONLINE ?? INI STUCK
    delay(100); // DELAY LOOP 4 ?? Ada kemungkinan karena delay di dalam kode nonblocking ini

    if (dataIsAvailable)
    {
      putarBelOtomatis(json);
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

    // // biar mudah di true aja di onlinenya biar bagian ini ga nyetop pemutaran, kalau di buat gini bisa juga cuman ntr ga bisa stop lagi, ga bisa stop pake onlen
    // gini bisa sbenernya, bisa cuman ya ga bisa ngecek yg onlen hhe, kalau paka yg dibawah ini wajib di letakan sebelum yg onlen, klo ga pake if ini taruh setelahnya
    // if (!sedangMemutarAudio) {
    //   putarBelManual(infoPlay, infoPilihanPutar); // PUTAR MANUAL DARI ONLINE ?? INI STUCK
    //   delay(100); // DELAY LOOP 4 ?? Ada kemungkinan karena delay di dalam kode nonblocking ini
    // }

    // digitalWrite(LED_3_COLOR_PIN, sedangMemutarAudio ? HIGH : LOW);
    digitalWrite(LED_2_RED_PIN, sedangMemutarAudio ? HIGH : LOW); // Pinjem punya wifi manager dlu

    serialMonitor();
    lcdMonitor(1);
  }

  if (infoPlay)
  {
    if (currentMillis - pvMillisObservePlayStatus >= (interval * 300)) // 5 menit cek perubahan pemutar manual di firebase
    {
        pvMillisObservePlayStatus = currentMillis;
        setDisablePutarManual();
    }
  }

  // Update tiap 10 detik
  if (currentMillis - previousMillisB >= (interval * 10))
  {
    if (!hariLibur)
    {
      previousMillisB = currentMillis;
      Serial.println(" ");
      if (sdStatus == 0) {
        clearDisplayOffline = false;
        lcdMonitor(0, 4);
        return;
      } 
      displayTime = !displayTime;
      getJsonData();
      delay(100); // DELAY LOOP 7
    }
  }

  // Clear memori tiap 2 Jam
  if (currentMillis - previousMillisC >= (interval * 7200)) // 2 Jam sekali clear memori stream
  {
    if (!hariLibur)
    {
      previousMillisC = currentMillis;
      // To pause stream
      stream.pauseFirebase(true);
      stream.clear(); // close session and release memory

      delay(1000); // DELAY LOOP 7

      // To resume stream with callback
      stream.pauseFirebase(false);
      Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
    }
  }
}