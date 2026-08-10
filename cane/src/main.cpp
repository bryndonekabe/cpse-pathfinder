#include "../include/main.hpp"
#include "../include/config.hpp"
#include "DFRobotDFPlayerMini.h"

Preferences prefs;

// tof sensors
TOFSensor tof_left{TOF_LEFT_ADDR, TOF_LEFT_YPR};
TOFSensor tof_right{TOF_RIGHT_ADDR, TOF_RIGHT_YPR};

// motor
Motor motor_left{MOTOR_LEFT_PIN};
Motor motor_right{MOTOR_RIGHT_PIN};

double threshold_near_mm = VIBRATION_NEAR_PLANE_MM;
double threshold_far_mm = VIBRATION_FAR_PLANE_MM;

double motor_mult_left = 1.0;
double motor_mult_right = 1.0;

// camera
SSCMA camera_ai;

// speaker
DFRobotDFPlayerMini df_player;

// imu
IMU imu{IMU_ADDR};

// websocket
AsyncWebServer main_server{MAIN_WS_PORT};
AsyncWebSocket main_ws{MAIN_WS_EXTENSION};
AsyncWebSocket preview_ws{PREVIEW_WS_EXTENSION};

void main_setup() {
  Serial.begin(SERIAL_BAUD);

  delay(2000);
}

void main_loop() { return; }
