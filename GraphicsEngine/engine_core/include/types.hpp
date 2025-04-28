#ifndef ENGINE_CORE_TYPES_H
#define ENGINE_CORE_TYPES_H

#include <cstddef>
#include <cstdint>

namespace ala::types {

using uint8 = std::uint8_t;
using int8 = std::int8_t;

using uint16 = std::uint16_t;
using int16 = std::int16_t;

using uint32 = std::uint32_t;
using int32 = std::int32_t;

using uint64 = std::uint64_t;
using int64 = std::int64_t;

using usize = std::size_t;
using isize = std::ptrdiff_t;

using uintptr = std::uintptr_t;
using intptr = std::intptr_t;

using wchar = wchar_t;

using char8 = char8_t;
using char16 = char16_t;
using char32 = char32_t;

using float32 = float;
using float64 = double;

// Type constraints

template <typename TypeA, typename TypeB>
constexpr bool is_same_v = std::is_same_v<TypeA, TypeB>;

template <typename T>
constexpr bool is_float_v = std::is_floating_point_v<T>;

template <typename T>
constexpr bool is_number_v = (std::is_arithmetic_v<T>) && (!std::is_same_v<T, bool>);

}  // namespace ala::types

#endif
