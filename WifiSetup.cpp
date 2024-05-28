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

// Fungsi untuk mengecek koneksi internet dengan ping
void checkInternetConnection(bool onSetup)
{
  if (Ping.ping("www.google.com"))
  {
    Serial.println("Ping successful");
    Serial.println("Sistem telah tersambung ke internet");
    internetAvailable = true;
    dataFetched = true;
    startWifiConfig = false;

    if (onSetup)
    {
      ntpSetup();
      delay(100);
      firebaseSetup();
    }
    else
    {
      timeClient.forceUpdate();
    }
  }
  else
  {
    Serial.println("Ping failed");
    Serial.println("Sistem tidak terhubung internet saat ini");
    internetAvailable = false;
    dataFetched = false;
  }
}

void wifiSetup()
{
  // Inisialisasi SPIFFS
  spiffConfig();

  Serial.print("Connecting to WiFi : ");
  Serial.println(readSSID().c_str());

  // Coba koneksi ke WiFi dengan SSID dan password default
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(readSSID().c_str(), readPassword().c_str());
  }

  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 10)
  {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  // Jika koneksi gagal, aktifkan mode server konfigurasi dengan mengganti status hotspot ke True
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nGagal terhubung ke WiFi menggunakan SSID dan password default.");

    if (!hotspotStatus && !startWifiConfig)
    {
      startConfigServer();
      startWifiConfig = true;
      hotspotStatus = true;
      digitalWrite(LED_1_GREEN_PIN, HIGH);
    }

    delay(100);
    return;
  }
  else
  {
    Serial.println("\nTerhubung ke WiFi dengan SSID dan password default.");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    digitalWrite(LED_1_GREEN_PIN, LOW);
    delay(100);

    checkInternetConnection(true);
  }

  delay(100);
}

// Fungsi untuk memutar bel
void putarBelKelasWiFi(int pilihan)
{
  // Implementasi pemutaran bel sesuai pilihan
  Serial.printf("Memutar bel nomor %d\n", pilihan);
}

void startConfigServer()
{
  // Pengaturan jaringan untuk hotspot
  IPAddress local_IP(10, 0, 0, 1);    // Alamat IP yang ingin kamu tetapkan untuk hotspot
  IPAddress gateway(10, 0, 0, 1);     // Alamat IP gateway (biasanya sama dengan alamat IP hotspot)
  IPAddress subnet(255, 255, 255, 0); // Subnet mask

  Serial.println("\nMemulai hotspot konfigurasi WiFi.");

  // Mengonfigurasi IP statis untuk hotspot
  if (!WiFi.softAPConfig(local_IP, gateway, subnet))
  {
    Serial.println("Gagal mengonfigurasi IP statis untuk hotspot");
  }

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

  // Handle root endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  // Handle untuk form reset WiFi
  server.on("/reset-wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/reset-wifi.html", "text/html"); });

  // Handle untuk set-credentials
  server.on("/set-credentials", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String newSSID = request->arg("ssid");
    String newPassword = request->arg("password");
    saveCredentials(newSSID, newPassword);
    request->send(200, "text/plain", "SSID dan password berhasil diperbarui. Perangkat akan direstart.");
    delay(1000);
    ESP.restart(); });

  server.on("/cek-ssid", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Implementasikan pengecekan SSID
    String currentSSID = readSSID().c_str();  // Ganti dengan implementasi sebenarnya
    request->send(200, "text/plain", "SSID saat ini: \n" + currentSSID); });

  server.on("/cek-ip", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Implementasikan pengecekan IP
    IPAddress ip = WiFi.localIP();
    request->send(200, "text/plain", "IP Address saat ini: \n" + ip.toString()); });

  // Handle untuk halaman play bell
  server.on("/putar-bel", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/putar-bel.html", "text/html"); });

  // Handle untuk play bell
  server.on("/selected-bell", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    if (request->hasArg("bell")) {
      int bellChoice = request->arg("bell").toInt();
      putarBelKelasWiFi(bellChoice);

      // Putar bel manual
      String message;
      if (!signupOK)
      {
        message = "Belum terhubung dengan server";
        request->send(200, "text/plain", message);
        return;
      }

      if (!Firebase.ready())
      {
        message = fbdo.errorReason();
        request->send(200, "text/plain", message);
        return;
      }

      putarBelManual(true, bellChoice); 

      request->send(200, "text/plain", "Memutar Bel pilihan ke-" + String(bellChoice));
      delay(1000);
    } else {
      request->send(400, "text/plain", "Gagal memutar bel");
    } });

  server.on("/hentikan-bel", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Implementasikan penghentian bel
    String message;
    if (!signupOK)
    {
      message = "Belum terhubung dengan server";
      request->send(200, "text/plain", message);
      return;
    }

    if (!Firebase.ready())
    {
      message = fbdo.errorReason();
      request->send(200, "text/plain", message);
      return;
    }

    if (!digitalRead(DFPLAYER_BUSY_PIN))
    {
      message = "Pemutaran bel dihentikan";
      sedangMemutarAudio = false;
      isPlaying = false;
      Serial.println(message);
      setBelKelasTrue(false, 0);
      myDFPlayer.stop();
    }
    else
    {
      message = "Tidak sedang memutar audio";
      Serial.println(message);
    }
    
    request->send(200, "text/plain", message); });

  server.on("/restart-wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Implementasikan restart WiFi
    Serial.println("\nMencoba menghubungkan perangkat dengan WiFi yang terdaftar ...");
    if (WiFi.status() == WL_CONNECTED)
    {
      internetAvailable = true;
      internetAvailable = true;
      WiFi.disconnect(true);
      delay(2000);
    }
    request->send(200, "text/plain", "Merestart WiFi sistem"); });

  server.on("/restart-perangkat", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Implementasikan restart perangkat
    request->send(200, "text/plain", "Perangkat di-restart");
    delay(100);
    ESP.restart(); });

  // Memulai server web
  server.begin();
}

void startHotspot(long currentMillis)
{
  button2State = digitalRead(BUTTON_1_PIN);

  if (button2State == LOW && !button2WasPressed)
  {
    button2WasPressed = true;
    button2LastPressTime = currentMillis; // Catat waktu saat tombol pertama kali ditekan
  }

  if (button2WasPressed && (currentMillis - button2LastPressTime >= debounceInterval))
  {
    // Jika tombol dilepas atau debounce interval telah berlalu
    if (digitalRead(BUTTON_1_PIN) == HIGH)
    {
      button2WasPressed = false;

      hotspotStatus = !hotspotStatus;
      digitalWrite(LED_1_GREEN_PIN, hotspotStatus ? HIGH : LOW);

      if (hotspotStatus)
      {
        startConfigServer();
      }
      else
      {
        Serial.println("\nMenonaktifkan hotspot konfigurasi WiFi.");
        WiFi.softAPdisconnect(true);
      }
    }
  }
}
