#include "../include/runtime.hpp"
#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/main.hpp"
#include "../include/mat.hpp"
#include "../include/motor.hpp"
#include "../include/vec.hpp"
#include <Arduino.h>
#include <cmath>

/* Point cloud stuff */
// The coordinate system internally is right-handed, meaning:
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

/* Zones and setting motor intensity */
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

// normalized intensity from 0 - 1.0
double linear_intensity(double dist) {
  // NOTE: these are the near and far planes for the
  // sensor vibrations
  const double MIN = threshold_near_mm;
  const double MAX = threshold_far_mm;
  if (dist == INFINITY)
    return 0.0;
  dist = std::clamp(dist, MIN, MAX);
  return 1.0 - (dist - MIN) / (MAX - MIN);
}
// use an exponential equation
double exp_intensity(double dist) {
  double linear = linear_intensity(dist);
  return pow(linear, 2.0);
}
// use a linear equation
double log_intensity(double dist) {
  double linear = linear_intensity(dist);
  if (linear <= 0.0)
    return 0.0;
  constexpr double k = 9.0; // k determines the curve
  return log1p(k * linear) / log1p(k);
}
// stepwise piecewise intensity setting
double piecewise_intensity(double dist) {
  // 4m, 2.5m, and 1m
  if (dist == INFINITY)
    return 0.0;

  if (dist > 2500.0)
    return piecewise_levels[0]; // 2.5m–4m, level 1

  if (dist > 1000.0)
    return piecewise_levels[1]; // 1m–2.5m, level 2

  return piecewise_levels[2]; // 0–1m, level 3
}
double intensity(double dist) {
  switch (motor_equation) {
  case MotorEquation::Logarithmic:
    return log_intensity(dist);
    break;
  case MotorEquation::Piecewise:
    return piecewise_intensity(dist);
    break;
  case MotorEquation::Exponential:
    return exp_intensity(dist);
    break;
  case MotorEquation::Linear: // fallthrough intentionally
  default:
    return linear_intensity(dist);
    break;
  }
}

void hydrate_motors() {
  double l = intensity(left.nearest);
  double c = intensity(center.nearest);
  double r = intensity(right.nearest);

  // NOTE: currently merging center and left/right nearest values
  double left_intensity = (l + c) * 0.5;
  double right_intensity = (r + c) * 0.5;
  // TODO: apply vibration motors
  // motor_left.set_intensity(left_intensity * 255);
  // motor_right.set_intensity(right_intensity * 255);

  double clamped_l = std::clamp(left_intensity * motor_mult_left, 0.0, 1.0);
  double clamped_r = std::clamp(right_intensity * motor_mult_right, 0.0, 1.0);

  if (clamped_l >= clamped_r) {
    motor_left.set_intensity(clamped_l * 255);
    motor_right.set_intensity(0);
  } else {
    motor_left.set_intensity(0);
    motor_right.set_intensity(clamped_r * 255);
  }

  // Serial.printf("motorl: %i\n", motor_left.get_intensity());
  // Serial.printf("motorr: %i\n", motor_right.get_intensity());
}

/* Camera stuff */
// TODO: create 3d spaces bounding boxes
struct Bound {
  dvec2 box;           // width, height in world mm
  dvec3 pos;           // x, y, and z world position mm
  unsigned int target; // based on class target
  unsigned int score;  // 0 - 100
};
std::vector<Bound> bounds;

// NOTE: in pixels
// TODO: change based on the model used
constexpr size_t CAMERA_WIDTH = CAMERA_AI_WIDTH_PIXELS;
constexpr size_t CAMERA_HEIGHT = CAMERA_AI_HEIGHT_PIXELS;
constexpr double CAMERA_HFOV_DEG = CAMERA_AI_FOV_H_DEG;
constexpr double CAMERA_VFOV_DEG = CAMERA_AI_FOV_V_DEG;
// Camera optical center.
const double CAMERA_CX = (CAMERA_WIDTH - 1) * 0.5;
const double CAMERA_CY = (CAMERA_HEIGHT - 1) * 0.5;
// Focal lengths derived from FOV.
const double CAMERA_FX = CAMERA_CX / tan(rad(CAMERA_HFOV_DEG) * 0.5);
const double CAMERA_FY = CAMERA_CY / tan(rad(CAMERA_VFOV_DEG) * 0.5);
// Convert camera pixel -> normalized camera-local ray.
// +X = right
// +Y = up
// -Z = forward
dvec3 camera_ray(double px, double py) {
  const double x = (px - CAMERA_CX) / CAMERA_FX;
  const double y = -(py - CAMERA_CY) / CAMERA_FY;
  dvec3 ray(x, y, -1.0);
  // --------------------------------------------------
  // TODO: apply camera yaw/pitch/roll here.
  // This converts the ray from camera-local space
  // into world-space
  // --------------------------------------------------
  // Example eventually:
  // ray = yaw_pitch_roll_rad(camera_rot_rad) * dvec4(ray, 0.0);
  // Use w = 0 because this is a direction, not a position.
  // NOTE: currently, it just assumes the camera is facing straight on
  return ray.normalized();
}

constexpr double CAMERA_TOF_MIN_ALIGNMENT =
    cos(rad(CAMERA_TOF_MAX_ANGLE_TOLERANCE_DEG));
// return the tof point length of the point with the smallest angular distance
// from the camera ray
// NOTE: returns a POSITIVE value, not a world-space negative depth for forward
double closest_depth_mm(const dvec3 &camera_ray) {
  double closest = INFINITY;
  double best_alignment = CAMERA_TOF_MIN_ALIGNMENT;

  for (const dvec3 &point : point_cloud) {
    // Ignore points behind the camera.
    if (point.z >= 0.0)
      continue;
    const double distance = point.length();
    if (distance <= 0.0)
      continue;
    const dvec3 point_dir = point / distance;
    // 1.0 = exactly same direction
    // 0.0 = 90 degrees apart
    // -1.0 = opposite direction
    const double alignment = dvec3::dot(camera_ray, point_dir);
    // Only consider points reasonably close to the
    // camera ray.
    if (alignment > best_alignment) {
      best_alignment = alignment;
      closest = distance;
    }
  }

  return closest;
}

Bound generate_bound(const boxes_t &box, double depth_mm) {
  Bound bound;
  bound.target = box.target;
  bound.score = box.score;
  // Center ray
  const dvec3 ray = camera_ray(box.x, box.y);
  // Put center at the measured depth.
  // NOTE: forward is NEGATIVE depth
  const double z = -depth_mm;
  bound.pos = ray * (z / ray.z);
  // Convert pixel dimensions -> angular dimensions.
  const double angular_width =
      (box.w / (double)CAMERA_WIDTH) * rad(CAMERA_HFOV_DEG);
  const double angular_height =
      (box.h / (double)CAMERA_HEIGHT) * rad(CAMERA_VFOV_DEG);
  // Physical dimensions at this depth.
  bound.box = dvec2(2.0 * depth_mm * tan(angular_width * 0.5),
                    2.0 * depth_mm * tan(angular_height * 0.5));
  return bound;
}

void hydrate_bounds() {
  bounds.clear();
  for (const auto &box : camera_ai.boxes()) {
    auto closest = closest_depth_mm(camera_ray(box.x, box.y));
    if (closest == INFINITY)
      continue;
    bounds.push_back(generate_bound(box, closest));
  }
}

/* Audio stuff */
enum class Target { Default, Person };
enum class Distance { Close, Far };
enum class Direction { Left, Right, Ahead };
uint8_t target_audio(Target target) {
  switch (target) {
  case Target::Person:
    return FILE_PERSON_DETECTED;
    break;
  default:
    return FILE_DEFAULT_AUDIO;
    break;
  }
}
uint8_t dist_audio(Distance dist) {
  switch (dist) {
  case Distance::Close:
    return FILE_DISTANCE_CLOSE;
    break;
  case Distance::Far:
    return FILE_DISTANCE_FAR;
    break;
  default:
    return FILE_DEFAULT_AUDIO;
    break;
  }
}
uint8_t dir_audio(Direction dir) {
  switch (dir) {
  case Direction::Left:
    return FILE_DIRECTION_LEFT;
    break;
  case Direction::Right:
    return FILE_DIRECTION_RIGHT;
    break;
  case Direction::Ahead:
    return FILE_DIRECTION_AHEAD;
    break;
  default:
    return FILE_DEFAULT_AUDIO;
    break;
  }
}
Target get_target(const Bound &bound) {
  switch (bound.target) {
  case CAMERA_TARGET_ID_PERSON:
    return Target::Person;
    break;
  default:
    return Target::Default;
    break;
  }
}
Distance get_dist(const Bound &bound) {
  double dist = bound.pos.length();
  if (dist >= threshold_far_mm) {
    return Distance::Far;
  } else {
    return Distance::Close;
  }
}
Direction get_dir(const Bound &bound) {
  double angle = atan2(bound.pos.x, -bound.pos.z);
  double lbound = rad(VIBRATION_LEFT_BOUND_DEG);
  double rbound = rad(VIBRATION_RIGHT_BOUND_DEG);
  if (angle < lbound) {
    return Direction::Left;
  } else if (angle > rbound) {
    return Direction::Right;
  } else {
    return Direction::Ahead;
  }
}

void spatial_audio(Target target, Distance dist, Direction dir) {
  audio_manager.queue(target_audio(target));
  audio_manager.queue(dist_audio(dist));
  audio_manager.queue(dir_audio(dir));
}

void queue_audio() {
  if (bounds.empty())
    return;

  const Bound *closest = nullptr;
  double closest_dist = INFINITY;
  for (const auto &bound : bounds) {
    // look through for closest bound
    // queue audio for that bound
    const double dist = bound.pos.length();
    if (dist < closest_dist) {
      closest_dist = dist;
      closest = &bound;
    }
  }
  if (closest == nullptr)
    return;

  // get distance and direction enums
  const auto target = get_target(*closest);
  const auto dist = get_dist(*closest);
  const auto dir = get_dir(*closest);

  spatial_audio(target, dist, dir);
}

bool detected() {
  for (const auto &bound : bounds) {
    // if you are past the confidence threshold
    if (bound.score >= CAMERA_AI_CONFIDENCE_THRESHOLD) {
      return true;
    }
  }
  return false;
}

void try_cue() {
  static bool present = false;
  static unsigned long last_seen = 0;
  if (detected()) {
    last_seen = millis();

    if (!present) {
      queue_audio();
      present = true;
    }
  } else if (millis() - last_seen > CAMERA_AI_LOST_TIMEOUT_MS) {
    present = false;
  }
}

void rt_setup() { generate_rays(); }

// TODO: run audio based on directions and depth of bounding boxes
void rt_loop() {
  hydrate_cloud();
  hydrate_zones();
  hydrate_motors();
  hydrate_bounds();

  // TODO: generate audio to play for closest bound based on:
  // closest bound, target, direction, and distance
  // attempt to do an audio cue
  try_cue();

  Serial.printf("L nearest=%.1f C nearest=%.1f R nearest=%.1f | "
                "L motor=%d R motor=%d\n",
                left.nearest, center.nearest, right.nearest,
                motor_left.get_intensity(), motor_right.get_intensity());

  /*
    NOTE: what i think we should do here is apply vibration
    based on "zones" left, right, center
    We average out the depth values and apply intensity based
    on that. If center overpowers both, then idk
    do we just vibrate both at same intensity? we'll figure it out
   */
}
