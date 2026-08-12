#pragma once
#include "../include/config.hpp"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <deque>

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266)) // Using a soft serial port
#include <SoftwareSerial.h>
inline SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

class AudioManager {
  uint8_t spk_rx_pin, spk_tx_pin;
  DFRobotDFPlayerMini df_player;
  bool playing = false;
  std::deque<uint8_t> audios;

  void play(uint8_t audio) {
    playing = true;
    df_player.playMp3Folder(audio);
  }
  void try_next_audio() {
    if (!audios.empty()) {
      uint8_t next_audio = audios.front();
      audios.pop_front();
      play(next_audio);
    }
  }

  void p_detail(uint8_t type, int value) {
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

public:
  AudioManager(uint8_t rx, uint8_t tx) : spk_rx_pin(rx), spk_tx_pin(tx) {}
  // TODO: init
  void init() {
#if (defined ESP32)
    FPSerial.begin(9600, SERIAL_8N1, spk_rx_pin, spk_tx_pin);
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
  }
  void queue(uint8_t audio) {
    // if its the first audio added, then play it outright
    if (!playing) {
      play(audio);
    } else {
      audios.push_back(audio);
    }
  }
  bool is_playing() { return playing; }
  void step() {
    if (!df_player.available())
      return;

    uint8_t type = df_player.readType();
    int value = df_player.read();
    if (type == DFPlayerPlayFinished) {
      playing = false;
      try_next_audio();
    }
    Serial.print("DFPlayer:");
    p_detail(type, value);
  }
  void wait() {
    while (playing) {
      step();
      delay(10);
    }
  }
  // NOTE: normalized from 0-1
  void volume(double pct) {
    double number = pct * 30;
    df_player.volume((unsigned int)number);
  }
};
void speaker_setup();
void speaker_loop();
