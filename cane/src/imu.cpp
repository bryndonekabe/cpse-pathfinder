#include "../include/imu.hpp"
#include "../include/main.hpp"

void imu_setup() { imu.init(); }
void imu_loop() { imu.get_data(); }
