#include "WifiSetup.h"

void spiffConfig()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("Gagal menginisialisasi SPIFFS.");
    return;
  }
}

void saveCredentials(const String &newSSID, const String &newPassword)
{
  // Menyimpan SSID ke file di SPIFFS
  File ssidFile = SPIFFS.open("/ssid.txt", "w");
  if (ssidFile)
  {
    ssidFile.print(newSSID);
    ssidFile.close();
    Serial.println("SSID berhasil disimpan.");
  }
  else
  {
    Serial.println("Gagal menyimpan SSID.");
  }

  // Menyimpan password ke file di SPIFFS
  File passwordFile = SPIFFS.open("/password.txt", "w");
  if (passwordFile)
  {
    passwordFile.print(newPassword);
    passwordFile.close();
    Serial.println("Password berhasil disimpan.");
  }
  else
  {
    Serial.println("Gagal menyimpan password.");
  }
}

void wifiSetup()
{
  // Inisialisasi SPIFFS
  spiffConfig();

  Serial.print("Connecting to WiFi : ");
  Serial.println(readSSID().c_str());

  // Coba koneksi ke WiFi dengan SSID dan password default
  WiFi.begin(readSSID().c_str(), readPassword().c_str());
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20)
  {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  // Menonaktifkan Bluetooth
  startWifiConfig = false;
  lampuStatus = false;
  digitalWrite(LED1REDPIN, LOW);

  // Jika koneksi gagal, aktifkan mode server konfigurasi dengan mengganti status hotspot ke True
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nGagal terhubung ke WiFi menggunakan SSID dan password default.");
    delay(100);
  }
  else
  {
    Serial.println("\nTerhubung ke WiFi dengan SSID dan password default.");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    ntpSetup();
    delay(100);
    firebaseSetup();
  }

  delay(100);
}

// void startConfigServer() {
//   Serial.println("\nMemulai hotspot konfigurasi WiFi.");

//   // Mengatur mode AP
//   // Mode simple tanpa password (Hanya SSID)
//   if(apSSID == ""){
//     Serial.println("SSID Kosong, SSID Default: WiFi Manager");
//     apSSID = "WiFi Manager";
//   }

//   if(apPassword == ""){
//     WiFi.softAP(apSSID);
//   }
//   else{
//     WiFi.softAP(apSSID, apPassword);
//   }

//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("Alamat IP AP: ");
//   Serial.println(IP);

//   // Menentukan handle untuk root endpoint ("/")
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     // Membaca isi file HTML dari SPIFFS dan mengirimkannya sebagai respons
//     File file = SPIFFS.open("/index.html", "r");
//     if (file) {
//       request->send(SPIFFS,  "/index.html", "text/html", false);
//     } else {
//       request->send(404, "text/plain", "Halaman tidak ditemukan.");
//     }
//   });

//   // Menentukan handle untuk endpoint "/set-credentials"
//   server.on("/set-credentials", HTTP_POST, [](AsyncWebServerRequest *request) {
//     // Mendapatkan SSID dan password dari formulir yang disubmit
//     String newSSID = request->arg("ssid");
//     String newPassword = request->arg("password");

//     // Menyimpan SSID dan password ke dalam file SPIFFS
//     saveCredentials(newSSID, newPassword);

//     // Mengirim respons bahwa perubahan berhasil
//     request->send(200, "text/plain", "SSID dan password berhasil diperbarui. Perangkat akan direstart.");
//     delay(1000);

//     // Merestart perangkat setelah merubah konfigurasi
//     ESP.restart();
//   });

//   // Memulai server web
//   server.begin();
// }

// void startHotspot() {
//   button2State = digitalRead(BUTTON2PIN );
//   if (button2State == LOW) {
//     while (button2State == LOW) {
//       delay(10);
//     }

//     hotspotStatus = !hotspotStatus;
//     digitalWrite(LED2GREENPIN, hotspotStatus ? HIGH : LOW);

//     if (hotspotStatus) {
//       startConfigServer();
//       delay(100);
//     } else {
//       Serial.println("\nMenonaktifkan hotspot konfigurasi WiFi.");
//       WiFi.softAPdisconnect(true);
//       delay(100);
//     }
//   }
// }