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

// NOTE: takes dvec3 in rad
dmat4 yaw_pitch_roll_rad(dvec3 rot) {
  return dmat4::rotate_z(rot.z) * // roll
         dmat4::rotate_x(rot.y) * // pitch
         dmat4::rotate_y(rot.x);  // yaw
}

constexpr size_t WIDTH = TOF_MAT_WIDTH;
constexpr size_t HEIGHT = TOF_MAT_HEIGHT;
constexpr double HFOV_DEG = TOF_FOV_H_DEG;
constexpr double VFOV_DEG = TOF_FOV_V_DEG;
// generates an array of normalized rays the correspond to
// tof matrix points
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

// convert sensor point into 3D point in the world
dvec3 sensor_point_world(TOFSensor &sensor, size_t idx) {
  uint16_t depth_mm = sensor.buf()[idx];

  // convert from degrees -> rad
  const dvec3 sensor_rot_rad =
      dvec3(rad(sensor.rot().x), rad(sensor.rot().y), rad(sensor.rot().z));

  // this is the point in local space coords
  // use the ray to get proper angles
  const dvec3 point_local = rays[idx] * static_cast<double>(depth_mm);

  // this is the point in world space coords
  const dvec4 point_world =
      yaw_pitch_roll_rad(sensor_rot_rad) * dvec4(point_local, 1.0);

  // NOTE: add position to sensor?
  // point = yaw_pitch_roll_rad(sensor.rot()) * point + sensor.position;
  return dvec3(point_world.x, point_world.y, point_world.z);
}

// upload tof sensor buffer to point cloud
void upload_tof(TOFSensor &sensor) {
  for (int i = 0; i < TOF_MAT_ELEMENTS; ++i) {
    uint16_t depth_mm = sensor.buf()[i];
    // multiplying by zero would be a problem
    if (depth_mm == 0 || depth_mm == TOF_DEPTH_INVALID)
      continue;

    point_cloud.push_back(sensor_point_world(sensor, i));
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

  // TODO: apply vibration motors
  /*
    NOTE: what i think we should do here is apply vibration
    based on "zones" left, right, center
    We average out the depth values and apply intensity based
    on that. If center overpowers both, then idk
    do we just vibrate both at same intensity? we'll figure it out
   */
}
