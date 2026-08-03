#include "../include/main.hpp"
#include <Wire.h>

void tof_setup() {
  Wire.begin(21, 22);
  Wire.setClock(400000);

  tof_left.init();
  delay(100);

  tof_right.init();
  delay(100);
}

void tof_loop() {
  uint32_t t = micros();

  tof_left.get_data();

  // Serial.printf("left: %lu us\n", micros() - t);

  t = micros();

  tof_right.get_data();

  // Serial.printf("right: %lu us\n", micros() - t);
}
