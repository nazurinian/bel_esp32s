#include "FirebaseDataFetch.h"

void fetchDataFromFirebase(String day)
{ // CURIGA NOMOR 1 - CURIGA NOMOR 2 KARENA BANYAK VAR DAN FUNGSI
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

        // Parse JSON
        DeserializationError error = deserializeJson(json, payload);
        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
        }
        else
        {
            dataIsAvailable = true;
        }
    }
    else
    {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
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
        fetchDataFromFirebase("JUMAT");
        Serial.println("Memperoleh jadwal Jum'at");
    }
    else
    {
        hariLibur = true;
        Serial.println("Sekarang waktu libur Sabtu dan Minggu");
    }
}
