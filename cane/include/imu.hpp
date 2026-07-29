#pragma once
#include <Arduino.h>
#include <DFRobot_BMI160.h>

class IMU {
  DFRobot_BMI160 bmi;
  int16_t _accel_gyro[6] = {0};
  uint8_t _addr = NULL;

public:
  IMU(uint8_t x) : _addr(x), bmi() {}
  void init() {
    if (bmi.I2cInit(_addr) != BMI160_OK) {
      Serial.println("IMU init fail");
    }
  }

  // first three are gyro data in deg/s,
  // next three are accel data in g-force
  // (9.81 m/s^2)
  void get_data() {
    // get both accel and gyro data from bmi160
    // parameter accelGyro is the pointer to store the data
    int res = bmi.getAccelGyroData(_accel_gyro);
    if (res != 0) {
      Serial.println("IMU read err");
    }
  }

  int16_t *accel_gyro() { return _accel_gyro; }
};

void imu_setup();
void imu_loop();
