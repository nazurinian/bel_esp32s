#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DFRobotDFPlayerMini.h>
#include <FirebaseESP32.h>
#include "FirebaseInitialSetup.h"
#include "Config.h"
#include "Variables.h"
#include "DFPlayerSetup.h"

void putarBelKelas(int infoPilihanPutar);
void cekWaktu(JadwalData masuk, int jamKe);
void putarBelOtomatis(JsonDocument &json);
void cekJumlahPemutaran();
void putarBelManual(bool mainkan = false, int choice = 0);
void stopAudioPlay(long currentMillis);

#endif
