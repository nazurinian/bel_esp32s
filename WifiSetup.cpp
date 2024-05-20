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
  while (WiFi.status() != WL_CONNECTED && attempt < 20 && internetAvailable)
  {
    if (attempt == 19)
    {
      internetAvailable = false;
    }
    
    delay(500);
    Serial.print(".");
    attempt++;
  }

  // Menonaktifkan Bluetooth
  startWifiConfig = false;
  lampuStatus = false;
  digitalWrite(LED_1_GREEN_PIN, LOW);
  delay(500);

  // Jika koneksi gagal, aktifkan mode server konfigurasi dengan mengganti status hotspot ke True
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nGagal terhubung ke WiFi menggunakan SSID dan password default.");
    // Hapus aja
    // startConfigServer();
    // digitalWrite(LED_2_RED_PIN, LOW);
    // hotspotStatus = true;
    delay(100);
    return;
  }
  else
  {
    Serial.println("\nTerhubung ke WiFi dengan SSID dan password default.");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (Ping.ping("www.google.com")) {
      Serial.println("Ping successful");
      Serial.println("Sistem telah tersambung ke internet");
      internetAvailable = true;
      ntpSetup();
      delay(100);
      firebaseSetup();
    } else {
      Serial.println("Ping failed");
      Serial.println("Sistem tidak terhubung internet saat ini");
      internetAvailable = false;
      return;
    }
  }

  delay(100);
}

// void startConfigServer()
// {
//   Serial.println("\nMemulai hotspot konfigurasi WiFi.");

//   if (AP_PASSWORD == "")
//   {
//     WiFi.softAP(AP_SSID);
//   }
//   else
//   {
//     WiFi.softAP(AP_SSID, AP_PASSWORD);
//   }

//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("Alamat IP AP: ");
//   Serial.println(IP);

//   // Menentukan handle untuk root endpoint ("/")
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
//             {
//     // Membaca isi file HTML dari SPIFFS dan mengirimkannya sebagai respons
//     File file = SPIFFS.open("/index.html", "r");
//     if (file) {
//       request->send(SPIFFS,  "/index.html", "text/html", false);
//     } else {
//       request->send(404, "text/plain", "Halaman tidak ditemukan.");
//     } });

//   // Menentukan handle untuk endpoint "/set-credentials"
//   server.on("/set-credentials", HTTP_POST, [](AsyncWebServerRequest *request)
//             {
//     // Mendapatkan SSID dan password dari formulir yang disubmit
//     String newSSID = request->arg("ssid");
//     String newPassword = request->arg("password");

//     // Menyimpan SSID dan password ke dalam file SPIFFS
//     saveCredentials(newSSID, newPassword);

//     // Mengirim respons bahwa perubahan berhasil
//     request->send(200, "text/plain", "SSID dan password berhasil diperbarui. Perangkat akan direstart.");
//     delay(1000);

//     // Merestart perangkat setelah merubah konfigurasi
//     ESP.restart(); });

//   // Memulai server web
//   server.begin();
// }

// Fungsi untuk memutar bel
void putarBelKelasWiFi(int pilihan) {
  // Implementasi pemutaran bel sesuai pilihan
  Serial.printf("Memutar bel nomor %d\n", pilihan);
}

void startConfigServer() {
  Serial.println("\nMemulai hotspot konfigurasi WiFi.");

  if (AP_PASSWORD == "")
  {
    WiFi.softAP(AP_SSID);
  }
  else
  {
    WiFi.softAP(AP_SSID, AP_PASSWORD);
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Alamat IP AP: ");
  Serial.println(IP);

  // Serial.println("\nMemulai hotspot konfigurasi WiFi.");
  // WiFi.softAP(AP_SSID, AP_PASSWORD);
  // IPAddress IP = WiFi.softAPIP();
  // Serial.print("Alamat IP AP: ");
  // Serial.println(IP);

  // Handle root endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Handle untuk form reset WiFi
  server.on("/reset-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reset-wifi.html", "text/html");
  });

  // Handle untuk set-credentials
  server.on("/set-credentials", HTTP_POST, [](AsyncWebServerRequest *request){
    String newSSID = request->arg("ssid");
    String newPassword = request->arg("password");
    saveCredentials(newSSID, newPassword);
    request->send(200, "text/plain", "SSID dan password berhasil diperbarui. Perangkat akan direstart.");
    delay(1000);
    ESP.restart();
  });

  server.on("/cek-ssid", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan pengecekan SSID
    // Misalnya: String currentSSID = WiFi.SSID();
    String currentSSID = "Your_SSID_Here";  // Ganti dengan implementasi sebenarnya
    request->send(200, "text/plain", "SSID saat ini: \n" + currentSSID);
  });

  server.on("/cek-ip", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan pengecekan IP
    IPAddress ip = WiFi.localIP();
    request->send(200, "text/plain", "IP Address saat ini: \n" + ip.toString());
  });

  // Handle untuk halaman play bell
  server.on("/putar-bel", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/putar-bel.html", "text/html");
  });

  // Handle untuk play bell
  server.on("/selected-bell", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasArg("bell")) {
      int bellChoice = request->arg("bell").toInt();
      putarBelKelasWiFi(bellChoice);
      request->send(200, "text/plain", "Bel diputar.");
    } else {
      request->send(400, "text/plain", "Gagal memutar bel");
    }
  });

  server.on("/hentikan-bel", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan penghentian bel
    Serial.println("Bel dihentikan.");
    request->send(200, "text/plain", "Bel dihentikan.");
  });

  // ------------------------------- Ga Jadi Pakek Bluetooth ------------------------------- 
  server.on("/nonaktifkan-bluetooth", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan nonaktifkan bluetooth
    Serial.println("Bluetooth dinonaktifkan.");
    request->send(200, "text/plain", "Fitur ini belum tersedia"); // Bluetooth dinonaktifkan.
  });
  // ------------------------------- Ga Jadi Pakek Bluetooth ------------------------------- 

  server.on("/restart-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan restart WiFi
    Serial.println("WiFi di-restart.");
    request->send(200, "text/plain", "WiFi di-restart.");
  });

  server.on("/restart-perangkat", HTTP_GET, [](AsyncWebServerRequest *request){
    // Implementasikan restart perangkat
    request->send(200, "text/plain", "Perangkat di-restart.");
    delay(1000);
    ESP.restart();
  });

  // Memulai server web
  server.begin();
}

void startHotspot() {
    button2State = digitalRead(BUTTON_1_PIN);

    if (button2State == LOW && !button2WasPressed) {
        button2WasPressed = true;
        button2LastPressTime = millis();  // Catat waktu saat tombol pertama kali ditekan
    }

    if (button2WasPressed && (millis() - button2LastPressTime >= debounceInterval)) {
        // Jika tombol dilepas atau debounce interval telah berlalu
        if (digitalRead(BUTTON_1_PIN) == HIGH) {
            button2WasPressed = false;

            hotspotStatus = !hotspotStatus;
            digitalWrite(LED_1_GREEN_PIN, hotspotStatus ? HIGH : LOW);

            if (hotspotStatus) {
                startConfigServer();
            } else {
                Serial.println("\nMenonaktifkan hotspot konfigurasi WiFi.");
                WiFi.softAPdisconnect(true);
            }
        }
    }
}
