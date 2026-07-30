#pragma once

#include "types.hpp"
#include <math.h>

constexpr f64 pi = M_PI;
constexpr f64 rad(f64 deg) { return deg * (pi / 180); }
constexpr f64 deg(f64 rad) { return rad * (180 / pi); }
template <typename T> constexpr auto square(T x) { return x * x; }
f64 sqrt(f64 x);
f64 cos(f64 rad);
f64 sin(f64 rad);
f64 tan(f64 rad);
