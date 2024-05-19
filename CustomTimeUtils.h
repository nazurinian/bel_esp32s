#ifndef CUSTOMTIMEUTILS_H
#define CUSTOMTIMEUTILS_H

#include <Arduino.h>
#include <NTPClient.h>
#include "Structs.h"
#include "Variables.h"

CustomTime getCurrentTime(NTPClient &timeClient);

#endif
