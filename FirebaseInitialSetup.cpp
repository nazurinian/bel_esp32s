#include "FirebaseInitialSetup.h"

void jadwalKelas()
{
    // Firebase.pathExist
    if (!Firebase.pathExist(fbdo, "/jadwal"))
    {
        Serial.println("Jadwal Kelas belum ada");
        Serial.println("Jadwal Kelas akan dibuat ...");

        // Jadwal Senin - Kamis
        for (int i = 0; i < 16; i++)
        {
            // Set waktu masuk
            Firebase.setInt(fbdo, SENIN_SAMPAI_KAMIS + String(i + 1) + JAM_MASUK, jadwal[i].jam);
            Firebase.setInt(fbdo, SENIN_SAMPAI_KAMIS + String(i + 1) + MENIT_MASUK, jadwal[i].menit);
            // Set info aktif untuk waktu masuk
            Firebase.setBool(fbdo, SENIN_SAMPAI_KAMIS + String(i + 1) + STATUS_MASUK, true);
        }

        Serial.println("Selesai membuat Jadwal Kelas Senin - Kamis");
        delay(1000);

        // Jadwal Jum'at (Khusus)
        for (int i = 0; i < 16; i++)
        {
            int jamKe = i + 1;
            // Set waktu masuk
            Firebase.setInt(fbdo, JUMAT + String(i + 1) + JAM_MASUK, jadwal[i].jam);
            Firebase.setInt(fbdo, JUMAT + String(i + 1) + MENIT_MASUK, jadwal[i].menit);
            // Set info aktif untuk waktu masuk
            if (jamKe >= 11 && jamKe <= 13)
            {
                Firebase.setBool(fbdo, JUMAT + String(i + 1) + STATUS_MASUK, false);
            }
            else
            {
                Firebase.setBool(fbdo, JUMAT + String(i + 1) + STATUS_MASUK, true);
            }
        }

        Serial.println("Selesai membuat Jadwal Kelas Jum'at");
        delay(1000);
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {
            // Set info aktif untuk waktu masuk senin-kamis
            Firebase.setBool(fbdo, SENIN_SAMPAI_KAMIS + String(i + 1) + STATUS_MASUK, true);

            // Set info aktif untuk waktu masuk jumat
            int jamKe = i + 1;
            if (jamKe >= 11 && jamKe <= 13)
            {
                Firebase.setBool(fbdo, JUMAT + String(i + 1) + STATUS_MASUK, false);
            }
            else
            {
                Firebase.setBool(fbdo, JUMAT + String(i + 1) + STATUS_MASUK, true);
            }
        }
        Serial.println("Jadwal Kelas sudah ada");
        delay(1000);
    }
}

// Setting dan Cek apakah sistem putar manual sudah tersedia?
void settingBelManual()
{
    if (!Firebase.pathExist(fbdo, "/putar-manual"))
    {
        Serial.println("Membuat sistem pemutar bel manual secara online");
        setBelKelasTrue(false, 0);
    }
    else
    {
        Serial.println("Bel manual telah siap");
        setBelKelasTrue(false, 0);
    }
}