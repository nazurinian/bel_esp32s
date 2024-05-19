#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DFRobotDFPlayerMini.h>
#include <FirebaseESP32.h>
#include "Config.h"
#include "Variables.h"

void putarBelKelas(int pilihanPutar);
void cekWaktu(JadwalData masuk, int jamKe);
void putarBelOtomatis(JsonDocument &json);
void cekPemutaranManualLebih1x();
void putarBelManual(bool mainkan = false, int choice = 0);

#endif
