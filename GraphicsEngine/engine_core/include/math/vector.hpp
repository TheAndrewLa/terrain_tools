#ifndef ENGINE_CORE_MATH_VECTOR_H
#define ENGINE_CORE_MATH_VECTOR_H

#include <math/utils.hpp>

#include <types.hpp>

#include <cassert>
#include <cmath>

#include <stdexcept>

/*
=====================
    MATH LIBRARY
=====================

TODO: Write tests for all math/geometry library
TODO: Check compiler's output (to see optimizations)
*/

namespace ala::math {

using usize = types::usize;
using isize = types::isize;

// Generic versions of vector
// NOTE: think about boolean vectors

template <typename T>
  requires(types::is_number_v<T>)
struct tvec2;

template <typename T>
  requires(types::is_number_v<T>)
struct tvec3;

template <typename T>
  requires(types::is_number_v<T>)
struct tvec4;

// TODO: Implement increment and decrement (pre/post) for vectors

template <typename T>
  requires(types::is_number_v<T>)
struct tvec2 {
  using index_t = usize;
  using value_t = T;

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  public:
  constexpr tvec2(value_t x, value_t y) noexcept : vals_({x, y}) {}

  explicit constexpr tvec2(value_t value) noexcept : tvec2(value, value) {}

  constexpr tvec2() noexcept : tvec2(zero) {}

  template <typename PowerFn = libc_powers<value_t>>
  constexpr value_t length() const noexcept
    requires(types::is_float_v<value_t>)
  {
    return PowerFn::hypot(vals_[0], vals_[1]);
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec2& normalize() noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();

    vals_[0] /= len;
    vals_[1] /= len;

    return *this;
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec2 get_normalized() const noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();
    return tvec2(vals_[0] / len, vals_[1] / len);
  }

  constexpr value_t x() const noexcept { return vals_[0]; }

  constexpr value_t y() const noexcept { return vals_[1]; }

  tvec2& assign(index_t index, value_t value) {
    if (index >= 2) {
      throw std::out_of_range("Index of tvec2's component should be less that 2!");
    }

    vals_[index] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index >= 2) {
      throw std::out_of_range("Index of tvec2's component should be less that 2!");
    }

    return vals_[index];
  }

  const value_t& at(index_t index) const {
    if (index >= 2) {
      throw std::out_of_range("Index of tvec2's component should be less that 2!");
    }

    return vals_[index];
  }

  constexpr value_t& operator[](index_t index) noexcept { return vals_[index]; }

  constexpr bool operator==(const tvec2& other) const noexcept {
    return (vals_[0] == other.vals_[0]) && (vals_[1] == other.vals_[1]);
  }

  constexpr tvec2 operator+(const tvec2& other) const noexcept {
    return tvec2(vals_[0] + other.vals_[0], vals_[1] + other.vals_[1]);
  }

  constexpr tvec2 operator-(const tvec2& other) const noexcept {
    return tvec2(vals_[0] - other.vals_[0], vals_[1] - other.vals_[1]);
  }

  constexpr tvec2& operator+=(const tvec2& other) noexcept {
    vals_[0] += other.vals_[0];
    vals_[1] += other.vals_[1];

    return *this;
  }

  constexpr tvec2& operator-=(const tvec2& other) noexcept {
    vals_[0] -= other.vals_[0];
    vals_[1] -= other.vals_[1];

    return *this;
  }

  constexpr tvec2 operator*(value_t scale) const noexcept {
    return tvec2(vals_[0] * scale, vals_[1] * scale);
  }

  constexpr tvec2 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tvec2(vals_[0] / scale, vals_[1] / scale);
  }

  constexpr tvec2& operator*=(value_t scale) noexcept {
    vals_[0] *= scale;
    vals_[1] *= scale;

    return *this;
  }

  constexpr tvec2& operator/=(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    vals_[0] /= scale;
    vals_[1] /= scale;

    return *this;
  }

  constexpr friend tvec2 operator*(value_t scale, const tvec2& vec) noexcept {
    return tvec2(vec.vals_[0] * scale, vec.vals_[1] * scale);
  }

  private:
  value_t vals_[2];
};

template <typename T>
  requires(types::is_number_v<T>)
struct tvec3 {
  using index_t = usize;
  using value_t = T;

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  public:
  constexpr tvec3(value_t x, value_t y, value_t z) noexcept : vals_({x, y, z}) {}

  constexpr tvec3(value_t x, value_t y) noexcept : vals_({x, y, zero}) {}

  constexpr tvec3(const tvec2<value_t>& vec, value_t value) noexcept : tvec3(vec.x(), vec.y(), value) {}

  explicit constexpr tvec3(const tvec2<value_t>& vec) noexcept : tvec3(vec, zero) {}

  explicit constexpr tvec3(value_t value) noexcept : tvec3(value, value, value) {}

  constexpr tvec3() noexcept : tvec3(zero) {}

  template <typename PowerFn = libc_powers<value_t>>
  constexpr value_t length() const noexcept
    requires(types::is_float_v<value_t>)
  {
    return PowerFn::hypot(vals_[0], vals_[1], vals_[2]);
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec3& normalize() noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();

    vals_[0] /= len;
    vals_[1] /= len;
    vals_[2] /= len;

    return *this;
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec3 get_normalized() const noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();
    return tvec3(vals_[0] / len, vals_[1] / len, vals_[2] / len);
  }

  constexpr value_t x() const noexcept { return vals_[0]; }

  constexpr value_t y() const noexcept { return vals_[1]; }

  constexpr value_t z() const noexcept { return vals_[2]; }

  tvec3& assign(index_t index, value_t value) {
    if (index >= 3) {
      throw std::out_of_range("Index of tvec3's component should be less that 3!");
    }

    vals_[index] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index >= 3) {
      throw std::out_of_range("Index of tvec3's component should be less that 3!");
    }

    return vals_[index];
  }

  const value_t& at(index_t index) const {
    if (index >= 3) {
      throw std::out_of_range("Index of tvec3's component should be less that 3!");
    }

    return vals_[index];
  }

  constexpr value_t& operator[](index_t index) noexcept { return vals_[index]; }

  constexpr const value_t& operator[](index_t index) const noexcept { return vals_[index]; }

  constexpr bool operator==(const tvec3& other) const noexcept {
    return (vals_[0] == other[0]) && (vals_[1] == other[1]) && (vals_[2] == other[2]);
  }

  constexpr tvec3 operator+(const tvec3& other) const noexcept {
    return tvec3(vals_[0] + other.vals_[0], vals_[1] + other.vals_[1], vals_[2] + other.vals_[2]);
  }

  constexpr tvec3 operator-(const tvec3& other) const noexcept {
    return tvec3(vals_[0] - other.vals_[0], vals_[1] - other.vals_[1], vals_[2] - other.vals_[2]);
  }

  constexpr tvec3& operator+=(const tvec3& other) noexcept {
    vals_[0] += other.vals_[0];
    vals_[1] += other.vals_[1];
    vals_[2] += other.vals_[2];

    return *this;
  }

  constexpr tvec3& operator-=(const tvec3& other) noexcept {
    vals_[0] -= other.vals_[0];
    vals_[1] -= other.vals_[1];
    vals_[2] -= other.vals_[2];

    return *this;
  }

  constexpr tvec3 operator*(value_t scale) const noexcept {
    return tvec3(vals_[0] * scale, vals_[1] * scale, vals_[2] * scale);
  }

  constexpr tvec3 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tvec3(vals_[0] / scale, vals_[1] / scale, vals_[2] / scale);
  }

  constexpr tvec3& operator*=(value_t scale) noexcept {
    vals_[0] *= scale;
    vals_[1] *= scale;
    vals_[2] *= scale;

    return *this;
  }

  constexpr tvec3& operator/=(value_t scale) noexcept
    requires(types::is_float_v<value_t>)
  {
    vals_[0] /= scale;
    vals_[1] /= scale;
    vals_[2] /= scale;

    return *this;
  }

  constexpr friend tvec3 operator*(value_t scale, const tvec3& vec) noexcept {
    return tvec3(vec.vals_[0] * scale, vec.vals_[1] * scale, vec.vals_[2] * scale);
  }

  private:
  value_t vals_[3];
};

template <typename T>
  requires(types::is_number_v<T>)
struct tvec4 {
  using index_t = usize;
  using value_t = T;

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  public:
  constexpr tvec4(value_t x, value_t y, value_t z, value_t w) noexcept : vals_({x, y, z, w}) {}

  constexpr tvec4(value_t x, value_t y, value_t z) noexcept : tvec4(x, y, z, zero) {}

  constexpr tvec4(value_t x, value_t y) noexcept : tvec4(x, y, zero, zero) {}

  constexpr tvec4(const tvec3<value_t>& vec, value_t value) noexcept
      : tvec4(vec.x(), vec.y(), vec.z(), value) {}

  explicit tvec4(const tvec3<value_t>& vec) noexcept : tvec4(vec, zero) {}

  explicit constexpr tvec4(value_t scalar) noexcept : tvec4(scalar, scalar, scalar, scalar) {}

  constexpr tvec4() noexcept : tvec4(zero) {}

  template <typename PowerFn = libc_powers<value_t>>
  constexpr value_t length() const noexcept
    requires(types::is_float_v<value_t>)
  {
    return PowerFn::hypot(vals_[0], vals_[1], vals_[2], vals_[3]);
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec4& normalize() noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();

    vals_[0] /= len;
    vals_[1] /= len;
    vals_[2] /= len;
    vals_[3] /= len;

    return *this;
  }

  template <typename PowerFn = libc_powers<value_t>>
  constexpr tvec4 get_normalized() const noexcept
    requires(types::is_float_v<value_t>)
  {
    value_t len = length<PowerFn>();
    return tvec4(vals_[0] / len, vals_[1] / len, vals_[2] / len, vals_[3] / len);
  }

  constexpr value_t x() const noexcept { return vals_[0]; }

  constexpr value_t y() const noexcept { return vals_[1]; }

  constexpr value_t z() const noexcept { return vals_[2]; }

  constexpr value_t w() const noexcept { return vals_[3]; }

  constexpr tvec4& assign(index_t index, value_t value) {
    if (index >= 4) {
      throw std::out_of_range("Index of tvec4's component should be less that 4!");
    }

    vals_[index] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index >= 4) {
      throw std::out_of_range("Index of tvec4's component should be less that 4!");
    }

    return vals_[index];
  }

  const value_t& at(index_t index) const {
    if (index >= 4) {
      throw std::out_of_range("Index of tvec4's component should be less that 4!");
    }

    return vals_[index];
  }

  constexpr value_t& operator[](index_t index) noexcept { return vals_[index]; }

  constexpr const value_t& operator[](index_t index) const noexcept { return vals_[index]; }

  constexpr bool operator==(const tvec4& other) const noexcept {
    return (vals_[0] == other.vals_[0]) && (vals_[1] == other.vals_[1]) && (vals_[2] == other.vals_[2]) &&
           (vals_[3] == other.vals_[3]);
  }

  constexpr tvec4 operator+(const tvec4& other) const noexcept {
    return tvec4(vals_[0] + other.vals_[0], vals_[1] + other.vals_[1], vals_[2] + other.vals_[2],
                 vals_[3] + other.vals_[3]);
  }

  constexpr tvec4 operator-(const tvec4& other) const noexcept {
    return tvec4(vals_[0] - other.vals_[0], vals_[1] - other.vals_[1], vals_[2] - other.vals_[2],
                 vals_[3] - other.vals_[3]);
  }

  constexpr tvec4& operator+=(const tvec4& other) noexcept {
    vals_[0] += other.vals_[0];
    vals_[1] += other.vals_[1];
    vals_[2] += other.vals_[2];
    vals_[3] += other.vals_[3];

    return *this;
  }

  constexpr tvec4& operator-=(const tvec4& other) noexcept {
    vals_[0] -= other.vals_[0];
    vals_[1] -= other.vals_[1];
    vals_[2] -= other.vals_[2];
    vals_[3] -= other.vals_[3];

    return *this;
  }

  constexpr tvec4 operator*(value_t scale) const noexcept {
    return tvec4(vals_[0] * scale, vals_[1] * scale, vals_[2] * scale, vals_[3] * scale);
  }

  constexpr tvec4 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tvec4(vals_[0] / scale, vals_[1] / scale, vals_[2] / scale, vals_[3] / scale);
  }

  constexpr tvec4& operator*=(value_t scale) noexcept {
    vals_[0] *= scale;
    vals_[1] *= scale;
    vals_[2] *= scale;
    vals_[3] *= scale;

    return *this;
  }

  constexpr tvec4& operator/(value_t scale) noexcept
    requires(types::is_float_v<value_t>)
  {
    vals_[0] /= scale;
    vals_[1] /= scale;
    vals_[2] /= scale;
    vals_[3] /= scale;

    return *this;
  }

  constexpr friend tvec4 operator*(value_t scale, const tvec4& vec) noexcept {
    return tvec4(vec.vals_[0] * scale, vec.vals_[1] * scale, vec.vals_[2] * scale, vec.vals_[3] * scale);
  }

  private:
  value_t vals_[4];
};

// Functions associated with vectors of different sizes
// Dot product

template <typename T>
  requires(types::is_float_v<T>)
T dot(const tvec2<T>& vec1, const tvec2<T>& vec2) {
  auto x_prod = vec1.x() * vec2.x();
  auto y_prod = vec1.y() * vec2.y();

  return x_prod + y_prod;
}

template <typename T>
  requires(types::is_float_v<T>)
T dot(const tvec3<T>& vec1, const tvec3<T>& vec2) {
  auto x_prod = vec1.x() * vec2.x();
  auto y_prod = vec1.y() * vec2.y();
  auto z_prod = vec1.z() * vec2.z();

  return x_prod + y_prod + z_prod;
}

template <typename T>
  requires(types::is_float_v<T>)
T dot(const tvec4<T>& vec1, const tvec4<T>& vec2) {
  auto x_prod = vec1.x() * vec2.x();
  auto y_prod = vec1.y() * vec2.y();
  auto z_prod = vec1.z() * vec2.z();
  auto w_prod = vec1.w() * vec2.w();

  return x_prod + y_prod + z_prod + w_prod;
}

// Cross product
// NOTE: think about adding platform-dependent code with accelerated with SSE/AVX/AVX2 cross product

template <typename T>
  requires(types::is_float_v<T>)
tvec3<T> cross(const tvec3<T>& vec1, const tvec3<T>& vec2) {
  auto x = (vec1.y() * vec2.z()) - (vec1.z() * vec2.y());
  auto y = (vec1.x() * vec2.z()) - (vec1.z() * vec2.x());
  auto z = (vec1.x() * vec2.y()) - (vec1.y() * vec2.x());

  return tvec3<T>(x, y, z);
}

}  // namespace ala::math

namespace ala::types {

// Type constraits for vector
// TODO: write **actual** constraints with requires statement

template <typename T>
concept vector_type = std::is_same_v<T, ::ala::math::tvec2<typename T::value_t>> ||
                      std::is_same_v<T, ::ala::math::tvec3<typename T::value_t>> ||
                      std::is_same_v<T, ::ala::math::tvec4<typename T::value_t>>;

// Useful type aliases of vectors

using vec2 = math::tvec2<float32>;
using vec3 = math::tvec3<float32>;
using vec4 = math::tvec4<float32>;

using ivec2 = math::tvec2<int32>;
using ivec3 = math::tvec3<int32>;
using ivec4 = math::tvec4<int32>;

using uvec2 = math::tvec2<uint32>;
using uvec3 = math::tvec3<uint32>;
using uvec4 = math::tvec4<uint32>;

}  // namespace ala::types

#endif
