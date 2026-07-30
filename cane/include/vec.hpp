#pragma once
#include "math.hpp"
#include "types.hpp"

// xy, st, uv, rg
template <typename T> struct tvec2 {
  union {
    T elements[2];
    struct {
      T x, y;
    };
    struct {
      T u, v;
    };
    struct {
      T r, g;
    };
    struct {
      T s, t;
    };
  };

  // ctors
  constexpr tvec2() : x(0), y(0) {}
  constexpr tvec2(T s) : x(s), y(s) {}
  constexpr tvec2(T _x, T _y) : x(_x), y(_y) {}
  template <typename U>
  constexpr tvec2(const tvec2<U> &v) : x((T)v.x), y((T)v.y) {}

  // statics
  constexpr static T dot(const tvec2 &a, const tvec2 &b) {
    return (a.x * b.x) + (a.y * b.y);
  }
  static auto distance(const tvec2 &a, const tvec2 &b) {
    return (a - b).length();
  }

  // methods
  constexpr auto length_squared() const { return square(x) + square(y); }
  auto length() const { return sqrt(length_squared()); }
  tvec2 normalized() const {
    auto len = length();
    return len != 0 ? *this / len : tvec2{};
  }

  // unary ops
  constexpr const T &operator[](size_t i) const { return elements[i]; }
  constexpr T &operator[](size_t i) { return elements[i]; }
  constexpr tvec2 operator-() const { return tvec2{-x, -y}; }
  constexpr tvec2 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tvec2 &a, const tvec2 &b) {
    return a.x == b.x && a.y == b.y;
  }

  constexpr tvec2 operator*(T s) const { return tvec2{x * s, y * s}; }
  constexpr tvec2 operator*(const tvec2 &v) const {
    return tvec2{v.x * x, v.y * y};
  }
  constexpr friend tvec2 operator*(T s, const tvec2 &v) { return v * s; }
  constexpr tvec2 &operator*=(T s) {
    x *= s;
    y *= s;
    return *this;
  }
  constexpr tvec2 &operator*=(const tvec2 &v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  constexpr tvec2 operator+(T s) const { return tvec2{x + s, y + s}; }
  constexpr tvec2 operator+(const tvec2 &v) const {
    return tvec2{v.x + x, v.y + y};
  }
  constexpr friend tvec2 operator+(T s, const tvec2 &v) { return v + s; }
  constexpr tvec2 &operator+=(T s) {
    x += s;
    y += s;
    return *this;
  }
  constexpr tvec2 &operator+=(const tvec2 &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  constexpr tvec2 operator/(T s) const { return tvec2{x / s, y / s}; }
  constexpr tvec2 operator/(const tvec2 &v) const {
    return tvec2{x / v.x, y / v.y};
  }
  constexpr tvec2 &operator/=(T s) {
    x /= s;
    y /= s;
    return *this;
  }
  constexpr tvec2 &operator/=(const tvec2 &v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  constexpr tvec2 operator-(T s) const { return tvec2{x - s, y - s}; }
  constexpr tvec2 operator-(const tvec2 &v) const {
    return tvec2{x - v.x, y - v.y};
  }
  constexpr tvec2 &operator-=(T s) {
    x -= s;
    y -= s;
    return *this;
  }
  constexpr tvec2 &operator-=(const tvec2 &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
};

// xyz, rgb, stp
template <typename T> struct tvec3 {
  union {
    T elements[3];
    struct {
      T x, y, z;
    };
    struct {
      T r, g, b;
    };
    struct {
      T s, t, p;
    };
  };

  // ctors
  constexpr tvec3() : x(0), y(0), z(0) {}
  constexpr tvec3(T s) : x(s), y(s), z(s) {}
  constexpr tvec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  template <typename U>
  constexpr tvec3(const tvec3<U> &v) : x((T)v.x), y((T)v.y), z((T)v.z) {}

  // vec upcast
  constexpr tvec3(const tvec2<T> &v, T _z) : x(v.x), y(v.y), z(_z) {}
  constexpr tvec3(T _x, const tvec2<T> &v) : x(_x), y(v.x), z(v.y) {}

  // statics
  constexpr static T dot(const tvec3 &a, const tvec3 &b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
  }
  static auto distance(const tvec3 &a, const tvec3 &b) {
    return (a - b).length();
  }

  // methods
  constexpr auto length_squared() const {
    return square(x) + square(y) + square(z);
  }
  auto length() const { return sqrt(length_squared()); }
  tvec3 normalized() const {
    auto len = length();
    return len != 0 ? *this / len : tvec3{};
  }

  // unary ops
  constexpr const T &operator[](size_t i) const { return elements[i]; }
  constexpr T &operator[](size_t i) { return elements[i]; }
  constexpr tvec3 operator-() const { return tvec3{-x, -y, -z}; }
  constexpr tvec3 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tvec3 &a, const tvec3 &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
  }

  constexpr tvec3 operator*(T s) const { return tvec3{x * s, y * s, z * s}; }
  constexpr tvec3 operator*(const tvec3 &v) const {
    return tvec3{v.x * x, v.y * y, v.z * z};
  }
  constexpr friend tvec3 operator*(T s, const tvec3 &v) { return v * s; }
  constexpr tvec3 &operator*=(T s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  constexpr tvec3 &operator*=(const tvec3 &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  constexpr tvec3 operator+(T s) const { return tvec3{x + s, y + s, z + s}; }
  constexpr tvec3 operator+(const tvec3 &v) const {
    return tvec3{v.x + x, v.y + y, v.z + z};
  }
  constexpr friend tvec3 operator+(T s, const tvec3 &v) { return v + s; }
  constexpr tvec3 &operator+=(T s) {
    x += s;
    y += s;
    z += s;
    return *this;
  }
  constexpr tvec3 &operator+=(const tvec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  constexpr tvec3 operator/(T s) const { return tvec3{x / s, y / s, z / s}; }
  constexpr tvec3 operator/(const tvec3 &v) const {
    return tvec3{x / v.x, y / v.y, z / v.z};
  }
  constexpr tvec3 &operator/=(T s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }
  constexpr tvec3 &operator/=(const tvec3 &v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  constexpr tvec3 operator-(T s) const { return tvec3{x - s, y - s, z - s}; }
  constexpr tvec3 operator-(const tvec3 &v) const {
    return tvec3{x - v.x, y - v.y, z - v.z};
  }
  constexpr tvec3 &operator-=(T s) {
    x -= s;
    y -= s;
    z -= s;
    return *this;
  }
  constexpr tvec3 &operator-=(const tvec3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
};

// xyzw, rgba, stpq
template <typename T> struct tvec4 {
  union {
    T elements[4];
    struct {
      T x, y, z, w;
    };
    struct {
      T r, g, b, a;
    };
    struct {
      T s, t, p, q;
    };
  };

  // ctors
  constexpr tvec4() : x(0), y(0), z(0), w(0) {}
  constexpr tvec4(T s) : x(s), y(s), z(s), w(s) {}
  constexpr tvec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
  template <typename U>
  constexpr tvec4(const tvec4<U> &v)
      : x((T)v.x), y((T)v.y), z((T)v.z), w((T)v.w) {}

  // vec upcast
  // vec2
  constexpr tvec4(const tvec2<T> &v, T _z, T _w) // (vec2, z, w)
      : x(v.x), y(v.y), z(_z), w(_w) {}
  constexpr tvec4(T _x, const tvec2<T> &v, T _w) // (x, vec2, w)
      : x(_x), y(v.x), z(v.y), w(_w) {}
  constexpr tvec4(T _x, T _y, const tvec2<T> &v) // (x, y, vec2)
      : x(_x), y(_y), z(v.x), w(v.y) {}
  // vec3
  constexpr tvec4(const tvec3<T> &v, T _w) : x(v.x), y(v.y), z(v.z), w(_w) {}
  constexpr tvec4(T _x, const tvec3<T> &v) : x(_x), y(v.x), z(v.y), w(v.z) {}

  // statics
  constexpr static T dot(const tvec4 &a, const tvec4 &b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
  }
  static auto distance(const tvec4 &a, const tvec4 &b) {
    return (a - b).length();
  }

  // methods
  constexpr auto length_squared() const {
    return square(x) + square(y) + square(z) + square(w);
  }
  auto length() const { return sqrt(length_squared()); }
  tvec4 normalized() const {
    auto len = length();
    return len != 0 ? *this / len : tvec4{};
  }

  // unary ops
  constexpr const T &operator[](size_t i) const { return elements[i]; }
  constexpr T &operator[](size_t i) { return elements[i]; }
  constexpr tvec4 operator-() const { return tvec4{-x, -y, -z, -w}; }
  constexpr tvec4 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tvec4 &a, const tvec4 &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
  }

  constexpr tvec4 operator*(T s) const {
    return tvec4{x * s, y * s, z * s, w * s};
  }
  constexpr tvec4 operator*(const tvec4 &v) const {
    return tvec4{v.x * x, v.y * y, v.z * z, v.w * w};
  }
  constexpr friend tvec4 operator*(T s, const tvec4 &v) { return v * s; }
  constexpr tvec4 &operator*=(T s) {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
  }
  constexpr tvec4 &operator*=(const tvec4 &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  constexpr tvec4 operator+(T s) const {
    return tvec4{x + s, y + s, z + s, w + s};
  }
  constexpr tvec4 operator+(const tvec4 &v) const {
    return tvec4{v.x + x, v.y + y, v.z + z, v.w + w};
  }
  constexpr friend tvec4 operator+(T s, const tvec4 &v) { return v + s; }
  constexpr tvec4 &operator+=(T s) {
    x += s;
    y += s;
    z += s;
    w += s;
    return *this;
  }
  constexpr tvec4 &operator+=(const tvec4 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  constexpr tvec4 operator/(T s) const {
    return tvec4{x / s, y / s, z / s, w / s};
  }
  constexpr tvec4 operator/(const tvec4 &v) const {
    return tvec4{x / v.x, y / v.y, z / v.z, w / v.w};
  }
  constexpr tvec4 &operator/=(T s) {
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
  }
  constexpr tvec4 &operator/=(const tvec4 &v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  constexpr tvec4 operator-(T s) const {
    return tvec4{x - s, y - s, z - s, w - s};
  }
  constexpr tvec4 operator-(const tvec4 &v) const {
    return tvec4{x - v.x, y - v.y, z - v.z, w - v.w};
  }
  constexpr tvec4 &operator-=(T s) {
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
  }
  constexpr tvec4 &operator-=(const tvec4 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }
};

using u8vec2 = tvec2<u8>;
using u8vec3 = tvec3<u8>;
using u8vec4 = tvec4<u8>;
using u16vec2 = tvec2<u16>;
using u16vec3 = tvec3<u16>;
using u16vec4 = tvec4<u16>;
using uvec2 = tvec2<u32>;
using uvec3 = tvec3<u32>;
using uvec4 = tvec4<u32>;
using u64vec2 = tvec2<u64>;
using u64vec3 = tvec3<u64>;
using u64vec4 = tvec4<u64>;

using i8vec2 = tvec2<i8>;
using i8vec3 = tvec3<i8>;
using i8vec4 = tvec4<i8>;
using i16vec2 = tvec2<i16>;
using i16vec3 = tvec3<i16>;
using i16vec4 = tvec4<i16>;
using ivec2 = tvec2<i32>;
using ivec3 = tvec3<i32>;
using ivec4 = tvec4<i32>;
using i64vec2 = tvec2<i64>;
using i64vec3 = tvec3<i64>;
using i64vec4 = tvec4<i64>;

using vec2 = tvec2<f32>;
using vec3 = tvec3<f32>;
using vec4 = tvec4<f32>;
using dvec2 = tvec2<f64>;
using dvec3 = tvec3<f64>;
using dvec4 = tvec4<f64>;
