#include "../include/main.hpp"
#include "DFRobot_MatrixLidar.h"

DFRobot_MatrixLidar_I2C tof(0x33); // Default I2C address 0x33
uint16_t buf[64] = {};             // Data from a total of 64 points

void main_setup() {
  Serial.begin(115200); // Set the serial communication baud rate to 115200
  while (tof.begin() != 0) {
    Serial.println("begin err");
  }
  Serial.println("begin success");
  // config matrix mode
  while (tof.setRangingMode(eMatrix_8X8) != 0) { // Set to 8*8 mode
    Serial.println("init error !!!!!");
    delay(1000);
  }
  Serial.println("init success");
}

void main_loop() {
  tof.getAllData(buf);
  // for (uint8_t i = 0; i < 8; i++) {
  //   Serial.print("Y");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   for (uint8_t j = 0; j < 8; j++) {
  //     Serial.print(buf[i * 8 + j]);
  //     Serial.print(",");

  //   }
  //   Serial.println("");
  // }
  // Serial.println("------------------------------");
  delay(100); // Setting this time allows adjustment of the interval for reading
              // distances.
}
