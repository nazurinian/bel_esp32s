#include "BluetoothSerialControl.h"
#include "Variables.h" // Pastikan semua variabel global dan library lainnya di-include di sini

void startBluetooth()
{
  SerialBT.begin("BSIB-BT"); // Nama perangkat Bluetooth
  Serial.print("\nBluetooth Serial telah diaktifkan");
  delay(1000); // DELAY SETUP 6
}

void stopBluetooth() {
  SerialBT.end(); // Menonaktifkan Bluetooth Serial
  Serial.println("\nBluetooth Serial dinonaktifkan");

  delay(1000);
}

void serialBTMonitor()
{
  button1State = digitalRead(BUTTON1PIN);

  if (!lampuStatus && !startWifiConfig)
  {
    // SerialBT.end(); // Menonaktifkan Bluetooth Serial
    // Serial.println("Bluetooth Serial dinonaktifkan");

    // delay(1000);
    stopBluetooth();
    startWifiConfig = true;
  }

  if (lampuStatus && startWifiConfig)
  {
    bluetoothOptionMenu();
  }

  // Periksa jika tombol ditekan
  if (button1State == LOW)
  { //
    // Tunggu hingga tombol dilepas
    while (digitalRead(BUTTON1PIN) == LOW)
    {
      delay(10);
    }

    // Ubah status lampu
    lampuStatus = !lampuStatus;

    // Update keadaan lampu
    digitalWrite(LED1REDPIN, lampuStatus ? HIGH : LOW);

    if (lampuStatus)
    {
      if (!SerialBT.available())
      {
        startBluetooth();
      }
    }
    else
    {
      stopBluetooth();
      // SerialBT.end(); // Menonaktifkan Bluetooth Serial
      // Serial.println("\nBluetooth Serial dinonaktifkan");

      // delay(1000);
    }
  }
}

void resetCredentials() {
  SerialBT.println(">>> Kosongkan isian untuk menghentikan <<<");
  // Setting ulang SSID
  SerialBT.print(" > Masukkan SSID: ");
  while (!SerialBT.available()) {
    delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  }
  // Baca SSID dari BluetoothSerial
  newSSIDBT = SerialBT.readStringUntil('\n');
  newSSIDBT.trim();

  if (newSSIDBT.length() == 0) {
    SerialBT.println("SSID harus diisi. Silahkan ulangi pilihan 1.");
    return;
  }

  // Setting ulang Password
  SerialBT.print(" > Masukkan Password: (min 8 char)");
  while (!SerialBT.available()) {
    delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  }
  // Baca Password dari BluetoothSerial
  newPasswordBT = SerialBT.readStringUntil('\n');
  newPasswordBT.trim();

  if (newPasswordBT.length() == 0) {
    SerialBT.println("Password harus diisi. Silahkan ulangi pilihan 1.");
    return;
  } else if (newPasswordBT.length() < 8) {
    SerialBT.println("Password minimal 8 karakter. Silahkan ulangi pilihan 1.");
    return;
  }

  // Memastikan terlebih dahulu apakah perubahan sudah sesuai yang diinginkan?
  SerialBT.print("    Apakah sudah yakin? (ya/tidak) ");
  while (!SerialBT.available()) {
    delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  }

  // Baca pilihan "ya" atau "tidak" dari BluetoothSerial
  String confirmChoice = SerialBT.readStringUntil('\n');
  confirmChoice.trim();

  if (confirmChoice.equalsIgnoreCase("ya")) {
    // Simpan SSID dan password ke SPIFFS
    saveCredentials(newSSIDBT, newPasswordBT);
    SerialBT.println("SSID dan password berhasil disimpan.");

    // Coba koneksi WiFi dengan SSID dan password baru
    SerialBT.println(" ");
    SerialBT.println("Mencoba menghubungkan perangkat dengan SSID dan Password baru ...");
    delay(100);
    wifiSetup();

    if (!WiFi.status() == WL_CONNECTED) {
      SerialBT.println("Gagal terhubung ke WiFi dengan SSID dan password baru.");
    }
    
  } else {
    SerialBT.println("Pilihan tidak valid. Silahkan ulangi pilihan 1.");
  }
}

void handleBluetoothMenu(int choice)
{
  switch (choice)
  {
  case 1:
    // Konfigurasi ulang SSID dan Password
    resetCredentials();
    delay(1000);
    break;

  case 2:
    // Cek SSID WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
      SerialBT.println("Anda belum terhubung ke WiFi.");
    }
    else
    {
      SerialBT.println("SSID WiFi saat ini: " + WiFi.SSID());
    }
    delay(1000);
    break;

  case 3:
    // Cek IP Address
    if (WiFi.status() != WL_CONNECTED)
    {
      SerialBT.println("Anda belum terhubung ke WiFi.");
    }
    else
    {
      SerialBT.print("IP Address WiFi saat ini: ");
      SerialBT.println(WiFi.localIP());
    }
    delay(1000);
    break;

  case 4:
    // Putar bel manual
    if (!signupOK)
    {
      SerialBT.print("Belum terhubung dengan server");
      break;
    }

    if (!Firebase.ready())
    {
      SerialBT.println(fbdo.errorReason());
      break;
    }

    Firebase.setBool(fbdo, putarManual + statusPutar, true);
    Firebase.setInt(fbdo, putarManual + pilihanPutar, 1); // MASIH 1 ya blum ada choice nya

    // Tambahkan fungsi choice seperti ganti ssid dan passwordnya

    putarBelManual(true, 1); // PUTAR MANUAL DARI BLUETOOTH, BELUM DIATUR FUNGSI UNTUK LOOP NYA ATAU PILIHANNYA
    delay(1000);
    break;

  case 5:
    // Hentikan pemutaran bel
    if (!signupOK)
    {
      SerialBT.print("Belum terhubung dengan server");
      break;
    }

    if (!Firebase.ready())
    {
      SerialBT.println(fbdo.errorReason());
      break;
    }

    if (!digitalRead(DFPLAYERBUSYPIN))
    {
      SerialBT.print("Menghentikan pemutaran audio");
      Firebase.setBool(fbdo, putarManual + statusPutar, false);
      Firebase.setInt(fbdo, putarManual + pilihanPutar, 0);
      myDFPlayer.stop();
    }
    else
    {
      SerialBT.print("Tidak sedang memutar audio");
    }
    delay(1000);
    break;

  case 8:
    // Menonaktifkan Bluetooth
    startWifiConfig = false;
    lampuStatus = false;
    digitalWrite(LED1REDPIN, LOW);
    stopBluetooth();
    // SerialBT.end();
    // delay(1000);
    break;

  case 9:
    SerialBT.println("\nMencoba menghubungkan perangkat dengan WiFi yang terdaftar ...");
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect(true);
    }
    delay(1000);
    break;

  case 0:
    ESP.restart();
    break;

  default:
    SerialBT.println("Pilihan tidak valid. Silahkan pilih 1-5|9|0.");
    delay(100);
    break;
  }

  SerialBT.println(" ");
}

void bluetoothOptionMenu()
{
  if (SerialBT.connected())
  {
    if (!menuDisplayed)
    { // Menampilkan menu jika belum ditampilkan
      SerialBT.println("Silahkan pilih menu bluetooth:");
      SerialBT.println("------------------------------------------------------");
      SerialBT.println("1: Reset SSID dan Password WiFi");
      SerialBT.println("2: Cek SSID WiFi");
      SerialBT.println("3: Cek IP Address");
      SerialBT.println("4: Putar Bel Manual");
      SerialBT.println("5: Hentikan Pemutaran Bel");
      SerialBT.println("8: Nonaktifkan Bluetooth");
      SerialBT.println("9: Restart WiFi");
      SerialBT.println("0: Restart Perangkat");
      SerialBT.println(" ");
      SerialBT.print("Pilihan: ");
      menuDisplayed = true; // Set menuDisplayed menjadi true setelah menu ditampilkan
    }

    if (!choiceMade && SerialBT.available())
    {
      int choice = SerialBT.parseInt(); // Membaca pilihan menu sebagai integer
      while (SerialBT.available())
      {
        SerialBT.read(); // Baca dan buang data tambahan
      }
      handleBluetoothMenu(choice);
      choiceMade = true; // Set choiceMade menjadi true setelah pilihan dibuat
    }
  }

  // Jika pilihan telah dibuat, reset penanda untuk siklus berikutnya
  if (choiceMade)
  {
    menuDisplayed = false;
    choiceMade = false;
  }
}