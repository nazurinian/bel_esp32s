#ifndef BLUETOOTH_SERIAL_CONTROL_H
#define BLUETOOTH_SERIAL_CONTROL_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "AudioManager.h"
#include "WifiSetup.h"
#include "Config.h"

void startBluetooth();
void serialBTMonitor();
void resetCredentials();
void handleBluetoothMenu(int choice);
void bluetoothOptionMenu();

#endif