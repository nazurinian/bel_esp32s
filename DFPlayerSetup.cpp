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

    LCD.clear();
    lcdMonitor(0, 4);
    delay(1000);
  }

  LCD.clear();
  lcdMonitor(0, 5);

  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(2000);
  myDFPlayer.volume(30);             // Set volume level (0-30)
  pinMode(DFPLAYER_BUSY_PIN, INPUT); // Mengatur pin BUSY sebagai input
}

// Bisa atur pake button juga klo mau
void volumeControl()
{
  volumeLevel = map(analogRead(DFPLAYER_VOLUME_PIN), 0, 4095, 0, 30); // scale the pot value and volume level
  myDFPlayer.volume(volumeLevel);
  Serial.print("Volume Level : ");
  Serial.println(volumeLevel);
}

// Fungsi untuk mendapatkan jumlah file dalam folder
int getNumFilesInFolder(int folder)
{
  // myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  int numFiles = myDFPlayer.readFileCountsInFolder(folder);
  Serial.print("Jumlah file di folder ");
  Serial.print(folder - 1);
  Serial.print(": ");
  Serial.println(numFiles);
  return numFiles;
}

// myDFPlayer.volumeUp(); //Volume Up
// myDFPlayer.volumeDown(); //Volume Down
// myDFPlayer.readVolume(); //Read Volume
// Serial.print("folderCount DF "); // Mencetak jumlah folder di kartu SD
// Serial.println(folderCount);
// Serial.println(myDFPlayer.readState());      // Membaca dan mencetak status MP3 saat ini
// Serial.println(myDFPlayer.readVolume());     // Membaca dan mencetak volume audio saat ini
// Serial.println(myDFPlayer.readEQ());         // Membaca dan mencetak pengaturan EQ saat ini
// Serial.println(myDFPlayer.readFileCounts()); // Membaca dan mencetak jumlah total file di kartu SD
// Serial.println(myDFPlayer.readCurrentFileNumber());