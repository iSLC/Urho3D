//
// Copyright (c) 2008-2020 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

/*
 * Macro concatenation helpers.
*/

#define U3D_CONCAT_(a, b) a##b
#define U3D_CONCAT(a, b) U3D_CONCAT_(a, b)

/*
 * Macro string(ification) helpers.
*/

#define U3D_TRUESTRINGIZE(x) #x
#define U3D_STRINGIZEWRAP(x) U3D_TRUESTRINGIZE(x)

/*
 * Compiler identification.
*/

// Clang pretending to be MSC
#if defined(__clang__) && defined(_MSC_VER)
    // Assume compatibility for both
    #define U3D_CLANG __clang__
    #define U3D_MSC _MSC_VER
    // Clang version macros
    #define U3D_CLANG_MAJOR __clang_major__
    #define U3D_CLANG_MINOR __clang_minor__
    #define U3D_CLANG_PATCH __clang_patchlevel__
    // MSC version macros
    #if defined(_MSC_FULL_VER)
        #define U3D_MSC_MAJOR (_MSC_FULL_VER / 1000000)
        #define U3D_MSC_MINOR ((_MSC_FULL_VER % 1000000) / 10000)
        #define U3D_MSC_PATCH (_MSC_FULL_VER % 10000)
    #else
        #define U3D_MSC_MAJOR (_MSC_VER / 100)
        #define U3D_MSC_MINOR (_MSC_VER % 100)
        #define U3D_MSC_PATCH (0)
    #endif
    // Host compiler version macros
    #define U3D_COMPILER_MAJOR __clang_major__
    #define U3D_COMPILER_MINOR __clang_minor__
    #define U3D_COMPILER_PATCH __clang_patchlevel__
    // Clang offers MSC compatibility
    #define U3D_MSC_COMPAT _MSC_VER
    // Compiler matching selection macros
    #define U3D_GNUC_ONLY(VALUE)
    #define U3D_MINGW_ONLY(VALUE)
    #define U3D_CLANG_ONLY(VALUE) VALUE
    #define U3D_MSC_ONLY(VALUE) VALUE
    // Compiler non-matching selection macros
    #define U3D_EXCEPT_GNUC(VALUE) VALUE
    #define U3D_EXCEPT_MINGW(VALUE) VALUE
    #define U3D_EXCEPT_CLANG(VALUE)
    #define U3D_EXCEPT_MSC(VALUE)
    // GCC compatibility matching and non-matching selection macros
    #define U3D_GNUC_COMPAT_ONLY(VALUE)
    #define U3D_EXCEPT_GNUC_COMPAT(VALUE) VALUE
    // MSC compatibility matching and non-matching selection macros
    #define U3D_MSC_COMPAT_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_MSC_COMPAT(VALUE)
// Clang
#elif defined(__clang__)
    #define U3D_CLANG __clang__
    // Clang version macros
    #define U3D_CLANG_MAJOR __clang_major__
    #define U3D_CLANG_MINOR __clang_minor__
    #define U3D_CLANG_PATCH __clang_patchlevel__
    // Host compiler version macros
    #define U3D_COMPILER_MAJOR __clang_major__
    #define U3D_COMPILER_MINOR __clang_minor__
    #define U3D_COMPILER_PATCH __clang_patchlevel__
    // Clang offers GCC compatibility
    #ifdef __GNUC__
        #define U3D_GNUC_COMPAT __GNUC__
    #endif
    // Compiler matching selection macros
    #define U3D_GNUC_ONLY(VALUE)
    #define U3D_MINGW_ONLY(VALUE)
    #define U3D_CLANG_ONLY(VALUE) VALUE
    #define U3D_MSVC_ONLY(VALUE)
    // Compiler non-matching selection macros
    #define U3D_EXCEPT_GNUC(VALUE) VALUE
    #define U3D_EXCEPT_MINGW(VALUE) VALUE
    #define U3D_EXCEPT_CLANG(VALUE)
    #define U3D_EXCEPT_MSVC(VALUE) VALUE
    // GCC compatibility matching and non-matching selection macros
    #define U3D_GNUC_COMPAT_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_GNUC_COMPAT(VALUE)
    // MSC compatibility matching and non-matching selection macros
    #define U3D_MSC_COMPAT_ONLY(VALUE)
    #define U3D_EXCEPT_MSC_COMPAT(VALUE) VALUE
// MSC
#elif defined(_MSC_VER)
    #define U3D_MSC _MSC_VER
    // MSC version macros
    #if defined(_MSC_FULL_VER)
        #define U3D_MSC_MAJOR (_MSC_FULL_VER / 1000000)
        #define U3D_MSC_MINOR ((_MSC_FULL_VER % 1000000) / 10000)
        #define U3D_MSC_PATCH (_MSC_FULL_VER % 10000)
    #else
        #define U3D_MSC_MAJOR (_MSC_VER / 100)
        #define U3D_MSC_MINOR (_MSC_VER % 100)
        #define U3D_MSC_PATCH (0)
    #endif
    // Host compiler version macros
    #define U3D_COMPILER_MAJOR U3D_MSC_MAJOR
    #define U3D_COMPILER_MINOR U3D_MSC_MINOR
    #define U3D_COMPILER_PATCH U3D_MSC_PATCH
    // Obviously MSC is MSC compatible
    #define U3D_MSC_COMPAT _MSC_VER
    // Compiler matching selection macros
    #define U3D_GNUC_ONLY(VALUE)
    #define U3D_MINGW_ONLY(VALUE)
    #define U3D_CLANG_ONLY(VALUE)
    #define U3D_MSC_ONLY(VALUE) VALUE
    // Compiler non-matching selection macros
    #define U3D_EXCEPT_GNUC(VALUE) VALUE
    #define U3D_EXCEPT_MINGW(VALUE) VALUE
    #define U3D_EXCEPT_CLANG(VALUE) VALUE
    #define U3D_EXCEPT_MSC(VALUE)
    // GCC compatibility matching and non-matching selection macros
    #define U3D_GNUC_COMPAT_ONLY(VALUE)
    #define U3D_EXCEPT_GNUC_COMPAT(VALUE) VALUE
    // MSC compatibility matching and non-matching selection macros
    #define U3D_MSC_COMPAT_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_MSC_COMPAT(VALUE)
// GCC (a.k.a. GNUC)
#elif defined(__GNUC__)
    #define U3D_GNUC __GNUC__
    // GCC version macros
    #define U3D_GNUC_MAJOR __GNUC__
    #define U3D_GNUC_MINOR __GNUC_MINOR__
    #define U3D_GNUC_PATCH __GNUC_PATCHLEVEL__
    // Host compiler version macros
    #define U3D_COMPILER_MAJOR __GNUC__
    #define U3D_COMPILER_MINOR __GNUC_MINOR__
    #define U3D_COMPILER_PATCH __GNUC_PATCHLEVEL__
    // Obviously GCC is GNUC compatible
    #define U3D_GNUC_COMPAT __GNUC__
    // Compiler matching selection macros
    #define U3D_GNUC_ONLY(VALUE) VALUE
    #define U3D_CLANG_ONLY(VALUE)
    #define U3D_MSC_ONLY(VALUE)
    // Compiler non-matching selection macros
    #define U3D_EXCEPT_GNUC(VALUE)
    #define U3D_EXCEPT_CLANG(VALUE) VALUE
    #define U3D_EXCEPT_MSC(VALUE) VALUE
    // MinGW is GNUC but with a touch of Windows
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define U3D_MINGW __GNUC__
        #define U3D_MINGW_ONLY(VALUE) VALUE
        #define U3D_EXCEPT_MINGW(VALUE)
    #else
        #define U3D_MINGW_ONLY(VALUE)
        #define U3D_EXCEPT_MINGW(VALUE) VALUE
    #endif
    // GCC compatibility matching and non-matching selection macros
    #define U3D_GNUC_COMPAT_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_GNUC_COMPAT(VALUE)
    // MSC compatibility matching and non-matching selection macros
    #define U3D_MSC_COMPAT_ONLY(VALUE)
    #define U3D_EXCEPT_MSC_COMPAT(VALUE) VALUE
// Unknown
#else
    #error Unknown or unsupported compiler. Must implement support for it.
#endif

// Select the value according to the current compiler.
#define U3D_GNUC_OR(GV, OV) U3D_GNUC_ONLY(GV) U3D_EXCEPT_GNUC(OV)
#define U3D_MINGW_OR(MV, OV) U3D_MINGW_ONLY(MV) U3D_EXCEPT_MINGW(OV)
#define U3D_CLANG_OR(CV, OV) U3D_CLANG_ONLY(CV) U3D_EXCEPT_CLANG(OV)
#define U3D_MSC_OR(MV, OV) U3D_MSC_ONLY(MV) U3D_EXCEPT_MSC(OV)
#define U3D_GNUC_COMPAT_OR(GV, OV) U3D_GNUC_COMPAT_ONLY(GV) U3D_EXCEPT_GNUC_COMPAT(OV)
#define U3D_MSC_COMPAT_OR(MV, OV) U3D_MSC_COMPAT_ONLY(GV) U3D_EXCEPT_MSC_COMPAT(OV)

// Version is 0xVVRRPPPP
#define U3D_VERSION_NUMBER(v, r, p) (((v) << 24) + ((r) << 16) + (p))
// Current compiler version
#define U3D_COMPILER_VERSION U3D_COMPILER_VERSION(U3D_COMPILER_MAJOR, U3D_COMPILER_MINOR, U3D_COMPILER_PATCH)

/*
 * Macros with identifiers of C version. What `__STDC_VERSION__` is defined to be.
 * Don't really need anything lower than C11 but they're added for consistency.
*/

#define U3D_C89_STANDARD (199409L)
#define U3D_C99_STANDARD (199901L)
#define U3D_C11_STANDARD (201112L)
#define U3D_C18_STANDARD (201710L)

/*
 * Macro that can be used to identify C version.
*/

#if defined(__STDC_VERSION__)
    #define U3D_C_STANDARD __STDC_VERSION__
#elif defined(__STDC__)
    #define U3D_C_STANDARD __STDC__
#else
    // Default to minimum C standard (even though c11 is required for the C++ we're using)
    #define U3D_C_STANDARD U3D_C89_STANDARD
#endif

/*
 * Macros with identifiers of C++ version. What `__cplusplus` is defined to be.
 * Don't really need anything lower than C++17 but they're added for consistency.
*/

#define U3D_CPP98_STANDARD (199711L)
#define U3D_CPP11_STANDARD (201103L)
#define U3D_CPP14_STANDARD (201402L)
#define U3D_CPP17_STANDARD (201703L)
#define U3D_CPP20_STANDARD (202002L)

/*
 * Macro that can be used to identify C++ version.
*/

#ifndef U3D_CPP_STANDARD
    #define U3D_CPP_STANDARD __cplusplus
#endif

/*
 * Make sure C++17 standard is available. Superficial check as it doesn't take into account partial implementations.
 * Some compilers may cheat on standard support. They will be "blocked" later on in compiler detection.
*/

#if U3D_CPP_STANDARD < U3D_CPP17_STANDARD
    #error This compiler does not support or have enabled the C++17 standard.
#endif

/*
 * Expression selection based on available C++ standard.
*/

// C++ == 11/14/17/20+
#if U3D_CPP_STANDARD == U3D_CPP11_STANDARD
    #define U3D_CPP11_ONLY(x) x
    #define U3D_CPP14_ONLY(x)
    #define U3D_CPP17_ONLY(x)
    #define U3D_CPP20_ONLY(x)
    #define U3D_EXCEPT_CPP11(x)
    #define U3D_EXCEPT_CPP14(x) x
    #define U3D_EXCEPT_CPP17(x) x
    #define U3D_EXCEPT_CPP20(x) x
#elif U3D_CPP_STANDARD == U3D_CPP14_STANDARD
    #define U3D_CPP11_ONLY(x)
    #define U3D_CPP14_ONLY(x) x
    #define U3D_CPP17_ONLY(x)
    #define U3D_CPP20_ONLY(x)
    #define U3D_EXCEPT_CPP11(x) x
    #define U3D_EXCEPT_CPP14(x)
    #define U3D_EXCEPT_CPP17(x) x
    #define U3D_EXCEPT_CPP20(x) x
#elif U3D_CPP_STANDARD == U3D_CPP17_STANDARD
    #define U3D_CPP11_ONLY(x)
    #define U3D_CPP14_ONLY(x)
    #define U3D_CPP17_ONLY(x) x
    #define U3D_CPP20_ONLY(x)
    #define U3D_EXCEPT_CPP11(x) x
    #define U3D_EXCEPT_CPP14(x) x
    #define U3D_EXCEPT_CPP17(x)
    #define U3D_EXCEPT_CPP20(x) x
#elif U3D_CPP_STANDARD == U3D_CPP20_STANDARD
    #define U3D_CPP11_ONLY(x)
    #define U3D_CPP14_ONLY(x)
    #define U3D_CPP17_ONLY(x)
    #define U3D_CPP20_ONLY(x) x
    #define U3D_EXCEPT_CPP11(x) x
    #define U3D_EXCEPT_CPP14(x) x
    #define U3D_EXCEPT_CPP17(x) x
    #define U3D_EXCEPT_CPP20(x)
#else
    #define U3D_CPP11_ONLY(x)
    #define U3D_CPP14_ONLY(x)
    #define U3D_CPP17_ONLY(x)
    #define U3D_CPP20_ONLY(x)
    #define U3D_EXCEPT_CPP11(x) x
    #define U3D_EXCEPT_CPP14(x) x
    #define U3D_EXCEPT_CPP17(x) x
    #define U3D_EXCEPT_CPP20(x) x
#endif

// C++ >= 11/14/17/20+
#if U3D_CPP_STANDARD >= U3D_CPP20_STANDARD
    #define U3D_LEAST_CPP11(x) x
    #define U3D_LEAST_CPP14(x) x
    #define U3D_LEAST_CPP17(x) x
    #define U3D_LEAST_CPP20(x) x
#elif U3D_CPP_STANDARD >= U3D_CPP17_STANDARD
    #define U3D_LEAST_CPP11(x) x
    #define U3D_LEAST_CPP14(x) x
    #define U3D_LEAST_CPP17(x) x
    #define U3D_LEAST_CPP20(x)
#elif U3D_CPP_STANDARD >= U3D_CPP14_STANDARD
    #define U3D_LEAST_CPP11(x) x
    #define U3D_LEAST_CPP14(x) x
    #define U3D_LEAST_CPP17(x)
    #define U3D_LEAST_CPP20(x)
#elif U3D_CPP_STANDARD >= U3D_CPP11_STANDARD
    #define U3D_LEAST_CPP11(x) x
    #define U3D_LEAST_CPP14(x)
    #define U3D_LEAST_CPP17(x)
    #define U3D_LEAST_CPP20(x)
#else
    #define U3D_LEAST_CPP11(x)
    #define U3D_LEAST_CPP14(x)
    #define U3D_LEAST_CPP17(x)
    #define U3D_LEAST_CPP20(x)
#endif

// Select the expression according to the available c++ standard.
#define U3D_CPP11(x) U3D_LEAST_CPP11(x)
#define U3D_CPP11_OR(x, y) U3D_CPP11_ONLY(x) U3D_EXCEPT_CPP11(x)
#define U3D_CPP14(x) U3D_LEAST_CPP14(x)
#define U3D_CPP14_OR(x, y) U3D_CPP14_ONLY(x) U3D_EXCEPT_CPP14(x)
#define U3D_CPP17(x) U3D_LEAST_CPP17(x)
#define U3D_CPP17_OR(x, y) U3D_CPP17_ONLY(x) U3D_EXCEPT_CPP17(x)
#define U3D_CPP20(x) U3D_LEAST_CPP20(x)
#define U3D_CPP20_OR(x, y) U3D_CPP20_ONLY(x) U3D_EXCEPT_CPP20(x)

/*
 * Pointer size identification.
*/

#if defined(U3D_GNUC) || defined(U3D_CLANG)
    // GNUC compatible compilers should have this nice built-in
    #define U3D_POINTER_BYTES __SIZEOF_POINTER__
    #define U3D_POINTER_SIZE __SIZEOF_POINTER__
    // We can probably avoid having to use an expression 
    #ifdef __INTPTR_WIDTH__
        #define U3D_POINTER_BITS __INTPTR_WIDTH__
    #elif __SIZE_WIDTH__
        #define U3D_POINTER_BITS __SIZE_WIDTH__
    // If all else fails, use an expression. We may not be able to use macro concatenation and expect the same result
    #else
        #define U3D_POINTER_BITS (__SIZEOF_POINTER__ * 8)
    #endif
#elif defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) || defined(__aarch64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64)
    // Got it through means of architecture identification
    #define U3D_POINTER_BITS 64
    #define U3D_POINTER_BYTES 8
    #define U3D_POINTER_SIZE 8
#elif
    // We can assume this is 32-bit
    #define U3D_POINTER_BITS 32
    #define U3D_POINTER_BYTES 4
    #define U3D_POINTER_SIZE 4
#endif

#if U3D_POINTER_SIZE == 8
    #define U3D64
    #define U3D64_ONLY(VALUE) VALUE
    #define U3D32_ONLY(VALUE)
#else
    // Some day
    #define U3D32
    #define U3D64_ONLY(VALUE)
    #define U3D32_ONLY(VALUE) VALUE
#endif

// Select the value according to the current pointer size.
#define U3D_ARCH_SELECT(X32V, X64V) U3D32_ONLY(X32V) U3D64_ONLY(X64V)

/*
 * Platform identification.
*/

#if defined(_WIN64) || defined(_WIN32)
    #define U3D_WINDOWS 1
    #define U3D_PLATFORM_WINDOWS 1
    #define U3D_PLATFORM_DESKTOP 1
    #define U3D_PLATFORM_MICROSOFT 1
    #define U3D_PLATFORM_NAME "Windows"
    // Both _WIN32 and _WIN64 are defined when compiling for Win-64
    #ifdef _WIN64
        #define U3D_WIN64 1
    #else
        #define U3D_WIN32 1
    #endif
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define U3D__PLATFORM_MINGW 1
    #endif
    // Identify other platform traits
    #if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
        #define U3D_X86_64 1
    #elif defined(_M_IX86) || defined(_X86_)
        #define U3D_X86 1
    #elif defined(_M_IA64) || defined(_IA64_)
        #define U3D_IA64 1
    #elif defined(_M_ARM)
        #define U3D_ARM32 1
    #elif defined(_M_ARM64)
        #define U3D_ARM64 1
    #else
        #error Unknown processor
    #endif
#elif defined(__ANDROID__)
    #define U3D_ANDROID 1
    #define U3D_PLATFORM_ANDROID 1
    #define U3D_PLATFORM_MOBILE 1
    #define U3D_PLATFORM_LINUX 1
    #define U3D_PLATFORM_POSIX 1
    #define U3D_PLATFORM_UNIX 1
    #define U3D_PLATFORM_NAME "Android"
    // Identify other platform traits
    #if defined(__arm__)
        #define U3D_ARM32 1
    #elif defined(__aarch64__)
        #define U3D_ARM64 1
    #elif defined(__i386__)
        #define U3D_X86 1
    #elif defined(__x86_64)
        #define U3D_X86_64 1
    #else
        #error Unknown processor
    #endif
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #define U3D_PLATFORM_APPLE 1
    #define U3D_PLATFORM_POSIX 1
    // Is this mobile?
    #if defined(__IPHONE__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
        #define U3D_IOS 1
        #define U3D_PLATFORM_IPHONE 1
        #define U3D_PLATFORM_MOBILE 1
        #define U3D_PLATFORM_NAME "iPhone"
        // Identify other platform traits
        #if defined(__arm__)
            #define U3D_ARM32 1
        #elif defined(__aarch64__) || defined(__AARCH64)
            #define U3D_ARM64 1
        #elif defined(__i386__)
            #define U3D_X86 1
        #elif defined(__x86_64) || defined(__amd64)
            #define U3D_X86_64 1
        #else
            #error Unknown processor
        #endif
    // Is this desktop?
    #elif defined(__MACH__)
        #define U3D_OSX 1
        #define U3D_PLATFORM_OSX 1
        #define U3D_PLATFORM_DESKTOP 1
        #define U3D_PLATFORM_UNIX 1
        #define U3D_PLATFORM_NAME "OSX"
        // Identify other platform traits
        #if defined(__i386__) || defined(__intel__)
            #define U3D_X86 1
        #elif defined(__x86_64) || defined(__amd64)
            #define U3D_X86_64 1
        #elif defined(__arm__)
            #define U3D_ARM32 1
        #elif defined(__aarch64__) || defined(__AARCH64)
            #define U3D_ARM64 1
        #else
            #error Unknown processor
        #endif
    #endif
#elif defined(__linux__)
    #define U3D_LINUX 1
    #define U3D_PLATFORM_LINUX 1
    #define U3D_PLATFORM_DESKTOP 1
    #define U3D_PLATFORM_POSIX
    #define U3D_PLATFORM_UNIX
    #define U3D_PLATFORM_NAME "Linux"
    // Identify other platform traits
    #if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
        #define U3D_X86 1
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
        #define U3D_ARM32 1
    #elif defined(__aarch64__) || defined(__AARCH64)
        #define U3D_ARM64 1
    #elif defined(__x86_64__)
        #define U3D_X86_64 1
    #else
        #error Unknown processor
    #endif
#else
    // Must be implemented and tested first
    #error This platform is not implemented
#endif

// Default to little-endian unless explicitly marked otherwise during platform detection
#if !defined(U3D_BIG_ENDIAN) && !defined(U3D_LITTLE_ENDIAN)
    #define U3D_LITTLE_ENDIAN 1
#endif

/*
 * Value selection depending on the platform.
*/

#if defined(U3D_WINDOWS)
    #define U3D_WINDOWS_ONLY(VALUE) VALUE
    #define U3D_ANDROID_ONLY(VALUE)
    #define U3D_LINUX_ONLY(VALUE)
    #define U3D_IOS_ONLY(VALUE)
    #define U3D_OSX_ONLY(VALUE)
    #define U3D_EXCEPT_WINDOWS(VALUE)
    #define U3D_EXCEPT_ANDROID(VALUE) VALUE
    #define U3D_EXCEPT_LINUX(VALUE) VALUE
    #define U3D_EXCEPT_IOS(VALUE) VALUE
    #define U3D_EXCEPT_OSX(VALUE) VALUE
#elif defined(U3D_ANDROID)
    #define U3D_WINDOWS_ONLY(VALUE)
    #define U3D_ANDROID_ONLY(VALUE) VALUE
    #define U3D_LINUX_ONLY(VALUE)
    #define U3D_IOS_ONLY(VALUE)
    #define U3D_OSX_ONLY(VALUE)
    #define U3D_EXCEPT_WINDOWS(VALUE) VALUE
    #define U3D_EXCEPT_ANDROID(VALUE)
    #define U3D_EXCEPT_LINUX(VALUE) VALUE
    #define U3D_EXCEPT_IOS(VALUE) VALUE
    #define U3D_EXCEPT_OSX(VALUE) VALUE
#elif defined(U3D_LINUX)
    #define U3D_WINDOWS_ONLY(VALUE)
    #define U3D_ANDROID_ONLY(VALUE)
    #define U3D_LINUX_ONLY(VALUE) VALUE
    #define U3D_IOS_ONLY(VALUE)
    #define U3D_OSX_ONLY(VALUE)
    #define U3D_EXCEPT_WINDOWS(VALUE) VALUE
    #define U3D_EXCEPT_ANDROID(VALUE) VALUE
    #define U3D_EXCEPT_LINUX(VALUE)
    #define U3D_EXCEPT_IOS(VALUE) VALUE
    #define U3D_EXCEPT_OSX(VALUE) VALUE
#elif defined(U3D_IOS)
    #define U3D_WINDOWS_ONLY(VALUE)
    #define U3D_ANDROID_ONLY(VALUE)
    #define U3D_LINUX_ONLY(VALUE)
    #define U3D_IOS_ONLY(VALUE) VALUE
    #define U3D_OSX_ONLY(VALUE)
    #define U3D_EXCEPT_WINDOWS(VALUE) VALUE
    #define U3D_EXCEPT_ANDROID(VALUE) VALUE
    #define U3D_EXCEPT_LINUX(VALUE) VALUE
    #define U3D_EXCEPT_IOS(VALUE)
    #define U3D_EXCEPT_OSX(VALUE) VALUE
#elif defined(U3D_OSX)
    #define U3D_WINDOWS_ONLY(VALUE)
    #define U3D_ANDROID_ONLY(VALUE)
    #define U3D_LINUX_ONLY(VALUE)
    #define U3D_IOS_ONLY(VALUE)
    #define U3D_OSX_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_WINDOWS(VALUE) VALUE
    #define U3D_EXCEPT_ANDROID(VALUE) VALUE
    #define U3D_EXCEPT_LINUX(VALUE) VALUE
    #define U3D_EXCEPT_IOS(VALUE) VALUE
    #define U3D_EXCEPT_OSX(VALUE)
#else
    #define U3D_WINDOWS_ONLY(VALUE)
    #define U3D_ANDROID_ONLY(VALUE)
    #define U3D_LINUX_ONLY(VALUE)
    #define U3D_IOS_ONLY(VALUE)
    #define U3D_OSX_ONLY(VALUE)
    #define U3D_EXCEPT_WINDOWS(VALUE) VALUE
    #define U3D_EXCEPT_ANDROID(VALUE) VALUE
    #define U3D_EXCEPT_LINUX(VALUE) VALUE
    #define U3D_EXCEPT_IOS(VALUE) VALUE
    #define U3D_EXCEPT_OSX(VALUE) VALUE
#endif

// Select the value according to the current platform.
#define U3D_WINDOWS_OR(WV, OV) U3D_WINDOWS_ONLY(WV) U3D_EXCEPT_WINDOWS(OV)
#define U3D_ANDROID_OR(WV, OV) U3D_ANDROID_ONLY(WV) U3D_EXCEPT_ANDROID(OV)
#define U3D_LINUX_OR(LV, OV) U3D_LINUX_ONLY(LV) U3D_EXCEPT_LINUX(OV)
#define U3D_IOS_OR(MV, OV) U3D_IOS_ONLY(MV) U3D_EXCEPT_IOS(OV)
#define U3D_OSX_OR(UV, OV) U3D_OSX_ONLY(UV) U3D_EXCEPT_OSX(OV)

/*
 * Intrinsics.
*/

#if defined(U3D_GNUC) || defined(U3D_CLANG)
    #define U3D_EXPECT(expr, val) __builtin_expect(expr, val)
#else
    #define U3D_EXPECT(expr, val) (expr)
#endif

// Branch prediction information.
#define U3D_LIKELY(expr) U3D_EXPECT(expr, 1)
#define U3D_UNLIKELY(expr) U3D_EXPECT(expr, 0)

// Make sure U3D_IS_DEBUG is defined whenever _DEBUG is. So that we have a consistent method of identifying debug mode.
#if defined(_DEBUG) && !defined(U3D_IS_DEBUG)
    #define U3D_IS_DEBUG 1
#endif

/*
 * Assertion macros, if required and allowed.
*/

#if defined(U3D_IS_DEBUG) && !defined(U3D_NO_ASSERT)
    #define U3D_ASSERT(e) assert(e);
    #define U3D_ASSERT_ONLY(x) x
    #define U3D_EXCEPT_ASSERT(x)
    #define U3D_ASSERT_ENABLED 1
#else
    #define U3D_ASSERT(e)
    #define U3D_ASSERT_ONLY(x)
    #define U3D_EXCEPT_ASSERT(x) x
    #undef U3D_ASSERT_ENABLED
#endif

// Select the value according to the current assert status.
#define U3D_ASSERT_OR(AV, OV) U3D_ASSERT_ONLY(AV) U3D_EXCEPT_ASSERT(OV)

/*
 * Debug or release mode code selection.
*/

#ifdef U3D_IS_DEBUG
    #define U3D_DEBUG_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_DEBUG(VALUE)
    #define U3D_RELEASE_ONLY(VALUE)
    #define U3D_EXCEPT_RELEASE(VALUE) VALUE
#else
    #define U3D_DEBUG_ONLY(VALUE)
    #define U3D_EXCEPT_DEBUG(VALUE) VALUE
    #define U3D_RELEASE_ONLY(VALUE) VALUE
    #define U3D_EXCEPT_RELEASE(VALUE)
#endif

// Select the value according to the current build mode.
#define U3D_DEBUG_OR(DV, OV) U3D_DEBUG_ONLY(DV) U3D_EXCEPT_DEBUG(OV)
#define U3D_RELEASE_OR(RV, OV) U3D_RELEASE_ONLY(RV) U3D_EXCEPT_RELEASE(OV)

/*
 * Cache line alignment. A reasonable default guess based on the supported architectures.
 *
 * Note: Overestimates tend to waste more space, while underestimates tend to waste more time.
 * ARM: These values are not strictly correct since cache line sizes depend on implementations, not architectures.
 *      There are even implementations with cache line sizes configurable at boot time.
*/

#ifndef U3D_CACHELINE_SIZE
    #if   defined(U3D_X86)      
        // This is the minimum possible value.
        #define U3D_CACHELINE_SIZE 32
    #elif defined(U3D_X86_64)  
        // This is the minimum possible value
        #define U3D_CACHELINE_SIZE 64
    #elif defined(U3D_ARM32)
        // This varies between implementations and is usually 32 or 64. 
        #define U3D_CACHELINE_SIZE 32
    #elif defined(U3D_ARM64)
        // Cache line Cortex-A8  (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
        #define U3D_CACHELINE_SIZE 64
    #elif (U3D_POINTER_SIZE == 4)
        // This is the minimum possible value
        #define U3D_CACHELINE_SIZE 32
    #else
        // This is the minimum possible value
        #define U3D_CACHELINE_SIZE 64
    #endif
#endif

/*
 * Inline helpers.
*/

#if defined(U3D_MSC)
    #define U3D_INLINE __forceinline
    #define U3D_NOINLINE
#elif defined(U3D_GNUC) || defined(U3D_CLANG)
    #define U3D_INLINE __attribute__((always_inline))
    #define U3D_NOINLINE __attribute__((noinline))
#else
    #define U3D_INLINE inline
    #define U3D_NOINLINE
#endif

/*
 * Unreachable code. Mark code paths that are known to not be reached.
*/

// GCC 4.8+, Clang, Intel and other compilers compatible with GCC
#if defined(U3D_GNUC) || defined(U3D_CLANG)
    [[noreturn]] inline __attribute__((always_inline)) void Unreachable() { __builtin_unreachable(); }
    #define U3D_UNREACHABLE ::Ax::Unreachable();
// MSVC
#elif defined(U3D_MSC)
    [[noreturn]] __forceinline void Unreachable() { __assume(false); }
    #define U3D_UNREACHABLE ::Ax::Unreachable();
// Failsafe
#else
    #define U3D_UNREACHABLE U3D_ASSERT(0) // TODO: do a better job that works in release mode as well
#endif

/*
 * is_constant_evaluated feature.
*/

// Can it be detected via __has_builtin method?
#if defined(__has_builtin)
    #if __has_builtin(__builtin_is_constant_evaluated)
        #define U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
    #endif
// GCC and Clang introduced this in version 9
#elif (defined(U3D_GNUC) || defined(U3D_CLANG)) && (U3D_COMPILER_MAJOR >= 9)
    #define U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
// MSVC seems to have added this in version 19.25
#elif defined(U3D_MSC) && (U3D_COMPILER_MAJOR >= 19) && (U3D_COMPILER_MINOR >= 25)
    #define U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
#else
    #undef U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED
#endif

/*
 * constant detection feature.
*/

// Can it be detected via __has_builtin method?
#if defined(__has_builtin)
    #if __has_builtin(__builtin_constant_p)
        #define U3D_HAVE_BUILTIN_CONSTANT_P 1
    #endif
// GCC and Clang should already have this
#elif defined(U3D_GNUC) || defined(U3D_CLANG)
    #define U3D_HAVE_BUILTIN_CONSTANT_P 1
// R.I.P for MSVC and the rest
#else
    #undef U3D_HAVE_BUILTIN_CONSTANT_P
#endif

/*
 * Include other headers.
*/

#ifdef URHO3D_IS_BUILDING
    #include "Urho3D.h"
#else
    #include <Urho3D/Urho3D.h>
#endif

/*
 * Case insensitive string comparison. This may differ in name depending on the platform.
*/

#ifdef U3D_WINDOWS
   #define U3D_STRICMP(a,b) stricmp(a,b)
   #define U3D_STRNICMP(a,b,n) strnicmp(a,b,n)
#else
   #define U3D_STRICMP(a,b) strcasecmp(a,b)
   #define U3D_STRNICMP(a,b,n) strncasecmp(a,b,n)
#endif
