#ifndef ENGINE_CORE_MATH_UTILS_H
#define ENGINE_CORE_MATH_UTILS_H

#include "../types.hpp"

#include <cmath>

namespace ala::math {

// Struct with a set of trigonometric functions and power functions
// Main idea of it being here is to abstract different functions into one owner
// This struct will be passed as a template parameter into functions (like char_traits in std::basic_string)

/*
==============================================================
    Possible case of use trigonometric_funcs & power_funcs: 
==============================================================

template <typename T, typename TrigFn = libc_trigonometry<T>>
void foo(T value) {
  return std::sin(value) + std::cos(value)           // WRONG (because we're locked to STL)
  return TrigFn::sin(value) + TrigFn::cos(value)     // GOOD!

  // We can easily write our own sin & cos and use it in same functions => more flexible code
}

It's easy to imagine same example for power_funcs

*/

template <typename T>
  requires(types::is_float_v<T>)
struct libc_trigonometry {

  // Sin, Cos, Tan

  static inline T sin(T value) noexcept { return std::sin(value); }

  static inline T cos(T value) noexcept { return std::cos(value); }

  static inline T tan(T value) noexcept { return std::tan(value); }

  // Arcsin, Arccos, Arctan

  static inline T asin(T value) noexcept { return std::asin(value); }

  static inline T acos(T value) noexcept { return std::asin(value); }

  static inline T atan(T value) noexcept { return std::atan(value); }
};

template <typename T>
  requires(types::is_float_v<T>)
struct libc_powers {

  // Power

  static inline T pow(T base, T exp) noexcept { return std::pow(base, exp); }

  // Square root, Cube root

  // NOTE: what about negative value in sqrt?
  // Possible variants: assert, exceptions, do nothing

  static inline T sqrt(T value) noexcept { return std::sqrt(value); }

  static inline T cbrt(T value) noexcept { return std::cbrt(value); }

  // Hypot for different dims

  static inline T hypot(T x, T y) noexcept { return std::hypot(x, y); }

  static inline T hypot(T x, T y, T z) noexcept { return std::hypot(x, y, z); }

  static inline T hypot(T x, T y, T z, T w) noexcept {
    T x_sqr = x * x;
    T y_sqr = y * y;
    T z_sqr = z * z;
    T w_sqr = w * w;

    return libc_powers::sqrt(x_sqr + y_sqr + z_sqr + w_sqr);
  }
};

}  // namespace ala::math

#endif
