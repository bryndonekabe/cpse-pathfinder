#include "../include/websocket.hpp"
#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "../include/motor.hpp"
#include "../include/runtime.hpp"
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// MAIN WEBSOCKET
MotorEquation get_equation(const String &str) {
  if (str == "linear") {
    return MotorEquation::Linear;
  } else if (str == "exponential") {
    return MotorEquation::Exponential;
  } else if (str == "logarithmic") {
    return MotorEquation::Logarithmic;
  } else if (str == "piecewise") {
    return MotorEquation::Piecewise;
  } else {
    return MotorEquation::Linear;
  }
}
void handle_user_settings(JsonObject &settings) {
  double near_m = settings["threshold_near"];
  double far_m = settings["threshold_far"];
  double mult_left = settings["motor_left_mult"];
  double mult_right = settings["motor_right_mult"];
  JsonArray levels = settings["piecewise_levels"];
  String equation = settings["motor_equation"];

  threshold_near_mm = near_m * 1000.0;
  threshold_far_mm = far_m * 1000.0;
  motor_mult_left = mult_left;
  motor_mult_right = mult_right;
  for (int i = 0; i < 3; ++i) {
    piecewise_levels[i] = levels[i];
  }
  motor_equation = get_equation(equation);
}

// Timing variable for data broadcast
unsigned long lastBroadcast = 0;
void main_handle_ws_msg(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
    String message((const char *)data, len);
    // Allocate JSON document
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("JSON Deserialization failed: "));
      Serial.println(error.f_str());
      return;
    }
    Serial.printf("Got JSON: %s\n", message.c_str());
    // Extract incoming key-value pairs
    if (doc.containsKey("command")) {
      String cmd = doc["command"];
      if (cmd == "settings") {
        JsonObject settings = doc["settings"];
        handle_user_settings(settings);
      }
    }
  }
}

// 2. WEBSOCKET EVENT ROUTER
void main_on_event(AsyncWebSocket *server, AsyncWebSocketClient *client,
                   AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                  client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    main_handle_ws_msg(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

// NOTE: for debug purposes only really
String point_cloud_json() {
  JsonDocument packet;

  JsonArray points = packet.createNestedArray("points");

  for (auto &pt : point_cloud) {
    JsonArray p = points.createNestedArray();

    p.add(pt.x);
    p.add(pt.y);
    p.add(pt.z);
  }

  String output;
  serializeJson(packet, output);
  return output;
}
String sensor_json() {
  JsonDocument packet;

  JsonObject motors = packet.createNestedObject("motors");
  motors["left"] = motor_left.get_intensity() / 255.0;
  motors["right"] = motor_right.get_intensity() / 255.0;
  /* motors  : {
      left : rand_range(0, 1),
      right : rand_range(0, 1)
    }, */

  JsonObject diagnostics = packet.createNestedObject("diagnostics");
  // TODO: these diagnostics?
  diagnostics["cpu"] = 30.0;
  diagnostics["battery"] = 100.0;
  diagnostics["refresh_rate"] = 30.0;
  diagnostics["speed"] = 1.0;
  diagnostics["bottleneck"] = "none";
  diagnostics["uptime"] = millis() / 1000.0;
  diagnostics["temp"] = 40.0;
  diagnostics["signal"] = -60.0;
  /* diagnostics : {
      cpu: rand_range(20, 60),
      battery: rand_range(50, 100),
      refresh_rate: 30,
      speed: rand_range(0, 1),
      bottleneck: "none",
      uptime: process.uptime(),
      temp: rand_range(35, 50),
      signal: rand_range(-70, -40),
    }, */

  JsonArray updates = packet.createNestedArray("updates");
  for (int i = 0; i < 64; ++i) {
    uint16_t prev_d = tof_left.prev()[i];
    uint16_t d = tof_left.buf()[i];
    if (d == 4000 || prev_d == d)
      continue;
    JsonObject update = updates.createNestedObject();
    int row = i / 8;
    int col = i % 8;
    update["i"] = row * 16 + col;
    update["d"] = (double)d / (double)1000.0f;
  }
  for (int i = 0; i < 64; ++i) {
    uint16_t prev_d = tof_right.prev()[i];
    uint16_t d = tof_right.buf()[i];
    if (d == 4000 || prev_d == d)
      continue;
    JsonObject update = updates.createNestedObject();
    int row = i / 8;
    int col = i % 8;
    update["i"] = row * 16 + (col + 8);
    update["d"] = (double)d / (double)1000.0f;
  }
  /* updates : new Array(64).fill({i : 0, d : 0}).map(function(val, i) {
      return {i : rand_int(0, 63), d : rand_range(0, 3.5)};
    }), */

  packet["timestamp"] = millis();
  /* timestamp : Date.now(), */

  String json_str;
  serializeJson(packet, json_str);
  return json_str;
}
// 3. BROADCAST OUTGOING JSON DATA
void broadcast_sensor_data() {
  uint32_t t = micros();

  // Serial.printf("json generation: %lu us\n", micros() - t);

  t = micros();

  // Broadcast text string to all connected web clients
  String json_str = sensor_json();
  // String json_str = point_cloud_json();
  // Serial.printf("JSON: %s\n", json_str.c_str());
  // Serial.printf("JSON\n");
  main_ws.textAll(json_str);
  // Serial.printf("ws send: %lu us\n", micros() - t);
}

// PREVIEW WEBSOCKET
String preview_json() {
  String image = camera_ai.last_image();

  Serial.printf("Image length: %u\n", image.length());

  JsonDocument packet;
  packet["image"] = image;

  String json_str;
  serializeJson(packet, json_str);

  Serial.printf("Preview JSON length: %u\n", json_str.length());

  return json_str;
}
void preview_handle_ws_msg(AsyncWebSocketClient *client, void *arg,
                           uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
    String message((const char *)data, len);

    // Allocate JSON document
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.print(F("JSON Deserialization failed: "));
      Serial.println(error.f_str());
      return;
    }

    Serial.printf("Got JSON: %s\n", message.c_str());

    // Extract incoming key-value pairs
    if (doc.containsKey("command")) {
      String cmd = doc["command"];
      if (cmd == "request_frame") {
        // TODO: send base64 string
        String response = preview_json();
        Serial.printf("Sending preview: %u bytes\n", response.length());
        client->text(response);
        // preview_ws.textAll(preview_json());
      }
    }
  }
}

void preview_on_event(AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
  case WS_EVT_CONNECT:
    Serial.printf("Preview WebSocket client #%u connected from %s\n",
                  client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("Preview WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    preview_handle_ws_msg(client, arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}
void ws_setup() {
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nConnected! IP Address: %s\n",
                WiFi.localIP().toString().c_str());

  // Attach WebSocket handlers
  main_ws.onEvent(main_on_event);
  preview_ws.onEvent(preview_on_event);

  main_server.addHandler(&main_ws);
  main_server.addHandler(&preview_ws);

  // Start the server
  main_server.begin();
}

void ws_loop() {
  main_ws.cleanupClients();

  // NOTE: preview websocket must be queried via command
  // preview_ws.cleanupClients();

  // Send sensor data periodically over main server
  if (millis() - lastBroadcast >= MAIN_WS_INTERVAL) {
    lastBroadcast = millis();
    uint32_t before = micros();
    broadcast_sensor_data();
    uint32_t after = micros();

    Serial.printf("WS broadcast: %lu us \n", after - before);
  }
}
