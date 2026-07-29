#include "../include/main.hpp"
#include <Wire.h>

TOFSensor tof_left{0x32};
TOFSensor tof_right{0x33};
void main_setup() {
  Serial.begin(115200); // Set the serial communication baud rate to 115200

  // Wire.begin(21, 22);
  // Wire.setClock(10000);

  // tof_left.init();
  // tof_right.init();
}

void main_loop() {
  tof_left.get_data();
  tof_right.get_data();

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
  // delay(100); // Setting this time allows adjustment of the interval for
  // reading distances.
}
