#pragma once
// main.hpp
#include "../include/config.hpp"
#include "../include/imu.hpp"
#include "../include/motor.hpp"
#include "../include/tof.hpp"
#include "BluetoothA2DPSource.h"
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

// imu
extern IMU imu;

// websocket
extern AsyncWebServer server;
extern AsyncWebSocket ws;

// bluetooth
extern esp_bd_addr_t saved_device;
extern BluetoothA2DPSource a2dp_source;
