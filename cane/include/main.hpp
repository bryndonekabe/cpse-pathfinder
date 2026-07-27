#pragma once
// main.hpp
#include <Arduino.h>

void main_setup();
void main_loop();

extern uint16_t buf[64]; // Data from a total of 64 points
