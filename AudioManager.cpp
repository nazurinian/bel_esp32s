#include "AudioManager.h"

void putarBelKelas(int pilihan)
{
    sedangMemutarAudio = true;
    setBelKelasTrue(true, pilihan);

    switch (pilihan)
    {
    case 1:
        // Bel Literasi Pagi
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(13);
        break;
    case 2:
        // Bel Awal Masuk Pelajaran
        // Putar 2x panjang
        myDFPlayer.play(11);
        playState = 1;
        break;
    case 3:
        // Bel Pertengahan Pelajaran
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(33);
        break;
    case 4:
        // Audio 5 Menit Sebelum Bel
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(12);
        break;
    case 5:
        // Bel Istirahat
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(15);
        break;
    case 6:
        // Bel Pulang
        // myDFPlayer.play(35);
        myDFPlayer.play(35);
        break;
    default:
        Serial.println("Tidak memutar apa-apa karena pilihan putar yang dipilih adalah 0");
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

    // Nomor 1: 
    // Tambahkan variabel isPlayed, kondisi true kalau masuk kedalam if putar, 
    // false kalau diluar if putar, untuk kondisi if putranya yg diluar sendiri 
    // syaratnya ada dua, 1. Syaratnya kalau isPlayed = true dan menitbel atau 
    // menit 5 menit sblm msk itu tidak sama dengan menit saat ini

    if ((waktu5MenitSebelumMasuk.minutes != currentTime.minutes || masuk.menit != currentTime.minutes) && sedangMemutarAudio)
    {
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        Serial.println("Bel selesai diputar");
        sedangMemutarAudio = false;
        setBelKelasTrue(false, 0);
        delay(100);
        return;
    }
    else
    {
        // Memutar sound 5 menit sebelum masuk kelas
        if (currentTime.hours == waktu5MenitSebelumMasuk.hours && currentTime.minutes == waktu5MenitSebelumMasuk.minutes && masuk.aktif && !sedangMemutarAudio)
        {
            if (jamKe == 1)
            {
                sedangMemutarAudio = true;
                Serial.println("Memutar Sound 5 menit sebelum Literasi Pagi");
                Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
                putarBelKelas(4);
                delay(400);
                return;
            }
            if (jamKe == 2 || jamKe == 7 || jamKe == 14)
            {
                sedangMemutarAudio = true;
                Serial.println("Memutar Sound 5 menit sebelum masuk Jam ke-" + String(jamKe));
                Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
                putarBelKelas(4);
                delay(400);
                return;
            }
        }

        if (currentTime.hours == masuk.jam && currentTime.minutes == masuk.menit && masuk.aktif && !sedangMemutarAudio)
        {
            sedangMemutarAudio = true;
            if (jamKe == 1)
            {
                Serial.println("Memutar Bel Pulang");
                putarBelKelas(1);
                delay(400);
            }
            else if (jamKe == 6 || jamKe == 13)
            { // Waktu Mulai Istirahat (Bel putar 2x panjang)
                Serial.println("Memutar Bel Istirahat");
                putarBelKelas(5);
                delay(400);
            }
            else if (jamKe == 16)
            { // Waktu Mulai Istirahat (Bel putar 3x panjang)
                Serial.println("Memutar Bel Pulang");
                putarBelKelas(6);
                delay(400);
            }
            else
            {
                Serial.println("Memutar Bel Masuk Jam ke-" + String(jamKe));
                if (jamKe == 2 || jamKe == 7 || jamKe == 14)
                {
                    putarBelKelas(2);
                    delay(400);
                }
                else
                {
                    putarBelKelas(3);
                    delay(400);
                }
            }
            Serial.println("Jadwal : " + String(masuk.jam) + "." + String(masuk.menit));
        }
    }
}

void cekPemutaranManualLebih1x()
{
    switch (playState)
    {
    case 0:
        break;

    case 1:
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        playState = 2;
        sedangMemutarAudio = true;
        break;

    case 2:
        playState = 3;
        sedangMemutarAudio = true;
        myDFPlayer.play(33);
        break;

    case 3:
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        playState = 0;
        Serial.println("Bel selesai diputar");
        sedangMemutarAudio = false;
        setBelKelasTrue(false, 0);
        break;
    }
}

void putarBelOtomatis(JsonDocument &json)
{
    if (!digitalRead(DFPLAYER_BUSY_PIN))
    {
        delay(100);
        return;
    }

    // if (playState > 0)
    // {
    //     return;
    // }

    // if (sedangMemutarAudio)
    // {
    //     Serial.println("Bel selesai diputar");
    //     sedangMemutarAudio = false;
    //     setBelKelasTrue(false, 0);
    //     delay(100);
    //     return;
    // }

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
            Serial.print("Menghentikan pemutaran audio");
            sedangMemutarAudio = false;
            playState = 0;
            myDFPlayer.stop();
            return;
        }

        Serial.println("Sedang memutar bel / audio");
        delay(100);
        return;
    }

    if (mainkan && !sedangMemutarAudio)
    {
        Serial.println("Memutar bel secara manual");
        putarBelKelas(choice);
    }
}

void stopAudioPlay(long currentMillis) {
    button3State = digitalRead(BUTTON_2_PIN);

    if (button3State == LOW && !button3WasPressed) {
        button3WasPressed = true;
        button3LastPressTime = currentMillis; 
    }

    if (button3WasPressed && (currentMillis - button3LastPressTime >= debounceInterval)) {
        if (digitalRead(BUTTON_2_PIN) == HIGH) {
            button3WasPressed = false;

            if (sedangMemutarAudio) {
                sedangMemutarAudio = false;
                Serial.println("Menghentikan pemutaran audio");
                setBelKelasTrue(false, 0);
                myDFPlayer.stop();
            } else {
                Serial.println("Tidak sedang memutar audio apapun");
            }
        }
    }
}

/* #include "AudioManager.h"

void putarBelKelas(int pilihan)
{
    sedangMemutarAudio = true;
    setBelKelasTrue(true, pilihan);

    switch (pilihan)
    {
    case 1:
        // Bel Literasi Pagi
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(13);
        break;
    case 2:
        // Bel Awal Masuk Pelajaran
        // Putar 2x panjang
        myDFPlayer.play(11);
        playState = 1;
        break;
    case 3:
        // Bel Pertengahan Pelajaran
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(33);
        break;
    case 4:
        // Audio 5 Menit Sebelum Bel
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(12);
        break;
    case 5:
        // Bel Istirahat
        // myDFPlayer.play(random(1, 39));
        myDFPlayer.play(15);
        break;
    case 6:
        // Bel Pulang
        // myDFPlayer.play(35);
        myDFPlayer.play(35);
        break;
    default:
        Serial.println("Tidak memutar apa-apa karena pilihan putar yang dipilih adalah 0");
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

    // Nomor 1:
    // Tambahkan variabel isPlayed, kondisi true kalau masuk kedalam if putar,
    // false kalau diluar if putar, untuk kondisi if putranya yg diluar sendiri
    // syaratnya ada dua, 1. Syaratnya kalau isPlayed = true dan menitbel atau
    // menit 5 menit sblm msk itu tidak sama dengan menit saat ini

    if ((waktu5MenitSebelumMasuk.minutes != currentTime.minutes || masuk.menit != currentTime.minutes) && sedangMemutarAudio)
    {
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        Serial.println("Bel selesai diputar");
        sedangMemutarAudio = false;
        setBelKelasTrue(false, 0);
        delay(100);
        return;
    }
    else
    {
        // Memutar sound 5 menit sebelum masuk kelas
        if (currentTime.hours == waktu5MenitSebelumMasuk.hours && currentTime.minutes == waktu5MenitSebelumMasuk.minutes && masuk.aktif && !sedangMemutarAudio)
        {
            if (jamKe == 1)
            {
                sedangMemutarAudio = true;
                Serial.println("Memutar Sound 5 menit sebelum Literasi Pagi");
                Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
                putarBelKelas(4);
                delay(400);
                return;
            }
            if (jamKe == 2 || jamKe == 7 || jamKe == 14)
            {
                sedangMemutarAudio = true;
                Serial.println("Memutar Sound 5 menit sebelum masuk Jam ke-" + String(jamKe));
                Serial.println("Jadwal : " + String(waktu5MenitSebelumMasuk.hours) + "." + String(waktu5MenitSebelumMasuk.minutes));
                putarBelKelas(4);
                delay(400);
                return;
            }
        }

        if (currentTime.hours == masuk.jam && currentTime.minutes == masuk.menit && masuk.aktif && !sedangMemutarAudio)
        {
            sedangMemutarAudio = true;
            if (jamKe == 1)
            {
                Serial.println("Memutar Bel Pulang");
                putarBelKelas(1);
                delay(400);
            }
            else if (jamKe == 6 || jamKe == 13)
            { // Waktu Mulai Istirahat (Bel putar 2x panjang)
                Serial.println("Memutar Bel Istirahat");
                putarBelKelas(5);
                delay(400);
            }
            else if (jamKe == 16)
            { // Waktu Mulai Istirahat (Bel putar 3x panjang)
                Serial.println("Memutar Bel Pulang");
                putarBelKelas(6);
                delay(400);
            }
            else
            {
                Serial.println("Memutar Bel Masuk Jam ke-" + String(jamKe));
                if (jamKe == 2 || jamKe == 7 || jamKe == 14)
                {
                    putarBelKelas(2);
                    delay(400);
                }
                else
                {
                    putarBelKelas(3);
                    delay(400);
                }
            }
            Serial.println("Jadwal : " + String(masuk.jam) + "." + String(masuk.menit));
        }
    }
}

void cekPemutaranManualLebih1x(long currentMillis)
{
    switch (playState)
    {
    case 0:
        break;

    case 1:
        // if (currentMillis - previousLoopCheck >= intervalCheck)
        // {
        //     previousLoopCheck = currentMillis;
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        playState = 2;
        sedangMemutarAudio = true;
        // }
        break;

    case 2:
        playState = 3;
        sedangMemutarAudio = true;
        myDFPlayer.play(33);
        break;

    case 3:
        if (!digitalRead(DFPLAYER_BUSY_PIN))
        {
            delay(100);
            return;
        }
        playState = 0;
        Serial.println("Bel selesai diputar");
        sedangMemutarAudio = false;
        setBelKelasTrue(false, 0);
        break;
    }
}

void putarBelOtomatis(JsonDocument &json)
{
    if (!digitalRead(DFPLAYER_BUSY_PIN))
    {
        delay(100);
        return;
    }

    // if (playState > 0)
    // {
    //     return;
    // }

    // if (sedangMemutarAudio)
    // {
    //     Serial.println("Bel selesai diputar");
    //     sedangMemutarAudio = false;
    //     setBelKelasTrue(false, 0);
    //     delay(100);
    //     return;
    // }

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
        if (!infoPlay) // Gara-gara ini makanya ga bisa jalan bersamaan dengan yg online
        {
            Serial.print("Menghentikan pemutaran audio");
            sedangMemutarAudio = false;
            playState = 0;
            myDFPlayer.stop();
            return;
        }

        Serial.println("Sedang memutar bel / audio");
        delay(100);
        return;
    }

    // if (playState > 0)
    // {
    //     return;
    // }

    // if (sedangMemutarAudio)
    // {
    //     Serial.println("Bel selesai diputar");
    //     setBelKelasTrue(false, 0);
    //     delay(100);
    //     return;
    // }

    if (mainkan && !sedangMemutarAudio)
    {
        Serial.println("Memutar bel secara manual");
        putarBelKelas(choice);
    }
}

void stopAudioPlay(long currentMillis)
{
    // button3State = digitalRead(BUTTON_3_PIN); // Pinjem punya wifi manager dlu
    button3State = digitalRead(BUTTON_2_PIN);

    if (button3State == LOW && !button3WasPressed)
    {
        button3WasPressed = true;
        button3LastPressTime = currentMillis; // Catat waktu saat tombol pertama kali ditekan
    }

    if (button3WasPressed && (currentMillis - button3LastPressTime >= debounceInterval))
    {
        // Jika tombol dilepas atau debounce interval telah berlalu
        // if (digitalRead(BUTTON_3_PIN) == HIGH) {  // Pinjem punya wifi manager dlu
        if (digitalRead(BUTTON_2_PIN) == HIGH)
        {
            button3WasPressed = false;

            if (sedangMemutarAudio)
            {
                sedangMemutarAudio = false;
                Serial.println("Menghentikan pemutaran audio");
                setBelKelasTrue(false, 0);
                myDFPlayer.stop();
            }
            else
            {
                Serial.println("Tidak sedang memutar audio apapun");
            }
        }
    }
}
 */