#include "../include/main.hpp"
#include <Wire.h>

void tof_setup() {
  // Wire.begin(22, 21);
  Wire.setClock(400000);

  tof_left.init();
  delay(500);

  tof_right.init();
  delay(500);
}

void tof_loop() {
  uint32_t start = micros();

  tof_left.get_data();
  tof_right.get_data();

  uint32_t elapsed = micros() - start;
  Serial.printf("Read time: %lu us | %.2f Hz\n", elapsed, 1000000.0f / elapsed);
}
