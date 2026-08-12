#pragma once
#include <Arduino.h>

enum class MotorEquation : uint8_t {
  Linear,
  Exponential,
  Logarithmic,
  Piecewise
};
class Motor {
  int addr;
  uint8_t last_intensity = 0;

public:
  Motor(int x) : addr(x) {}
  void init() { pinMode(addr, OUTPUT); }
  void set_intensity(uint8_t x) {
    analogWrite(addr, x);
    last_intensity = x;
  }
  uint8_t get_intensity() { return last_intensity; }
};

void motor_setup();
void motor_loop();
