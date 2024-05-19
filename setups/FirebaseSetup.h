#ifndef FIREBASE_SETUP_H
#define FIREBASE_SETUP_H

#include <Arduino.h>
#include <FirebaseESP32.h>
#include "../Config.h"
#include "../models/Variables.h"
#include "FirebaseInitialSetup.h"

void streamCallback(StreamData data);
// void streamCallbackPutar(StreamData data);
// void streamCallbackChoice(StreamData data);
void streamTimeoutCallback(bool timeout);
void firebaseSetup();

#endif
