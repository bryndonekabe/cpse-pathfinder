#pragma once
#include "driver/i2s.h"

// serial baud rate
#define SERIAL_BAUD 115200

// tof sensors
#define TOF_LEFT_ADDR 0x32
#define TOF_RIGHT_ADDR 0x33
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
#define MOTOR_LEFT_PIN 9
#define MOTOR_RIGHT_PIN 10

#define VIBRATION_LEFT_BOUND_DEG -20.0
#define VIBRATION_RIGHT_BOUND_DEG 20.0

#define VIBRATION_NEAR_PLANE_MM 0.0
#define VIBRATION_FAR_PLANE_MM 3000.0

// camera stuff
#define CAMERA_AI_FILTER_RESULTS false
#define CAMERA_AI_SHOW_PREVIEW true
#define CAMERA_AI_CONFIDENCE_THRESHOLD 70
// how long before an object is considered "lost"
#define CAMERA_AI_LOST_TIMEOUT_MS 1000
#define CAMERA_TARGET_ID_PERSON 0

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
#define SPEAKER_RX_PIN D3
// NOTE: dfplayer RX is connected here
#define SPEAKER_TX_PIN D4

// NOTE: goes from 0-30
#define SPEAKER_DEFAULT_VOLUME 15

#define FILE_POWER_ON 2
#define FILE_WIFI_CONNECTED 3
#define FILE_PERSON_DETECTED 4
