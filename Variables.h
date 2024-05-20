
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Config.h"
#include "Libraries.h"
#include "Structs.h"

extern LiquidCrystal_I2C LCD;
// extern BluetoothSerial SerialBT;

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
extern unsigned long previousMillisResetWifi;
extern unsigned long pvMillisObservePlayStatus;
extern const long interval;

extern int button1State;
extern bool menuDisplayed;
extern bool choiceMade;

extern bool displayTime;
extern bool displaySchedule;
extern bool displayNextSchedule;
extern bool clearDisplayOffline;

extern int volumeLevel;
extern bool sedangMemutarAudio;

extern int playState;
extern unsigned long previousLoopCheck;
extern unsigned long intervalCheck;

extern String newSSIDBT;
extern String newPasswordBT;

extern bool lampuStatus;
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

extern int infoPilihanPutar;
extern bool infoPlay;
extern bool internetAvailable;

#endif