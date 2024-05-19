#include "FirebaseSetup.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

void streamCallback(StreamData data)
{
    if (data.dataType() == "null")
    {
        Serial.println("Data kosong.");
        return;
    }

    // Informasi Streamnya : 
    // 1. stream path, "/putar-manual", data.streamPath().c_str() (karena disini itu make pathnya utama yg artinya objeknya langsung, maka jika path cabang yg berubah brt liat eventnya)
    // 2. event path, "/", data.dataPath().c_str()
    // 3. data type, "json", data.dataType().c_str()
    // 4. event type, put, data.eventType().c_str())
    if (data.streamPath() == PUTAR_MANUAL)
    {
        if (data.dataPath() == STATUS_PUTAR) {
            infoPlay = data.boolData(); // data.to<bool>();
            // Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            // Serial.print("Path yang diterima: ");
            // Serial.println(data.streamPath());

            // Serial.print("Nilai infoPlay: ");
            // Serial.println(infoPlay ? "true" : "false");
        } else if (data.dataPath() == PILIHAN_PUTAR) {
            infoPilihanPutar = data.intData(); // data.to<int>();
            // Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            // Serial.print("Path yang diterima: ");
            // Serial.println(data.streamPath());

            // Serial.print("Nilai PILIHAN_PUTAR: ");
            // Serial.println(infoPilihanPutar);
        } else { // ini event pathnya "/" ya...
            FirebaseJson json = data.jsonObject();
            Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            FirebaseJsonData jsonData;
            if (json.get(jsonData, STATUS_PUTAR)) {
                infoPlay = jsonData.boolValue;
                // Serial.print("Nilai infoPlay: ");
                // Serial.println(infoPlay);
            }
            if (json.get(jsonData, PILIHAN_PUTAR))
            {
                infoPilihanPutar = jsonData.intValue;
                // Serial.print("Nilai infoPilihanPutar: ");
                // Serial.println(infoPilihanPutar);
            }
        }
    }
}

void streamTimeoutCallback(bool timeout)
{
    if (timeout)
        Serial.println("stream timed out, resuming...\n");

    if (!stream.httpConnected())
        Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void firebaseSetup()
{
    Serial.println(" ");
    Serial.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Sign up */
    // Assign the user sign in credentials
    auth.user.email = "admin@mail.com";
    auth.user.password = "12345678";

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    // Assign the callback function for the long running token generation task
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Assign the maximum retry of token generation
    config.max_token_generation_retry = 5;

    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
    Firebase.reconnectNetwork(true);

    // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
    // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
    stream.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

    // Initialize the library with the Firebase authen and config
    Firebase.begin(&config, &auth);
    stream.keepAlive(5, 5, 1);

    // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    while ((auth.token.uid) == "")
    {
        Serial.print('.');
        delay(1000);
    }

    // Print user UID
    signupOK = true;
    String uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid + "\n");

    if (WiFi.status() == WL_CONNECTED)
    {
        // Mulai stream data dari Firebase di sini
        if (!Firebase.beginStream(stream, PUTAR_MANUAL))
        {
            Serial.printf("Stream begin error, %s\n\n", stream.errorReason().c_str());
        }

        Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);

        if (Firebase.ready() && signupOK)
        {
            Serial.println("SAMPE FIREBASE AMAN - SETUP AWAL STRUKTUR FIREBASE");
            jadwalKelas();
            settingBelManual();
            delay(100);
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
    else
    {
        Serial.println("Gagal menyiapkan jadwal kelas dan bel online");
        delay(100);
    }
}
