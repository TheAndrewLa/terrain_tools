#ifndef ENGINE_CORE_MATH_GEOMETRY_H
#define ENGINE_CORE_MATH_GEOMETRY_H

#include <math/matrix.hpp>
#include <math/utils.hpp>
#include <math/vector.hpp>

#include <types.hpp>

#include <stdexcept>

/*
========================
    GEOMETRY LIBRARY
========================
*/

namespace ala::geometry {

template <typename T, typename PowerFn, typename TrigFn>
  requires(std::is_floating_point_v<T>)
struct cartesian_coords_2D;

template <typename T, typename PowerFn, typename TrigFn>
  requires(std::is_floating_point_v<T>)
struct polar_coords;

template <typename T, typename PowerFn, typename TrigFn>
  requires(std::is_floating_point_v<T>)
struct cartesian_coords_3D;

template <typename T, typename PowerFn = math::libc_powers<T>, typename TrigFn = math::libc_trigonometry<T>>
  requires(std::is_floating_point_v<T>)
struct cartesian_coords_2D {
  using value_t = T;

  using vec_t = math::tvec2<T>;
  using mat_t = math::tmat2x2<T>;

  using vec_ext_t = math::tvec3<T>;
  using mat_ext_t = math::tmat3x3<T>;

  struct point {
    value_t x;
    value_t y;
  };

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  vec_t global_offset_;
};

template <typename T, typename PowerFn = math::libc_powers<T>, typename TrigFn = math::libc_trigonometry<T>>
  requires(std::is_floating_point_v<T>)
struct polar_coords {
  using value_t = T;

  using vec_t = math::tvec2<T>;
  using mat_t = math::tmat2x2<T>;

  using vec_ext_t = math::tvec3<T>;
  using mat_ext_t = math::tmat3x3<T>;

  struct point {
    value_t radius;
    value_t angle;
  };

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  vec_t global_offset_;
};

// 3-dimensional coordinate systems
// TODO: spherical & cylindrical coordinates

template <typename T, typename PowerFn = math::libc_powers<T>, typename TrigFn = math::libc_trigonometry<T>>
  requires(std::is_floating_point_v<T>)
struct cartesian_coords_3D {
  using value_t = T;

  using vec_t = math::tvec3<T>;
  using mat_t = math::tmat3x3<T>;

  using vec_ext_t = math::tvec4<T>;
  using mat_ext_t = math::tmat4x4<T>;

  struct point {
    value_t x;
    value_t y;
    value_t z;
  };

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  vec_t global_offset_;
};

};  // namespace ala::geometry

#endif
