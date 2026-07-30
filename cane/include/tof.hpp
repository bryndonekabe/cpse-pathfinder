#pragma once
#include "../include/config.hpp"
#include "../include/vec.hpp"
#include "DFRobot_MatrixLidar.h"

class TOFSensor {
  DFRobot_MatrixLidar_I2C tof;
  uint16_t prev_buf[TOF_MAT_HEIGHT * TOF_MAT_WIDTH] = {0};
  uint16_t curr_buf[TOF_MAT_HEIGHT * TOF_MAT_WIDTH] = {0};
  const dvec3 rotation{0}; // yaw, pitch, roll
  uint8_t addr;

public:
  TOFSensor(uint8_t x, dvec3 rot) : addr(x), tof(x), rotation(rot) {}
  void init() {
    Serial.printf("Initializing 0x%02X\n", addr);

    while (tof.begin() != NULL) {
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

  void get_data() {
    // move current into previous
    memcpy(prev_buf, curr_buf, sizeof(uint16_t) * 64);
    tof.getAllData(curr_buf);
  }
  const uint16_t *prev() { return prev_buf; }
  const uint16_t *buf() { return curr_buf; }
  const dvec3 &rot() { return rotation; }
};

void tof_setup();
void tof_loop();
