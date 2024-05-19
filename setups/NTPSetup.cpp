#include "NTPSetup.h"

void ntpSetup()
{
  Serial.println("Initializing NTP...");

  // Memulai client NTP
  timeClient.begin();

  // Menunggu hingga waktu NTP diperbarui
  Serial.print("Menunggu waktu NTP...");
  int retryCount = 0;
  while (!timeClient.update())
  {
    // Jika gagal memperbarui waktu setelah beberapa kali coba, keluar dari loop
    if (retryCount > 10)
    {
      Serial.println("Gagal memperbarui waktu dari NTP setelah beberapa percobaan. Menggunakan waktu terakhir yang diketahui.");
      break;
    }
    // Jika gagal memperbarui waktu, tunggu sebentar dan coba lagi
    retryCount++;
    timeClient.forceUpdate();
    delay(500);
  }

  // Mencetak waktu NTP
  Serial.print("Waktu NTP sekarang: ");
  Serial.println(timeClient.getFormattedTime());

  delay(100);
}
