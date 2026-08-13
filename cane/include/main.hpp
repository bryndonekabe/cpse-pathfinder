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
#include <atomic>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

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
extern std::atomic<double> threshold_near_mm;
extern std::atomic<double> threshold_far_mm;
extern std::atomic<double> motor_mult_left;
extern std::atomic<double> motor_mult_right;
extern std::atomic<double> piecewise_level_one;
extern std::atomic<double> piecewise_level_two;
extern std::atomic<double> piecewise_level_three;

extern std::atomic<MotorEquation> motor_equation;

// camera
extern SSCMA camera_ai;
// NOTE: we need this mutex to avoid race conditions from the asynchronous
// servers
extern SemaphoreHandle_t invoke_mutex;

// speaker
extern AudioManager audio_manager;

// imu
extern IMU imu;

// websocket
extern AsyncWebServer main_server;
extern AsyncWebSocket main_ws;
extern AsyncWebSocket preview_ws;
