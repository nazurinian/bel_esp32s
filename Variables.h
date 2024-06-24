
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Config.h"
#include "Libraries.h"
#include "Structs.h"

extern LiquidCrystal_I2C LCD;

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

extern FirebaseData fbdo;
extern FirebaseData stream;

extern FirebaseAuth auth;
extern FirebaseConfig config;
extern DynamicJsonDocument json;

extern AsyncWebServer server;
extern DFRobotDFPlayerMini myDFPlayer;

extern bool signupOK;
extern bool dataIsAvailable;
extern bool hariLibur;

extern unsigned long previousMillisA;
extern unsigned long previousMillisB;
extern unsigned long previousMillisC;
extern unsigned long pvMillisObservePlayStatus;
extern unsigned long previousLoopCheck;
extern const long interval;

extern bool displayTime;
extern bool displaySchedule;
extern bool displayNextSchedule;
extern bool clearDisplayOffline;

extern int volumeLevel;
extern bool sedangMemutarAudio;
extern int playState;

extern int button1State;
extern bool startWifiConfig;
extern int button2State;
extern bool hotspotStatus;
extern int button3State;

extern bool button1WasPressed;
extern bool button2WasPressed;
extern bool button3WasPressed;
extern unsigned long button1LastPressTime;
extern unsigned long button2LastPressTime;
extern unsigned long button3LastPressTime;
extern const long debounceInterval;

// Array untuk menyimpan semua jadwal
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

extern bool internetAvailable;
extern bool dataFetched;
extern int infoPilihanPutar;
extern bool infoPlay;
extern bool isPlaying;
extern int menitPutar;
extern int jumlahPutar;
extern bool mulaiPutarOnline;

extern int previousSdStatus;
extern int numFilesInFolder1;
extern int numFilesInFolder2;
extern int numFilesInFolder3;
extern int numFilesInFolder4;

#endif