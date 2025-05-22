#ifndef ENGINE_CORE_PLATFORM_H
#define ENGINE_CORE_PLATFORM_H

#include <boost/predef.h>

#if BOOST_ENDIAN_BIG_BYTE
#error "Ala engine can not work for architecture with big endian"
#endif

// Headers for SIMD
// Only x86-64 SIMD extensions are supported for now

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE_VERSION)))
#include <xmmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE2_VERSION)))
#include <emmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE3_VERSION)))
#include <pmmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_1_VERSION)))
#include <smmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_2_VERSION)))
#include <nmmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX_VERSION)
#include <xmmintrin.h>
#endif

#if BOOST_ENDIAN_LITTLE_BYTE && (BOOST_ARCH_X86 || BOOST_ARCH_X86_64) && \
    (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX2_VERSION)
#include <xmmintrin.h>
#endif

namespace ala::platform {

// Variables for architecture

#if BOOST_ARCH_X86 && !BOOST_ARCH_X86_64

static_assert(sizeof(void*) == 0x4);

constexpr bool x86 = true;
constexpr bool x86_64 = false;
constexpr bool arm64 = false;

#elif BOOST_ARCH_X86_64

static_assert(sizeof(void*) == 0x8);

constexpr bool x86 = false;
constexpr bool x86_64 = true;
constexpr bool arm64 = false;

#elif BOOST_ARCH_ARM_64

static_assert(sizeof(void*) == 0x8);

constexpr bool x86 = false;
constexpr bool x86_64 = false;
constexpr bool arm64 = true;

#else
#error "Unknown arch!"
#endif

// Variables for SIMD extensions (only x86)
// There's no AVX512 and FMA extensions

#if BOOST_ENDIAN_LITTLE_BYTE && \
    (BOOST_ARCH_X86_64 || (BOOST_ARCH_X86 && (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE_VERSION)))
constexpr bool sse = true;
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

// Variables for OS
// Only Windows, MacOS & Linux are supported

#if BOOST_OS_WINDOWS

constexpr bool windows = true;
constexpr bool linux = false;
constexpr bool macos = false;

#elif BOOST_OS_LINUX

constexpr bool windows = false;
constexpr bool linux = true;
constexpr bool macos = false;

#elif BOOST_MAC_OS

constexpr bool windows = false;
constexpr bool linux = false;
constexpr bool macos = true;

#endif

}  // namespace ala::platform

#endif
