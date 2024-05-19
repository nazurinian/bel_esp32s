#include "FirebaseInitialSetup.h"

void jadwalKelas()
{
    if (!Firebase.get(fbdo, "/jadwal"))
    {
        Serial.println("Jadwal Kelas belum ada");
        Serial.println("Jadwal Kelas akan dibuat ...");

        // Jadwal Senin - Kamis
        if (!Firebase.get(fbdo, seninSampaiKamis))
        {
            for (int i = 0; i < 16; i++)
            {
                // Set waktu masuk
                Firebase.setInt(fbdo, seninSampaiKamis + String(i + 1) + jamMasuk, jadwal[i].jam);
                Firebase.setInt(fbdo, seninSampaiKamis + String(i + 1) + menitMasuk, jadwal[i].menit);
                // Set info aktif untuk waktu masuk
                Firebase.setBool(fbdo, seninSampaiKamis + String(i + 1) + statusMasuk, true);
            }

            Serial.println("Selesai membuat Jadwal Kelas Senin - Kamis");
        }

        // Jadwal Jum'at (Khusus)
        if (!Firebase.get(fbdo, jumat))
        {
            for (int i = 0; i < 16; i++)
            {
                int jamKe = i + 1;
                // Set waktu masuk
                Firebase.setInt(fbdo, jumat + String(i + 1) + jamMasuk, jadwal[i].jam);
                Firebase.setInt(fbdo, jumat + String(i + 1) + menitMasuk, jadwal[i].menit);
                // Set info aktif untuk waktu masuk
                if (jamKe >= 11 && jamKe <= 13)
                {
                    Firebase.setBool(fbdo, jumat + String(i + 1) + statusMasuk, false);
                }
                else
                {
                    Firebase.setBool(fbdo, jumat + String(i + 1) + statusMasuk, true);
                }
            }

            Serial.println("Selesai membuat Jadwal Kelas Jum'at");
        }
    }
    else
    {
        Serial.println("Jadwal Kelas sudah ada");
    }
}

// Setting dan Cek apakah sistem putar manual sudah tersedia?
void settingBelManual()
{
    if (!Firebase.get(fbdo, "/putar-manual"))
    {
        Serial.println("Membuat sistem pemutar bel manual secara online");
        Firebase.setBool(fbdo, putarManual + statusPutar, false);
        Firebase.setInt(fbdo, putarManual + pilihanPutar, 0);
    }
    else
    {
        Serial.println("Bel manual telah siap");
        Firebase.setBool(fbdo, putarManual + statusPutar, false);
        Firebase.setInt(fbdo, putarManual + pilihanPutar, 0);
    }
}
