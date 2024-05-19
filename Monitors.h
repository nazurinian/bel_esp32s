#ifndef MONITORS_H
#define MONITORS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Functions.h"
#include "Structs.h"
#include "Variables.h"

void lcdMonitor(int type, int error = 0);
void serialMonitor();

extern LiquidCrystal_I2C LCD;

#endif