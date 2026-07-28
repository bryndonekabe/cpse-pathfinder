#pragma once
// main.hpp
#include "../include/TOFSensor.hpp"
#include <Arduino.h>

void main_setup();
void main_loop();

extern TOFSensor tof_left;
extern TOFSensor tof_right;
