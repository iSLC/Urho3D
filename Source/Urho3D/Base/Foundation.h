//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include <Urho3D/Urho3D.h>

namespace Urho3D
{

// Allow literals to be imported into any namespace without importing the base namespace.
namespace Literals { }
// Import literals into base namespace by default
using namespace Literals;

/// Disambiguation tag that can be passed as argument to indicate that initialization is not necessary.
struct NoInit_t { } inline constexpr NoInit{ };
/// Disambiguation tag that can be passed as argument to indicate that no references should be stored.
struct NoRefs_t { } inline constexpr NoRefs{ };
/// Disambiguation tag that can be passed as argument to indicate that default initialization is not necessary after releasing something.
struct NoTidy_t { } inline constexpr NoTidy{ };
/// Disambiguation tag that can be passed as argument to indicate that free/release of something (i.e. memory) is not necessary.
struct NoFree_t { } inline constexpr NoFree{ };
/// Disambiguation tag that can be passed as argument to indicate initialization is actually necessary.
struct DoInit_t { } inline constexpr DoInit{ };
/// Disambiguation tag that can be passed as argument to indicate that a transfer of something should be made instead of replicating it.
struct DoMove_t { } inline constexpr DoMove{ };
/// Disambiguation tag that can be passed as argument to indicate that a replica of something should be made instead of owning it.
struct DoCopy_t { } inline constexpr DoCopy{ };
/// Disambiguation tag that can be passed as argument to indicate that sanity checks should not be performed as they were most likely performed already.
struct IsPure_t { } inline constexpr IsPure{ };
/// Disambiguation tag that can be passed as argument to indicate that case insensitivity should be preferred.
struct NoCase_t { } inline constexpr NoCase{ };
/// Disambiguation tag that can be passed as argument to indicate that something is static ...
struct Static_t { } inline constexpr Static{ };
/// Disambiguation tag that can be passed as argument to indicate that ownership should be taken of something.
struct OwnIt_t { } inline constexpr OwnIt{ };

/// Disambiguation tag that can be passed as argument to indicate that increment should be performed.
struct DoIncrement_t { } inline constexpr DoIncrement{ };
/// Disambiguation tag that can be passed as argument to indicate that decrement should be performed.
struct DoDecrement_t { } inline constexpr DoDecrement{ };
/// Disambiguation tag that can be passed as argument to indicate that pre-increment should be performed.
struct DoPreInc_t { } inline constexpr DoPreInc{ };
/// Disambiguation tag that can be passed as argument to indicate that pre-decrement should be performed.
struct DoPreDec_t { } inline constexpr DoPreDec{ };
/// Disambiguation tag that can be passed as argument to indicate that post-increment should be performed.
struct DoPostInc_t { } inline constexpr DoPostInc{ };
/// Disambiguation tag that can be passed as argument to indicate that post-decrement should be performed.
struct DoPostDec_t { } inline constexpr DoPostDec{ };

/// Helper used to store a NULL terminated C style string associated with a type that represents it's name.
template < class T > struct BasicTypeName {
    static constexpr inline const char * value = nullptr; ///< The name_ is unknown (NULL).
};

/// Helper macro used to define the `BasicTypeName` specialization for a given type and the name_ is taken implicitly.
#define UH_BASIC_TYPE_NAME(type) template < > struct BasicTypeName< type > { /*
*/  static constexpr inline const char * value = #type;  /*
*/ }; /*
*/
/// Helper macro used to define the `BasicTypeName` specialization for a given type and the name_ is given explicitly.
#define UH_BASIC_TYPE_NAME_AS(type, name) template < > struct BasicTypeName< type > { /*
*/  static constexpr inline const char * value = name;  /*
*/ }; /*
*/

UH_BASIC_TYPE_NAME_AS(bool, "bool")
UH_BASIC_TYPE_NAME_AS(char, "char")
UH_BASIC_TYPE_NAME_AS(signed char, "signed char")
UH_BASIC_TYPE_NAME_AS(unsigned char, "unsigned char")
UH_BASIC_TYPE_NAME_AS(signed short, "signed short")
UH_BASIC_TYPE_NAME_AS(unsigned short, "unsigned short")
UH_BASIC_TYPE_NAME_AS(signed int, "signed int")
UH_BASIC_TYPE_NAME_AS(unsigned int, "unsigned int")
UH_BASIC_TYPE_NAME_AS(signed long, "signed long")
UH_BASIC_TYPE_NAME_AS(unsigned long, "unsigned long")
UH_BASIC_TYPE_NAME_AS(signed long long, "signed long long")
UH_BASIC_TYPE_NAME_AS(unsigned long long, "unsigned long long")
UH_BASIC_TYPE_NAME_AS(float, "float")
UH_BASIC_TYPE_NAME_AS(double, "double")

/// Disambiguation tag that can be passed as argument to indicate that the formatting should be constructed in-place.
struct InPlaceFormat_t {
    explicit InPlaceFormat_t() = default;
} inline constexpr InPlaceFormat{ };

/// Disambiguation tag that can be passed as argument to indicate that the contained object should be constructed in-place.
struct InPlace_t {
    explicit InPlace_t() = default;
} inline constexpr InPlace{ };

/// Used in the constructor's or functions parameter list to match the %InPlaceType tag.
template< class > struct InPlaceType_t {
    explicit InPlaceType_t() = default;
};
/// Disambiguation tag that can be passed as argument to indicate that the contained object should be constructed in-place, including its type.
template < class T > inline constexpr InPlaceType_t< T > InPlaceType{};

/// Used in the constructor's or functions parameter list to match the %InPlaceIndex tag.
template < size_t > struct InPlaceIndex_t {
    explicit InPlaceIndex_t() = default;
};

/// Disambiguation tag that can be passed as argument to indicate that the contained object should be constructed in-place, including its position.
template < size_t I > inline constexpr InPlaceIndex_t< I > InPlaceIndex{};

/// Helper function to retrieve the size of an array of types in therms of bytes.
template < class T > UH_INLINE constexpr size_t ByteSz(size_t len) noexcept { return len * sizeof(T); }

/// Helper function used to constrain a value between a specific range.
template < class T >
[[nodiscard]] constexpr const T & Clamp(const T & val, const T & min, const T & max) noexcept
{
    UH_ASSERT(max >= min)
    return (val <= min) ? min : (max <= val) ? max : val;
}
/// Helper function used to constrain a value between a specific range using a specific functor to perform the comparison.
template < class T, class Compare >
[[nodiscard]] constexpr const T & Clamp(const T & val, const T & min, const T & max, Compare cmp) noexcept
{
    UH_ASSERT(!cmp(max, min))
    return cmp(val, min) ? min : cmp(max, val) ? max : val;
}

/// Helper function used to obtain the smaller of the two given values.
template < class T >
[[nodiscard]] constexpr const T & Min(const T & a, const T & b) noexcept { return b < a ? b : a; }
///  Helper function used to obtain the smaller of the two given values using a specific functor to perform the comparison.
template < class T, class Compare >
[[nodiscard]] constexpr const T & Min(const T & a, const T & b, Compare cmp) noexcept { return cmp(b, a) ? b : a; }

/// Helper function used to obtain the greater of the two given values.
template < class T >
[[nodiscard]] constexpr const T & Max(const T & a, const T & b) noexcept { return a < b ? b : a; }
///  Helper function used to obtain the greater of the two given values using a specific functor to perform the comparison.
template < class T, class Compare >
[[nodiscard]] constexpr const T & Max(const T & a, const T & b, Compare cmp) noexcept { return cmp(a, b) ? b : a; }

/// Check if the specified address is aligned on the boundary specified by \p A.
/// Alignment must be known at compile time and shall be a power of two.
template < size_t A >
[[nodiscard]] constexpr bool IsAligned(const uintptr_t p) noexcept
{
    static_assert((A & (A - 1)) == 0, "Alignment must be a power of 2");
    return (p & (A - 1)) == 0;
};
/// Check if the specified pointer is aligned on the boundary specified by \p A.
/// Alignment must be known at compile time and shall be a power of two.
template < size_t A, class T >
[[nodiscard]] constexpr bool IsAligned(const T * p) noexcept
{
    static_assert((A & (A - 1)) == 0, "Alignment must be a power of 2");
    return (reinterpret_cast< uintptr_t >(p) & (A - 1)) == 0;
};

/// Detect whether the function call occurs within a constant-evaluated context.
/// Returns true if the evaluation of the call occurs within the evaluation of an expression or conversion that is
/// manifestly constant-evaluated; otherwise returns false. \remark See `std::is_constant_evaluated`
UH_INLINE constexpr bool IsConstantEvaluated() noexcept
{
#ifdef UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED
    return __builtin_is_constant_evaluated();
#else
    static_assert(false);
#endif
}

/// Causes abnormal program termination unless SIGABRT is being caught by a signal handler and the handler does not return.
[[noreturn]] inline void AbortProgram() noexcept;

/// Custom assert handler fired by the UH_ASSERT... macros. It includes the source and location where it 
void AssertHandler(const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * msg, const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * src, unsigned loc);

/*
 * Character array manipulation.
*/

/// Wrapper for: memchr
[[nodiscard]] inline void * MemChr(void * p, int c, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memchr, memchr)(p, c, n); }
/// Wrapper for: memchr
[[nodiscard]] inline const void * MemChr(const void * p, int c, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memchr, memchr)(p, c, n); }
/// Wrapper for: memcmp
[[nodiscard]] inline int MemCmp(const void * a, const void * b, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memcmp, memcmp)(a, b, n); }
/// Wrapper for: memcpy
inline void * MemCpy(void * UH_RESTRICT d, const void * UH_RESTRICT s, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memcpy, memcpy)(d, s, n); }
/// Wrapper for: memmove
inline void * MemMove(void * d, const void * s, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memmove, memmove)(d, s, n); }
/// Wrapper for: memset
inline void * MemSet(void * d, int c, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_memset, memset)(d, c, n); }

/// Specialized wrapper for: memset
template < class T > inline void * MemClear(T & m) noexcept { return MemSet(&m, 0, sizeof(T)); }
/// Wrapper for: memcpy With fall-back to memset
inline size_t MemSCpy(void * d, size_t dn, const void * s, size_t sn) noexcept { return (dn >= sn) ? (MemCpy(d, s, sn), 0) : (MemSet(d, 0, dn), -1); }

/*
 * String manipulation
*/
/// Wrapper for: strcpy
inline char * StrCpy(char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strcpy, strcpy)(d, s); }
/// Wrapper for: strncpy
inline char * StrCpy(char * d, const char * s, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strncpy, strncpy)(d, s, n); }
/// Wrapper for: strcat
inline char * StrCat(char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strcat, strcat)(d, s); }
/// Wrapper for: strncat
inline char * StrCat(char * d, const char * s, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strncat, strncat)(d, s, n); }

/* ====================================================================================================================
 * String examination.
*/
/// Wrapper for: strlen
[[nodiscard]] inline size_t StrLen(const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strlen, strlen)(s); }
/// Wrapper for: strcmp
[[nodiscard]] inline int StrCmp(const char * a, const char * b) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strcmp, strcmp)(a, b); }
/// Wrapper for: strncmp
[[nodiscard]] inline int StrCmp(const char * a, const char * b, size_t n) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strncmp, strncmp)(a, b, n); }
/// Wrapper for: strchr
[[nodiscard]] inline char * StrChr(char * s, int c ) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strchr, strchr)(s, c); }
/// Wrapper for: strchr
[[nodiscard]] inline const char * StrChr(const char* s, int c) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strchr, strchr)(s, c); }
/// Wrapper for: strrchr
[[nodiscard]] inline char * StrRChr(char * s, int c) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strrchr, strrchr)(s, c); }
/// Wrapper for: strrchr
[[nodiscard]] inline const char * StrRChr(const char * s, int c) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strrchr, strrchr)(s, c); }
/// Wrapper for: strspn
[[nodiscard]] inline size_t StrSpn(const char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strspn, strspn)(d, s); }
/// Wrapper for: strcspn
[[nodiscard]] inline size_t StrCSpn(const char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strcspn, strcspn)(d, s); }
/// Wrapper for: strpbrk
[[nodiscard]] inline char * StrPBrk(char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strpbrk, strpbrk)(d, s); }
/// Wrapper for: strpbrk
[[nodiscard]] inline const char * StrPBrk(const char * d, const char * s) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strpbrk, strpbrk)(d, s); }
/// Wrapper for: strstr
[[nodiscard]] inline char * StrStr(char * s, const char * t) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strstr, strstr)(s, t); }
/// Wrapper for: strstr
[[nodiscard]] inline const char * StrStr(const char * s, const char * t) noexcept { return UH_GNUC_COMPAT_OR(__builtin_strstr, strstr)(s, t); }

} // Namespace:: Urho3D
