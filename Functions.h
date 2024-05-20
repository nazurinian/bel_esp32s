#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "CustomTimeUtils.h"
#include "Structs.h"
#include "Variables.h"

// Fungsi untuk mendapatkan jadwal saat ini
Schedule getScheduleTitle(DynamicJsonDocument &json);

// Fungsi untuk mendapatkan jadwal berikutnya
Schedule nextTimeSchedule(DynamicJsonDocument &json);

#endif
