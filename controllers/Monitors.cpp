#include "Monitors.h"

void lcdMonitor(int type, int error)
{
    // Implementasi fungsi lcdMonitor
    // 1. LCD Monitor Online
    // 2. LCD Monitor Offline
    // 3. Show Schedule
    // 4. Show NextSchedule

    if (type == 1)
    {
        clearDisplayOffline = true;
        if (displayTime)
        {
            // Mendapatkan waktu saat ini
            String formattedTime = timeClient.getFormattedTime();

            // Reset LCD
            displaySchedule = true;
            if (displaySchedule && displayNextSchedule)
            {
                LCD.clear();
                displayNextSchedule = false;
            }

            // Penanganan hari normal dan hari libur
            if (currentTime.weekday >= 1 && currentTime.weekday <= 5)
            {
                nowOrNext = jadwalSaatIni.title;
            }
            else if (currentTime.weekday == 6 || currentTime.weekday == 0)
            {
                nowOrNext = "Hari Libur";
            }

            // Menampilkan jadwal bel sekarang dan waktu saat ini di LCD
            LCD.setCursor(0, 0);
            LCD.print("Now " + nowOrNext);
            LCD.setCursor(0, 1);
            LCD.print("Time: " + formattedTime);
        }
        else
        {
            // Tampilkan jadwal dan waktu selanjutnya
            String waktuBelSelanjutnya = _formathm(jadwalSelanjutnya.jam) + ":" + _formathm(jadwalSelanjutnya.menit);

            // Penanganan hari normal dan hari libur
            if (currentTime.weekday >= 1 && currentTime.weekday <= 5)
            {
                nowOrNext = jadwalSelanjutnya.title;
                timeOrBel = waktuBelSelanjutnya;
            }
            else if (currentTime.weekday == 6 || currentTime.weekday == 0)
            {
                nowOrNext = "Hari Senin";
                timeOrBel = "--:--";
            }

            // Reset LCD
            displayNextSchedule = true;
            if (displaySchedule && displayNextSchedule)
            {
                LCD.clear();
                displaySchedule = false;
            }

            // Menampilkan jadwal bel sekarang dan waktu saat ini di LCD
            LCD.setCursor(0, 0);
            LCD.print("Next " + nowOrNext);
            LCD.setCursor(0, 1);
            LCD.print("Bel : " + timeOrBel);
        }
    }
    else
    {
        // Daftar pilihan waktu offline di LCD
        // 1. Masalah belum terhubung wifi
        // 2. Masalah sudah terhubung wifi tapi terputus (misal router bermasalah atau internet tidak lost)
        // 3. Masalah koneksi ke server bermasalah (Firebase)
        if (clearDisplayOffline)
        {
            clearDisplayOffline = false;
            LCD.clear();
        }
        // Menampilkan waktu offline di LCD
        switch (error)
        {
        case 1:
            LCD.setCursor(0, 0);
            LCD.print("Belum Terhubung");
            LCD.setCursor(0, 1);
            LCD.print("WiFi");
            break;
        case 2:
            LCD.setCursor(0, 0);
            LCD.print("Tidak Ada");
            LCD.setCursor(0, 1);
            LCD.print("Internet");
            break;
        case 3:
            LCD.setCursor(0, 0);
            LCD.print("Koneksi Server");
            LCD.setCursor(0, 1);
            LCD.print("Gagal");
            break;
        default:
            LCD.setCursor(0, 0);
            LCD.print("Something");
            LCD.setCursor(0, 1);
            LCD.print("Went Wrong");
            break;
        }
    }
}

void serialMonitor()
{
    // Menampilkan tanggal dan waktu lengkap
    Serial.print("Tahun: ");
    Serial.print(currentTime.year);
    Serial.print(", Bulan: ");
    Serial.print(currentTime.month);
    Serial.print(", Hari: ");
    Serial.print(currentTime.day);
    Serial.print(", Jam: ");
    Serial.print(currentTime.hours);
    Serial.print(", Menit: ");
    Serial.print(currentTime.minutes);
    Serial.print(", Detik: ");
    Serial.println(currentTime.seconds);

    // Menampilkan jadwal bel saat ini dan yang akan datang
    String waktuBelSaatIni = _formathm(jadwalSaatIni.jam) + ":" + _formathm(jadwalSaatIni.menit);
    String waktuBelSelanjutnya = _formathm(jadwalSelanjutnya.jam) + ":" + _formathm(jadwalSelanjutnya.menit);

    String now = "";
    String next = "";

    // Penanganan hari normal dan hari libur
    if (currentTime.weekday >= 1 && currentTime.weekday <= 5)
    {
        now = jadwalSaatIni.title + " || " + waktuBelSaatIni;
        next = jadwalSelanjutnya.title + " || " + waktuBelSelanjutnya;
    }
    else if (currentTime.weekday == 6 || currentTime.weekday == 0)
    {
        now = "Hari Libur";
        next = "Hari Senin";
    }

    Serial.println("Jadwal bel saat ini : " + now);
    Serial.println("Jadwal bel yang akan datang : " + next);

    // Mendapatkan waktu saat ini
    String formattedTime = timeClient.getFormattedTime();
    Serial.print("Time : ");
    Serial.println(formattedTime);
}
