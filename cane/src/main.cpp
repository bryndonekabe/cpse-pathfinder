#include "../include/main.hpp"
#include "../include/config.hpp"

Preferences prefs;

// tof sensors
TOFSensor tof_left{TOF_LEFT_ADDR, TOF_LEFT_YPR};
TOFSensor tof_right{TOF_RIGHT_ADDR, TOF_RIGHT_YPR};

// motor
Motor motor1{MOTOR1_PIN};

// imu
IMU imu{IMU_ADDR};

// websocket
AsyncWebServer server{WS_PORT};
AsyncWebSocket ws{WS_EXTENSION};

// bluetooth
BluetoothA2DPSource a2dp_source;
esp_bd_addr_t saved_device;

void main_setup() { Serial.begin(SERIAL_BAUD); }

void main_loop() { return; }
