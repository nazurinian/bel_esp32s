#ifndef FIREBASE_DATA_FETCH_H
#define FIREBASE_DATA_FETCH_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "CustomTimeUtils.h"
#include "Variables.h" 

void fetchDataFromFirebase(String day);
void getJsonData();

#endif
