#ifndef CUSTOMTIMEUTILS_H
#define CUSTOMTIMEUTILS_H

#include <Arduino.h>
#include <NTPClient.h>
#include "../models/Structs.h"
#include "../models/Variables.h"

CustomTime getCurrentTime(NTPClient &timeClient);

#endif
