#include "Functions.h"

Schedule getScheduleTitle(const JsonArray &json, NTPClient &timeClient)
{
  int currentMinutes = currentTime.hours * 60 + currentTime.minutes;
  Schedule currentSchedule;

  for (size_t i = 1; i < json.size(); i++)
  {
    int scheduleMinutes = (json[i]["jam"].as<int>() * 60) + json[i]["menit"].as<int>();
    int firstScheduleMinutes = (json[1]["jam"].as<int>() * 60) + json[1]["menit"].as<int>();
    int lastScheduleMinutes = (json[json.size() - 1]["jam"].as<int>() * 60) + json[json.size() - 1]["menit"].as<int>();

    // Penanganan sebelum masuk sekolah
    if (currentMinutes < firstScheduleMinutes)
    {
      currentSchedule.jam = -1; // Menandakan di luar jadwal
      currentSchedule.menit = -1;
      currentSchedule.title = "Belum masuk";
      break;
    }
    else
    {
      // Penanganan setelah selesai sekolah
      if (currentMinutes >= lastScheduleMinutes + 60)
      {
        currentSchedule.jam = -1; // Menandakan di luar jadwal
        currentSchedule.menit = -1;
        currentSchedule.title = "Luar jadwal";
      }
      else
      {
        // Penanganan saat di sekolah
        if (currentMinutes >= scheduleMinutes)
        {
          currentSchedule.jam = json[i]["jam"].as<int>();
          currentSchedule.menit = json[i]["menit"].as<int>();
          currentSchedule.title = jamTitles[i - 1];
        }
      }
    }
  }

  return currentSchedule;
}

Schedule nextTimeSchedule(const JsonArray &json, NTPClient &timeClient)
{
  int currentMinutes = currentTime.hours * 60 + currentTime.minutes;
  Schedule nextSchedule;

  for (size_t i = 1; i < json.size(); i++)
  {
    int jadwalJam = json[i]["jam"].as<int>();
    int jadwalMenit = json[i]["menit"].as<int>();

    int scheduleMinutes = jadwalJam * 60 + jadwalMenit;
    int firstScheduleMinutes = (json[1]["jam"].as<int>() * 60) + json[1]["menit"].as<int>();
    int lastScheduleMinutes = (json[json.size() - 1]["jam"].as<int>() * 60) + json[json.size() - 1]["menit"].as<int>();

    // Penanganan sebelum masuk sekolah
    if (currentMinutes < firstScheduleMinutes)
    {
      nextSchedule.jam = json[1]["jam"].as<int>();
      nextSchedule.menit = json[1]["menit"].as<int>();
      nextSchedule.title = jamTitles[0];
      break;
    }
    else
    {
      // Penanganan setelah selesai sekolah
      if (currentMinutes >= lastScheduleMinutes)
      {
        nextSchedule.jam = -1; // Menandakan di luar jadwal
        nextSchedule.menit = -1;
        nextSchedule.title = "---";
      }
      else
      {
        // Penanganan saat di sekolah
        if (currentMinutes >= scheduleMinutes)
        {
          nextSchedule.jam = json[i + 1]["jam"].as<int>();
          nextSchedule.menit = json[i + 1]["menit"].as<int>();
          nextSchedule.title = jamTitles[i];
        }
      }
    }
  }

  return nextSchedule;
}
