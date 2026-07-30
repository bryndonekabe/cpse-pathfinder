#include "../include/runtime.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "../include/mat.hpp"
#include "../include/vec.hpp"
#include <Arduino.h>

// The coordinate system internally of nanus is right-handed, meaning:
// + +X = move to the right
// + +Y = move up
// + -Z = move forward
// PLEASE NOTE how as Z gets more negative, it means you're moving more forward.
// This system applies to every matrix calculation, vector transformations, etc.

std::vector<dvec3> point_cloud;

dmat4 yaw_pitch_roll(vec3 rot) {
  return dmat4::rotate_x(rot.y) * dmat4::rotate_y(rot.x) *
         dmat4::rotate_z(rot.z);
}

constexpr size_t WIDTH = TOF_MAT_WIDTH;
constexpr size_t HEIGHT = TOF_MAT_HEIGHT;
constexpr double HFOV_DEG = TOF_FOV_H_DEG;
constexpr double VFOV_DEG = TOF_FOV_V_DEG;
dvec3 rays[WIDTH * HEIGHT];
void generate_rays() {
  const double hfov = rad(HFOV_DEG);
  const double vfov = rad(VFOV_DEG);

  for (int row = 0; row < HEIGHT; ++row) {
    for (int col = 0; col < WIDTH; ++col) {
      // Pixel-center angles
      const double yaw = (col - (WIDTH - 1) * 0.5) * (hfov / WIDTH);
      const double pitch = ((HEIGHT - 1) * 0.5 - row) * (vfov / HEIGHT);

      const double cos_p = cos(pitch);
      const double sin_p = sin(pitch);
      const double cos_y = cos(yaw);
      const double sin_y = sin(yaw);

      // +X = right
      // +Y = up
      // -Z = forward
      rays[row * WIDTH + col] =
          dvec3(cos_p * sin_y, sin_p, -cos_p * cos_y).normalized();
    }
  }
}

void upload_tof(TOFSensor &sensor) {
  for (int i = 0; i < 64; ++i) {
    uint16_t depth_mm = sensor.buf()[i];
    if (depth_mm == 0 || depth_mm == 4000)
      continue;
    dvec3 point = rays[i] * static_cast<double>(depth_mm);
    dvec4 vec4_point = yaw_pitch_roll(sensor.rot()) * dvec4(point, 1.0);
    // NOTE: add position to sensor?
    // point = yaw_pitch_roll(sensor.rot()) * point + sensor.position;
    point_cloud.push_back(dvec3(vec4_point.x, vec4_point.y, vec4_point.z));
  }
}

void hydrate_cloud() {
  point_cloud.clear();

  // TODO: use an array of tof sensors over
  // a bunch of globals
  upload_tof(tof_left);
  upload_tof(tof_right);
}

void rt_setup() { generate_rays(); }

void rt_loop() {
  // hydrate point cloud
  hydrate_cloud();
  // apply vibration motors

  // debug print NOTE: remove later bruh
  // Serial.printf("Point cloud size: %i", point_cloud.size());
  // Serial.println();
}
