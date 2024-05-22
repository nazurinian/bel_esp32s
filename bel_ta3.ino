
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
  // bool isPlaying = false;
  stream.pauseFirebase(true);
  stream.clear();
  delay(1000);
  stream.pauseFirebase(false);
  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
}

void loop() {
  unsigned long currentMillis = millis();
  int sdStatus = myDFPlayer.readState();

  // Fungsi-fungsi periodik
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

/* 
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

    if (WiFi.status() != WL_CONNECTED)
    {
      lcdMonitor(0, 1);
      Serial.println("Anda belum terhubung ke WiFi.");
      return;
    }  

    if (!internetAvailable)
    {
      lcdMonitor(0, 2);
      Serial.println("Tidak ada koneksi internet.");
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

    Serial.print("SEDANG PUTAR YA GUYS: ");
    Serial.println(sedangMemutarAudio ? "true" : "false");

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
    // if (WiFi.status() != WL_CONNECTED)
    if (WiFi.status() != WL_CONNECTED || !internetAvailable)
    {
      Serial.println("Mencoba menghubungkan ulang sistem dengan WiFi yang terdaftar.");
      wifiSetup();
      return;
    }  

    if (!hariLibur)
    {
      previousMillisB = currentMillis;
      Serial.println(" ");
      if (sdStatus == 0) {
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
      bool isPlaying = false;
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
*/