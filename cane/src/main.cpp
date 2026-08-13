#include "../include/main.hpp"
#include "../include/config.hpp"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>

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

double piecewise_levels[3] = {VIBRATION_LVL_1_DEFAULT, VIBRATION_LVL_2_DEFAULT,
                              VIBRATION_LVL_3_DEFAULT};
MotorEquation motor_equation = MOTOR_EQUATION_DEFAULT;

// camera
SSCMA camera_ai;

// speaker
AudioManager audio_manager{SPEAKER_RX_PIN, SPEAKER_TX_PIN};

// imu
IMU imu{IMU_ADDR};

// websocket
AsyncWebServer main_server{MAIN_WS_PORT};
AsyncWebSocket main_ws{MAIN_WS_EXTENSION};
AsyncWebSocket preview_ws{PREVIEW_WS_EXTENSION};

bool last_pin_state = HIGH;
void main_setup() {
  Serial.begin(SERIAL_BAUD);
  delay(2000);

  pinMode(SOFTWARE_RESET_PIN, INPUT_PULLUP);
  last_pin_state = digitalRead(SOFTWARE_RESET_PIN);
}

void main_loop() {
  bool curr_state = digitalRead(SOFTWARE_RESET_PIN);
  // check if pressed
  if (last_pin_state == HIGH && curr_state == LOW) {
    Serial.println("Restarting...");
    delay(50);
    ESP.restart();
  }

  last_pin_state = curr_state;
}
