#include "../include/main.hpp"
#include <Wire.h>

void tof_setup() {

  // Wire.begin(21, 22);
  // Wire.setClock(10000);

  tof_left.init();
  tof_right.init();
}

void tof_loop() {
  tof_left.get_data();
  tof_right.get_data();
}
