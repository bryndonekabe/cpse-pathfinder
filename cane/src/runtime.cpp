#include "../include/runtime.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "../include/mat.hpp"
#include "../include/motor.hpp"
#include "../include/vec.hpp"
#include <Arduino.h>
#include <cmath>

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

struct Zone {
  double nearest = INFINITY;
};
Zone left, center, right;
void hydrate_zones() {
  // left and right fov bounds that define the center
  static constexpr double LEFT_BOUND = rad(VIBRATION_LEFT_BOUND_DEG);
  static constexpr double RIGHT_BOUND = rad(VIBRATION_RIGHT_BOUND_DEG);

  left.nearest = INFINITY;
  center.nearest = INFINITY;
  right.nearest = INFINITY;
  for (const dvec3 &p : point_cloud) {
    // Ignore points behind us.
    if (p.z >= 0.0)
      continue;
    double angle = atan2(p.x, -p.z);
    double dist = p.length();

    if (angle < LEFT_BOUND) {
      left.nearest = std::min(left.nearest, dist);
    } else if (angle > RIGHT_BOUND) {
      right.nearest = std::min(right.nearest, dist);
    } else {
      center.nearest = std::min(center.nearest, dist);
    }
  }
}
double linear_intensity(double dist) {
  // NOTE: these are the near and far planes for the
  // sensor vibrations
  constexpr double MIN = VIBRATION_NEAR_PLANE_MM;
  constexpr double MAX = VIBRATION_FAR_PLANE_MM;
  if (dist == INFINITY)
    return 0.0;
  dist = std::clamp(dist, MIN, MAX);
  return 1.0 - (dist - MIN) / (MAX - MIN);
}
double exp_intensity(double dist) {
  double linear = linear_intensity(dist);
  return pow(linear, 2.0);
}

double intensity(double dist) { return exp_intensity(dist); }

void hydrate_motors() {
  double l = intensity(left.nearest);
  double c = intensity(center.nearest);
  double r = intensity(right.nearest);

  // NOTE: currently merging center and left/right nearest values
  double left_motor_intensity = (l + c) * 0.5;
  double right_motor_intensity = (r + c) * 0.5;
  // TODO: apply vibration motors
  motor_left.set_intensity(left_motor_intensity * 255);
  motor_right.set_intensity(right_motor_intensity * 255);

  Serial.printf("motorl: %i\n", motor_left.get_intensity());
  Serial.printf("motorr: %i\n", motor_right.get_intensity());
}

void rt_loop() {
  hydrate_cloud();
  hydrate_zones();
  hydrate_motors();

  /*
    NOTE: what i think we should do here is apply vibration
    based on "zones" left, right, center
    We average out the depth values and apply intensity based
    on that. If center overpowers both, then idk
    do we just vibrate both at same intensity? we'll figure it out
   */
}
