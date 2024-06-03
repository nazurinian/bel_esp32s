
#include "Variables.h"

LiquidCrystal_I2C LCD(0x27, 16, 2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", GMT_OFFSET_SEC);

FirebaseData fbdo;
FirebaseData stream;

FirebaseAuth auth;
FirebaseConfig config;
DynamicJsonDocument json(10240);

AsyncWebServer server(80);
DFRobotDFPlayerMini myDFPlayer;

bool signupOK = false;
bool dataIsAvailable = false;
bool hariLibur = false;

unsigned long previousMillisA = 0;
unsigned long previousMillisB = 0;
unsigned long previousMillisC = 0;
unsigned long pvMillisObservePlayStatus = 0;
unsigned long previousLoopCheck = 0; // Untuk melacak waktu sebelumnya
const long interval = 1000;

bool displayTime = true;
bool displaySchedule = false;
bool displayNextSchedule = false;
bool clearDisplayOffline = true;

int volumeLevel = 0;
bool sedangMemutarAudio = false;
int playState = 0; // 0: tidak memutar, 1: memutar pertama, 2: memutar kedua

int button1State = 0;
bool startWifiConfig = false;
int button2State = 0;
bool hotspotStatus = false;
int button3State = 0;

bool button1WasPressed = false;
bool button2WasPressed = false;
bool button3WasPressed = false;
unsigned long button1LastPressTime = 0;
unsigned long button2LastPressTime = 0;
unsigned long button3LastPressTime = 0;
const long debounceInterval = 10; // interval debounce dalam milidetik

// Array untuk menyimpan semua jadwal
Waktu jadwal[JUMLAH_JADWAL] = {
    {7, 10},  // 1 (Literasi Pagi)
    {7, 30},  // 2
    {8, 0},   // 3
    {8, 30},  // 4
    {9, 0},   // 5
    {9, 30},  // 6 (Istirahat)
    {10, 0},  // 7
    {10, 30}, // 8
    {11, 0},  // 9
    {11, 30}, // 10
    {12, 0},  // 11 (Sholat Jum'at)
    {12, 25}, // 12 (Sholat Jum'at)
    {12, 45}, // 13 (Istirahat)
    {14, 0},  // 14
    {14, 30}, // 15
    {15, 0}   // 16 + Pulang
};

String jamTitles[] = {
    "Literasi",
    "Jam ke-1",
    "Jam ke-2",
    "Jam ke-3",
    "Jam ke-4",
    "Istirahat 1",
    "Jam ke-5",
    "Jam ke-6",
    "Jam ke-7",
    "Jam ke-8",
    "Jam ke-9",
    "Jam ke-10",
    "Istirahat 2",
    "Jam ke-11",
    "Jam ke-12",
    "Jam pulang"};

String _formathm(int value)
{
  if (value < 0)
    return "--";
  if (value >= 0 && value < 10)
    return "0" + String(value);
  return String(value);
}

String readSSID()
{
  File ssidFile = SPIFFS.open("/ssid.txt");
  if (ssidFile)
  {
    String ssid = ssidFile.readString();
    ssidFile.close();
    return ssid;
  }
  else
  {
    Serial.println("Gagal membaca SSID.");
    return "";
  }
}

String readPassword()
{
  File passwordFile = SPIFFS.open("/password.txt");
  if (passwordFile)
  {
    String password = passwordFile.readString();
    passwordFile.close();
    return password;
  }
  else
  {
    Serial.println("Gagal membaca password.");
    return "";
  }
}

CustomTime currentTime = {0, 0, 0, 0, 0, 0, 0, ""};

Schedule jadwalSaatIni;
Schedule jadwalSelanjutnya;
String nowOrNext;
String timeOrBel;

bool internetAvailable = true;
bool dataFetched = false;
int infoPilihanPutar;
bool infoPlay;
bool isPlaying = false;
int menitPutar = 0;
int jumlahPutar = 0;
