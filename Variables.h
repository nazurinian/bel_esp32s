
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>
#include "Libraries.h"
#include "Structs.h" // Untuk menggunakan struct Waktu

// Deklarasi variabel global
extern LiquidCrystal_I2C LCD;
extern BluetoothSerial SerialBT;

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern FirebaseData fbdo;
extern FirebaseData stream;
// extern FirebaseData streamPutar;
// extern FirebaseData streamChoice;

extern FirebaseAuth auth;
extern FirebaseConfig config;
extern DynamicJsonDocument json;

// extern AsyncWebServer server;
extern DFRobotDFPlayerMini myDFPlayer;

extern bool signupOK;
extern bool dataIsAvailable;
extern bool hariLibur;

extern const char *apSSID;
extern const char *apPassword;

extern unsigned long previousMillisA;
extern unsigned long previousMillisB;
extern unsigned long previousMillisResetWifi;
extern const long interval;

extern unsigned long previousMillisSignup;
extern unsigned long previousMillisTokenRefresh;
extern unsigned long previousMillisCheckManual;
extern unsigned long previousMillisJsonData;

extern int button1State;
extern bool menuDisplayed;
extern bool choiceMade;

extern bool displayTime;
extern bool displaySchedule;
extern bool displayNextSchedule;
extern bool clearDisplayOffline;

extern int volumeLevel;

// Path jadwal masuk dan keluar pada database
extern const String seninSampaiKamis;
extern const String jumat;
extern const String jamMasuk;
extern const String menitMasuk;
extern const String statusMasuk;

extern const String putarManual;
extern const String statusPutar;
extern const String pilihanPutar;
extern bool sedangMemutarAudio;

extern int playState;
extern unsigned long loopPlay;
extern unsigned long previousLoopCheck;
extern unsigned long intervalCheck;

extern const long gmtOffset_sec;
extern String newSSIDBT;
extern String newPasswordBT;

extern bool lampuStatus;
extern bool startWifiConfig;
extern int button2State;

// Array untuk menyimpan semua jadwal
extern const int jumlahJadwal;
extern Waktu jadwal[];

// Fungsi untuk mendapatkan nama hari
extern String jamTitles[];
extern String _formathm(int value);
extern String readSSID();
extern String readPassword();
extern CustomTime currentTime;

extern Schedule jadwalSaatIni;
extern Schedule jadwalSelanjutnya;
extern String nowOrNext;
extern String timeOrBel;

// extern int cekBelPilihan();
// extern bool cekBelManual();

extern int infoPilihanPutar;
extern bool infoPlay;

#endif