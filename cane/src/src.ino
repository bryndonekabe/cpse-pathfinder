#include "../include/bluetooth.hpp"
#include "../include/imu.hpp"
#include "../include/main.hpp"
#include "../include/motor.hpp"
#include "../include/runtime.hpp"
#include "../include/speaker.hpp"
#include "../include/tof.hpp"
#include "../include/websocket.hpp"

void setup() {
  main_setup();

  Serial.println("TOF:");
  tof_setup();
  /* Serial.println("IMU:"); */
  /* imu_setup(); */
  Serial.println("Motor:");
  motor_setup();

  Serial.println("Runtime:");
  rt_setup();

  Serial.println("Websocket:");
  ws_setup();
  /* Serial.println("Bluetooth:"); */
  /* bt_setup(); */
  /* speaker_setup(); */
}

void loop() {
  main_loop();
  tof_loop();
  /* imu_loop(); */
  motor_loop();

  rt_loop();

  ws_loop();
  /* bt_loop(); */
  /* speaker_loop(); */
}
