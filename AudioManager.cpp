#include "AudioManager.h"

void putarBelKelas(int pilihan)
{
    sedangMemutarAudio = true;
    isPlaying = true;
    menitPutar = currentTime.minutes;
    digitalWrite(LED_2_RED_PIN, HIGH);
    setBelKelasTrue(true, pilihan);

    switch (pilihan)
    {
    case 1:
        // Bel Literasi Pagi | 3x
        myDFPlayer.play(3);
        delay(500);
        break;
    case 2:
        // Bel Awal Masuk Pelajaran | 3x
        myDFPlayer.play(3);
        delay(500);
        break;
    case 3:
        // Bel Pertengahan Pelajaran | 1x
        myDFPlayer.play(1);
        delay(500);
        break;
    case 4:
        // Audio 5 Menit Sebelum Bel | Qur'an / Hadis
        // myDFPlayer.play(random(9, 17));
        myDFPlayer.playFolder(1, random(1, numFilesInFolder1 + 1)); // Random audio dari Folder 1 (disini hanya ada 8 audio)
        delay(500);
        break;
    case 5:
        // Bel Istirahat | 2x
        myDFPlayer.play(2);
        delay(500);
        break;
    case 6:
        // Bel Pulang | 4x
        myDFPlayer.play(4);
        delay(500);
        playState = 1;
        // jumlahPutar = 4;
        break;
    case 7:
        // Alarm Keadaan Darurat
        // myDFPlayer.play(5);
        // myDFPlayer.playFolder(3, 1); // Putar 1 audio dari Folder 3 (disini hanya ada 1 audio)
        myDFPlayer.playFolder(3, random(1, numFilesInFolder3 + 1)); // Random audio dari Folder 2
        delay(500);
        break;
    default:
        Serial.println("Tidak memutar apa-apa karena tidak ada pilihan putar yang dipilih");
        break;
    }
}

void cekWaktu(JadwalData masuk, int jamKe)
{
    // Cocokkan waktu untuk 5 menit sebelum masuk kelas
    CustomTime waktu5MenitSebelumMasuk;
    waktu5MenitSebelumMasuk.minutes = (masuk.menit - 5);
    waktu5MenitSebelumMasuk.hours = masuk.jam;

    // Penanganan jika minutes menjadi negatif
    if (waktu5MenitSebelumMasuk.minutes < 0)
    {
        waktu5MenitSebelumMasuk.minutes += 60;
        waktu5MenitSebelumMasuk.hours -= 1;
    }

    // Memutar sound 5 menit sebelum masuk kelas
    if (currentTime.hours == waktu5MenitSebelumMasuk.hours && currentTime.minutes == waktu5MenitSebelumMasuk.minutes && masuk.aktif && !sedangMemutarAudio)
    {
        if (jamKe == 1)
        {
            Serial.println("Memutar Sound 5 menit sebelum Literasi Pagi");
            Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
            putarBelKelas(4);
            mulaiPutarOnline = true;
            return;
        }
        if (jamKe == 2 || jamKe == 7 || jamKe == 14)
        {
            Serial.println("Memutar Sound 5 menit sebelum masuk Jam ke-" + String(jamKe));
            Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
            putarBelKelas(4);
            mulaiPutarOnline = true;
            return;
        }
    }

    if (currentTime.hours == masuk.jam && currentTime.minutes == masuk.menit && masuk.aktif && !sedangMemutarAudio)
    {
        mulaiPutarOnline = true;
        if (jamKe == 1)
        {
            Serial.println("Memutar Bel Pulang");
            putarBelKelas(1);
        }
        else if (jamKe == 6 || jamKe == 13)
        { // Waktu Mulai Istirahat (Bel putar 2x panjang)
            Serial.println("Memutar Bel Istirahat");
            putarBelKelas(5);
        }
        else if (jamKe == 16)
        { // Waktu Mulai Istirahat (Bel putar 3x panjang)
            Serial.println("Memutar Bel Pulang");
            putarBelKelas(6);
        }
        else
        {
            Serial.println("Memutar Bel Masuk Jam ke-" + String(jamKe));
            if (jamKe == 2 || jamKe == 7 || jamKe == 14)
            {
                putarBelKelas(2);
            }
            else
            {
                putarBelKelas(3);
            }
        }
        Serial.println("Jadwal : " + String(masuk.jam) + "." + String(masuk.menit));
    }
}

void cekJumlahPemutaran()
{
    switch (playState)
    {
    case 0:
        break;

    case 1: // Putar Ke-2
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        delay(100);

        // Play 3 random audios from random (Do'a) (disini hanya ada 3 audio)
        sedangMemutarAudio = true;
        myDFPlayer.playFolder(2, random(1, numFilesInFolder2 + 1)); // Random audio dari Folder 2
        playState = 2;
        delay(500);
        break;

    case 2: // Stop Putar
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        
        delay(100);
        setBelKelasTrue(false, 0);
        delay(500);
        if (!infoPlay)
        {
            playState = 0;
            sedangMemutarAudio = false;
            isPlaying = false;
            mulaiPutarOnline = false;
            Serial.println("Bel selesai diputar");
        }
    }
}

void putarBelOtomatis(JsonDocument &json)
{
    if (!digitalRead(DFPLAYER_BUSY_PIN))
    {
        delay(100);
        return;
    }

    if (sedangMemutarAudio)
    {
        if (currentTime.minutes == menitPutar)
        {
            isPlaying = false;
            return;
        }
        else if (currentTime.minutes != menitPutar)
        {
            setBelKelasTrue(false, 0);
            delay(500);
            if (!infoPlay)
            {
                sedangMemutarAudio = false;
                isPlaying = false;
                mulaiPutarOnline = false;
                Serial.println("Bel selesai diputar");
            }
            return;
        }
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

void putarBelManual(bool mainkan, int choice)
{
    if (!digitalRead(DFPLAYER_BUSY_PIN))
    {
        if (!infoPlay)
        {
            Serial.println("Menghentikan pemutaran audio");
            sedangMemutarAudio = false;
            isPlaying = false;
            mulaiPutarOnline = false;
            playState = 0;
            myDFPlayer.stop();
            return;
        }

        Serial.println("Sedang memutar bel / audio");
        delay(100);
        return;
    }

    if (sedangMemutarAudio)
    {
        if (currentTime.minutes == menitPutar)
        {
            isPlaying = false;
            return;
        }
        else if (currentTime.minutes != menitPutar)
        {
            setBelKelasTrue(false, 0);
            delay(500);
            if (!infoPlay)
            {
                sedangMemutarAudio = false;
                isPlaying = false;
                mulaiPutarOnline = false;
                Serial.println("Bel selesai diputar");
            }
            return;
        }
    }

    if (mainkan && !sedangMemutarAudio)
    {
        Serial.println("Memutar bel secara manual");
        putarBelKelas(choice);
    }
}

void stopAudioPlay(long currentMillis)
{
    button3State = digitalRead(BUTTON_2_PIN);

    if (button3State == LOW && !button3WasPressed)
    {
        button3WasPressed = true;
        button3LastPressTime = currentMillis;
    }

    if (button3WasPressed && (currentMillis - button3LastPressTime >= debounceInterval))
    {
        if (digitalRead(BUTTON_2_PIN) == HIGH)
        {
            button3WasPressed = false;

            if (sedangMemutarAudio)
            {
                setBelKelasTrue(false, 0);
                delay(500);
                if (!infoPlay)
                {
                    sedangMemutarAudio = false;
                    isPlaying = false;
                    mulaiPutarOnline = false;
                    Serial.println("Menghentikan pemutaran audio");
                    myDFPlayer.stop();
                    delay(100);
                }
                return;
            }
            else
            {
                Serial.println("Tidak sedang memutar audio apapun");
            }
        }
    }
}
