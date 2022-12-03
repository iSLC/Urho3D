//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Urho3DConfig.h"

namespace Urho3D {

/// Enumeration constants of type \ref FloatRoundStyle indicate the rounding style used by floating-point arithmetics
/// whenever a result of an expression is stored in an object of a floating-point type. This is used in the NumericLimits class.
enum FloatRoundStyle : int // Type is added explicitly so we can forward declare this enum without having to include this header.
{
    RoundIndeterminate      = -1, ///< Rounding style cannot be determined.
    RoundTowardZero         = 0,  ///< Rounding toward zero.
    RoundToNearest          = 1,  ///< Rounding toward nearest representable value.
    RoundTowardInfinity     = 2,  ///< Rounding toward positive infinity.
    RoundTowardNegInfinity  = 3   ///< Rounding toward negative infinity.
};

/// Describes the denormalization for floating-point types. This type is used in the NumericLimits class.
/// These values represent the presence or absence of a variable number of exponent bits.
enum FloatDenormStyle : int // Type is added explicitly so we can forward declare this enum without having to include this header.
{
    DenormIndeterminate = -1,  ///< Indeterminate at compile time whether denormalized values are allowed.
    DenormAbsent        = 0,   ///< The type does not allow denormalized values.
    DenormPresent       = 1    ///< The type allows denormalized values.
};

namespace Impl {

/// Given the number of digits in base-radix that can be represented by a type without change,
/// calculate the number of base-10 digits that can be represented by the type without change.
template < unsigned N > inline constexpr int NumericLimitsIntDigits10 = N * 643 / 2136;

/// Offers the foundation (default values) for numeric limits that are not specialized.
struct BaseNumericLimits
{
    /// Identifies types for which \c NumericLimits is specialized.
    /// This will be true for all fundamental types (which have specializations), and false for everything else.
    static inline constexpr bool IsSpecialized = false;
    /// The number of \c Radix Digits that can be represented without change.
    /// For integer types, the number of non-sign bits in the mantissa; for floating types, the number of \c Radix Digits in the mantissa.
    static inline constexpr int Digits = 0;
    /// The number of base 10 Digits that can be represented without change.
    static inline constexpr int Digits10 = 0;
    /// The number of base 10 Digits required to ensure that values which differ are always differentiated.
    static inline constexpr int MaxDigits10 = 0;
    /// Identifies signed types.
    static inline constexpr bool IsSigned = false;
    /// Identifies integer types.
    static inline constexpr bool IsInteger = false;
    /// Identifies exact types.
    /// All integer types are exact, but not all exact types are integer. For example, rational and fixed-exponent representations are exact but not integer.
    static inline constexpr bool IsExact = false;
    /// The radix or integer base used by the representation of the given type.
    /// For integer types, specifies the base of the representation. For floating types, specifies the base of the exponent representation.
    static inline constexpr int Radix = 0;
    /// One more than the smallest negative power of the radix that is a valid normalized floating-point value.
    static inline constexpr int MinExponent = 0;
    /// The smallest negative power of ten that is a valid normalized floating-point value.
    static inline constexpr int MinExponent10 = 0;
    /// One more than the largest integer power of the radix that is a valid finite floating-point value.
    static inline constexpr int MaxExponent = 0;
    /// The largest integer power of 10 that is a valid finite floating-point value.
    static inline constexpr int MaxExponent10 = 0;
    /// Identifies floating-point types that can represent the special value "positive infinity".
    static inline constexpr bool HasInfinity = false;
    /// Identifies floating-point types that can represent the special value "quiet not-a-number" (NaN).
    static inline constexpr bool HasQuietNaN = false;
    /// Identifies floating-point types that can represent the special value "signaling not-a-number" (NaN).
    static inline constexpr bool HasSignalingNaN = false;
    /// Identifies the denormalization style used by the floating-point type.
    static inline constexpr FloatDenormStyle HasDenorm = DenormAbsent;
    /// Identifies the floating-point types that detect loss of precision as denormalization loss rather than inexact result.
    static inline constexpr bool HasDenormLoss = false;
    /// Identifies the IEC 559/IEEE 754 floating-point types.
    static inline constexpr bool IsIEC559 = false;
    /// Identifies types that represent a finite set of values.
    /// All built-in types are bounded, this member would be false for arbitrary precision types.
    static inline constexpr bool IsBounded = false;
    /// Identifies types that handle overflows with modulo arithmetic.
    /// A type is modulo if, for any operation involving +, -, or * on values of that type whose result would fall outside the range [Min(), Max()],
    /// the value returned differs from the true value by an integer multiple of Max() - Min() + 1.
    /// On most machines, this is false for floating types, true for unsigned integers, and true for signed integers.
    static inline constexpr bool IsModulo = false;
    /// Identifies types which can cause arithmetic operations to trap.
    static inline constexpr bool Traps = false;
    /// Identifies floating-point types that detect tinyness before rounding.
    static inline constexpr bool TinynessBefore = false;
    /// Identifies the rounding style used by the type.
    /// This is only meaningful for floating types; integer types will all be RoundTowardZero.
    static inline constexpr FloatRoundStyle RoundStyle = RoundTowardZero;
};

/// Offers the foundation (default values) for integer numeric limits.
struct BaseIntNumericLimits : public BaseNumericLimits
{
    static inline constexpr bool IsSpecialized = true;
    static inline constexpr bool IsInteger = true;
    static inline constexpr bool IsExact = true;
    static inline constexpr int Radix = 2;
    static inline constexpr bool IsBounded = true;
#ifndef UH_MSC
    static inline constexpr bool Traps = true;
#endif
};

/// Offers the foundation (default values) for floating point numeric limits.
struct BaseFloatNumericLimits : public BaseNumericLimits
{
    static inline constexpr bool IsSpecialized = true;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Radix = FLT_RADIX;
    static inline constexpr bool HasInfinity = true;
    static inline constexpr bool HasQuietNaN = true;
    static inline constexpr bool HasSignalingNaN = true;
    static inline constexpr FloatDenormStyle HasDenorm = DenormPresent;
    static inline constexpr bool IsIEC559 = true;
    static inline constexpr bool IsBounded = true;
    static inline constexpr FloatRoundStyle RoundStyle = RoundToNearest;
};

} // Namespace:: Impl

/// Properties of fundamental types.
/// This class allows a program to obtain information about the representation of a fundamental type on a given platform.
/// For non-fundamental types, the functions will return 0 and the data members will all be \c false.
template < class T > struct NumericLimits : public Impl::BaseNumericLimits
{
    /// Used internally to reference to the specified type.
    using Type = T;
    /// Returns the smallest finite value, or for floating types with denormalization, the minimum positive normalized value.
    [[nodiscard]] static constexpr T Min() noexcept { return T(); }
    /// Returns the largest finite value.
    [[nodiscard]] static constexpr T Max() noexcept { return T(); }
    /// A finite value x such that there is no other finite value y where y < x.
    [[nodiscard]] static constexpr T Lowest() noexcept { return T(); }
    /// Returns the difference between 1.0 and the next representable value of the given floating-point type.
    [[nodiscard]] static constexpr T Epsilon() noexcept { return T(); }
    /// Returns the maximum rounding error of the given floating-point type.
    [[nodiscard]] static constexpr T RoundError() noexcept { return T(); }
    /// Returns the representation of positive infinity, if \c HasInfinity.
    [[nodiscard]] static constexpr T Infinity() noexcept { return T(); }
    /// Returns the representation of a quiet Not a Number, if @c HasQuietNaN.
    [[nodiscard]] static constexpr T QuietNaN() noexcept { return T(); }
    /// Returns the representation of a signaling Not a Number, if @c HasSignalingNaN.
    [[nodiscard]] static constexpr T SignalingNaN() noexcept { return T(); }
    /// Returns the minimum positive denormalized value.  For types where \c HasDenorm is false, this is the minimum positive normalized value.
    [[nodiscard]] static constexpr T DenormMin() noexcept { return T(); }
};

/// Discard const qualifier.
template < class T > struct NumericLimits< const T > : public NumericLimits< T > { };
/// Discard volatile qualifier.
template < class T > struct NumericLimits< volatile T > : public NumericLimits< T > { };
/// Discard const volatile qualifiers.
template < class T > struct NumericLimits< const volatile T > : public NumericLimits< T > { };

/// NumericLimits< bool > specialization.
template < > struct NumericLimits< bool > : public Impl::BaseIntNumericLimits
{
    using Type = bool;
    static inline constexpr int Digits = 1;
    [[nodiscard]] static constexpr bool Min() noexcept { return false; }
    [[nodiscard]] static constexpr bool Max() noexcept { return true; }
    [[nodiscard]] static constexpr bool Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr bool Epsilon() noexcept { return false; }
    [[nodiscard]] static constexpr bool RoundError() noexcept { return false; }
    [[nodiscard]] static constexpr bool Infinity() noexcept { return false; }
    [[nodiscard]] static constexpr bool QuietNaN() noexcept { return false; }
    [[nodiscard]] static constexpr bool SignalingNaN() noexcept { return false; }
    [[nodiscard]] static constexpr bool DenormMin() noexcept { return false; }
};

/// NumericLimits< char > specialization.
template < > struct NumericLimits< char > : public Impl::BaseIntNumericLimits
{
    using Type = char;
    static inline constexpr bool IsSigned = CHAR_MIN != 0;
    static inline constexpr int Digits = sizeof(char) * 8 - UH_CHAR_SIGN_SELECT(1, 0);
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = CHAR_MIN == 0;
    [[nodiscard]] static constexpr char Min() noexcept { return CHAR_MIN; }
    [[nodiscard]] static constexpr char Max() noexcept { return CHAR_MAX; }
    [[nodiscard]] static constexpr char Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr char Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr char Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char DenormMin() noexcept { return 0; }
};

/// NumericLimits< signed char > specialization.
template < > struct NumericLimits< signed char > : public Impl::BaseIntNumericLimits
{
    using Type = signed char;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Digits = sizeof(signed char) * 8 - 1;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    [[nodiscard]] static constexpr signed char Min() noexcept { return SCHAR_MIN; }
    [[nodiscard]] static constexpr signed char Max() noexcept { return SCHAR_MAX; }
    [[nodiscard]] static constexpr signed char Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr signed char Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr signed char DenormMin() noexcept { return 0; }
};

/// NumericLimits< unsigned char > specialization.
template < > struct NumericLimits< unsigned char > : public Impl::BaseIntNumericLimits
{
    using Type = unsigned char;
    static inline constexpr int Digits = sizeof(unsigned char) * 8;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr unsigned char Min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char Max() noexcept { return UCHAR_MAX; }
    [[nodiscard]] static constexpr unsigned char Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr unsigned char Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned char DenormMin() noexcept { return 0; }
};

/// NumericLimits< wchar_t > specialization.
template < > struct NumericLimits< wchar_t > : public Impl::BaseIntNumericLimits
{
    using Type = wchar_t;
    static inline constexpr bool IsSigned = WCHAR_MIN != 0;
    static inline constexpr int Digits = sizeof(wchar_t) * 8 - UH_WCHAR_SIGN_SELECT(1, 0);
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = WCHAR_MIN == 0;
    [[nodiscard]] static constexpr wchar_t Min() noexcept { return WCHAR_MIN; }
    [[nodiscard]] static constexpr wchar_t Max() noexcept { return WCHAR_MAX; }
    [[nodiscard]] static constexpr wchar_t Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr wchar_t Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr wchar_t DenormMin() noexcept { return 0; }
};

#ifdef UH_HAS_CHAR8_TYPE
/// NumericLimits< char8_t > specialization.
template < > struct NumericLimits< char8_t > : public Impl::BaseIntNumericLimits
{
    using Type = char8_t;
    static inline constexpr int Digits = sizeof(char8_t) * 8 - (IsSigned ? 1 : 0);
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr char8_t Min() noexcept { return 0 }
    [[nodiscard]] static constexpr char8_t Max() noexcept { return UCHAR_MAX; }
    [[nodiscard]] static constexpr char8_t Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr char8_t Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char8_t DenormMin() noexcept { return 0; }
};
#endif

/// NumericLimits< char16_t > specialization.
template < > struct NumericLimits< char16_t > : public Impl::BaseIntNumericLimits
{
    using Type = char16_t;
    static inline constexpr int Digits = sizeof(char16_t) * 8 - (IsSigned ? 1 : 0);
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr char16_t Min() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t Max() noexcept { return USHRT_MAX; }
    [[nodiscard]] static constexpr char16_t Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr char16_t Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char16_t DenormMin() noexcept { return 0; }
};

/// NumericLimits< char32_t > specialization.
template < > struct NumericLimits< char32_t > : public Impl::BaseIntNumericLimits
{
    using Type = char32_t;
    static inline constexpr int Digits = sizeof(char32_t) * 8 - (IsSigned ? 1 : 0);
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr char32_t Min() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t Max() noexcept { return UINT_MAX; }
    [[nodiscard]] static constexpr char32_t Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr char32_t Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr char32_t DenormMin() noexcept { return 0; }
};

/// NumericLimits< short > specialization.
template < > struct NumericLimits< short > : public Impl::BaseIntNumericLimits
{
    using Type = short;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Digits = sizeof(short) * 8 - 1;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    [[nodiscard]] static constexpr short Min() noexcept { return SHRT_MIN; }
    [[nodiscard]] static constexpr short Max() noexcept { return SHRT_MAX; }
    [[nodiscard]] static constexpr short Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr short Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr short RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr short Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr short QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr short SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr short DenormMin() noexcept { return 0; }
};

// --------------------------------------------------------------------------------------------------------------------
/// NumericLimits< unsigned short > specialization.
template < > struct NumericLimits< unsigned short > : public Impl::BaseIntNumericLimits
{
    using Type =  unsigned short;
    static inline constexpr int Digits = sizeof(unsigned short) * 8;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr unsigned short Min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short Max() noexcept { return USHRT_MAX; }
    [[nodiscard]] static constexpr unsigned short Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr unsigned short Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned short DenormMin() noexcept { return 0; }
};

/// NumericLimits< int > specialization.
template < > struct NumericLimits< int > : public Impl::BaseIntNumericLimits
{
    using Type = int;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Digits = sizeof(int) * 8 - 1;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    [[nodiscard]] static constexpr int Min() noexcept { return INT_MIN; }
    [[nodiscard]] static constexpr int Max() noexcept { return INT_MAX; }
    [[nodiscard]] static constexpr int Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr int Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr int RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr int Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr int QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr int SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr int DenormMin() noexcept { return 0; }
};

/// NumericLimits< unsigned int > specialization.
template < > struct NumericLimits< unsigned int > : public Impl::BaseIntNumericLimits
{
    using Type = unsigned int;
    static inline constexpr int Digits = sizeof(unsigned int) * 8;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr unsigned int Min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int Max() noexcept { return UINT_MAX; }
    [[nodiscard]] static constexpr unsigned int Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr unsigned int Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned int DenormMin() noexcept { return 0; }
};

/// NumericLimits< long > specialization.
template < > struct NumericLimits< long > : public Impl::BaseIntNumericLimits
{
    using Type = long;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Digits = sizeof(long) * 8 - 1;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    [[nodiscard]] static constexpr long Min() noexcept { return LONG_MIN; }
    [[nodiscard]] static constexpr long Max() noexcept { return LONG_MAX; }
    [[nodiscard]] static constexpr long Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr long Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr long RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr long Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr long QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long DenormMin() noexcept { return 0; }
};

/// NumericLimits< unsigned long > specialization.
template < > struct NumericLimits< unsigned long > : public Impl::BaseIntNumericLimits
{
    using Type = unsigned long;
    static inline constexpr int Digits = sizeof(unsigned long) * 8;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr unsigned long Min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long Max() noexcept { return ULONG_MAX; }
    [[nodiscard]] static constexpr unsigned long Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr unsigned long Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long DenormMin() noexcept { return 0; }
};

/// NumericLimits< long long > specialization.
template < > struct NumericLimits< long long > : public Impl::BaseIntNumericLimits
{
    using Type = long long;
    static inline constexpr bool IsSigned = true;
    static inline constexpr int Digits = sizeof(long long) * 8 - 1;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    [[nodiscard]] static constexpr long long Min() noexcept { return LLONG_MIN; }
    [[nodiscard]] static constexpr long long Max() noexcept { return LLONG_MAX; }
    [[nodiscard]] static constexpr long long Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr long long Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr long long RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr long long Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr long long QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long long SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr long long DenormMin() noexcept { return 0; }
};

/// NumericLimits< unsigned long long > specialization.
template < > struct NumericLimits< unsigned long long > : public Impl::BaseIntNumericLimits
{
    using Type = unsigned long long;
    static inline constexpr int Digits = sizeof(unsigned long long) * 8;
    static inline constexpr int Digits10 = Impl::NumericLimitsIntDigits10< Digits >;
    static inline constexpr bool IsModulo = true;
    [[nodiscard]] static constexpr unsigned long long Min() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long Max() noexcept { return ULLONG_MAX; }
    [[nodiscard]] static constexpr unsigned long long Lowest() noexcept { return Min(); }
    [[nodiscard]] static constexpr unsigned long long Epsilon() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long RoundError() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long Infinity() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long QuietNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long SignalingNaN() noexcept { return 0; }
    [[nodiscard]] static constexpr unsigned long long DenormMin() noexcept { return 0; }
};

/// NumericLimits< float > specialization.
template < > struct NumericLimits< float > : public Impl::BaseFloatNumericLimits
{
    using Type = float;
    static inline constexpr int Digits = FLT_MANT_DIG;
    static inline constexpr int Digits10 = FLT_DIG;
    static inline constexpr int MaxDigits10 = static_cast< int >(2 + FLT_MANT_DIG * 643L / 2136);
    static inline constexpr int MinExponent = FLT_MIN_EXP;
    static inline constexpr int MinExponent10 = FLT_MIN_10_EXP;
    static inline constexpr int MaxExponent = FLT_MAX_EXP;
    static inline constexpr int MaxExponent10 = FLT_MAX_10_EXP;
    static inline constexpr bool HasInfinity = UH_FLT_HAS_INFINITY;
    static inline constexpr bool HasQuietNaN = UH_FLT_HAS_QUIET_NAN;
    static inline constexpr bool HasSignalingNaN = HasQuietNaN;
    static inline constexpr FloatDenormStyle HasDenorm = FLT_HAS_SUBNORM ? DenormPresent : DenormAbsent;
    static inline constexpr bool IsIEC559 = HasInfinity && HasQuietNaN && HasDenorm == DenormPresent;
    static inline constexpr FloatRoundStyle RoundStyle = RoundToNearest;
    [[nodiscard]] static constexpr float Min() noexcept { return FLT_MIN; }
    [[nodiscard]] static constexpr float Max() noexcept { return FLT_MAX; }
    [[nodiscard]] static constexpr float Lowest() noexcept { return -FLT_MAX; }
    [[nodiscard]] static constexpr float Epsilon() noexcept { return FLT_EPSILON; }
    [[nodiscard]] static constexpr float RoundError() noexcept { return 0.5F; }
    [[nodiscard]] static constexpr float Infinity() noexcept { return __builtin_huge_valf(); }
    [[nodiscard]] static constexpr float QuietNaN() noexcept { return __builtin_nanf(UH_GNUC_COMPAT_OR("", "0")); }
    [[nodiscard]] static constexpr float SignalingNaN() noexcept { return __builtin_nansf(UH_GNUC_COMPAT_OR("", "1")); }
    [[nodiscard]] static constexpr float DenormMin() noexcept { return FLT_TRUE_MIN; }
};

/// NumericLimits< double > specialization.
template < > struct NumericLimits< double > : public Impl::BaseFloatNumericLimits
{
    using Type = double;
    static inline constexpr int Digits = DBL_MANT_DIG;
    static inline constexpr int Digits10 = DBL_DIG;
    static inline constexpr int MaxDigits10 = static_cast< int >(2 + DBL_MANT_DIG * 643L / 2136);
    static inline constexpr int MinExponent = DBL_MIN_EXP;
    static inline constexpr int MinExponent10 = DBL_MIN_10_EXP;
    static inline constexpr int MaxExponent = DBL_MAX_EXP;
    static inline constexpr int MaxExponent10 = DBL_MAX_10_EXP;
    static inline constexpr bool HasInfinity = UH_DBL_HAS_INFINITY;
    static inline constexpr bool HasQuietNaN = UH_DBL_HAS_QUIET_NAN;
    static inline constexpr bool HasSignalingNaN = HasQuietNaN;
    static inline constexpr FloatDenormStyle HasDenorm = DBL_HAS_SUBNORM ? DenormPresent : DenormAbsent;
    static inline constexpr bool IsIEC559 = HasInfinity && HasQuietNaN && HasDenorm == DenormPresent;
    static inline constexpr FloatRoundStyle RoundStyle = RoundToNearest;
    [[nodiscard]] static constexpr double Min() noexcept { return DBL_MIN; }
    [[nodiscard]] static constexpr double Max() noexcept { return DBL_MAX; }
    [[nodiscard]] static constexpr double Lowest() noexcept { return -DBL_MAX; }
    [[nodiscard]] static constexpr double Epsilon() noexcept { return DBL_EPSILON; }
    [[nodiscard]] static constexpr double RoundError() noexcept { return 0.5; }
    [[nodiscard]] static constexpr double Infinity() noexcept { return __builtin_huge_val(); }
    [[nodiscard]] static constexpr double QuietNaN() noexcept { return __builtin_nan(UH_GNUC_COMPAT_OR("", "0")); }
    [[nodiscard]] static constexpr double SignalingNaN() noexcept { return __builtin_nans(UH_GNUC_COMPAT_OR("", "1")); }
    [[nodiscard]] static constexpr double DenormMin() noexcept { return DBL_TRUE_MIN; }
};
// --------------------------------------------------------------------------------------------------------------------
/// NumericLimits< long double > specialization.
template < > struct NumericLimits< long double > : public Impl::BaseFloatNumericLimits
{
    using Type = long double ;
    static inline constexpr int Digits = LDBL_MANT_DIG;
    static inline constexpr int Digits10 = LDBL_DIG;
    static inline constexpr int MaxDigits10 = static_cast< int >(2 + LDBL_MANT_DIG * 643L / 2136);
    static inline constexpr int MinExponent = LDBL_MIN_EXP;
    static inline constexpr int MinExponent10 = LDBL_MIN_10_EXP;
    static inline constexpr int MaxExponent = LDBL_MAX_EXP;
    static inline constexpr int MaxExponent10 = LDBL_MAX_10_EXP;
    static inline constexpr bool HasInfinity = UH_LDBL_HAS_INFINITY;
    static inline constexpr bool HasQuietNaN = UH_LDBL_HAS_QUIET_NAN;
    static inline constexpr bool HasSignalingNaN = HasQuietNaN;
    static inline constexpr FloatDenormStyle HasDenorm = LDBL_HAS_SUBNORM ? DenormPresent : DenormAbsent;
    static inline constexpr bool IsIEC559 = HasInfinity && HasQuietNaN && HasDenorm == DenormPresent;
    static inline constexpr FloatRoundStyle RoundStyle = RoundToNearest;
    [[nodiscard]] static constexpr long double Min() noexcept { return LDBL_MIN; }
    [[nodiscard]] static constexpr long double Max() noexcept { return LDBL_MAX; }
    [[nodiscard]] static constexpr long double Lowest() noexcept { return -LDBL_MAX; }
    [[nodiscard]] static constexpr long double Epsilon() noexcept { return LDBL_EPSILON; }
    [[nodiscard]] static constexpr long double RoundError() noexcept { return 0.5L; }
    [[nodiscard]] static constexpr long double Infinity() noexcept { return UH_GNUC_COMPAT_OR(__builtin_huge_vall(), __builtin_huge_val()); }
    [[nodiscard]] static constexpr long double QuietNaN() noexcept { return UH_GNUC_COMPAT_OR(__builtin_nanl(""), __builtin_nan("0")); }
    [[nodiscard]] static constexpr long double SignalingNaN() noexcept { return UH_GNUC_COMPAT_OR(__builtin_nansl(""), __builtin_nans("1")); }
    [[nodiscard]] static constexpr long double DenormMin() noexcept { return LDBL_TRUE_MIN ; }
};

/// Utility that can be used to retrieve the minimum value of a type. Uses %NumericLimits but requires less typing.
template < class T > inline constexpr T MinOf_v = NumericLimits< T >::Min();
/// Utility that can be used to retrieve the minimum value of a type. Uses %NumericLimits but requires less typing.
template < class T > inline constexpr T MaxOf_v = NumericLimits< T >::Max();

} // Namespace:: Urho3D