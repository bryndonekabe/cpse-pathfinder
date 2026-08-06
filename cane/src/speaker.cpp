#include "../include/speaker.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266)) // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

void printDetail(uint8_t type, int value);

void speaker_setup() {
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, SPEAKER_RX_PIN, SPEAKER_TX_PIN);
#else
  FPSerial.begin(9600);
#endif

  if (!df_player.begin(
          FPSerial, /*isACK = */ true,
          /*softwareReset = */ true)) { // Use serial to communicate with mp3.
    Serial.println(F("Unable to begin DFPlayer:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
    }
  }

  delay(5000);

  Serial.println(F("DFPlayer Mini online."));
  df_player.setTimeOut(2000); // Set serial communictaion time out 2000ms
  //----Set volume----
  df_player.volume(SPEAKER_DEFAULT_VOLUME); // Set volume value (0~30).
  // df_player.volumeUp(); //Volume Up
  // df_player.volumeDown(); //Volume Down

  //----Set different EQ----
  df_player.EQ(DFPLAYER_EQ_NORMAL);
  df_player.outputDevice(DFPLAYER_DEVICE_SD);

  //----Mp3 control----
  //  df_player.sleep();     //sleep

  //----Mp3 play----
  // df_player.pause();  //pause the mp3
  // df_player.start();  //start the mp3 from the pause
  // df_player.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3;
  // Folder Name(1~99); File Name(1~255)
  // df_player.enableLoopAll(); // loop all mp3 files.
  // df_player.disableLoopAll(); //stop loop all mp3 files.
  // df_player.playMp3Folder(
  // 1); // play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)

  //----Read imformation----
  // Serial.print("Files: ");
  // Serial.println(df_player.readFileCounts());
  // Serial.print("Current: ");
  // Serial.println(df_player.readCurrentFileNumber());

  // Serial.println(df_player.readState()); //read mp3 state
  // Serial.println(df_player.readVolume()); //read current volume
  // Serial.println(df_player.readEQ()); //read EQ setting
  // Serial.println(df_player.readFileCounts()); //read all file counts in
  // SD card Serial.println(df_player.readCurrentFileNumber()); //read
  // current play file number
  // Serial.println(df_player.readFileCountsInFolder(3));
  // //read file counts in folder SD:/03
}

void speaker_loop() {
  if (df_player.available()) {
    Serial.print("DFPlayer:");
    printDetail(df_player.readType(),
                df_player.read()); // Print the detail message from DFPlayer to
                                   // handle different errors and states.
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
  case TimeOut:
    Serial.println(F("Time Out!"));
    break;
  case WrongStack:
    Serial.println(F("Stack Wrong!"));
    break;
  case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    break;
  case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    break;
  case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    break;
  case DFPlayerUSBInserted:
    Serial.println("USB Inserted!");
    break;
  case DFPlayerUSBRemoved:
    Serial.println("USB Removed!");
    break;
  case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    break;
  case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value) {
    case Busy:
      Serial.println(F("Card not found"));
      break;
    case Sleeping:
      Serial.println(F("Sleeping"));
      break;
    case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      break;
    case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      break;
    case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      break;
    case FileMismatch:
      Serial.println(F("Cannot Find File"));
      break;
    case Advertise:
      Serial.println(F("In Advertise"));
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}
