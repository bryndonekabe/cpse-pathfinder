#pragma once
// main.hpp
#include "../include/config.hpp"
#include "../include/imu.hpp"
#include "../include/motor.hpp"
#include "../include/tof.hpp"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <WiFi.h>

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

extern double threshold_near_mm;
extern double threshold_far_mm;

extern double motor_mult_left;
extern double motor_mult_right;

// speaker
extern DFRobotDFPlayerMini df_player;

// imu
extern IMU imu;

// websocket
extern AsyncWebServer server;
extern AsyncWebSocket ws;
