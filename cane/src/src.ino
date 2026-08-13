#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/imu.hpp"
#include "../include/main.hpp"
#include "../include/motor.hpp"
#include "../include/runtime.hpp"
#include "../include/speaker.hpp"
#include "../include/tof.hpp"
#include "../include/websocket.hpp"

void setup() {
  main_setup();

  Serial.println("Speaker:");
  speaker_setup();

  audio_manager.queue(FILE_POWER_ON);
  audio_manager.wait();

  Serial.println("Camera:");
  cam_setup();
  audio_manager.queue(FILE_CAM_INIT);
  audio_manager.wait();

  Serial.println("TOF:");
  tof_setup();
  audio_manager.queue(FILE_SENSOR_INIT);
  audio_manager.wait();

  /* Serial.println("IMU:"); */
  /* imu_setup(); */

  Serial.println("Motor:");
  motor_setup();
  audio_manager.queue(FILE_MOTOR_INIT);
  audio_manager.wait();

  Serial.println("Runtime:");
  rt_setup();

  Serial.println("Websocket:");
  ws_setup();
  audio_manager.queue(FILE_SERVER_INIT);
  audio_manager.wait();
}

void loop() {
  main_loop();

  speaker_loop();

  tof_loop();

  /* imu_loop(); */

  motor_loop();

  cam_loop();

  rt_loop();

  ws_loop();

  Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
}
