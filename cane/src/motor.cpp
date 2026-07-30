#include "../include/motor.hpp"
#include "../include/main.hpp"

void motor_setup() { motor1.init(); }
void motor_loop() {
  static uint8_t i = 0;
  i += 5;
  motor1.set_intensity(i);
}
