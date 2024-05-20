#include "DFPlayerSetup.h"

void setupDFPlayer()
{
  Serial2.begin(9600);
  delay(100);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  while (!myDFPlayer.begin(Serial2))
  {
    Serial.println("Unable to begin:");
    Serial.println("1. Please recheck the connection!");
    Serial.println("2. Please insert the SD card!");
    lcdMonitor(0, 4);
    delay(1000);
  }

  LCD.clear();
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20);           // Set volume level (0-30)
  pinMode(DFPLAYER_BUSY_PIN, INPUT); // Mengatur pin BUSY sebagai input
}

// Bisa atur pake button juga klo mau
// myDFPlayer.volumeUp(); //Volume Up
// myDFPlayer.volumeDown(); //Volume Down
// myDFPlayer.readVolume(); //Read Volume
void volumeControl()
{
  volumeLevel = map(analogRead(DFPLAYER_VOLUME_PIN), 0, 4095, 0, 30); // scale the pot value and volume level
  myDFPlayer.volume(volumeLevel);
  Serial.print("Volume Level : ");
  Serial.println(volumeLevel);
}
