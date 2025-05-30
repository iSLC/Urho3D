//
// Copyright (c) 2008-2025 the Urho3D project.
//

#pragma once

/*
 * Fundamental headers.
*/

#ifdef __cplusplus
    #include <cstddef>
    #include <climits>
    #include <cstdint>
    #include <cfloat>
#else
    #include <stddef.h>
    #include <limits.h>
    #include <stdint.h>
    #include <float.h>
#endif

/*
 * Macro concatenation helpers.
*/

#define UH_CONCAT_(a, b) a##b
#define UH_CONCAT(a, b) UH_CONCAT_(a, b)

/*
 * Macro string(ification) helpers.
*/

#define UH_TRUESTRINGIZE(x) #x
#define UH_STRINGIZEWRAP(x) UH_TRUESTRINGIZE(x)

/*
 * Make sure UH_IS_DEBUG is defined whenever _DEBUG is. So that we have a consistent method of identifying debug mode.
*/

#if defined(_DEBUG) && !defined(UH_IS_DEBUG)
    #define UH_IS_DEBUG 1
#endif

/*
 * Compiler identification.
*/
// Clang pretending to be MSC
#if defined(__clang__) && defined(_MSC_VER)
    // Compiler name (i.e. name prefix)
    #define UH_COMPILER_NAME Clang
    // Compiler text (i.e. textual representation)
    #define UH_COMPILER_TEXT "Clang"
    // Identification macro (assume compatibility for both)
    #define UH_CLANG __clang__
    #define UH_MSC _MSC_VER
    // Clang version macros
    #define UH_CLANG_MAJOR __clang_major__
    #define UH_CLANG_MINOR __clang_minor__
    #define UH_CLANG_PATCH __clang_patchlevel__
    // MSC version macros
    #define UH_MSC_MAJOR (_MSC_VER / 100)
    #define UH_MSC_MINOR (_MSC_VER % 100)
    #define UH_MSC_PATCH (_MSC_FULL_VER % 100000)
    // Host compiler version macros
    #define UH_COMPILER_MAJOR UH_CLANG_MAJOR
    #define UH_COMPILER_MINOR UH_CLANG_MINOR
    #define UH_COMPILER_PATCH UH_CLANG_PATCH
    // Clang offers MSC compatibility
    #define UH_MSC_COMPAT _MSC_VER
    // Compiler matching selection macros
    #define UH_GNUC_ONLY(VALUE)
    #define UH_MINGW_ONLY(VALUE)
    #define UH_CLANG_ONLY(VALUE) VALUE
    #define UH_MSC_ONLY(VALUE) VALUE
    // Compiler non-matching selection macros
    #define UH_EXCEPT_GNUC(VALUE) VALUE
    #define UH_EXCEPT_MINGW(VALUE) VALUE
    #define UH_EXCEPT_CLANG(VALUE)
    #define UH_EXCEPT_MSC(VALUE)
// Clang
#elif defined(__clang__)
    // Compiler name (i.e. name prefix)
    #define UH_COMPILER_NAME Clang
    // Compiler text (i.e. textual representation)
    #define UH_COMPILER_TEXT "Clang"
    // Identification macro
    #define UH_CLANG __clang__
    // Clang version macros
    #define UH_CLANG_MAJOR __clang_major__
    #define UH_CLANG_MINOR __clang_minor__
    #define UH_CLANG_PATCH __clang_patchlevel__
    // Host compiler version macros
    #define UH_COMPILER_MAJOR UH_CLANG_MAJOR
    #define UH_COMPILER_MINOR UH_COMPILER_MINOR
    #define UH_COMPILER_PATCH UH_COMPILER_PATCH
    // Clang offers GNUC compatibility
    #ifdef __GNUC__
        #define UH_GNUC_COMPAT __GNUC__
    #endif
    // Compiler matching selection macros
    #define UH_GNUC_ONLY(VALUE)
    #define UH_MINGW_ONLY(VALUE)
    #define UH_CLANG_ONLY(VALUE) VALUE
    #define UH_MSC_ONLY(VALUE)
    // Compiler non-matching selection macros
    #define UH_EXCEPT_GNUC(VALUE) VALUE
    #define UH_EXCEPT_MINGW(VALUE) VALUE
    #define UH_EXCEPT_CLANG(VALUE)
    #define UH_EXCEPT_MSC(VALUE) VALUE
// Microsoft Visual C++
#elif defined(_MSC_VER)
    // Compiler name (i.e. name prefix)
    #define UH_COMPILER_NAME MSC
    // Compiler text (i.e. textual representation)
    #define UH_COMPILER_TEXT "Microsoft Visual C++"
    // Identification macro
    #define UH_MSC _MSC_VER
    // MSC version macros
    #define UH_MSC_MAJOR (_MSC_VER / 100)
    #define UH_MSC_MINOR (_MSC_VER % 100)
    #define UH_MSC_PATCH (_MSC_FULL_VER % 100000)
    // Host compiler version macros
    #define UH_COMPILER_MAJOR UH_MSC_MAJOR
    #define UH_COMPILER_MINOR UH_MSC_MINOR
    #define UH_COMPILER_PATCH UH_MSC_PATCH
    // Obviously MSC is MSC compatible
    #define UH_MSC_COMPAT _MSC_VER
    // Compiler matching selection macros
    #define UH_GNUC_ONLY(VALUE)
    #define UH_MINGW_ONLY(VALUE)
    #define UH_CLANG_ONLY(VALUE)
    #define UH_MSC_ONLY(VALUE) VALUE
    // Compiler non-matching selection macros
    #define UH_EXCEPT_GNUC(VALUE) VALUE
    #define UH_EXCEPT_MINGW(VALUE) VALUE
    #define UH_EXCEPT_CLANG(VALUE) VALUE
    #define UH_EXCEPT_MSC(VALUE)
// GNUC
#elif defined(__GNUC__)
    // Compiler name (i.e. name prefix)
    #define UH_COMPILER_NAME GCC
    // Compiler text (i.e. textual representation)
    #define UH_COMPILER_TEXT "GCC"
    // Identification macro
    #define UH_GNUC __GNUC__
    // GNUC version macros
    #define UH_GNUC_MAJOR __GNUC__
    #define UH_GNUC_MINOR __GNUC_MINOR__
    #define UH_GNUC_PATCH __GNUC_PATCHLEVEL__
    // Host compiler version macros
    #define UH_COMPILER_MAJOR UH_GNUC_MAJOR
    #define UH_COMPILER_MINOR UH_GNUC_MINOR
    #define UH_COMPILER_PATCH UH_GNUC_PATCH
    // Obviously GCC is GNUC compatible
    #define UH_GNUC_COMPAT __GNUC__
    // Compiler matching selection macros
    #define UH_GNUC_ONLY(VALUE) VALUE
    #define UH_CLANG_ONLY(VALUE)
    #define UH_MSC_ONLY(VALUE)
    #define UH_EMCC_ONLY(VALUE)
    // Compiler non-matching selection macros
    #define UH_EXCEPT_GNUC(VALUE)
    #define UH_EXCEPT_CLANG(VALUE) VALUE
    #define UH_EXCEPT_MSC(VALUE) VALUE
    #define UH_EXCEPT_EMCC(VALUE) VALUE
    // MinGW is GNUC but with a touch of Windows
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define UH_MINGW __GNUC__
        #define UH_MINGW_ONLY(VALUE) VALUE
        #define UH_EXCEPT_MINGW(VALUE)
    #else
        #define UH_MINGW_ONLY(VALUE)
        #define UH_EXCEPT_MINGW(VALUE) VALUE
    #endif
// Unknown
#else
    #error Unknown or unsupported compiler. Must implement support for it.
#endif

// Select the value according to the current compiler.
#define UH_GNUC_OR(GV, OV) UH_GNUC_ONLY(GV) UH_EXCEPT_GNUC(OV)
#define UH_MINGW_OR(MV, OV) UH_MINGW_ONLY(MV) UH_EXCEPT_MINGW(OV)
#define UH_CLANG_OR(CV, OV) UH_CLANG_ONLY(CV) UH_EXCEPT_CLANG(OV)
#define UH_MSC_OR(MV, OV) UH_MSC_ONLY(MV) UH_EXCEPT_MSC(OV)

// GNUC compatibility matching and non-matching selection macros
#ifdef UH_GNUC_COMPAT
    #define UH_GNUC_COMPAT_ONLY(VALUE) VALUE
    #define UH_EXCEPT_GNUC_COMPAT(VALUE)
#else
    #define UH_GNUC_COMPAT_ONLY(VALUE)
    #define UH_EXCEPT_GNUC_COMPAT(VALUE) VALUE
#endif

// MSC compatibility matching and non-matching selection macros
#ifdef UH_MSC_COMPAT
    #define UH_MSC_COMPAT_ONLY(VALUE) VALUE
    #define UH_EXCEPT_MSC_COMPAT(VALUE)
#else
    #define UH_MSC_COMPAT_ONLY(VALUE)
    #define UH_EXCEPT_MSC_COMPAT(VALUE) VALUE
#endif

// Select the value according to the available compatibility.
#define UH_GNUC_COMPAT_OR(GV, OV) UH_GNUC_COMPAT_ONLY(GV) UH_EXCEPT_GNUC_COMPAT(OV)
#define UH_MSC_COMPAT_OR(MV, OV) UH_MSC_COMPAT_ONLY(MV) UH_EXCEPT_MSC_COMPAT(OV)

// Version is 0xVVRRPPPP
#define UH_VERSION_NUMBER(v, r, p) (((v) << 24) + ((r) << 16) + (p))
// Current compiler version
#define UH_COMPILER_VERSION UH_VERSION_NUMBER(UH_COMPILER_MAJOR, UH_COMPILER_MINOR, UH_COMPILER_PATCH)

// Allow expressions to be filtered based on available language.
#ifdef __cplusplus
    #define UH_C_LANG_ONLY(x)
    #define UH_CPP_LANG_ONLY(x, y) x
#else
    #define UH_C_LANG_ONLY(x) x
    #define UH_CPP_LANG_ONLY(x, y)
#endif // __cplusplus

// Select the expression according to the available language.
#define UH_C_LANG_OR(CV, XV) UH_C_LANG_ONLY(CV)  UH_CPP_LANG_ONLY(XV)
#define UH_CPP_LANG_OR(XV, CV) UH_CPP_LANG_ONLY(XV) UH_C_LANG_ONLY(CV) 

/*
 * Macros with identifiers of C version. What `__STDC_VERSION__` is defined to be.
 * Don't really need anything lower than C11, but they're added for consistency.
*/

#define UH_C89_STANDARD (199409L) // GCC/Clang: -std=c89, -std=c90
#define UH_C99_STANDARD (199901L) // GCC/Clang: -std=c99, -std=c9x
#define UH_C11_STANDARD (201112L) // GCC/Clang: -std=c11, -std=c1x MSVC: /std:c11
#define UH_C17_STANDARD (201710L) // GCC/Clang: -std=c17, -std=c18 MSVC: /std:c17
#define UH_C23_STANDARD (202311L) // GCC/Clang: -std=c23, -std=c2x MSVC: /std:clatest
#define UH_C29_STANDARD (202400L) // GCC/Clang: -std=c2y MSVC: /std:clatest (SPECULATIVE! PLEASE UPDATE ACCORDINGLY!)

/*
 * Macro that can be used to identify C version.
 * __STDC__ can be 1 outside of C so fallback to C89 as minimum.
*/

#ifndef UH_C_STANDARD
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= UH_C89_STANDARD)
        #define UH_C_STANDARD __STDC_VERSION__
    #elif defined(__STDC__) && (__STDC__ >= UH_C89_STANDARD)
        #define UH_C_STANDARD __STDC__
    #else
        // Default to minimum C standard as last resort
        #define UH_C_STANDARD UH_C89_STANDARD
    #endif
#endif

/*
 * Expression selection based on available C standard.
*/

// C == 89
#if (UH_C_STANDARD == UH_C89_STANDARD)
    #define UH_C89_ONLY(x) x
    #define UH_C99_ONLY(x)
    #define UH_C11_ONLY(x)
    #define UH_C17_ONLY(x)
    #define UH_C23_ONLY(x)
    #define UH_C29_ONLY(x)
    #define UH_EXCEPT_C89(x)
    #define UH_EXCEPT_C99(x) x
    #define UH_EXCEPT_C11(x) x
    #define UH_EXCEPT_C17(x) x
    #define UH_EXCEPT_C23(x) x
    #define UH_EXCEPT_C29(x) x
// C == 99
#elif (UH_C_STANDARD > UH_C89_STANDARD) && (UH_C_STANDARD <= UH_C99_STANDARD)
    #define UH_C89_ONLY(x)
    #define UH_C99_ONLY(x) x
    #define UH_C11_ONLY(x)
    #define UH_C17_ONLY(x)
    #define UH_C23_ONLY(x)
    #define UH_C29_ONLY(x)
    #define UH_EXCEPT_C89(x) x
    #define UH_EXCEPT_C99(x)
    #define UH_EXCEPT_C11(x) x
    #define UH_EXCEPT_C17(x) x
    #define UH_EXCEPT_C23(x) x
    #define UH_EXCEPT_C29(x) x
// C == 11
#elif (UH_C_STANDARD > UH_C99_STANDARD) && (UH_C_STANDARD <= UH_C11_STANDARD)
    #define UH_C89_ONLY(x)
    #define UH_C99_ONLY(x)
    #define UH_C11_ONLY(x) x
    #define UH_C17_ONLY(x)
    #define UH_C23_ONLY(x)
    #define UH_C29_ONLY(x)
    #define UH_EXCEPT_C89(x) x
    #define UH_EXCEPT_C99(x) x
    #define UH_EXCEPT_C11(x)
    #define UH_EXCEPT_C17(x) x
    #define UH_EXCEPT_C23(x) x
    #define UH_EXCEPT_C29(x) x
// C == 17
#elif (UH_C_STANDARD > UH_C11_STANDARD) && (UH_C_STANDARD <= UH_C17_STANDARD)
    #define UH_C89_ONLY(x)
    #define UH_C99_ONLY(x)
    #define UH_C11_ONLY(x)
    #define UH_C17_ONLY(x) x
    #define UH_C23_ONLY(x)
    #define UH_C29_ONLY(x)
    #define UH_EXCEPT_C89(x) x
    #define UH_EXCEPT_C99(x) x
    #define UH_EXCEPT_C11(x) x
    #define UH_EXCEPT_C17(x)
    #define UH_EXCEPT_C23(x) x
    #define UH_EXCEPT_C29(x) x
// C == 23
#elif (UH_C_STANDARD > UH_C17_STANDARD) && (UH_C_STANDARD <= UH_C23_STANDARD)
    #define UH_C89_ONLY(x)
    #define UH_C99_ONLY(x)
    #define UH_C11_ONLY(x)
    #define UH_C17_ONLY(x)
    #define UH_C23_ONLY(x) x
    #define UH_C29_ONLY(x)
    #define UH_EXCEPT_C89(x) x
    #define UH_EXCEPT_C99(x) x
    #define UH_EXCEPT_C11(x) x
    #define UH_EXCEPT_C17(x) x
    #define UH_EXCEPT_C23(x)
    #define UH_EXCEPT_C29(x) x
// C == 29
#elif (UH_C_STANDARD > UH_C23_STANDARD) && (UH_C_STANDARD <= UH_C29_STANDARD)
    #define UH_C89_ONLY(x)
    #define UH_C99_ONLY(x)
    #define UH_C11_ONLY(x)
    #define UH_C17_ONLY(x)
    #define UH_C23_ONLY(x)
    #define UH_C29_ONLY(x) x
    #define UH_EXCEPT_C89(x) x
    #define UH_EXCEPT_C99(x) x
    #define UH_EXCEPT_C11(x) x
    #define UH_EXCEPT_C17(x) x
    #define UH_EXCEPT_C23(x) x
    #define UH_EXCEPT_C29(x)
// C++ != 89/99/11/17/23/29
#else
    // This will blow up whenever the standard goes above what we know atm.
    // And needs to have values and macros updated to account for newer C standards.
    #error Unknown or unsupported C standard.
#endif

// C >= 89/99/11/17/23/29+
#if UH_C_STANDARD > UH_C23_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x) x
    #define UH_LEAST_C11(x) x
    #define UH_LEAST_C17(x) x
    #define UH_LEAST_C23(x) x
    #define UH_LEAST_C29(x) x
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) x
    #define UH_LEAST_C11_OR(x, y) x
    #define UH_LEAST_C17_OR(x, y) x
    #define UH_LEAST_C23_OR(x, y) x
    #define UH_LEAST_C29_OR(x, y) x
    #define UH_SUPPORTS_C89 1
    #define UH_SUPPORTS_C99 1
    #define UH_SUPPORTS_C11 1
    #define UH_SUPPORTS_C17 1
    #define UH_SUPPORTS_C23 1
    #define UH_SUPPORTS_C29 1
// C >= 89/99/11/17/23+
#elif UH_C_STANDARD > UH_C17_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x) x
    #define UH_LEAST_C11(x) x
    #define UH_LEAST_C17(x) x
    #define UH_LEAST_C23(x) x
    #define UH_LEAST_C29(x)
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) x
    #define UH_LEAST_C11_OR(x, y) x
    #define UH_LEAST_C17_OR(x, y) x
    #define UH_LEAST_C23_OR(x, y) x
    #define UH_LEAST_C29_OR(x, y) y
    #define UH_SUPPORTS_C89 1
    #define UH_SUPPORTS_C99 1
    #define UH_SUPPORTS_C11 1
    #define UH_SUPPORTS_C17 1
    #define UH_SUPPORTS_C23 1
// C >= 89/99/11/17+
#elif UH_C_STANDARD > UH_C11_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x) x
    #define UH_LEAST_C11(x) x
    #define UH_LEAST_C17(x) x
    #define UH_LEAST_C23(x)
    #define UH_LEAST_C29(x)
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) x
    #define UH_LEAST_C11_OR(x, y) x
    #define UH_LEAST_C17_OR(x, y) x
    #define UH_LEAST_C23_OR(x, y) y
    #define UH_LEAST_C29_OR(x, y) y
    #define UH_SUPPORTS_C89 1
    #define UH_SUPPORTS_C99 1
    #define UH_SUPPORTS_C11 1
    #define UH_SUPPORTS_C17 1
// C >= 89/99/11+
#elif UH_C_STANDARD > UH_C99_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x) x
    #define UH_LEAST_C11(x) x
    #define UH_LEAST_C17(x)
    #define UH_LEAST_C23(x)
    #define UH_LEAST_C29(x)
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) x
    #define UH_LEAST_C11_OR(x, y) x
    #define UH_LEAST_C17_OR(x, y) y
    #define UH_LEAST_C23_OR(x, y) y
    #define UH_LEAST_C29_OR(x, y) y
    #define UH_SUPPORTS_C89 1
    #define UH_SUPPORTS_C99 1
    #define UH_SUPPORTS_C11 1
// C >= 89/99+
#elif UH_C_STANDARD > UH_C89_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x) x
    #define UH_LEAST_C11(x)
    #define UH_LEAST_C17(x)
    #define UH_LEAST_C23(x)
    #define UH_LEAST_C29(x)
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) x
    #define UH_LEAST_C11_OR(x, y) y
    #define UH_LEAST_C17_OR(x, y) y
    #define UH_LEAST_C23_OR(x, y) y
    #define UH_LEAST_C29_OR(x, y) y
    #define UH_SUPPORTS_C89 1
    #define UH_SUPPORTS_C99 1
// C >= 89+
#elif UH_C_STANDARD == UH_C89_STANDARD
    #define UH_LEAST_C89(x) x
    #define UH_LEAST_C99(x)
    #define UH_LEAST_C11(x)
    #define UH_LEAST_C17(x)
    #define UH_LEAST_C23(x)
    #define UH_LEAST_C29(x)
    #define UH_LEAST_C89_OR(x, y) x
    #define UH_LEAST_C99_OR(x, y) y
    #define UH_LEAST_C11_OR(x, y) y
    #define UH_LEAST_C17_OR(x, y) y
    #define UH_LEAST_C23_OR(x, y) y
    #define UH_LEAST_C29_OR(x, y) y
    #define UH_SUPPORTS_C89 1
// C < 89 (realistically, can't go lower than this!)
#else
    #error Unknown or unsupported C standard.
#endif

// Select the expression according to the available C standard.
#define UH_C89(x) UH_LEAST_C89(x)
#define UH_C89_OR(x, y) UH_C89_ONLY(x) UH_EXCEPT_C89(x)
#define UH_C99(x) UH_LEAST_C99(x)
#define UH_C99_OR(x, y) UH_C99_ONLY(x) UH_EXCEPT_C99(x)
#define UH_C11(x) UH_LEAST_C11(x)
#define UH_C11_OR(x, y) UH_C11_ONLY(x) UH_EXCEPT_C11(x)
#define UH_C17(x) UH_LEAST_C17(x)
#define UH_C17_OR(x, y) UH_C17_ONLY(x) UH_EXCEPT_C17(x)
#define UH_C23(x) UH_LEAST_C23(x)
#define UH_C23_OR(x, y) UH_C23_ONLY(x) UH_EXCEPT_C23(x)
#define UH_C29(x) UH_LEAST_C29(x)
#define UH_C29_OR(x, y) UH_C29_ONLY(x) UH_EXCEPT_C29(x)

/*
 * Macros with identifiers of C++ version. What `__cplusplus` is defined to be.
 * Don't really need anything lower than C++23, but they're added for consistency.
*/

#define UH_CPP98_STANDARD (199711L) // GCC/Clang: -std=c++98, -std=c++03
#define UH_CPP11_STANDARD (201103L) // GCC/Clang: -std=c++11, -std=c++0x
#define UH_CPP14_STANDARD (201402L) // GCC/Clang: -std=c++14, -std=c++1y MSVC: /std:c++14
#define UH_CPP17_STANDARD (201703L) // GCC/Clang: -std=c++17, -std=c++1z MSVC: /std:c++17
#define UH_CPP20_STANDARD (202002L) // GCC/Clang: -std=c++20, -std=c++2a MSVC: /std:c++20
#define UH_CPP23_STANDARD (202302L) // GCC/Clang: -std=c++23, -std=c++2b MSVC: /std:c++latest
#define UH_CPP26_STANDARD (202400L) // GCC/Clang: -std=c++26, -std=c++2c MSVC: /std:c++latest (SPECULATIVE! PLEASE UPDATE ACCORDINGLY!)

/*
 * Macro that can be used to identify C++ version.
*/

#ifndef UH_CPP_STANDARD
    #ifdef UH_MSC
        #define UH_CPP_STANDARD _MSVC_LANG
    #else
        #define UH_CPP_STANDARD __cplusplus
    #endif
#endif

/*
 * Expression selection based on available C++ standard.
*/

// C++ == 98/03
#if (UH_CPP_STANDARD == UH_CPP98_STANDARD)
    #define UH_CPP98_ONLY(x) x
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x)
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x) x
// C++ == 11
#elif (UH_CPP_STANDARD > UH_CPP98_STANDARD) && (UH_CPP_STANDARD <= UH_CPP11_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x) x
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x)
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x) x
// C++ == 14
#elif (UH_CPP_STANDARD > UH_CPP11_STANDARD) && (UH_CPP_STANDARD <= UH_CPP14_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x) x
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x)
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x) x
// C++ == 17
#elif (UH_CPP_STANDARD > UH_CPP14_STANDARD) && (UH_CPP_STANDARD <= UH_CPP17_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x) x
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x)
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x) x
// C++ == 20
#elif (UH_CPP_STANDARD > UH_CPP17_STANDARD) && (UH_CPP_STANDARD <= UH_CPP20_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x) x
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x)
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x) x
// C++ == 23
#elif (UH_CPP_STANDARD > UH_CPP20_STANDARD) && (UH_CPP_STANDARD <= UH_CPP23_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x) x
    #define UH_CPP26_ONLY(x)
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x)
    #define UH_EXCEPT_CPP26(x) x
// C++ == 26
#elif (UH_CPP_STANDARD > UH_CPP23_STANDARD) && (UH_CPP_STANDARD <= UH_CPP26_STANDARD)
    #define UH_CPP98_ONLY(x)
    #define UH_CPP11_ONLY(x)
    #define UH_CPP14_ONLY(x)
    #define UH_CPP17_ONLY(x)
    #define UH_CPP20_ONLY(x)
    #define UH_CPP23_ONLY(x)
    #define UH_CPP26_ONLY(x) x
    #define UH_EXCEPT_CPP98(x) x
    #define UH_EXCEPT_CPP11(x) x
    #define UH_EXCEPT_CPP14(x) x
    #define UH_EXCEPT_CPP17(x) x
    #define UH_EXCEPT_CPP20(x) x
    #define UH_EXCEPT_CPP23(x) x
    #define UH_EXCEPT_CPP26(x)
// C++ != 98/11/14/17/20/23/26
#elif defined(__cplusplus)
    // This will blow up whenever the standard goes above what we know atm.
    // And needs to have values and macros updated to account for newer C++ standards.
    #error Unknown or unsupported C++ standard.
#endif

// C++ >= 11/14/17/20/23/26+
#if UH_CPP_STANDARD > UH_CPP23_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x) x
    #define UH_LEAST_CPP17(x) x
    #define UH_LEAST_CPP20(x) x
    #define UH_LEAST_CPP23(x) x
    #define UH_LEAST_CPP26(x) x
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) x
    #define UH_LEAST_CPP17_OR(x, y) x
    #define UH_LEAST_CPP20_OR(x, y) x
    #define UH_LEAST_CPP23_OR(x, y) x
    #define UH_LEAST_CPP26_OR(x, y) x
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
    #define UH_SUPPORTS_CPP14 1
    #define UH_SUPPORTS_CPP17 1
    #define UH_SUPPORTS_CPP20 1
    #define UH_SUPPORTS_CPP23 1
    #define UH_SUPPORTS_CPP26 1
// C++ >= 11/14/17/20/23~
#elif UH_CPP_STANDARD > UH_CPP20_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x) x
    #define UH_LEAST_CPP17(x) x
    #define UH_LEAST_CPP20(x) x
    #define UH_LEAST_CPP23(x) x
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) x
    #define UH_LEAST_CPP17_OR(x, y) x
    #define UH_LEAST_CPP20_OR(x, y) x
    #define UH_LEAST_CPP23_OR(x, y) x
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
    #define UH_SUPPORTS_CPP14 1
    #define UH_SUPPORTS_CPP17 1
    #define UH_SUPPORTS_CPP20 1
    #define UH_SUPPORTS_CPP23 1
// C++ >= 11/14/17/20~
#elif UH_CPP_STANDARD > UH_CPP17_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x) x
    #define UH_LEAST_CPP17(x) x
    #define UH_LEAST_CPP20(x) x
    #define UH_LEAST_CPP23(x)
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) x
    #define UH_LEAST_CPP17_OR(x, y) x
    #define UH_LEAST_CPP20_OR(x, y) x
    #define UH_LEAST_CPP23_OR(x, y) y
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
    #define UH_SUPPORTS_CPP14 1
    #define UH_SUPPORTS_CPP17 1
    #define UH_SUPPORTS_CPP20 1
// C++ >= 11/14/17~
#elif UH_CPP_STANDARD > UH_CPP14_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x) x
    #define UH_LEAST_CPP17(x) x
    #define UH_LEAST_CPP20(x)
    #define UH_LEAST_CPP23(x)
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) x
    #define UH_LEAST_CPP17_OR(x, y) x
    #define UH_LEAST_CPP20_OR(x, y) y
    #define UH_LEAST_CPP23_OR(x, y) y
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
    #define UH_SUPPORTS_CPP14 1
    #define UH_SUPPORTS_CPP17 1
// C++ >= 11/14~
#elif UH_CPP_STANDARD > UH_CPP11_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x) x
    #define UH_LEAST_CPP17(x)
    #define UH_LEAST_CPP20(x)
    #define UH_LEAST_CPP23(x)
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) x
    #define UH_LEAST_CPP17_OR(x, y) y
    #define UH_LEAST_CPP20_OR(x, y) y
    #define UH_LEAST_CPP23_OR(x, y) y
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
    #define UH_SUPPORTS_CPP14 1
// C++ >= 11~
#elif UH_CPP_STANDARD > UH_CPP98_STANDARD
    #define UH_LEAST_CPP98(x) x
    #define UH_LEAST_CPP11(x) x
    #define UH_LEAST_CPP14(x)
    #define UH_LEAST_CPP17(x)
    #define UH_LEAST_CPP20(x)
    #define UH_LEAST_CPP23(x)
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) x
    #define UH_LEAST_CPP11_OR(x, y) x
    #define UH_LEAST_CPP14_OR(x, y) y
    #define UH_LEAST_CPP17_OR(x, y) y
    #define UH_LEAST_CPP20_OR(x, y) y
    #define UH_LEAST_CPP23_OR(x, y) y
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
    #define UH_SUPPORTS_CPP11 1
// C++ >= 98~
#elif UH_CPP_STANDARD >= UH_CPP98_STANDARD
    #define UH_LEAST_CPP98(x)
    #define UH_LEAST_CPP11(x)
    #define UH_LEAST_CPP14(x)
    #define UH_LEAST_CPP17(x)
    #define UH_LEAST_CPP20(x)
    #define UH_LEAST_CPP23(x)
    #define UH_LEAST_CPP26(x)
    #define UH_LEAST_CPP98_OR(x, y) y
    #define UH_LEAST_CPP11_OR(x, y) y
    #define UH_LEAST_CPP14_OR(x, y) y
    #define UH_LEAST_CPP17_OR(x, y) y
    #define UH_LEAST_CPP20_OR(x, y) y
    #define UH_LEAST_CPP23_OR(x, y) y
    #define UH_LEAST_CPP26_OR(x, y) y
    #define UH_SUPPORTS_CPP98 1
// C++ < 98 (realistically, can't go lower than this!)
#elif defined(__cplusplus)
    #error Unknown or unsupported C++ standard.
#endif

// Select the expression according to the available c++ standard.
#define UH_CPP98(x) UH_LEAST_CPP98(x)
#define UH_CPP98_OR(x, y) UH_CPP98_ONLY(x) UH_EXCEPT_CPP98(x)
#define UH_CPP11(x) UH_LEAST_CPP11(x)
#define UH_CPP11_OR(x, y) UH_CPP11_ONLY(x) UH_EXCEPT_CPP11(x)
#define UH_CPP14(x) UH_LEAST_CPP14(x)
#define UH_CPP14_OR(x, y) UH_CPP14_ONLY(x) UH_EXCEPT_CPP14(x)
#define UH_CPP17(x) UH_LEAST_CPP17(x)
#define UH_CPP17_OR(x, y) UH_CPP17_ONLY(x) UH_EXCEPT_CPP17(x)
#define UH_CPP20(x) UH_LEAST_CPP20(x)
#define UH_CPP20_OR(x, y) UH_CPP20_ONLY(x) UH_EXCEPT_CPP20(x)
#define UH_CPP23(x) UH_LEAST_CPP23(x)
#define UH_CPP23_OR(x, y) UH_CPP23_ONLY(x) UH_EXCEPT_CPP23(x)
#define UH_CPP26(x) UH_LEAST_CPP26(x)
#define UH_CPP26_OR(x, y) UH_CPP26_ONLY(x) UH_EXCEPT_CPP26(x)

/*
 * Pointer size identification.
*/

#if defined(UH_GNUC) || defined(UH_CLANG)
    // GNUC compatible compilers should have this nice built-in
    #define UH_POINTER_BYTES __SIZEOF_POINTER__
    #define UH_POINTER_SIZE __SIZEOF_POINTER__
    // We can probably avoid having to use an expression
    #ifdef __INTPTR_WIDTH__
        #define UH_POINTER_BITS __INTPTR_WIDTH__
    #elif __SIZE_WIDTH__
        #define UH_POINTER_BITS __SIZE_WIDTH__
    // If all else fails, use an expression. NOTE: We may not be able to use macro concatenation and expect the same result
    #else
        #define UH_POINTER_BITS (__SIZEOF_POINTER__ * 8)
    #endif
#elif defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) || defined(__aarch64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64)
    // Got it through means of architecture identification macros
    #define UH_POINTER_BITS 64
    #define UH_POINTER_BYTES 8
    #define UH_POINTER_SIZE 8
#else
    // We can assume this is 32-bit
    #define UH_POINTER_BITS 32
    #define UH_POINTER_BYTES 4
    #define UH_POINTER_SIZE 4
#endif

#if (UH_POINTER_SIZE == 8)
    #define UH64
    #define UH64_ONLY(VALUE) VALUE
    #define UH32_ONLY(VALUE)
#else
    #define UH32
    #define UH64_ONLY(VALUE)
    #define UH32_ONLY(VALUE) VALUE
#endif

// Select the value according to the current pointer size.
#define UH_ARCH_SELECT(X32V, X64V) UH32_ONLY(X32V) UH64_ONLY(X64V)

/*
 * Platform identification.
*/

#if defined(_WIN64) || defined(_WIN32)
    #define UH_WINDOWS 1
    #define UH_PLATFORM_WINDOWS 1
    #define UH_PLATFORM_DESKTOP 1
    #define UH_PLATFORM_MICROSOFT 1
    #define UH_PLATFORM_NAME Windows
    #define UH_PLATFORM_TEXT "Windows"
    // Both _WIN32 and _WIN64 are defined when compiling for Win-64
    #ifdef _WIN64
        #define UH_WIN64 1
    #else
        #define UH_WIN32 1
    #endif
    // When MinGW is running under windows
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define UH_PLATFORM_MINGW 1
    #endif
    // Identify other platform traits
    #if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
        #define UH_X86_64 1
    #elif defined(_M_IX86) || defined(_X86_) || defined(__i386__)
        #define UH_X86 1
    #elif defined(_M_ARM)
        #define UH_ARM32 1
    #elif defined(_M_ARM64)
        #define UH_ARM64 1
    #else
        #error Unknown processor
    #endif
    // Unicode Windows API
    #if defined(_UNICODE) || defined(UNICODE)
        #define UH_WINDOWS_UNICODE
    #else
        #undef UH_WINDOWS_UNICODE
    #endif
#elif defined(__ANDROID__)
    #define UH_ANDROID 1
    #define UH_PLATFORM_ANDROID 1
    #define UH_PLATFORM_MOBILE 1
    #define UH_PLATFORM_LINUX 1
    #define UH_PLATFORM_POSIX 1
    #define UH_PLATFORM_UNIX 1
    #define UH_PLATFORM_NAME Android
    #define UH_PLATFORM_TEXT "Android"
    // Identify other platform traits
    #if defined(__arm__)
        #define UH_ARM32 1
    #elif defined(__aarch64__)
        #define UH_ARM64 1
    #elif defined(__i386__)
        #define UH_X86 1
    #elif defined(__x86_64)
        #define UH_X86_64 1
    #else
        #error Unknown architecture
    #endif
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #define UH_PLATFORM_APPLE 1
    #define UH_PLATFORM_POSIX 1
    // Is this mobile?
    #if defined(__IPHONE__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
        #define UH_IOS 1
        #define UH_PLATFORM_IPHONE 1
        #define UH_PLATFORM_MOBILE 1
        #define UH_PLATFORM_NAME iPhone
        #define UH_PLATFORM_TEXT "iPhone"
        // Identify other platform traits
        #if defined(__arm__)
            #define UH_ARM32 1
        #elif defined(__aarch64__) || defined(__AARCH64)
            #define UH_ARM64 1
        #elif defined(__i386__)
            #define UH_X86 1
        #elif defined(__x86_64) || defined(__amd64)
            #define UH_X86_64 1
        #else
            #error Unknown architecture
        #endif
    // Is this desktop?
    #elif defined(__MACH__)
        #define UH_OSX 1
        #define UH_PLATFORM_OSX 1
        #define UH_PLATFORM_DESKTOP 1
        #define UH_PLATFORM_UNIX 1
        #define UH_PLATFORM_NAME OSX
        #define UH_PLATFORM_TEXT "OSX"
        // Identify other platform traits
        #if defined(__i386__) || defined(__intel__)
            #define UH_X86 1
        #elif defined(__x86_64) || defined(__amd64)
            #define UH_X86_64 1
        #elif defined(__arm__)
            #define UH_ARM32 1
        #elif defined(__aarch64__) || defined(__AARCH64)
            #define UH_ARM64 1
        #else
            #error Unknown architecture
        #endif
    #endif
#elif defined(__linux__)
    #define UH_LINUX 1
    #define UH_PLATFORM_LINUX 1
    #define UH_PLATFORM_DESKTOP 1
    #define UH_PLATFORM_POSIX 1
    #define UH_PLATFORM_UNIX 1
    #define UH_PLATFORM_NAME Linux
    #define UH_PLATFORM_TEXT "Linux"
    // Identify other platform traits
    #if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
        #define UH_X86 1
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
        #define UH_ARM32 1
    #elif defined(__aarch64__) || defined(__AARCH64)
        #define UH_ARM64 1
    #elif defined(__x86_64__)
        #define UH_X86_64 1
    #else
        #error Unknown architecture
    #endif
#else
    // Must be implemented and tested first
    #error This platform is not implemented
#endif

// Default to little-endian unless explicitly marked otherwise during platform detection
#if !defined(UH_BIG_ENDIAN) && !defined(UH_LITTLE_ENDIAN)
    #define UH_LITTLE_ENDIAN 1
#endif

/*
 * Value selection depending on the platform.
*/

#if defined(UH_WINDOWS)
    #define UH_WINDOWS_ONLY(VALUE) VALUE
    #define UH_ANDROID_ONLY(VALUE)
    #define UH_LINUX_ONLY(VALUE)
    #define UH_IOS_ONLY(VALUE)
    #define UH_OSX_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS(VALUE)
    #define UH_EXCEPT_ANDROID(VALUE) VALUE
    #define UH_EXCEPT_LINUX(VALUE) VALUE
    #define UH_EXCEPT_IOS(VALUE) VALUE
    #define UH_EXCEPT_OSX(VALUE) VALUE
#elif defined(UH_ANDROID)
    #define UH_WINDOWS_ONLY(VALUE)
    #define UH_ANDROID_ONLY(VALUE) VALUE
    #define UH_LINUX_ONLY(VALUE)
    #define UH_IOS_ONLY(VALUE)
    #define UH_OSX_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS(VALUE) VALUE
    #define UH_EXCEPT_ANDROID(VALUE)
    #define UH_EXCEPT_LINUX(VALUE) VALUE
    #define UH_EXCEPT_IOS(VALUE) VALUE
    #define UH_EXCEPT_OSX(VALUE) VALUE
#elif defined(UH_LINUX)
    #define UH_WINDOWS_ONLY(VALUE)
    #define UH_ANDROID_ONLY(VALUE)
    #define UH_LINUX_ONLY(VALUE) VALUE
    #define UH_IOS_ONLY(VALUE)
    #define UH_OSX_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS(VALUE) VALUE
    #define UH_EXCEPT_ANDROID(VALUE) VALUE
    #define UH_EXCEPT_LINUX(VALUE)
    #define UH_EXCEPT_IOS(VALUE) VALUE
    #define UH_EXCEPT_OSX(VALUE) VALUE
#elif defined(UH_IOS)
    #define UH_WINDOWS_ONLY(VALUE)
    #define UH_ANDROID_ONLY(VALUE)
    #define UH_LINUX_ONLY(VALUE)
    #define UH_IOS_ONLY(VALUE) VALUE
    #define UH_OSX_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS(VALUE) VALUE
    #define UH_EXCEPT_ANDROID(VALUE) VALUE
    #define UH_EXCEPT_LINUX(VALUE) VALUE
    #define UH_EXCEPT_IOS(VALUE)
    #define UH_EXCEPT_OSX(VALUE) VALUE
#elif defined(UH_OSX)
    #define UH_WINDOWS_ONLY(VALUE)
    #define UH_ANDROID_ONLY(VALUE)
    #define UH_LINUX_ONLY(VALUE)
    #define UH_IOS_ONLY(VALUE)
    #define UH_OSX_ONLY(VALUE) VALUE
    #define UH_EXCEPT_WINDOWS(VALUE) VALUE
    #define UH_EXCEPT_ANDROID(VALUE) VALUE
    #define UH_EXCEPT_LINUX(VALUE) VALUE
    #define UH_EXCEPT_IOS(VALUE) VALUE
    #define UH_EXCEPT_OSX(VALUE)
#else
    #define UH_WINDOWS_ONLY(VALUE)
    #define UH_ANDROID_ONLY(VALUE)
    #define UH_LINUX_ONLY(VALUE)
    #define UH_IOS_ONLY(VALUE)
    #define UH_OSX_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS(VALUE) VALUE
    #define UH_EXCEPT_ANDROID(VALUE) VALUE
    #define UH_EXCEPT_LINUX(VALUE) VALUE
    #define UH_EXCEPT_IOS(VALUE) VALUE
    #define UH_EXCEPT_OSX(VALUE) VALUE
#endif

// Select the value according to the current platform.
#define UH_WINDOWS_OR(WV, OV) UH_WINDOWS_ONLY(WV) UH_EXCEPT_WINDOWS(OV)
#define UH_ANDROID_OR(AV, OV) UH_ANDROID_ONLY(AV) UH_EXCEPT_ANDROID(OV)
#define UH_LINUX_OR(LV, OV) UH_LINUX_ONLY(LV) UH_EXCEPT_LINUX(OV)
#define UH_IOS_OR(IV, OV) UH_IOS_ONLY(IV) UH_EXCEPT_IOS(OV)
#define UH_OSX_OR(MV, OV) UH_OSX_ONLY(MV) UH_EXCEPT_OSX(OV)

/*
 * Windows unicode API.
*/
#ifdef UH_WINDOWS_UNICODE
    #define UH_WINDOWS_UNICODE_ONLY(VALUE) VALUE
    #define UH_EXCEPT_WINDOWS_UNICODE(VALUE)
    #define UH_WAPIS(S) UH_CONCAT_(L, S)
#else
    #define UH_WINDOWS_UNICODE_ONLY(VALUE)
    #define UH_EXCEPT_WINDOWS_UNICODE(VALUE) VALUE
    #define UH_WAPIS(S) S
#endif

// Select the value according to the current unicode API status.
#define UH_WINDOWS_UNICODE_OR(WV, AV) UH_WINDOWS_UNICODE_ONLY(WV) UH_EXCEPT_WINDOWS_UNICODE(AV)

/*
 * Symbol exporting macros.
*/

#ifdef UH_WINDOWS
    // Deal with some MSC specific issues
    #if defined(UH_MSC) && !defined(UH_CLANG)
        #pragma warning(disable: 4251)
        #pragma warning(disable: 4275)
    #endif
    #define UH_EXPORT_API __declspec(dllexport)
    #if defined(UH_STATIC) || defined(URHO3D_STATIC)
        #define UH_API
        #define UH_NO_EXPORT
    #else
        #ifndef UH_API
            #if defined(UH_EXPORTS) || defined(URHO3D_EXPORTS)
                /* We are building this library */
                #define UH_API UH_EXPORT_API
            #else
                /* We are using this library */
                #define UH_API __declspec(dllimport)
            #endif
        #endif

        #ifndef UH_NO_EXPORT
            #define UH_NO_EXPORT
        #endif
    #endif
#else
    #define UH_EXPORT_API __attribute__((visibility("default")))
    #if defined(UH_STATIC) || defined(URHO3D_STATIC)
        #ifndef UH_API
            #define UH_API
        #endif
        #define UH_NO_EXPORT
    #else
        #define UH_API UH_EXPORT_API
        #ifndef UH_NO_EXPORT
            #define UH_NO_EXPORT __attribute__((visibility("hidden")))
        #endif
    #endif
#endif

// Legacy compatibility
#ifndef URHO3D_API
    #define URHO3D_API UH_API
#endif

// GNUC built-ins matching and non-matching selection macros
#if defined(UH_GNUC_BUILTINS) || (defined(UH_GNUC_COMPAT) && !defined(UH_NO_GNUC_BUILTINS))
    #define UH_GNUC_BUILTIN_ONLY(VALUE) VALUE
    #define UH_EXCEPT_GNUC_BUILTIN(VALUE)
#else
    #define UH_GNUC_BUILTIN_ONLY(VALUE)
    #define UH_EXCEPT_GNUC_BUILTIN(VALUE) VALUE
#endif

// MSC built-ins matching and non-matching selection macros
#if defined(UH_MSC_BUILTINS) || (defined(UH_MSC_COMPAT) && !defined(UH_NO_MSC_BUILTINS))
    #define UH_MSC_BUILTIN_ONLY(VALUE) VALUE
    #define UH_EXCEPT_MSC_BUILTIN(VALUE)
#else
    #define UH_MSC_BUILTIN_ONLY(VALUE)
    #define UH_EXCEPT_MSC_BUILTIN(VALUE) VALUE
#endif

// Select the value according to the available selected option.
#define UH_GNUC_BUILTIN_OR(GV, OV) UH_GNUC_BUILTIN_ONLY(GV) UH_EXCEPT_GNUC_BUILTIN(OV)
#define UH_MSC_BUILTIN_OR(MV, OV) UH_MSC_BUILTIN_ONLY(MV) UH_EXCEPT_MSC_BUILTIN(OV)

/*
 * Branch prediction optimizations.
*/

#if defined(UH_GNUC) || defined(UH_CLANG)
    #define UH_EXPECT(expr, val) __builtin_expect(expr, val)
#else
    #define UH_EXPECT(expr, val) (expr)
#endif

// Branch prediction information.
#define UH_LIKELY(expr) UH_EXPECT(expr, 1)
#define UH_UNLIKELY(expr) UH_EXPECT(expr, 0)

/*
 * Number of bytes a C standard `long` type occupies.
*/

#ifdef __SIZEOF_LONG__
    #define UH_SIZEOF_LONG __SIZEOF_LONG__
// Try other common macros
#elif defined(__LP64__) || defined(_LP64)
    #define UH_SIZEOF_LONG 8
// On Windows this is always 4-bytes (at least for now)
#elif defined(UH_WINDOWS)
    #define UH_SIZEOF_LONG 4
// On most other platforms, this is similar to s/size_t
#elif (UH_POINTER_SIZE == 8)
    #define UH_SIZEOF_LONG 8
// Generic fall-back of commonly expected values
#else
    #define UH_SIZEOF_LONG 4
    #warning Cannot identify size of long type. Using fall-back value of 4
#endif

/*
 * Long size identification and selection macros.
*/
#if UH_SIZEOF_LONG > 4
    #define UH_LONG_64
    #define UH_LONG_SIZE 8
    #define UH_LONG_SELECT(X32, X64) X64
#else
    #define UH_LONG_32
    #define UH_LONG_SIZE 4
    #define UH_LONG_SELECT(X32, X64) X32
#endif

/*
 * Number of bytes a C standard `wchar_t` type occupies.
*/
#ifdef __SIZEOF_WCHAR_T__
    #define UH_SIZEOF_WCHAR_T __SIZEOF_WCHAR_T__
// On Windows, this is 16-bits with either MSVC or MinGW.
// This shouldn't be reached with MinGW as it'll have the above macros available.
#elif defined(UH_WINDOWS)
    #define UH_SIZEOF_WCHAR_T 2
// On Linux and other UNIX style platforms this is 32-bits (so far).
// This shouldn't be reached with supported compilers. Added for sanity.
#elif defined(UH_LINUX) || defined(UH_MACOS) || defined(UH_UNIX)
    #define UH_SIZEOF_WCHAR_T 4
// Generic fall-back of commonly expected values
#else
    #define UH_SIZEOF_WCHAR_T 2
#endif

/*
 * Whether `char` is unsigned.
*/
#if defined(__CHAR_UNSIGNED__) || (CHAR_MIN == 0)
    #define UH_CHAR_UNSIGNED
    #define UH_CHAR_SIGN_SELECT(SC, UC) (UC)
#else
    #define UH_CHAR_SIGN_SELECT(SC, UC) (SC)
#endif
/** Whether `wchar_t` is unsigned. */
#if defined(__WCHAR_UNSIGNED__) || defined(UH_MSC) // I don't want to include wchar.h so I can use (WCHAR_MIN == 0) for MSC
    #define UH_WCHAR_UNSIGNED
    #define UH_WCHAR_SIGN_SELECT(SC, UC) (UC)
#else
    #define UH_WCHAR_SIGN_SELECT(SC, UC) (SC)
#endif

/*
 * char8_t integral value availability and information.
*/
#ifdef __CHAR8_TYPE__
    #define UH_HAS_CHAR8_TYPE
    #define UH_CHAR8_TYPE char8_t
    #define UH_CHAR8_ONLY(VALUE) VALUE
    #define UH_EXCEPT_CHAR8(VALUE)
    #define UH_CHAR8_SELECT(CV, OV) CV
#else // Fall-back (NOTE: take into consideration when doing partial specializations alongside unsigned char)
    #undef UH_HAS_CHAR8_TYPE
    #define UH_CHAR8_TYPE unsigned char
    #define UH_CHAR8_ONLY(VALUE)
    #define UH_EXCEPT_CHAR8(VALUE) VALUE
    #define UH_CHAR8_SELECT(CV, OV) OV
#endif

// GNUC attribute support matching and non-matching selection macros
// i.e.: __attribute__((...))
#if defined(UH_GNUC) || defined(UH_CLANG)
    #define UH_HAS_GNUC_ATTR 1
    #define UH_GNUC_ATTR(VALUE) VALUE
    #define UH_GNUC_ATTR_ONLY(VALUE) VALUE
    #define UH_EXCEPT_GNUC_ATTR(VALUE)
#else
    #define UH_GNUC_ATTR(VALUE)
    #define UH_GNUC_ATTR_ONLY(VALUE)
    #define UH_EXCEPT_GNUC_ATTR(VALUE) VALUE
#endif

// Select the value according to the available GNUC attribute support.
#define UH_GNUC_ATTR_OR(AV, OV) UH_GNUC_ATTR_ONLY(AV) UH_EXCEPT_GNUC_ATTR(OV)

/*
 * Debug or release mode code selection.
*/

#ifdef UH_IS_DEBUG
    #define UH_DEBUG_ONLY(VALUE) VALUE
    #define UH_EXCEPT_DEBUG(VALUE)
    #define UH_RELEASE_ONLY(VALUE)
    #define UH_EXCEPT_RELEASE(VALUE) VALUE
#else
    #define UH_DEBUG_ONLY(VALUE)
    #define UH_EXCEPT_DEBUG(VALUE) VALUE
    #define UH_RELEASE_ONLY(VALUE) VALUE
    #define UH_EXCEPT_RELEASE(VALUE)
#endif

// Select the value according to the current build mode.
#define UH_DEBUG_OR(DV, OV) UH_DEBUG_ONLY(DV) UH_EXCEPT_DEBUG(OV)
#define UH_RELEASE_OR(RV, OV) UH_RELEASE_ONLY(RV) UH_EXCEPT_RELEASE(OV)

/*
 * Inline helpers.
*/

#if defined(UH_MSC)
    #define UH_INLINE __forceinline
    #define UH_NOINLINE
#elif defined(UH_GNUC) || defined(UH_CLANG)
    #define UH_INLINE __attribute__((always_inline))
    #define UH_NOINLINE __attribute__((noinline))
#else
    #define UH_INLINE inline
    #define UH_NOINLINE
#endif

/*
 * Cache line alignment. A reasonable default guess based on the supported architectures.
 *
 * Note: Overestimates tend to waste more space, while underestimates tend to waste more time.
 * ARM: These values are not strictly correct since cache line sizes depend on implementations, not architectures.
 *      There are even implementations with cache line sizes configurable at boot time.
*/

#ifndef UH_CACHELINE_SIZE
    #if   defined(UH_X86)
        // This is the minimum possible value.
        #define UH_CACHELINE_SIZE 32
    #elif defined(UH_X86_64)
        // This is the minimum possible value
        #define UH_CACHELINE_SIZE 64
    #elif defined(UH_ARM32)
        // This varies between implementations and is usually 32 or 64.
        #define UH_CACHELINE_SIZE 32
    #elif defined(UH_ARM64)
        // Cache line Cortex-A8  (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
        #define UH_CACHELINE_SIZE 64
    #elif (UH_POINTER_SIZE == 4)
        // This is the minimum possible value
        #define UH_CACHELINE_SIZE 32
    #else
        // This is the minimum possible value
        #define UH_CACHELINE_SIZE 64
    #endif
#endif

/*
 * is_constant_evaluated feature.
*/

// Can it be detected via __has_builtin method?
#if defined(__has_builtin)
    #if __has_builtin(__builtin_is_constant_evaluated)
        #define UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
    #endif
// GCC and Clang introduced this in version 9
#elif (defined(UH_GNUC) || defined(UH_CLANG)) && (UH_COMPILER_MAJOR >= 9)
    #define UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
// MSVC seems to have added this in version 19.25
#elif defined(UH_MSC) && (UH_COMPILER_MAJOR >= 19) && (UH_COMPILER_MINOR >= 25)
    #define UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED 1
#else
    #undef UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED
#endif

// This functionality plays an important role, so we need to be able to switch code when not available.
#ifdef UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED
    #define UH_BUILTIN_IS_CONSTANT_EVALUATED_ONLY(VALUE) VALUE
    #define UH_EXCEPT_BUILTIN_IS_CONSTANT_EVALUATED(VALUE)
#else
    #define UH_BUILTIN_IS_CONSTANT_EVALUATED_ONLY(VALUE)
    #define UH_EXCEPT_BUILTIN_IS_CONSTANT_EVALUATED(VALUE) VALUE
#endif

// Select the value according to the availability of __builtin_is_constant_evaluated.
#define UH_BUILTIN_IS_CONSTANT_EVALUATED_OR(CV, OV) UH_BUILTIN_IS_CONSTANT_EVALUATED_ONLY(CV) UH_EXCEPT_BUILTIN_IS_CONSTANT_EVALUATED(OV)

/*
 * Constant detection feature.
*/

// Can it be detected via __has_builtin method?
#if defined(__has_builtin)
    #if __has_builtin(__builtin_constant_p)
        #define UH_HAVE_BUILTIN_CONSTANT_P 1
    #endif
// GCC and Clang should already have this
#elif defined(UH_GNUC) || defined(UH_CLANG)
    #define UH_HAVE_BUILTIN_CONSTANT_P 1
// R.I.P. for MSVC and the rest
#else
    #undef UH_HAVE_BUILTIN_CONSTANT_P
#endif

/*
 * Restricted pointers.
*/

#ifdef UH_GNUC_COMPAT
    #ifdef UH_MINGW
        #define UH_RESTRICT __restrict
    #else
        #define UH_RESTRICT __restrict__
    #endif
    #define UH_RESTRICT_ONLY(VALUE) VALUE
    #define UH_EXCEPT_RESTRICT(VALUE)
#elif defined(US_MSC)
    #define UH_RESTRICT __restrict
    #define UH_RESTRICT_ONLY(VALUE) VALUE
    #define UH_EXCEPT_RESTRICT(VALUE)
#else
    #define UH_RESTRICT
    #define UH_RESTRICT_ONLY(VALUE)
    #define UH_EXCEPT_RESTRICT(VALUE) VALUE
#endif

// Select the value according to the availability of __restrict__ or __restrict.
#define UH_RESTRICT_OR(RV, OV) UH_RESTRICT_ONLY(RV) UH_EXCEPT_RESTRICT(OV)

/*
 * [[msvc::intrinsic]] attribute support.
*/
#if defined(UH_MSC) && (UH_MSC >= 1935)
    #define UH_MSVC_INTRINSIC [[msvc::intrinsic]]
    #define UH_HAVE_MSVC_INTRINSIC 1
#else
    #define UH_MSVC_INTRINSIC
#endif
