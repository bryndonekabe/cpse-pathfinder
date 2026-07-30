#pragma once
#include "driver/i2s.h"

// serial baud rate
#define SERIAL_BAUD 115200

// tof sensors
#define TOF_LEFT_ADDR 0x32
#define TOF_RIGHT_ADDR 0x33
// yaw pitch roll
#define TOF_LEFT_YPR dvec3(-30.0, 0, 0)
#define TOF_RIGHT_YPR dvec3(-30.0, 0, 0)

#define TOF_MAT_HEIGHT 8
#define TOF_MAT_WIDTH 8
#define TOF_MAT_ELEMENTS 64
#define TOF_FOV_H_DEG 60.0
#define TOF_FOV_V_DEG 60.0

// vibration motor pin
#define MOTOR1_PIN 12

// websocket stuff
#define WIFI_SSID "USG-Mobility"
#define WIFI_PASSWORD "shadygrove9631"
#define WS_PORT 8765
#define WS_EXTENSION "/"
#define WS_INTERVAL 500

// imu stuff
#define IMU_ADDR 0x69

/* i2s speaker nonsense */
// Define the I2S port
#define I2S_PORT I2S_NUM_0
// Define the GPIO pins matching our wiring diagram
#define I2S_BCLK_PIN 14
#define I2S_LRC_PIN 25
#define I2S_DIN_PIN 26
