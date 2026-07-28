#pragma once

#include "DFRobot_MatrixLidar.h"

class TOFSensor {
  uint8_t addr;
  DFRobot_MatrixLidar_I2C tof;
  uint16_t prev_buf[64] = {0};
  uint16_t curr_buf[64] = {0};

public:
  TOFSensor(uint8_t x) : addr(x), tof(x) {}
  void init() {
    while (tof.begin() != NULL) {
      Serial.println("begin err");
    }
    Serial.println("begin success");
    // config matrix mode
    // tof.setRangingMode(eMatrix_8X8);
    while (tof.setRangingMode(eMatrix_8X8) != 0) { // Set to 8*8 mode
      Serial.println("init error !!!!!");
      delay(1000);
    }
    Serial.println("init success");
  }

  void get_data() {
    // move current into previous
    memcpy(prev_buf, curr_buf, sizeof(uint16_t) * 64);
    tof.getAllData(curr_buf);
  }
  const uint16_t *prev() { return prev_buf; }
  const uint16_t *buf() { return curr_buf; }
};
