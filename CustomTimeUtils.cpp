#include "CustomTimeUtils.h" // Sertakan file header Variabels.h

const String getDayName(int dayOfWeek)
{
  const String dayNames[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
  return dayNames[dayOfWeek];
}

// Implementasi fungsi untuk mengambil elemen waktu saat ini
CustomTime getCurrentTime(NTPClient &timeClient)
{
  // Mendapatkan waktu saat ini
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeinfo;
  timeinfo = gmtime((time_t *)&epochTime);

  // Mendapatkan elemen waktu
  currentTime.year = timeinfo->tm_year + 1900; // tm_year dimulai dari tahun 1900
  currentTime.month = timeinfo->tm_mon + 1;    // tm_mon dimulai dari 0
  currentTime.day = timeinfo->tm_mday;
  currentTime.hours = timeinfo->tm_hour;
  currentTime.minutes = timeinfo->tm_min;
  currentTime.seconds = timeinfo->tm_sec;
  currentTime.weekday = timeinfo->tm_wday;             // Menetapkan nilai untuk hari, 0 untuk Minggu, 1 untuk Senin, dst.
  currentTime.dayName = getDayName(timeinfo->tm_wday); // Mendapatkan nama hari

  return currentTime;
}
