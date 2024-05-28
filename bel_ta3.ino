
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
  LCD.backlight();
  LCD.home();
  delay(100); // DELAY SETUP 2

  // DFPlayer Setup
  setupDFPlayer();
  delay(100); // DELAY SETUP 4

  // WiFi Setup -> NTPSetup Setup && Firebase Setup
  wifiSetup();
  delay(100); // DELAY SETUP 5

  // IO Setup
  pinMode(BUTTON_1_PIN, INPUT_PULLUP); // Bluetooth 
  pinMode(BUTTON_2_PIN, INPUT_PULLUP); // WiFi Manager
  pinMode(LED_PIN, OUTPUT);            // Lampu biru bawaan ESP32
  pinMode(LED_1_GREEN_PIN, OUTPUT);    // Lampu merah ESP32
  pinMode(LED_2_RED_PIN, OUTPUT);      // Lampu hijau ESP32
  pinMode(BUTTON_3_PIN, INPUT_PULLUP); // Tombol Stop Play
  pinMode(LED_3_COLOR_PIN, OUTPUT);    // Lampu hijau ESP32
}

bool checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    lcdMonitor(0, 1);
    Serial.println("Anda belum terhubung ke WiFi.");
    return false;
  }
  return true;
}

bool checkInternetAvailability() {
  if (!internetAvailable) {
    lcdMonitor(0, 2);
    Serial.println("Tidak ada koneksi internet.");
    return false;
  }
  return true;
}

bool checkFirebaseSignup() {
  if (!signupOK && internetAvailable) {
    ntpSetup();
    delay(100);
    firebaseSetup();
    delay(100);
    return false;
  }
  return true;
}

bool refreshFirebaseToken() {
  if (Firebase.isTokenExpired()) {
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
    delay(100);
    return false;
  }
  return true;
}

bool checkFirebaseReady() {
  if (!Firebase.ready()) {
    lcdMonitor(0, 3);
    Serial.println(fbdo.errorReason());
    return false;
  }
  return true;
}

bool checkFirebaseStreaming() {
  if (!stream.httpConnected()) {
    lcdMonitor(0, 3);
    Serial.println("Gagal melakukan streaming data pemutaran di server");
    return false;
  }
  return true;
}

bool checkSDStatus(int sdStatus) {
  if (sdStatus == 0) {
    lcdMonitor(0, 4);
    return true;
  }
  return false;
}

void displayPlaybackInfo() {
  Serial.print("Info Pilihan Putar: ");
  Serial.println(infoPilihanPutar);
  Serial.print("Info Putar Manual: ");
  Serial.println(infoPlay ? "true" : "false");

  Serial.print(" ----- SEDANG PUTAR YA GUYS: ");
  Serial.print(sedangMemutarAudio ? "true" : "false");
  Serial.println(" ----- ");
}

void handleAutomaticPlayback() {
  if (dataIsAvailable) {
    putarBelOtomatis(json);
    delay(100);
  } else {
    if (hariLibur) {
      Serial.println("Hari ini adalah hari libur, Tidak ada jadwal bel yang aktif");
    } else {
      getJsonData();
      delay(100);
    }
  }

  digitalWrite(LED_2_RED_PIN, sedangMemutarAudio ? HIGH : LOW);
}

void clearFirebaseStream() {
  stream.pauseFirebase(true);
  stream.clear();
  delay(1000);
  stream.pauseFirebase(false);
  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
}

void loop() {
  unsigned long currentMillis = millis();
  int sdStatus = myDFPlayer.readState();

  startHotspot(currentMillis);
  stopAudioPlay(currentMillis);

  // Update setiap detik
  if (currentMillis - previousMillisA >= interval) {
    previousMillisA = currentMillis;
    timeClient.update();
    currentTime = getCurrentTime(timeClient);

    Serial.println(" ");
    
    if (!checkWiFiConnection()) return;
    if (!checkInternetAvailability()) return;
    if (!checkFirebaseSignup()) return;
    if (!refreshFirebaseToken()) return;
    if (!checkFirebaseReady()) return;
    if (!checkFirebaseStreaming()) return;
    if (checkSDStatus(sdStatus)) return;

    cekPemutaranManualLebih1x(currentMillis);

    displayPlaybackInfo();
    putarBelManual(infoPlay, infoPilihanPutar);
    delay(100);

    handleAutomaticPlayback();

    serialMonitor();
    lcdMonitor(1);
  }

  // Update setiap 10 detik
  if (currentMillis - previousMillisB >= (interval * 10)) {
    if (WiFi.status() != WL_CONNECTED || !internetAvailable)
    {
      Serial.println("Mencoba menghubungkan ulang sistem dengan WiFi yang terdaftar.");
      wifiSetup();
      return;
    }  

    if (!hariLibur) {
      previousMillisB = currentMillis;
      Serial.println(" ");
      if (checkSDStatus(sdStatus)) return;
      displayTime = !displayTime;
      if (!dataFetched && dataIsAvailable){
        checkInternetConnection(false);
        return;
      }
      getJsonData();
      delay(100);
    }
  }

  // Periksa status pemutaran manual setiap 5 menit
  if (infoPlay)
  {
    if (currentMillis - pvMillisObservePlayStatus >= (interval * 300))
    {
        pvMillisObservePlayStatus = currentMillis;
        // setDisablePutarManual();
        setBelKelasTrue(false, 0);
    }
  }

  // Clear memori setiap 2 jam
  if (currentMillis - previousMillisC >= (interval * 7200)) {
    if (!hariLibur) {
      previousMillisC = currentMillis;
      clearFirebaseStream();
    }
  }
}
