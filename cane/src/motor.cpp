#include "../include/motor.hpp"
#include "../include/main.hpp"

void motor_setup() { motor1.init(); }
void motor_loop() {
  static int i = 0;
  ++i;
  motor1.set_intensity(i % 255);
}
