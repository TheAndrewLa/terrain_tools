#ifndef ENGINE_CORE_MATH_MATRIX_H
#define ENGINE_CORE_MATH_MATRIX_H

#include <math/utils.hpp>
#include <math/vector.hpp>

#include <types.hpp>

namespace ala::math {

// Generic versions of matrix
// NOTE: think about using non-square matrices (mat3x2, mat4x3)
// NOTE: think about boolean matrices

template <typename T>
  requires(types::is_number_v<T>)
struct tmat2x2;

template <typename T>
  requires(types::is_number_v<T>)
struct tmat3x3;

template <typename T>
  requires(types::is_number_v<T>)
struct tmat4x4;

// IDEA: use SIMD intrinsics where possible

template <typename T>
  requires(types::is_number_v<T>)
struct tmat2x2 {
  using index_t = std::pair<usize, usize>;
  using value_t = T;

  // NOTE: using tvec<N> as row_t can increase size of code because of instantiations tvec2<N> functions
  // TODO: replace it with 2D array
  // TODO: decide which format of storing matrix in 2D array should be chosen

  using row_index_t = usize;
  using row_t = tvec2<T>;

  tmat2x2() = delete;

  explicit constexpr tmat2x2(value_t diagonal_value) noexcept
      : rows_({row_t(diagonal_value, zero), row_t(zero, diagonal_value)}) {}

  constexpr tmat2x2(const row_t& diagonal) noexcept
      : rows_({row_t(diagonal[0], zero), row_t(zero, diagonal[1])}) {}

  constexpr tmat2x2(value_t d1, value_t d2) noexcept : rows_({row_t(d1, zero), row_t(zero, d2)}) {}

  constexpr tmat2x2(const row_t& row1, const row_t& row2) noexcept : rows_({row1, row2}) {}

  static tmat2x2 get_identity() noexcept { return tmat2x2(static_cast<value_t>(1)); }

  constexpr tmat2x2& transpose() noexcept {
    rows_[0] = row_t(rows_[0][0], rows_[1][0]);
    rows_[1] = row_t(rows_[0][1], rows_[1][1]);

    return *this;
  }

  constexpr tmat2x2 get_transposed() const noexcept {
    auto row1 = row_t(rows_[0][0], rows_[1][0]);
    auto row2 = row_t(rows_[0][1], rows_[1][1]);

    return tmat2x2(row1, row2);
  }

  constexpr row_t x() const noexcept { return rows_[0]; }

  constexpr row_t y() const noexcept { return rows_[1]; }

  tmat2x2& assign(index_t index, value_t value) {
    if (index.first >= 2 || index.second >= 2) {
      throw std::out_of_range("Each component of tmat2x2's index should be less than 2!");
    }

    rows_[index.first][index.second] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index.first >= 2 || index.second >= 2) {
      throw std::out_of_range("Each component of tmat2x2's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  const value_t& at(index_t index) const {
    if (index.first >= 2 || index.second >= 2) {
      throw std::out_of_range("Each component of tmat2x2's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  constexpr row_t& operator[](row_index_t index) noexcept { return rows_[index]; }

  constexpr const row_t& operator[](row_index_t index) const noexcept { return rows_[index]; }

  constexpr bool operator==(const tmat2x2& other) const noexcept {
    return (other.rows_[0] == rows_[0]) && (other.rows_[1] == rows_[1]);
  }

  constexpr tmat2x2 operator+(const tmat2x2& other) const noexcept {
    return tmat2x2(rows_[0] + other.rows_[0], rows_[1] + other.rows_[1]);
  }

  constexpr tmat2x2 operator-(const tmat2x2& other) const noexcept {
    return tmat2x2(rows_[0] - other.rows_[0], rows_[1] - other.rows_[1]);
  }

  constexpr tmat2x2& operator+=(const tmat2x2& other) noexcept {
    rows_[0] += other.rows_[0];
    rows_[1] += other.rows_[1];

    return *this;
  }

  constexpr tmat2x2& operator-=(const tmat2x2& other) noexcept {
    rows_[0] -= other.rows_[0];
    rows_[1] -= other.rows_[1];

    return *this;
  }

  constexpr tmat2x2 operator*(value_t scale) const noexcept {
    return tmat2x2(rows_[0] * scale, rows_[1] * scale);
  }

  constexpr tmat2x2 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tmat2x2(rows_[0] / scale, rows_[1] / scale);
  }

  constexpr tmat2x2& operator*=(value_t scale) noexcept {
    rows_[0] *= scale;
    rows_[1] *= scale;

    return *this;
  }

  constexpr tmat2x2& operator/=(value_t scale) noexcept
    requires(types::is_float_v<value_t>)
  {
    rows_[0] /= scale;
    rows_[1] /= scale;

    return *this;
  }

  constexpr friend tmat2x2 operator*(value_t scale, const tmat2x2& mat) noexcept {
    return tmat2x2(mat.rows_[0] * scale, mat.rows_[1] * scale);
  }

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  row_t rows_[2];
};

template <typename T>
  requires(types::is_number_v<T>)
struct tmat3x3 {
  using index_t = std::pair<usize, usize>;
  using value_t = T;

  using row_index_t = usize;
  using row_t = tvec3<T>;

  tmat3x3() = delete;

  explicit constexpr tmat3x3(value_t diagonal_value) noexcept
      : rows_({row_t(diagonal_value, zero, zero), row_t(zero, diagonal_value, zero),
               row_t(zero, zero, diagonal_value)}) {}

  constexpr tmat3x3(const row_t& diagonal) noexcept
      : rows_({row_t(diagonal[0], zero, zero), row_t(zero, diagonal[1], zero),
               row_t(zero, zero, diagonal[2])}) {}

  constexpr tmat3x3(value_t d1, value_t d2, value_t d3) noexcept
      : rows_({row_t(d1, zero, zero), row_t(zero, d2, zero), row_t(zero, zero, d3)}) {}

  static tmat3x3 get_identity() noexcept { return tmat3x3(static_cast<value_t>(1)); }

  constexpr tmat3x3& transpose() noexcept {
    rows_[0] = row_t(rows_[0][0], rows_[1][0], rows_[2][0]);
    rows_[1] = row_t(rows_[0][1], rows_[1][1], rows_[2][1]);
    rows_[2] = row_t(rows_[0][2], rows_[1][2], rows_[2][2]);

    return *this;
  }

  constexpr tmat3x3 get_transposed() const noexcept {
    auto row1 = row_t(rows_[0][0], rows_[1][0], rows_[2][0]);
    auto row2 = row_t(rows_[0][1], rows_[1][1], rows_[2][1]);
    auto row3 = row_t(rows_[0][2], rows_[1][2], rows_[2][2]);

    return tmat3x3(row1, row2, row3);
  }

  constexpr row_t x() const noexcept { return rows_[0]; }

  constexpr row_t y() const noexcept { return rows_[1]; }

  constexpr row_t z() const noexcept { return rows_[2]; }

  tmat3x3& assign(index_t index, value_t value) {
    if (index.first >= 3 || index.second >= 3) {
      throw std::out_of_range("Each component of tmat3x3's index should be less than 2!");
    }

    rows_[index.first][index.second] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index.first >= 3 || index.second >= 3) {
      throw std::out_of_range("Each component of tmat3x3's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  const value_t& at(index_t index) const {
    if (index.first >= 3 || index.second >= 3) {
      throw std::out_of_range("Each component of tmat3x3's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  constexpr row_t& operator[](row_index_t index) noexcept { return rows_[index]; }

  constexpr const row_t& operator[](row_index_t index) const noexcept { return rows_[index]; }

  constexpr bool operator==(const tmat3x3& other) const noexcept {
    return (rows_[0] == other.rows_[0]) && (rows_[1] == other.rows_[1]) && (rows_[2] == other.rows_[2]);
  }

  constexpr tmat3x3 operator+(const tmat3x3& other) const noexcept {
    return tmat3x3(rows_[0] + other.rows_[0], rows_[1] + other.rows_[1], rows_[2] + other.rows_[2]);
  }

  constexpr tmat3x3 operator-(const tmat3x3& other) const noexcept {
    return tmat3x3(rows_[0] - other.rows_[0], rows_[1] - other.rows_[1], rows_[2] - other.rows_[2]);
  }

  constexpr tmat3x3& operator+=(const tmat3x3& other) noexcept {
    rows_[0] += other.rows_[0];
    rows_[1] += other.rows_[1];
    rows_[2] += other.rows_[2];

    return *this;
  }

  constexpr tmat3x3& operator-=(const tmat3x3& other) noexcept {
    rows_[0] -= other.rows_[0];
    rows_[1] -= other.rows_[1];
    rows_[2] -= other.rows_[2];

    return *this;
  }

  constexpr tmat3x3 operator*(value_t scale) const noexcept {
    return tmat3x3(rows_[0] * scale, rows_[1] * scale, rows_[2] * scale);
  }

  constexpr tmat3x3 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tmat3x3(rows_[0] / scale, rows_[1] / scale, rows_[2] / scale);
  }

  constexpr tmat3x3& operator*=(value_t scale) noexcept {
    rows_[0] *= scale;
    rows_[1] *= scale;
    rows_[2] *= scale;

    return *this;
  }

  constexpr tmat3x3& operator/=(value_t scale) noexcept
    requires(types::is_float_v<value_t>)
  {
    rows_[0] /= scale;
    rows_[1] /= scale;
    rows_[2] /= scale;

    return *this;
  }

  constexpr friend tmat3x3 operator*(value_t scale, const tmat3x3& mat) noexcept {
    return tmat3x3(mat.rows_[0] * scale, mat.rows_[1] * scale, mat.rows_[2] * scale);
  }

  private:
  static constexpr auto zero = static_cast<T>(0);

  row_t rows_[3];
};

template <typename T>
  requires(types::is_number_v<T>)
struct tmat4x4 {
  using index_t = std::pair<usize, usize>;
  using value_t = T;

  using row_index_t = usize;
  using row_t = tvec4<T>;

  tmat4x4() = delete;

  explicit constexpr tmat4x4(value_t diagonal) noexcept
      : rows_({row_t(diagonal, zero, zero, zero), row_t(zero, diagonal, zero, zero),
               row_t(zero, zero, diagonal, zero), row_t(zero, zero, zero, diagonal)}) {}

  constexpr tmat4x4(const row_t& diagonal) noexcept
      : rows_({row_t(diagonal[0], zero, zero, zero), row_t(zero, diagonal[1], zero, zero),
               row_t(zero, zero, diagonal[2], zero), row_t(zero, zero, zero, diagonal[3])}) {}

  constexpr tmat4x4(value_t d1, value_t d2, value_t d3, value_t d4) noexcept
      : rows_({row_t(d1, zero, zero, zero), row_t(zero, d2, zero, zero), row_t(zero, zero, d3, zero),
               row_t(zero, zero, zero, d4)}) {}

  static tmat4x4 get_identity() noexcept { return tmat4x4(static_cast<value_t>(1)); }

  constexpr tmat4x4& transpose() noexcept {
    rows_[0] = row_t(rows_[0][0], rows_[1][0], rows_[2][0], rows_[3][0]);
    rows_[1] = row_t(rows_[0][1], rows_[1][1], rows_[2][1], rows_[3][1]);
    rows_[2] = row_t(rows_[0][2], rows_[1][2], rows_[2][2], rows_[3][2]);
    rows_[3] = row_t(rows_[0][3], rows_[1][3], rows_[2][3], rows_[3][3]);

    return *this;
  }

  constexpr tmat4x4 get_transposed() const noexcept {
    auto row1 = row_t(rows_[0][0], rows_[1][0], rows_[2][0], rows_[3][0]);
    auto row2 = row_t(rows_[0][1], rows_[1][1], rows_[2][1], rows_[3][1]);
    auto row3 = row_t(rows_[0][2], rows_[1][2], rows_[2][2], rows_[3][2]);
    auto row4 = row_t(rows_[0][3], rows_[1][3], rows_[2][3], rows_[3][3]);

    return tmat4x4(row1, row2, row3, row4);
  }

  constexpr row_t x() const noexcept { return rows_[0]; }

  constexpr row_t y() const noexcept { return rows_[1]; }

  constexpr row_t z() const noexcept { return rows_[2]; }

  constexpr row_t w() const noexcept { return rows_[3]; }

  tmat4x4& assign(index_t index, value_t value) {
    if (index.first >= 4 || index.second >= 4) {
      throw std::out_of_range("Each component of tmat4x4's index should be less than 2!");
    }

    rows_[index.first][index.second] = value;

    return *this;
  }

  value_t& at(index_t index) {
    if (index.first >= 4 || index.second >= 4) {
      throw std::out_of_range("Each component of tmat4x4's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  const value_t& at(index_t index) const {
    if (index.first >= 4 || index.second >= 4) {
      throw std::out_of_range("Each component of tmat4x4's index should be less than 2!");
    }

    return rows_[index.first][index.second];
  }

  constexpr row_t& operator[](row_index_t index) noexcept { return rows_[index]; }

  constexpr const row_t& operator[](row_index_t index) const noexcept { return rows_[index]; }

  constexpr bool operator==(const tmat4x4& other) const noexcept {
    return (rows_[0] == other.rows_[0]) && (rows_[1] == other.rows_[1]) && (rows_[2] == other.rows_[2]) &&
           (rows_[3] == other.rows_[3]);
  }

  constexpr tmat4x4 operator+(const tmat4x4& other) const noexcept {
    return tmat4x4(rows_[0] + other.rows_[0], rows_[1] + other.rows_[1], rows_[2] + other.rows_[2],
                   rows_[3] + other.rows_[3]);
  }

  constexpr tmat4x4 operator-(const tmat4x4& other) const noexcept {
    return tmat4x4(rows_[0] - other.rows_[0], rows_[1] - other.rows_[1], rows_[2] - other.rows_[2],
                   rows_[3] - other.rows_[3]);
  }

  constexpr tmat4x4& operator+=(const tmat4x4& other) noexcept {
    rows_[0] += other.rows_[0];
    rows_[1] += other.rows_[1];
    rows_[2] += other.rows_[2];
    rows_[3] += other.rows_[3];

    return *this;
  }

  constexpr tmat4x4& operator-=(const tmat4x4& other) noexcept {
    rows_[0] -= other.rows_[0];
    rows_[1] -= other.rows_[1];
    rows_[2] -= other.rows_[2];
    rows_[3] -= other.rows_[3];

    return *this;
  }

  constexpr tmat4x4 operator*(value_t scale) const noexcept {
    return tmat4x4(rows_[0] * scale, rows_[1] * scale, rows_[2] * scale, rows_[3] * scale);
  }

  constexpr tmat4x4 operator/(value_t scale) const noexcept
    requires(types::is_float_v<value_t>)
  {
    return tmat4x4(rows_[0] / scale, rows_[1] / scale, rows_[2] / scale, rows_[3] / scale);
  }

  constexpr tmat4x4& operator*=(value_t scale) noexcept {
    rows_[0] *= scale;
    rows_[1] *= scale;
    rows_[2] *= scale;
    rows_[3] *= scale;

    return *this;
  }

  constexpr tmat4x4& operator/=(value_t scale) noexcept
    requires(types::is_float_v<value_t>)
  {
    rows_[0] /= scale;
    rows_[1] /= scale;
    rows_[2] /= scale;
    rows_[3] /= scale;

    return *this;
  }

  constexpr friend tmat4x4 operator*(value_t scale, const tmat4x4& mat) noexcept {
    return tmat4x4(mat.rows_[0] * scale, mat.rows_[1] * scale, mat.rows_[2] * scale, mat.rows_[3] * scale);
  }

  private:
  static constexpr auto zero = static_cast<value_t>(0);

  row_t rows_[4];
};

}  // namespace ala::math

namespace ala::types {

// Type constraits for matrix
// TODO: write **actual** constraints with requires statement

template <typename T>
concept matrix_type = std::is_same_v<T, math::tmat2x2<typename T::value_t>> ||
                      std::is_same_v<T, math::tmat3x3<typename T::value_t>> ||
                      std::is_same_v<T, math::tmat4x4<typename T::value_t>>;

// Useful type aliases for matrices

using mat2x2 = math::tmat2x2<float32>;
using mat3x3 = math::tmat3x3<float32>;
using mat4x4 = math::tmat4x4<float32>;

using imat2x2 = math::tmat2x2<int32>;
using imat3x3 = math::tmat3x3<int32>;
using imat4x4 = math::tmat4x4<int32>;

using umat2x2 = math::tmat2x2<uint32>;
using umat3x3 = math::tmat3x3<uint32>;
using umat4x4 = math::tmat4x4<uint32>;

};  // namespace ala::types

#endif
