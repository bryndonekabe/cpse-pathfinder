#include "../include/speaker.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "Arduino.h"

void speaker_setup() {
  audio_manager.init();
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

void speaker_loop() { audio_manager.step(); }
