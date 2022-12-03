//
// Copyright (c) 2008-2022 the Urho3D project.
//

#include <doctest/doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cmath>
#include <cfloat>
#include <limits>
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include <Urho3D/Base/Limits.h>

struct Unspecialized {
    bool operator == (const Unspecialized &) const { return true; }
};

#ifndef UH_HAS_CHAR8_TYPE
    using char8_t = unsigned char;
#endif

// Check our numeric limits if they're identical with the SDL ones
TEST_CASE_TEMPLATE("NumericLimits", T,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
    char8_t,
    char16_t,
    char32_t,
    short,
    unsigned short,
    int,
    unsigned int,
    long,
    unsigned long,
    long long,
    unsigned long long,
    float,    
    double,
    long double
) {
    using URH = Urho3D::NumericLimits< T >;
    using STL = std::numeric_limits< T >;
    // Properties
    CHECK_EQ(URH::Digits, STL::digits);
    CHECK_EQ(URH::Digits10, STL::digits10);
    CHECK_EQ(URH::MaxDigits10, STL::max_digits10);
    CHECK_EQ(URH::IsSigned, STL::is_signed);
    CHECK_EQ(URH::IsInteger, STL::is_integer);
    CHECK_EQ(URH::IsExact, STL::is_exact);
    CHECK_EQ(URH::Radix, STL::radix);
    CHECK_EQ(URH::MinExponent, STL::min_exponent);
    CHECK_EQ(URH::MinExponent10, STL::min_exponent10);
    CHECK_EQ(URH::MaxExponent, STL::max_exponent);
    CHECK_EQ(URH::MaxExponent10, STL::max_exponent10);
    CHECK_EQ(URH::HasInfinity, STL::has_infinity);
    CHECK_EQ(URH::HasQuietNaN, STL::has_quiet_NaN);
    CHECK_EQ(URH::HasSignalingNaN, STL::has_signaling_NaN);
    CHECK_EQ(static_cast< int >(URH::HasDenorm), static_cast< int >(STL::has_denorm));
    CHECK_EQ(URH::HasDenormLoss, STL::has_denorm_loss);
    CHECK_EQ(URH::IsIEC559, STL::is_iec559);
    CHECK_EQ(URH::IsBounded, STL::is_bounded);
    CHECK_EQ(URH::IsModulo, STL::is_modulo);
    CHECK_EQ(URH::Traps, STL::traps);
    CHECK_EQ(URH::TinynessBefore, STL::tinyness_before);
    CHECK_EQ(static_cast< int >(URH::RoundStyle), static_cast< int >(STL::round_style));
    // Functions
    if constexpr (std::is_floating_point_v< T >)
    {
        CHECK_EQ(URH::Min(), STL::min());
        CHECK_EQ(URH::Max(), STL::max());
        CHECK_EQ(URH::Lowest(), STL::lowest());
        CHECK_EQ(URH::Epsilon(), STL::epsilon());
        CHECK_EQ(URH::RoundError(), STL::round_error());
        CHECK_EQ(std::isinf(URH::Infinity()), std::isinf(STL::infinity()));
        CHECK_EQ(doctest::IsNaN(URH::QuietNaN()), doctest::IsNaN(STL::quiet_NaN()));
        CHECK_EQ(doctest::IsNaN(URH::SignalingNaN()), doctest::IsNaN(STL::signaling_NaN()));
        CHECK_EQ(URH::DenormMin(), STL::denorm_min());
    }
    else
    {
        CHECK_EQ(URH::Min(), STL::min());
        CHECK_EQ(URH::Max(), STL::max());
        CHECK_EQ(URH::Lowest(), STL::lowest());
        CHECK_EQ(URH::Epsilon(), STL::epsilon());
        CHECK_EQ(URH::RoundError(), STL::round_error());
        CHECK_EQ(URH::Infinity(), STL::infinity());
        CHECK_EQ(URH::QuietNaN(), STL::quiet_NaN());
        CHECK_EQ(URH::SignalingNaN(), STL::signaling_NaN());
        CHECK_EQ(URH::DenormMin(), STL::denorm_min());
    }
    // Miscellaneous
    CHECK_EQ(std::is_same< typename URH::Type, T >::value, true);
}
