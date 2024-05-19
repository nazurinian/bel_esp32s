#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "CustomTimeUtils.h"
#include "../models/Structs.h"
#include "../models/Variables.h"

// Fungsi untuk mendapatkan jadwal saat ini
Schedule getScheduleTitle(const JsonArray &json, NTPClient &timeClient);

// Fungsi untuk mendapatkan jadwal berikutnya
Schedule nextTimeSchedule(const JsonArray &json, NTPClient &timeClient);

#endif
