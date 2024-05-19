#ifndef MONITORS_H
#define MONITORS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../utils/Functions.h"
#include "../models/Structs.h"
#include "../models/Variables.h"

void lcdMonitor(int type, int error = 0);
void serialMonitor();

extern LiquidCrystal_I2C LCD;

#endif