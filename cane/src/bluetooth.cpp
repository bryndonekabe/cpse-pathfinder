#include "../include/bluetooth.hpp"
#include "../include/no_respect_raw.hpp"
#include "BluetoothA2DPSource.h"
#include <Arduino.h>
#include <Preferences.h>
#include <math.h>

// constexpr float SAMPLE_RATE = 44100.0f;
// constexpr float FREQ = 440.0f;
// constexpr int16_t AMPLITUDE = 12000;
// static float phase = 0.0f;
// const uint8_t *audio = no_respect_raw;
// const size_t audio_len = no_respect_raw_len;

Preferences prefs;
esp_bd_addr_t saved_device;
// esp_bd_addr_t is 6 bytes
constexpr size_t SIZEOF_ADDR = sizeof(esp_bd_addr_t);

void bt_key(char *buf, int i) { sprintf(buf, "mac%d", i); }
void save_device(esp_bd_addr_t addr) {
  prefs.begin("bt_devices", false);

  // prefs.getBytes(key, stored, SIZEOF_ADDR); // for duplciates
  // if (memcmp(stored, addr, SIZEOF_ADDR) == 0) {
  //   prefs.end();
  //   return;
  // }

  char key[8];
  bt_key(key, 0);
  prefs.putBytes(key, addr, SIZEOF_ADDR);
  Serial.println("Device saved");

  prefs.end();
}

void load_device() {
  prefs.begin("bt_devices", true);

  // load up devices
  char key[8];
  bt_key(key, 0);
  prefs.getBytes(key, saved_device, SIZEOF_ADDR);
  prefs.end();

  Serial.println("Device loaded");
}

static size_t offset = 0;
int32_t get_sound_data(Frame *data, int32_t len) {
  for (int i = 0; i < len; i++) {
    if (offset >= no_respect_raw_len) {
      offset = 0;
    }

    uint8_t lo = pgm_read_byte(&no_respect_raw[offset]);
    uint8_t hi = pgm_read_byte(&no_respect_raw[offset + 1]);

    int16_t sample = (int16_t)(lo | (hi << 8));

    offset += 2;

    data[i].channel1 = sample;
    data[i].channel2 = sample;
  }

  return len;
}

// a2dp callbacks
BluetoothA2DPSource a2dp_source;

// Callback function executed whenever a Bluetooth device is discovered
esp_bd_addr_t best_addr;
int best_rssi = -127;

bool ssid_callback(const char *ssid, esp_bd_addr_t address, int rssi) {
  Serial.printf("Found %s RSSI [%d]\n", ssid, rssi);
  if (rssi > best_rssi) {
    best_rssi = rssi;
    memcpy(best_addr, address, sizeof(esp_bd_addr_t));
  }
  return false; // keep scanning
}
// when discovery ends, connect to the strongest device we've got
void discovery_callback(esp_bt_gap_discovery_state_t state) {
  switch (state) {
  case ESP_BT_GAP_DISCOVERY_STARTED: {
    Serial.println("Discovery started");
    if (!a2dp_source.is_connected()) {
      Serial.println("Trying saved device...");
      a2dp_source.connect_to(saved_device);
    }
    break;
  }
  case ESP_BT_GAP_DISCOVERY_STOPPED: {
    Serial.println("Discovery stopped");
    if (!a2dp_source.is_connected() && best_rssi != -127) {
      Serial.println("Connecting to strongest device...");
      a2dp_source.connect_to(best_addr);
      save_device(best_addr);
      best_rssi = -127;
      memset(best_addr, 0, sizeof(best_addr));
    }
    break;
  }
  default: {
    break;
  }
  }
}

void connection_state_callback(esp_a2d_connection_state_t state, void *ref) {
  Serial.print("Connection state: ");
  Serial.println(a2dp_source.to_str(state));
}
void audio_state_callback(esp_a2d_audio_state_t state, void *ref) {
  Serial.print("Audio state: ");
  Serial.println(a2dp_source.to_str(state));
}

void bt_setup() {

  a2dp_source.set_ssid_callback(ssid_callback);
  a2dp_source.set_discovery_mode_callback(discovery_callback);
  a2dp_source.set_on_connection_state_changed(connection_state_callback);
  a2dp_source.set_on_audio_state_changed(audio_state_callback);
  a2dp_source.set_data_callback_in_frames(get_sound_data);

  load_device();

  a2dp_source.start("ESP32_Audio_Source");
  Serial.println("Scanning for A2DP sinks...");
}
void bt_loop() {
  // The library handles audio streaming and background tasks automatically
}
