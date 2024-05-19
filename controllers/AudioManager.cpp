#include "AudioManager.h"

void putarBelKelas(int pilihanPutar)
{
    switch (pilihanPutar)
    {
    case 1:
        // Bel Literasi Pagi
        myDFPlayer.play(random(1, 39));
        break;
    case 2:
        // Bel Awal Masuk Pelajaran
        // Putar 2x panjang
        myDFPlayer.play(11);
        playState = 1;
        break;
    case 3:
        // Bel Pertengahan Pelajaran
        myDFPlayer.play(random(1, 39));
        break;
    case 4:
        // Audio 5 Menit Sebelum Bel
        myDFPlayer.play(random(1, 39));
        break;
    case 5:
        // Bel Istirahat
        myDFPlayer.play(random(1, 39));
        break;
    case 6:
        // Bel Pulang
        myDFPlayer.play(random(1, 39));
        break;
    default:
        Serial.println("Tidak memutar apa-apa karena pilihan putar yang dipilih adalah 0");
        break;
    }
}

void cekWaktu(JadwalData masuk, int jamKe)
{
    // CustomTime currentTime = getCurrentTime(); // Waktu saat ini

    // Cocokkan waktu untuk 5 menit sebelum masuk kelas
    CustomTime waktu5MenitSebelumMasuk = currentTime;
    waktu5MenitSebelumMasuk.minutes -= 5;

    // Penanganan jika minutes menjadi negatif
    if (waktu5MenitSebelumMasuk.minutes < 0)
    {
        waktu5MenitSebelumMasuk.minutes += 60;
        waktu5MenitSebelumMasuk.hours -= 1;
    }

    // Memutar sound 5 menit sebelum masuk kelas
    if (waktu5MenitSebelumMasuk.hours == masuk.jam && waktu5MenitSebelumMasuk.minutes == masuk.menit && masuk.aktif)
    {
        if (jamKe == 1)
        {
            Serial.print("Memutar Sound 5 menit sebelum Literasi Pagi" + String(jamKe));
            Serial.println(jamKe);
            putarBelKelas(4);
            return;
        }
        if (jamKe == 2 || jamKe == 7 || jamKe == 14)
        {
            Serial.print("Memutar Sound 5 menit sebelum masuk Jam ke-" + String(jamKe));
            Serial.println(jamKe);
            Serial.print("Jadwal : " + String(masuk.jam) + "." + String(masuk.menit));
            putarBelKelas(4);
            return;
        }
    }

    if (currentTime.hours == masuk.jam && currentTime.minutes == masuk.menit && masuk.aktif)
    {
        if (jamKe == 1)
        {
            Serial.print("Memutar Bel Pulang");
            putarBelKelas(1);
        }
        else if (jamKe == 6 || jamKe == 13)
        { // Waktu Mulai Istirahat (Bel putar 2x panjang)
            Serial.print("Memutar Bel Istirahat");
            putarBelKelas(5);
        }
        else if (jamKe == 16)
        { // Waktu Mulai Istirahat (Bel putar 3x panjang)
            Serial.print("Memutar Bel Pulang");
            putarBelKelas(6);
        }
        else
        {
            Serial.print("Memutar Bel Masuk Jam ke-");
            Serial.println(jamKe);
            if (jamKe == 2 || jamKe == 7 || jamKe == 14)
            {
                putarBelKelas(2);
            }
            else
            {
                putarBelKelas(3);
            }
        }
        Serial.print("Jadwal : " + String(masuk.jam) + "." + String(masuk.menit));
    }
}

void putarBelOtomatis(JsonDocument &json)
{
    if (!digitalRead(DFPLAYERBUSYPIN))
    {
        delay(100);
        return;
    }

    for (int i = 1; i < json.size(); i++)
    {
        JadwalData jadwalMasuk;

        jadwalMasuk.aktif = json[i]["aktif"].as<bool>();
        jadwalMasuk.jam = json[i]["jam"].as<int>();
        jadwalMasuk.menit = json[i]["menit"].as<int>();

        cekWaktu(jadwalMasuk, i);
    }
}

void cekPemutaranManualLebih1x()
{
    switch (playState)
    {
    case 0:
        break;

    case 1:
        if (loopPlay - previousLoopCheck >= intervalCheck)
        {
            previousLoopCheck = loopPlay;
            if (digitalRead(DFPLAYERBUSYPIN))
            {
                playState = 2;
            }
        }
        break;

    case 2:
        myDFPlayer.play(13);
        playState = 3;
        break;

    case 3:
        if (loopPlay - previousLoopCheck >= intervalCheck)
        {
            previousLoopCheck = loopPlay;
            if (digitalRead(DFPLAYERBUSYPIN))
            {
                playState = 0;
                Firebase.setBool(fbdo, putarManual + statusPutar, false);
                Firebase.setInt(fbdo, putarManual + pilihanPutar, 0);
            }
        }
        break;
    }
}

void putarBelManual(bool mainkan, int choice)
{
    if (playState > 0)
    {
        return;
    }

    if (!digitalRead(DFPLAYERBUSYPIN))
    {
        // if (!cekBelManual())
        // {
        //     Serial.print("Menghentikan pemutaran audio");
        //     sedangMemutarAudio = false;
        //     myDFPlayer.stop();
        //     return;
        // }

        Serial.println("Sedang memutar bel / audio");
        delay(100);
        return;
    }

    if (sedangMemutarAudio)
    {
        sedangMemutarAudio = false;
        Serial.println("Bel selesai diputar");
        Firebase.setBool(fbdo, putarManual + statusPutar, false);
        Firebase.setInt(fbdo, putarManual + pilihanPutar, 0);
        delay(100);
        return;
    }

    if (mainkan)
    {
        Serial.println("Memutar bel secara manual");
        sedangMemutarAudio = true;
        putarBelKelas(choice);
    }
}
