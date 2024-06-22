#include "Monitors.h"

void lcdMonitor(int type, int error)
{
    // Implementasi fungsi lcdMonitor 1 / aktif
    // 1. Online Show Schedule
    // 2. Online Show NextSchedule

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
        // Daftar pilihan waktu offline di lcdMonitor 0 / tidak aktif
        // 1. Masalah belum/tidak terhubung wifi
        // 2. Masalah sudah terhubung wifi tapi terputus (misal router bermasalah atau internet tidak ada / lost)
        // 3. Masalah koneksi ke server bermasalah (Firebase)
        // 4. Masalah koneksi DFPlayer (koneksi dan microsd)
        // 5. Menyiapkan Alat... diawal Setup
        if (displayTime)
        {
            displayNextSchedule = true;
            displaySchedule = false;
        }
        else
        {
            displaySchedule = true;
            displayNextSchedule = false;
        }

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
        case 4:
            Serial.println("Mohon cek koneksi audio");
            Serial.println("Cek kartu SD, pastikan kartu SD sesuai dan tidak rusak");
            LCD.setCursor(0, 0);
            LCD.print("Cek koneksi!");
            LCD.setCursor(0, 1);
            LCD.print("Masukan MicroSD");
            break;
        case 5:
            Serial.println("Menyiapkan Alat...\n");
            LCD.setCursor(0, 0);
            LCD.print("Bel Sekolah By");
            LCD.setCursor(0, 1);
            LCD.print("Hikam Saif");
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
    // Mendapatkan waktu saat ini
    String formattedTime = timeClient.getFormattedTime();
    Serial.print("Time : ");
    Serial.println(formattedTime);

    Serial.println("Jadwal bel saat ini : " + now);
    Serial.println("Jadwal bel yang akan datang : " + next);
}
