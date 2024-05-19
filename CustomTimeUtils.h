#ifndef CUSTOMTIMEUTILS_H
#define CUSTOMTIMEUTILS_H

#include <Arduino.h>
#include <NTPClient.h>
#include "Structs.h"
#include "Variables.h"

// Deklarasi fungsi untuk mengambil elemen waktu saat ini
CustomTime getCurrentTime(NTPClient &timeClient);

#endif
