#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "../include/speaker.hpp"

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

void cam_loop() {
  // NOTE: last param is if you want to get base64 framebuffer

  if (xSemaphoreTake(invoke_mutex,
                     pdMS_TO_TICKS(INVOKE_MUTEX_TIMEOUT_CAMERA)) != pdTRUE) {
    Serial.println("mutex timeout cam loop");
    return;
  }

  if (!camera_ai.invoke(1, CAMERA_AI_FILTER_RESULTS, CAMERA_AI_SHOW_PREVIEW)) {
    cam_info();
    // try_cue();
  }

  xSemaphoreGive(invoke_mutex);
}
