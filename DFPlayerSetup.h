#ifndef DFPLAYER_SETUP_H
#define DFPLAYER_SETUP_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>
#include "Monitors.h"
#include "HardwareSerial.h"
#include "Config.h"
#include "Variables.h"

void setupDFPlayer();
void volumeControl();
int getNumFilesInFolder(int folder);

#endif
