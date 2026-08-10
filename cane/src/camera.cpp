#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"

void print_box(boxes_t &box) {
  Serial.print("target=");
  Serial.print(box.target);
  Serial.print(", score=");
  Serial.print(box.score);
  Serial.print(", x=");
  Serial.print(box.x);
  Serial.print(", y=");
  Serial.print(box.y);
  Serial.print(", w=");
  Serial.print(box.w);
  Serial.print(", h=");
  Serial.println(box.h);
}
void cam_perf() {
  Serial.println("invoke success");
  Serial.print("perf: prepocess=");
  Serial.print(camera_ai.perf().prepocess);
  Serial.print(", inference=");
  Serial.print(camera_ai.perf().inference);
  Serial.print(", postpocess=");
  Serial.println(camera_ai.perf().postprocess);
}
void cam_info() {
  // cam_perf();
  for (int i = 0; i < camera_ai.boxes().size(); i++) {
    print_box(camera_ai.boxes()[i]);
  }
  if (camera_ai.last_image().length() > 0) {
    Serial.print("Last image:");
    Serial.println(camera_ai.last_image().c_str());
  }
}
void cam_setup() { camera_ai.begin(); }

void person_cue() { df_player.playMp3Folder(FILE_PERSON_DETECTED); }
bool person_detected() {
  for (auto &box : camera_ai.boxes()) {
    // if you are a person, and past the confidence threshold
    if (box.target == CAMERA_TARGET_ID_PERSON &&
        box.score >= CAMERA_AI_CONFIDENCE_THRESHOLD) {
      return true;
    }
  }

  return false;
}
void try_cue() {
  static bool person_present = false;
  static unsigned long last_person_seen = 0;
  if (person_detected()) {
    last_person_seen = millis();

    if (!person_present) {
      person_cue();
      person_present = true;
    }
  } else if (millis() - last_person_seen > CAMERA_AI_LOST_TIMEOUT_MS) {
    person_present = false;
  }
}

void cam_loop() {
  // NOTE: last param is if you want to get base64 framebuffer
  if (!camera_ai.invoke(1, CAMERA_AI_FILTER_RESULTS, CAMERA_AI_SHOW_PREVIEW)) {
    cam_info();
    try_cue();
  }
}
