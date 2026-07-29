#include "../include/imu.hpp"

#define IMU_ADDR 0x69
IMU imu{IMU_ADDR};
void imu_setup() { imu.init(); }

void imu_loop() { imu.get_data(); }
