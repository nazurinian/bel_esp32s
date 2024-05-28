#include "FirebaseDataFetch.h"

void fetchDataFromFirebase(String day)
{
    HTTPClient http;

    // Build Firebase URL
    String url = String(DATABASE_URL) + "/jadwal/" + day + ".json?auth=" + String(DATABASE_SECRET_KEY);

    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
        String payload = http.getString(); // JSON DATA
        Serial.print("Firebase response: ");
        Serial.println(httpResponseCode);
        dataFetched = true;

        // Parse JSON
        DeserializationError error = deserializeJson(json, payload);
        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            dataIsAvailable = false;
        }
        else
        {
            dataIsAvailable = true;
            jadwalSaatIni = getScheduleTitle(json);
            jadwalSelanjutnya = nextTimeSchedule(json);
        }
    }
    else
    {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
        dataFetched = false;
    }

    http.end();
}

void getJsonData()
{
    // Pengecekan hari untuk menjalankan program (1 Senin, 2 Selasa, 3 Rabu, 4 Kamis, 5 Jum'at)
    if (currentTime.weekday >= 1 && currentTime.weekday <= 4)
    {
        hariLibur = false;
        fetchDataFromFirebase("senin-kamis");
        Serial.println("Memperoleh jadwal Senin-Kamis");
    }
    else if (currentTime.weekday == 5)
    {
        hariLibur = false;
        fetchDataFromFirebase("jumat");
        Serial.println("Memperoleh jadwal Jum'at");
    }
    else
    {
        hariLibur = true;
        Serial.println("Sekarang waktu libur Sabtu dan Minggu");
    }
}

void setBelKelasTrue(bool status, int bellChoice)
{
    // stream.pauseFirebase(true);
    // stream.clear();
    // delay(1000);
    FirebaseJson jsonUp;
    if (status)
    {
        jsonUp.add("putar", true);
        jsonUp.add("choice", bellChoice);
        Firebase.setJSON(fbdo, String(PUTAR_MANUAL), jsonUp);
    }
    else
    {
        jsonUp.add("putar", false);
        jsonUp.add("choice", bellChoice);
        Firebase.setJSON(fbdo, String(PUTAR_MANUAL), jsonUp);

        playState = 0;
    }

    // stream.pauseFirebase(false);
    // Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
}