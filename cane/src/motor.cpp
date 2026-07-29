#include "../include/motor.hpp"

#define MOTOR1_PIN 12
Motor motor1{MOTOR1_PIN};
void motor_setup() { motor1.init(); }
void motor_loop() {
  static int i = 0;
  ++i;
  motor1.set_intensity(i % 255);
}
