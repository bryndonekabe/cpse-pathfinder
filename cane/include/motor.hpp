#pragma once
#include <Arduino.h>

class Motor {
  int addr = NULL;

public:
  Motor(int x) : addr(x) {}
  void init() { pinMode(addr, OUTPUT); }
  void set_intensity(uint8_t x) { analogWrite(addr, x); }
};

void motor_setup();
void motor_loop();
