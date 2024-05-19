
#include "Variables.h"

LiquidCrystal_I2C LCD(0x27, 16, 2);
BluetoothSerial SerialBT;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", gmtOffset_sec);

FirebaseData fbdo;
FirebaseData stream;
// FirebaseData streamPutar;
// FirebaseData streamChoice;

FirebaseAuth auth;
FirebaseConfig config;
DynamicJsonDocument json(10240);

// AsyncWebServer server(81);
DFRobotDFPlayerMini myDFPlayer;

bool signupOK = false;
bool dataIsAvailable = false;
bool hariLibur = false;

const char *apSSID = "BSIB-AP";
const char *apPassword = ""; // config123

unsigned long previousMillisA = 0;
unsigned long previousMillisB = 0;
unsigned long previousMillisResetWifi = 0;
const long interval = 1000;

unsigned long previousMillisSignup = 0;
unsigned long previousMillisTokenRefresh = 0;
unsigned long previousMillisCheckManual = 0;
unsigned long previousMillisJsonData = 0;

int button1State = 0;
bool menuDisplayed = false;
bool choiceMade = false;

bool displayTime = true;
bool displaySchedule = false;
bool displayNextSchedule = false;
bool clearDisplayOffline = true;

int volumeLevel = 0;

// Path jadwal masuk dan keluar pada database
const String seninSampaiKamis = "/jadwal/senin-kamis/";
const String jumat = "/jadwal/jumat/";
const String jamMasuk = "/jam/";
const String menitMasuk = "/menit/";
const String statusMasuk = "/aktif/";

const String putarManual = "/putar-manual";
const String statusPutar = "/putar";
const String pilihanPutar = "/choice";
bool sedangMemutarAudio = false;

int playState = 0; // 0: tidak memutar, 1: memutar pertama, 2: memutar kedua
unsigned long loopPlay = millis();
unsigned long previousLoopCheck = 0; // Untuk melacak waktu sebelumnya
unsigned long intervalCheck = 200;   // Interval untuk memeriksa status DFPlayer

const long gmtOffset_sec = 7 * 3600;
String newSSIDBT = "";
String newPasswordBT = "";

bool lampuStatus = false;
bool startWifiConfig = false;
int button2State = 0;

// Array untuk menyimpan semua jadwal
const int jumlahJadwal = 16;
Waktu jadwal[jumlahJadwal] = {
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

CustomTime currentTime = {0, 0, 0, 0, 0, 0, 0, ""}; // Atur nilainya ke default jika diperlukan

Schedule jadwalSaatIni;
Schedule jadwalSelanjutnya;
String nowOrNext;
String timeOrBel;

/* int cekBelPilihan()
{
  if (Firebase.getInt(fbdo, pilihanPutar))
  {
    int infoPilihanPutar = fbdo.intData();
    return infoPilihanPutar;
  }
  else
  {
    return 0;
  }
}

bool cekBelManual()
{
  if (Firebase.getBool(fbdo, statusPutar))
  {
    bool infoPlay = fbdo.boolData();
    if (infoPlay)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
} */

bool infoPlay;
int infoPilihanPutar;
