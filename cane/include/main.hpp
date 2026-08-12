#pragma once
// main.hpp
#include "../include/config.hpp"
#include "../include/imu.hpp"
#include "../include/motor.hpp"
#include "../include/speaker.hpp"
#include "../include/tof.hpp"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Seeed_Arduino_SSCMA.h>
#include <WiFi.h>
#include <Wire.h>

void main_setup();
void main_loop();

// NVRAM data available across boots
extern Preferences prefs;

// tof sensors
extern TOFSensor tof_left;
extern TOFSensor tof_right;

// runtime
extern std::vector<dvec3> point_cloud;

// motor
extern Motor motor_left;
extern Motor motor_right;

// modifiers
extern double threshold_near_mm;
extern double threshold_far_mm;
extern double motor_mult_left;
extern double motor_mult_right;
extern double piecewise_levels[3];
extern MotorEquation motor_equation;

// camera
extern SSCMA camera_ai;

// speaker
extern AudioManager audio_manager;

// imu
extern IMU imu;

// websocket
extern AsyncWebServer main_server;
extern AsyncWebSocket main_ws;
extern AsyncWebSocket preview_ws;
