#include "BluetoothSerialControl.h"
#include "Variables.h" // Pastikan semua variabel global dan library lainnya di-include di sini

void startBluetooth()
{
  // Serial.print("\nBluetooth Serial sedang diaktifkan...");
  // int attempt = 0;
  // while (!SerialBT.begin("BSIB-BTs") && attempt < 20)
  // {
  //   delay(500);
  //   Serial.print(".");
  //   attempt++;
  // }
  // if (!SerialBT.begin("BSIB-BTs"))
  // {
  //   Serial.println("\nBluetooth Serial gagal diaktifkan");
  // }
  
  // delay(100); // DELAY SETUP 6
}

void stopBluetooth() {
  // SerialBT.end(); // Menonaktifkan Bluetooth Serial
  // Serial.println("\nBluetooth Serial dinonaktifkan");

  // delay(100);
}

//--------------------------------------------------------------------------------------------------------------------
// void setupBluetooth() {
//   // Menonaktifkan Bluetooth
//   startWifiConfig = false;
//   btStatus = false;
//   digitalWrite(LED_1_GREEN_PIN, LOW);

//   startBluetooth();
//   stopBluetooth();
  
//   // if (!SerialBT.begin("BSIB-BT")) { // Nama perangkat Bluetooth
//   //   Serial.println("\nBluetooth Serial gagal diaktifkan");
//   //   btStatus == false;
//   // } else {
//   //   Serial.println("\nBluetooth Serial sedang diaktifkan...");
//   //   // Tunggu hingga Bluetooth benar-benar aktif
//   //   unsigned long startTime = millis();
//   //   while (!SerialBT.hasClient()) {
//   //     // Anda dapat menambahkan timeout jika diperlukan
//   //     if (millis() - startTime > 5000) { // Timeout setelah 5 detik
//   //       Serial.println("\nBluetooth Serial gagal diaktifkan dalam waktu 5 detik");
//   //       return;
//   //     }
//   //     delay(100); // Delay singkat untuk mencegah loop terlalu cepat
//   //   }
//   //   Serial.println("\nBluetooth Serial telah diaktifkan");
//   //   // isBluetoothActive = true; // Set status menjadi aktif
//   // }
//   delay(100); // DELAY SETUP

// }

// void startBluetooth()
// {
//   SerialBT.begin("BSIB-BT"); // Nama perangkat Bluetooth
//   Serial.print("\nBluetooth Serial telah diaktifkan");
//   delay(1000); // DELAY SETUP 6
// }

// void stopBluetooth() {
//   SerialBT.end(); // Menonaktifkan Bluetooth Serial
//   Serial.println("\nBluetooth Serial dinonaktifkan");

//   delay(1000);
// }
//--------------------------------------------------------------------------------------------------------------------
void serialBTMonitor(long currentMillis) {
  // button1State = digitalRead(BUTTON_1_PIN);

  // if (!btStatus && !startWifiConfig) {
  //   // stopBluetooth();
  //   startWifiConfig = true;
  // }

  // if (btStatus && startWifiConfig) {
  //   bluetoothOptionMenu();
  // }

  // // Periksa jika tombol ditekan
  // if (button1State == LOW && !button1WasPressed) {
  //   button1WasPressed = true;
  //   button1LastPressTime = currentMillis; // Catat waktu ketika tombol pertama kali ditekan
  // }

  // // Cek apakah tombol telah dilepas atau apakah debounce interval telah berlalu
  // if (button1WasPressed && currentMillis - button1LastPressTime >= debounceInterval) {
  //   // Jika tombol dilepas
  //   if (digitalRead(BUTTON_1_PIN) == HIGH) {
  //     button1WasPressed = false;

  //     // Ubah status lampu
  //     btStatus = !btStatus;

  //     // Update keadaan lampu
  //     digitalWrite(LED_1_GREEN_PIN, btStatus ? HIGH : LOW);

  //     if (btStatus) {
  //       if (!SerialBT.available()) {
  //         // To pause stream
  //         stream.pauseFirebase(true);
  //         stream.clear(); // close session and release memory
  //         delay(100);
  //         startBluetooth();
  //       }
  //     } else {
  //       stopBluetooth();
  //       delay(100);
  //       // To resume stream with callback
  //       stream.pauseFirebase(false);
  //       Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
  //     }
  //   }
  // }
}

void resetCredentials() {
  // SerialBT.println(">>> Kosongkan isian untuk menghentikan <<<");
  // // Setting ulang SSID
  // SerialBT.print(" > Masukkan SSID: ");
  // while (!SerialBT.available()) {
  //   delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  // }
  // // Baca SSID dari BluetoothSerial
  // newSSIDBT = SerialBT.readStringUntil('\n');
  // newSSIDBT.trim();

  // if (newSSIDBT.length() == 0) {
  //   SerialBT.println("SSID harus diisi. Silahkan ulangi pilihan 1.");
  //   return;
  // }

  // // Setting ulang Password
  // SerialBT.print(" > Masukkan Password: (min 8 char)");
  // while (!SerialBT.available()) {
  //   delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  // }
  // // Baca Password dari BluetoothSerial
  // newPasswordBT = SerialBT.readStringUntil('\n');
  // newPasswordBT.trim();

  // if (newPasswordBT.length() == 0) {
  //   SerialBT.println("Password harus diisi. Silahkan ulangi pilihan 1.");
  //   return;
  // } else if (newPasswordBT.length() < 8) {
  //   SerialBT.println("Password minimal 8 karakter. Silahkan ulangi pilihan 1.");
  //   return;
  // }

  // // Memastikan terlebih dahulu apakah perubahan sudah sesuai yang diinginkan?
  // SerialBT.print("    Apakah sudah yakin? (ya/tidak) ");
  // while (!SerialBT.available()) {
  //   delay(10); // Menunggu sampai data tersedia di BluetoothSerial
  // }

  // // Baca pilihan "ya" atau "tidak" dari BluetoothSerial
  // String confirmChoice = SerialBT.readStringUntil('\n');
  // confirmChoice.trim();

  // if (confirmChoice.equalsIgnoreCase("ya")) {
  //   // Simpan SSID dan password ke SPIFFS
  //   saveCredentials(newSSIDBT, newPasswordBT);
  //   SerialBT.println("SSID dan password berhasil disimpan.");

  //   // Coba koneksi WiFi dengan SSID dan password baru
  //   SerialBT.println(" ");
  //   SerialBT.println("Mencoba menghubungkan perangkat dengan SSID dan Password baru ...");
  //   delay(100);
  //   wifiSetup();

  //   if (!WiFi.status() == WL_CONNECTED) {
  //     SerialBT.println("Gagal terhubung ke WiFi dengan SSID dan password baru.");
  //   }
    
  // } else {
  //   SerialBT.println("Pilihan tidak valid. Silahkan ulangi pilihan 1.");
  // }
}

void handleBluetoothMenu(int choice)
{
  // switch (choice)
  // {
  // case 1:
  //   // Konfigurasi ulang SSID dan Password
  //   resetCredentials();
  //   delay(1000);
  //   break;

  // case 2:
  //   // Cek SSID WiFi
  //   if (WiFi.status() != WL_CONNECTED)
  //   {
  //     SerialBT.println("Anda belum terhubung ke WiFi.");
  //   }
  //   else
  //   {
  //     SerialBT.println("SSID WiFi saat ini: " + WiFi.SSID());
  //   }
  //   delay(1000);
  //   break;

  // case 3:
  //   // Cek IP Address
  //   if (WiFi.status() != WL_CONNECTED)
  //   {
  //     SerialBT.println("Anda belum terhubung ke WiFi.");
  //   }
  //   else
  //   {
  //     SerialBT.print("IP Address WiFi saat ini: ");
  //     SerialBT.println(WiFi.localIP());
  //   }
  //   delay(1000);
  //   break;

  // case 4:
  //   // Putar bel manual
  //   if (!signupOK)
  //   {
  //     SerialBT.print("Belum terhubung dengan server");
  //     break;
  //   }

  //   if (!Firebase.ready())
  //   {
  //     SerialBT.println(fbdo.errorReason());
  //     break;
  //   }

  //   Firebase.setBool(fbdo, String(PUTAR_MANUAL) + String(STATUS_PUTAR), true);
  //   Firebase.setInt(fbdo, String(PUTAR_MANUAL) + String(PILIHAN_PUTAR), 1); // MASIH 1 ya blum ada choice nya

  //   // Tambahkan fungsi choice seperti ganti ssid dan passwordnya

  //   putarBelManual(true, 1); // PUTAR MANUAL DARI BLUETOOTH, BELUM DIATUR FUNGSI UNTUK LOOP NYA ATAU PILIHANNYA
  //   delay(1000);
  //   break;

  // case 5:
  //   // Hentikan pemutaran bel
  //   if (!signupOK)
  //   {
  //     SerialBT.print("Belum terhubung dengan server");
  //     break;
  //   }

  //   if (!Firebase.ready())
  //   {
  //     SerialBT.println(fbdo.errorReason());
  //     break;
  //   }

  //   if (!digitalRead(DFPLAYER_BUSY_PIN))
  //   {
  //     SerialBT.print("Menghentikan pemutaran audio");
  //     setDisablePutarManual();
  //     myDFPlayer.stop();
  //   }
  //   else
  //   {
  //     SerialBT.print("Tidak sedang memutar audio");
  //   }
  //   delay(1000);
  //   break;

  // case 8:
  //   // Menonaktifkan Bluetooth
  //   startWifiConfig = false;
  //   btStatus = false;
  //   digitalWrite(LED_1_GREEN_PIN, LOW);
  //   stopBluetooth();
  //   break;

  // case 9:
  //   SerialBT.println("\nMencoba menghubungkan perangkat dengan WiFi yang terdaftar ...");
  //   if (WiFi.status() == WL_CONNECTED)
  //   {
  //     WiFi.disconnect(true);
  //   }
  //   delay(1000);
  //   break;

  // case 0:
  //   ESP.restart();
  //   break;

  // default:
  //   SerialBT.println("Pilihan tidak valid. Silahkan pilih 1-5|9|0.");
  //   delay(100);
  //   break;
  // }

  // SerialBT.println(" ");
}

void bluetoothOptionMenu()
{
  // if (SerialBT.connected())
  // {
  //   if (!menuDisplayed)
  //   { // Menampilkan menu jika belum ditampilkan
  //     SerialBT.println("Silahkan pilih menu bluetooth:");
  //     SerialBT.println("------------------------------------------------------");
  //     SerialBT.println("1: Reset SSID dan Password WiFi");
  //     SerialBT.println("2: Cek SSID WiFi");
  //     SerialBT.println("3: Cek IP Address");
  //     SerialBT.println("4: Putar Bel Manual");
  //     SerialBT.println("5: Hentikan Pemutaran Bel");
  //     SerialBT.println("8: Nonaktifkan Bluetooth");
  //     SerialBT.println("9: Restart WiFi");
  //     SerialBT.println("0: Restart Perangkat");
  //     SerialBT.println(" ");
  //     SerialBT.print("Pilihan: ");
  //     menuDisplayed = true; // Set menuDisplayed menjadi true setelah menu ditampilkan
  //   }

  //   if (!choiceMade && SerialBT.available())
  //   {
  //     int choice = SerialBT.parseInt(); // Membaca pilihan menu sebagai integer
  //     while (SerialBT.available())
  //     {
  //       SerialBT.read(); // Baca dan buang data tambahan
  //     }
  //     handleBluetoothMenu(choice);
  //     choiceMade = true; // Set choiceMade menjadi true setelah pilihan dibuat
  //   }
  // }

  // // Jika pilihan telah dibuat, reset penanda untuk siklus berikutnya
  // if (choiceMade)
  // {
  //   menuDisplayed = false;
  //   choiceMade = false;
  // }
}
