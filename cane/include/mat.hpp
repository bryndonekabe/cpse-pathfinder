#pragma once
#include "math.hpp"
#include "types.hpp"
#include "vec.hpp"

/*
  mat * mat: (AB).col[i] = A * B.col[i]
  mat * vec: (MV) = (V[i]*M.col[i] + ...)
  NOTE: Matrices are COLUMN MAJOR
*/
template <typename T> struct tmat2 {
  union {
    tvec2<T> columns[2];
    struct {
      tvec2<T> col0, col1;
    };
  };

  // ctors
  tmat2() : columns{} {}
  tmat2(const tvec2<T> &v0, const tvec2<T> &v1) : col0(v0), col1(v1) {}

  // methods
  // swap row[i] with column[i]
  constexpr tmat2 transpose() const {
    return tmat2{{col0.x, col1.x}, (col0.y, col1.y)};
  }
  // how much the transformation scales space
  constexpr T determinant() const { return col0.x * col1.y - col1.x * col0.y; }
  // undoes the current matrix
  constexpr tmat2<T> inverse() const {
    T det = determinant();
    T inv_det = T(1) / det;

    return tmat2<T>{{col1.y, -col0.y}, {-col1.x, col0.x}} * inv_det;
  }

  // unary ops
  constexpr const tvec2<T> &operator[](size_t i) const { return columns[i]; }
  constexpr tvec2<T> &operator[](size_t i) { return columns[i]; }
  constexpr tmat2 operator-() const { return tmat2{-col0, -col1}; }
  constexpr tmat2 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tmat2 &a, const tmat2 &b) {
    return a.col0 == b.col0 && a.col1 == b.col1;
  }

  constexpr tmat2 operator*(T s) const { return tmat2{col0 * s, col1 * s}; }
  constexpr tvec2<T> operator*(const tvec2<T> &v) const {
    // mat * vec: (MV) = (V[i]*M.col[i] + ...)
    return (v[0] * col0) + (v[1] * col1);
  }
  constexpr tmat2 operator*(const tmat2 &m) const {
    // mat * mat: (AB).col[i] = A * B.col[i]
    return tmat2{*this * m.col0, *this * m.col1};
  }
  constexpr tmat2 &operator*=(T s) {
    col0 *= s;
    col1 *= s;
    return *this;
  }
  constexpr tmat2 &operator*=(const tmat2 &m) {
    *this = *this * m;
    return *this;
  }

  constexpr tmat2 operator/(T s) const { return *this * ((T)1 / s); }
  constexpr tmat2 &operator/=(T s) {
    *this = *this / s;
    return *this;
  }
};

template <typename T> struct tmat3 {
  union {
    tvec3<T> columns[3];
    struct {
      tvec3<T> col0, col1, col2;
    };
  };

  // ctors
  tmat3() : columns{} {}
  tmat3(const tvec3<T> &v0, const tvec3<T> &v1, const tvec3<T> &v2)
      : col0(v0), col1(v1), col2(v2) {}

  // methods
  // swap row[i] with column[i]
  constexpr tmat3 transpose() const {
    return tmat3{{col0.x, col1.x, col2.x},
                 {col0.y, col1.y, col2.y},
                 {col0.z, col1.z, col2.z}};
  }
  // how much the transformation scales space
  constexpr T determinant() const {
    T a = columns[0][0];
    T b = columns[1][0];
    T c = columns[2][0];
    T d = columns[0][1];
    T e = columns[1][1];
    T f = columns[2][1];
    T g = columns[0][2];
    T h = columns[1][2];
    T i = columns[2][2];
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
  }
  // undoes the current matrix
  constexpr tmat3 inverse() const {
    T inv_det = T(1) / determinant();
    T a = columns[0][0];
    T b = columns[1][0];
    T c = columns[2][0];
    T d = columns[0][1];
    T e = columns[1][1];
    T f = columns[2][1];
    T g = columns[0][2];
    T h = columns[1][2];
    T i = columns[2][2];
    return tmat3{{(e * i - f * h), -(d * i - f * g), (d * h - e * g)},
                 {-(b * i - c * h), (a * i - c * g), -(a * h - b * g)},
                 {(b * f - c * e), -(a * f - c * d), (a * e - b * d)}} *
           inv_det;
  }

  // unary ops
  constexpr const tvec3<T> &operator[](size_t i) const { return columns[i]; }
  constexpr tvec3<T> &operator[](size_t i) { return columns[i]; }
  constexpr tmat3 operator-() const { return tmat3{-col0, -col1, -col2}; }
  constexpr tmat3 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tmat3 &a, const tmat3 &b) {
    return a.col0 == b.col0 && a.col1 == b.col1 && a.col2 == b.col2;
  }

  constexpr tmat3 operator*(T s) const {
    return tmat3{col0 * s, col1 * s, col2 * s};
  }
  constexpr tvec3<T> operator*(const tvec3<T> &v) const {
    // mat * vec: (MV) = (V[i]*M.col[i] + ...)
    return (v[0] * col0) + (v[1] * col1) + (v[2] * col2);
  }
  constexpr tmat3 operator*(const tmat3 &m) const {
    // mat * mat: (AB).col[i] = A * B.col[i]
    return tmat3{*this * m.col0, *this * m.col1, *this * m.col2};
  }
  constexpr tmat3 &operator*=(T s) {
    col0 *= s;
    col1 *= s;
    col2 *= s;
    return *this;
  }
  constexpr tmat3 &operator*=(const tmat3 &m) {
    *this = *this * m;
    return *this;
  }

  constexpr tmat3 operator/(T s) const { return *this * ((T)1 / s); }
  constexpr tmat3 &operator/=(T s) {
    *this = *this / s;
    return *this;
  }
};

template <typename T> struct tmat4 {
  union {
    tvec4<T> columns[4];
    struct {
      tvec4<T> col0, col1, col2, col3;
    };
  };

  // ctors
  tmat4() : columns{} {}
  tmat4(const tvec4<T> &v0, const tvec4<T> &v1, const tvec4<T> &v2,
        const tvec4<T> &v3)
      : col0(v0), col1(v1), col2(v2), col3(v3) {}

  // statics
  // TODO: statics n stuff
  constexpr static tmat4 identity() {
    return tmat4{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
  }
  static tmat4 perspective(f64 fov_rad, f64 aspect, f64 near, f64 far) {
    f64 t = tan(fov_rad / 2);
    f64 fn = -((far + near) / (far - near));
    f64 fn2 = -((2 * far * near) / (far - near));
    return tmat4{{1 / (aspect * t), 0, 0, 0},
                 {0, 1 / t, 0, 0},
                 {0, 0, fn, -1},
                 {0, 0, fn2, 0}};
  }
  // https://www.brainvoyager.com/bv/doc/UsersGuide/CoordsAndTransforms/SpatialTransformationMatrices.html
  constexpr static tmat4 translate(const tvec3<T> &v) {
    return tmat4{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {v.x, v.y, v.z, 1}};
  }
  constexpr static tmat4 scale(const tvec3<T> &v) {
    return tmat4{{v.x, 0, 0, 0}, {0, v.y, 0, 0}, {0, 0, v.z, 0}, {0, 0, 0, 1}};
  }

  // around x
  static tmat4 rotate_x(T rad) {
    const auto c = cos(rad);
    const auto s = sin(rad);
    return tmat4{{1, 0, 0, 0}, {0, c, s, 0}, {0, -s, c, 0}, {0, 0, 0, 1}};
  }
  // around y
  static tmat4 rotate_y(T rad) {
    const auto c = cos(rad);
    const auto s = sin(rad);
    return tmat4{{c, 0, -s, 0}, {0, 1, 0, 0}, {s, 0, c, 0}, {0, 0, 0, 1}};
  }
  // around z
  static tmat4 rotate_z(T rad) {
    const auto c = cos(rad);
    const auto s = sin(rad);
    return tmat4{{c, s, 0, 0}, {-s, c, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
  }
  // TODO: math for deg -> rad and vice verse
  // TODO: quat -> euler, euler -> quat
  // TODO: quat -> mat4, mat4 -> quat

  // methods
  // swap row[i] with column[i]
  constexpr tmat4 transpose() const {
    return tmat4{
        {col0.x, col1.x, col2.x, col3.x},
        {col0.y, col1.y, col2.y, col3.y},
        {col0.z, col1.z, col2.z, col3.z},
        {col0.w, col1.w, col2.w, col3.w},
    };
  }
  // how much the transformation scales space
  constexpr T determinant() const {
    T s0 = columns[0][0] * columns[1][1] - columns[1][0] * columns[0][1];
    T s1 = columns[0][0] * columns[1][2] - columns[1][0] * columns[0][2];
    T s2 = columns[0][0] * columns[1][3] - columns[1][0] * columns[0][3];
    T s3 = columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2];
    T s4 = columns[0][1] * columns[1][3] - columns[1][1] * columns[0][3];
    T s5 = columns[0][2] * columns[1][3] - columns[1][2] * columns[0][3];
    T c0 = columns[2][0] * columns[3][1] - columns[3][0] * columns[2][1];
    T c1 = columns[2][0] * columns[3][2] - columns[3][0] * columns[2][2];
    T c2 = columns[2][0] * columns[3][3] - columns[3][0] * columns[2][3];
    T c3 = columns[2][1] * columns[3][2] - columns[3][1] * columns[2][2];
    T c4 = columns[2][1] * columns[3][3] - columns[3][1] * columns[2][3];
    T c5 = columns[2][2] * columns[3][3] - columns[3][2] * columns[2][3];
    return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
  }
  // undoes the current matrix
  constexpr tmat4 inverse() const {
    T s0 = columns[0][0] * columns[1][1] - columns[1][0] * columns[0][1];
    T s1 = columns[0][0] * columns[1][2] - columns[1][0] * columns[0][2];
    T s2 = columns[0][0] * columns[1][3] - columns[1][0] * columns[0][3];
    T s3 = columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2];
    T s4 = columns[0][1] * columns[1][3] - columns[1][1] * columns[0][3];
    T s5 = columns[0][2] * columns[1][3] - columns[1][2] * columns[0][3];
    T c0 = columns[2][0] * columns[3][1] - columns[3][0] * columns[2][1];
    T c1 = columns[2][0] * columns[3][2] - columns[3][0] * columns[2][2];
    T c2 = columns[2][0] * columns[3][3] - columns[3][0] * columns[2][3];
    T c3 = columns[2][1] * columns[3][2] - columns[3][1] * columns[2][2];
    T c4 = columns[2][1] * columns[3][3] - columns[3][1] * columns[2][3];
    T c5 = columns[2][2] * columns[3][3] - columns[3][2] * columns[2][3];
    T inv_det =
        T(1) / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
    tmat4<T> inv;
    inv[0][0] = columns[1][1] * c5 - columns[1][2] * c4 + columns[1][3] * c3;
    inv[0][1] = -columns[0][1] * c5 + columns[0][2] * c4 - columns[0][3] * c3;
    inv[0][2] = columns[3][1] * s5 - columns[3][2] * s4 + columns[3][3] * s3;
    inv[0][3] = -columns[2][1] * s5 + columns[2][2] * s4 - columns[2][3] * s3;
    inv[1][0] = -columns[1][0] * c5 + columns[1][2] * c2 - columns[1][3] * c1;
    inv[1][1] = columns[0][0] * c5 - columns[0][2] * c2 + columns[0][3] * c1;
    inv[1][2] = -columns[3][0] * s5 + columns[3][2] * s2 - columns[3][3] * s1;
    inv[1][3] = columns[2][0] * s5 - columns[2][2] * s2 + columns[2][3] * s1;
    inv[2][0] = columns[1][0] * c4 - columns[1][1] * c2 + columns[1][3] * c0;
    inv[2][1] = -columns[0][0] * c4 + columns[0][1] * c2 - columns[0][3] * c0;
    inv[2][2] = columns[3][0] * s4 - columns[3][1] * s2 + columns[3][3] * s0;
    inv[2][3] = -columns[2][0] * s4 + columns[2][1] * s2 - columns[2][3] * s0;
    inv[3][0] = -columns[1][0] * c3 + columns[1][1] * c1 - columns[1][2] * c0;
    inv[3][1] = columns[0][0] * c3 - columns[0][1] * c1 + columns[0][2] * c0;
    inv[3][2] = -columns[3][0] * s3 + columns[3][1] * s1 - columns[3][2] * s0;
    inv[3][3] = columns[2][0] * s3 - columns[2][1] * s1 + columns[2][2] * s0;
    return inv * inv_det;
  }

  // unary ops
  constexpr const tvec4<T> &operator[](size_t i) const { return columns[i]; }
  constexpr tvec4<T> &operator[](size_t i) { return columns[i]; }
  constexpr tmat4 operator-() const {
    return tmat4{-col0, -col1, -col2, -col3};
  }
  constexpr tmat4 operator+() const { return *this; }

  // binary ops
  constexpr friend bool operator==(const tmat4 &a, const tmat4 &b) {
    return a.col0 == b.col0 && a.col1 == b.col1 && a.col2 == b.col2 &&
           a.col3 == b.col3;
  }

  constexpr tmat4 operator*(T s) const {
    return tmat4{col0 * s, col1 * s, col2 * s, col3 * s};
  }
  constexpr tvec4<T> operator*(const tvec4<T> &v) const {
    // mat * vec: (MV) = (V[i]*M.col[i] + ...)
    return (v[0] * col0) + (v[1] * col1) + (v[2] * col2) + (v[3] * col3);
  }
  constexpr tmat4 operator*(const tmat4 &m) const {
    // mat * mat: (AB).col[i] = A * B.col[i]
    return tmat4{*this * m.col0, *this * m.col1, *this * m.col2,
                 *this * m.col3};
  }
  constexpr tmat4 &operator*=(T s) {
    col0 *= s;
    col1 *= s;
    col2 *= s;
    col3 *= s;
    return *this;
  }
  constexpr tmat4 &operator*=(const tmat4 &m) {
    *this = *this * m;
    return *this;
  }

  constexpr tmat4 operator/(T s) const { return *this * ((T)1 / s); }
  constexpr tmat4 &operator/=(T s) {
    *this = *this / s;
    return *this;
  }
};

using mat2 = tmat2<f32>;
using mat3 = tmat3<f32>;
using mat4 = tmat4<f32>;

using dmat2 = tmat2<f64>;
using dmat3 = tmat3<f64>;
using dmat4 = tmat4<f64>;
