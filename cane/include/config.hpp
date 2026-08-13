#pragma once
#include "../include/motor.hpp"
#include "driver/i2s.h"

// reset pin
#define SOFTWARE_RESET_PIN D6
// serial baud rate
#define SERIAL_BAUD 115200

// tof sensors
#define TOF_LEFT_ADDR 0x33
#define TOF_RIGHT_ADDR 0x32
// yaw pitch roll (in degrees)
#define TOF_LEFT_YPR dvec3(-30.0, 0, 0)
#define TOF_RIGHT_YPR dvec3(30.0, 0, 0)
#define TOF_MAT_HEIGHT 8
#define TOF_MAT_WIDTH 8
#define TOF_MAT_ELEMENTS (TOF_MAT_HEIGHT * TOF_MAT_WIDTH)
#define TOF_FOV_H_DEG 60.0
#define TOF_FOV_V_DEG 60.0
#define TOF_DEPTH_INVALID 4000

// vibration motor pin
// NOTE: d9, and d10
#define MOTOR_LEFT_PIN D9
#define MOTOR_RIGHT_PIN D10

#define VIBRATION_LEFT_BOUND_DEG -20.0
#define VIBRATION_RIGHT_BOUND_DEG 20.0

#define VIBRATION_NEAR_PLANE_MM 0.0
#define VIBRATION_FAR_PLANE_MM 3000.0

// NOTE: level 3 is the *strongest* level, meaning 1 is the closest
#define VIBRATION_LVL_1_DEFAULT 0.2
#define VIBRATION_LVL_2_DEFAULT 0.5
#define VIBRATION_LVL_3_DEFAULT 1.0
#define MOTOR_EQUATION_DEFAULT MotorEquation::Exponential
// camera stuff
#define CAMERA_AI_FILTER_RESULTS false
#define CAMERA_AI_SHOW_PREVIEW true
#define CAMERA_AI_CONFIDENCE_THRESHOLD 60
// how long before an object is considered "lost"
#define CAMERA_AI_LOST_TIMEOUT_MS 1000
#define CAMERA_TARGET_ID_PERSON 0

// NOTE: based on the person detection model
#define CAMERA_AI_WIDTH_PIXELS 320
#define CAMERA_AI_HEIGHT_PIXELS 240
// NOTE: double check these
#define CAMERA_AI_FOV_H_DEG 120.0
#define CAMERA_AI_FOV_V_DEG 90.0

// only considered "close" depth within 5 degrees
#define CAMERA_TOF_MAX_ANGLE_TOLERANCE_DEG 5.0

// websocket stuff
#define WIFI_SSID "USG-Mobility"
#define WIFI_PASSWORD "shadygrove9631"

#define MAIN_WS_INTERVAL 500
#define MAIN_WS_PORT 8765
#define MAIN_WS_EXTENSION "/"
#define PREVIEW_WS_EXTENSION "/preview"

// imu stuff
#define IMU_ADDR 0x69

// speaker stuff
// NOTE: dfplayer TX is connected here
#define SPEAKER_RX_PIN D2
// NOTE: dfplayer RX is connected here
#define SPEAKER_TX_PIN D3

// NOTE: goes from 0-30
#define SPEAKER_DEFAULT_VOLUME 15

// NOTE: file mapping based on sdcard layout
#define FILE_DEFAULT_AUDIO 1
#define FILE_POWER_ON 2
#define FILE_WIFI_CONNECTED 3

#define FILE_PERSON_DETECTED 4

#define FILE_DIRECTION_RIGHT 5
#define FILE_DIRECTION_LEFT 6
#define FILE_DIRECTION_AHEAD 7

#define FILE_DISTANCE_CLOSE 8
#define FILE_DISTANCE_FAR 9

#define FILE_SENSOR_INIT 10
#define FILE_CAM_INIT 11
#define FILE_MOTOR_INIT 12
#define FILE_SERVER_INIT 13
