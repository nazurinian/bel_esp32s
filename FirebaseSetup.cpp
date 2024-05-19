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
    if (data.streamPath() == putarManual)
    {
        if (data.dataPath() == statusPutar) {
            infoPlay = data.boolData(); // data.to<bool>();
            Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            Serial.print("Path yang diterima: ");
            Serial.println(data.streamPath());

            Serial.print("Nilai infoPlay: ");
            Serial.println(infoPlay ? "true" : "false");
        } else if (data.dataPath() == pilihanPutar) {
            infoPilihanPutar = data.intData(); // data.to<int>();
            Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            Serial.print("Path yang diterima: ");
            Serial.println(data.streamPath());

            Serial.print("Nilai pilihanPutar: ");
            Serial.println(infoPilihanPutar);
        } else { // ini event pathnya "/" ya...
            FirebaseJson json = data.jsonObject();
            Serial.printf("stream path, %s\n\n", data.streamPath().c_str());

            FirebaseJsonData jsonData;
            if (json.get(jsonData, "/putar")) {
                infoPlay = jsonData.boolValue;
                Serial.print("Nilai infoPlay: ");
                Serial.println(infoPlay);
            }
            if (json.get(jsonData, "/choice"))
            {
                infoPilihanPutar = jsonData.intValue;
                Serial.print("Nilai infoPilihanPutar: ");
                Serial.println(infoPilihanPutar);
            }
        }

        // Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
        //               data.streamPath().c_str(),
        //               data.dataPath().c_str(),
        //               data.dataType().c_str(),
        //               data.eventType().c_str());
        // Serial.println();
        // printResult(data); // see addons/RTDBHelper.h
        // Serial.println();

        // Serial.print("Nilai infoPlay: ");
        // Serial.println(infoPlay);

        // Serial.print("Nilai infoPilihanPutar: ");
        // Serial.println(infoPilihanPutar);

        // Serial.println((const char *)FPSTR("Pretty printed JSON data:"));
        // json->toString(Serial, true);
        // Serial.println();

    }
}

// void streamCallbackPutar(StreamData data)
// {
//     if (data.dataType() == "null")
//     {
//         Serial.println("Data kosong.");
//         return;
//     }

//     if (data.streamPath() == statusPutar)
//     {
//         infoPlay = data.boolData(); // data.to<bool>();

//         Serial.print("Path yang diterima: ");
//         Serial.println(data.streamPath());

//         Serial.print("Nilai infoPlay: ");
//         Serial.println(infoPlay ? "true" : "false");
//     }
// }

// void streamCallbackChoice(StreamData data)
// {
//     if (data.dataType() == "null")
//     {
//         Serial.println("Data kosong.");
//         return;
//     }
//     if (data.streamPath() == pilihanPutar)
//     {
//         infoPilihanPutar = data.intData(); // data.to<int>();

//         Serial.print("Path yang diterima: ");
//         Serial.println(data.streamPath());

//         Serial.print("Nilai pilihanPutar: ");
//         Serial.println(infoPilihanPutar);
//     }
// }

/* void streamCallback(StreamData data)
{
  Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); // see addons/RTDBHelper.h
  Serial.println();

  // This is the size of stream payload received (current and max value)
  // Max payload size is the payload size under the stream path since the stream connected
  // and read once and will not update until stream reconnection takes place.
  // This max value will be zero as no payload received in case of ESP8266 which
  // BearSSL reserved Rx buffer size is less than the actual stream payload.
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
} */

void streamTimeoutCallback(bool timeout)
{
    if (timeout)
        Serial.println("stream timed out, resuming...\n");

    if (!stream.httpConnected())
        Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());

    // if (!streamPutar.httpConnected())
    //     Serial.printf("error code: %d, reason: %s\n\n", streamPutar.httpCode(), streamPutar.errorReason().c_str());

    // if (!streamChoice.httpConnected())
    //     Serial.printf("error code: %d, reason: %s\n\n", streamChoice.httpCode(), streamChoice.errorReason().c_str());
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
    // streamPutar.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
    // streamChoice.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

    // Initialize the library with the Firebase authen and config
    Firebase.begin(&config, &auth);
    stream.keepAlive(5, 5, 1);
    // streamPutar.keepAlive(5, 5, 1);
    // streamChoice.keepAlive(5, 5, 1);

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
        if (!Firebase.beginStream(stream, putarManual))
        {
            Serial.printf("Stream begin error, %s\n\n", stream.errorReason().c_str());
        }

        Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);

        // if (!Firebase.beginStream(streamPutar, statusPutar))
        // {
        //     Serial.printf("Stream begin error, %s\n\n", streamPutar.errorReason().c_str());
        // }

        // Firebase.setStreamCallback(streamPutar, streamCallbackPutar, streamTimeoutCallback);
        // Firebase.setStreamCallback(streamPutar, streamCallback, streamTimeoutCallback);

        // delay(1000);

        // if (!Firebase.beginStream(streamChoice, pilihanPutar))
        // {
        //     Serial.printf("Stream begin error, %s\n\n", streamChoice.errorReason().c_str());
        // }

        // Firebase.setStreamCallback(streamChoice, streamCallbackChoice, streamTimeoutCallback);
        // Firebase.setStreamCallback(streamChoice, streamCallback, streamTimeoutCallback);

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

/*
void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    Serial.printf("Set bool... %s\n", Firebase.setBool(fbdo, F("/test/bool"), count % 2 == 0) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get bool... %s\n", Firebase.getBool(fbdo, FPSTR("/test/bool")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());

    bool bVal;
    Serial.printf("Get bool ref... %s\n", Firebase.getBool(fbdo, F("/test/bool"), &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());

    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/test/int"), count) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/test/int")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());

    int iVal = 0;
    Serial.printf("Get int ref... %s\n", Firebase.getInt(fbdo, F("/test/int"), &iVal) ? String(iVal).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/float"), count + 10.2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/float")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set double... %s\n", Firebase.setDouble(fbdo, F("/test/double"), count + 35.517549723765) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get double... %s\n", Firebase.getDouble(fbdo, F("/test/double")) ? String(fbdo.to<double>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/test/string"), "Hello World!") ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/test/string")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse.ino
    FirebaseJson json;

    if (count == 0)
    {
      json.set("value/round/" + String(count), F("cool!"));
      json.set(F("vaue/ts/.sv"), F("timestamp"));
      Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/test/json"), json) ? "ok" : fbdo.errorReason().c_str());
    }
    else
    {
      json.add(String(count), "smart!");
      Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
    }

    Serial.println();

    // For generic set/get functions.

    // For generic set, use Firebase.set(fbdo, <path>, <any variable or value>)

    // For generic get, use Firebase.get(fbdo, <path>).
    // And check its type with fbdo.dataType() or fbdo.dataTypeEnum() and
    // cast the value from it e.g. fbdo.to<int>(), fbdo.to<std::string>().

    // The function, fbdo.dataType() returns types String e.g. string, boolean,
    // int, float, double, json, array, blob, file and null.

    // The function, fbdo.dataTypeEnum() returns type enum (number) e.g. fb_esp_rtdb_data_type_null (1),
    // fb_esp_rtdb_data_type_integer, fb_esp_rtdb_data_type_float, fb_esp_rtdb_data_type_double,
    // fb_esp_rtdb_data_type_boolean, fb_esp_rtdb_data_type_string, fb_esp_rtdb_data_type_json,
    // fb_esp_rtdb_data_type_array, fb_esp_rtdb_data_type_blob, and fb_esp_rtdb_data_type_file (10)

    count++;
  }
}

/// PLEASE AVOID THIS ////

// Please avoid the following inappropriate and inefficient use cases
/**
 *
 * 1. Call get repeatedly inside the loop without the appropriate timing for execution provided e.g. millis() or conditional checking,
 * where delay should be avoided.
 *
 * Everytime get was called, the request header need to be sent to server which its size depends on the authentication method used,
 * and costs your data usage.
 *
 * Please use stream function instead for this use case.
 *
 * 2. Using the single FirebaseData object to call different type functions as above example without the appropriate
 * timing for execution provided in the loop i.e., repeatedly switching call between get and set functions.
 *
 * In addition to costs the data usage, the delay will be involved as the session needs to be closed and opened too often
 * due to the HTTP method (GET, PUT, POST, PATCH and DELETE) was changed in the incoming request.
 *
 *
 * Please reduce the use of swithing calls by store the multiple values to the JSON object and store it once on the database.
 *
 * Or calling continuously "set" or "setAsync" functions without "get" called in between, and calling get continuously without set
 * called in between.
 *
 * If you needed to call arbitrary "get" and "set" based on condition or event, use another FirebaseData object to avoid the session
 * closing and reopening.
 *
 * 3. Use of delay or hidden delay or blocking operation to wait for hardware ready in the third party sensor libraries, together with stream functions e.g. Firebase.RTDB.readStream and fbdo.streamAvailable in the loop.
 *
 * Please use non-blocking mode of sensor libraries (if available) or use millis instead of delay in your code.
 *
 * 4. Blocking the token generation process.
 *
 * Let the authentication token generation to run without blocking, the following code MUST BE AVOIDED.
 *
 * while (!Firebase.ready()) <---- Don't do this in while loop
 * {
 *     delay(1000);
 * }
 *
 */

/*
FirebaseData firebaseData;

void onDataCallback(StreamData data)
{
  if (data.dataType() == "null")
  {
    Serial.println("Data kosong.");
    return;
  }

  Serial.print("Data yang diterima: ");
  Serial.println(data.dataPath());

  if (data.dataType() == "string")
  {
    Serial.print("String: ");
    Serial.println(data.stringData());
  }
  else if (data.dataType() == "int")
  {
    Serial.print("Integer: ");
    Serial.println(data.intData());
  }
  else if (data.dataType() == "float")
  {
    Serial.print("Float: ");
    Serial.println(data.floatData(), 6);
  }
  else if (data.dataType() == "boolean")
  {
    Serial.print("Boolean: ");
    Serial.println(data.boolData() ? "true" : "false");
  }
  else if (data.dataType() == "json")
  {
    Serial.print("JSON: ");
    FirebaseJson &json = data.jsonObject();
    json.printTo(Serial);
    Serial.println();
  }
} */