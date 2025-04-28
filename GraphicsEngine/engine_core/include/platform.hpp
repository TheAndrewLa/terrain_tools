#ifndef ENGINE_CORE_PLATFORM_H
#define ENGINE_CORE_PLATFORM_H

#include <boost/predef.h>

namespace ala::platform {

#if defined(BOOST_ARCH_X86)

constexpr bool x86 = true;
constexpr bool x86_64 = false;
constexpr bool arm64 = false;

#elif defined(BOOST_ARCH_X86_64)

constexpr bool x86 = false;
constexpr bool x86_64 = true;
constexpr bool arm64 = false;

#elif defined(BOOST_ARCH_ARM_64)

constexpr bool x86 = false;
constexpr bool x86_64 = false;
constexpr bool arm64 = true;

#else
#error "Unknown arch!"
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE_VERSION)))
constexpr bool sse = true;  // Assume that every "modern" CPU has SSE
#else
constexpr bool sse = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE2_VERSION)))
constexpr bool sse2 = true;
#else
constexpr bool sse2 = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE3_VERSION)
constexpr bool sse3 = true;
#else
constexpr bool sse3 = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_1_VERSION)
constexpr bool sse4_1 = true;
#else
constexpr bool sse4_1 = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_2_VERSION)
constexpr bool sse4_2 = true;
#else
constexpr bool sse4_2 = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX_VERSION)
constexpr bool avx = true;
#else
constexpr bool avx = false;
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX2_VERSION)
constexpr bool avx2 = true;
#else
constexpr bool avx2 = false;
#endif

}  // namespace ala::platform

#endif
