#include "../include/bluetooth.hpp"
#include "../include/imu.hpp"
#include "../include/main.hpp"
#include "../include/motor.hpp"
#include "../include/speaker.hpp"
#include "../include/tof.hpp"
#include "../include/websocket.hpp"
void setup() {
  main_setup();
  tof_setup();
  imu_setup();
  motor_setup();
  /* ws_setup(); */
  /* bt_setup(); */
  /* speaker_setup(); */
}

void loop() {
  main_loop();
  tof_loop();
  imu_loop();
  motor_loop();
  /* ws_loop(); */
  /* speaker_loop(); */
}
