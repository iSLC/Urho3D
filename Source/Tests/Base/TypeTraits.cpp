#include <doctest/doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <utility>
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include <Urho3D/Base/TypeTraits.h>

TEST_SUITE_BEGIN("TypeTraits");

// Test IntegralConstant type.
TEST_CASE_TEMPLATE("IntegralConstant", T,
    char,
    signed char,
    unsigned char,
    short,
    unsigned short,
    int,
    unsigned int,
    long,
    unsigned long,
    long long,
    unsigned long long
) {
    using Urho3D::IntegralConstant;

    constexpr IntegralConstant< T, 1 > instance{};

    CHECK_EQ(IntegralConstant< T, 1 >::value, T(1));

    CHECK_EQ(instance, T(1)); // Check implicit conversion
    CHECK_EQ(instance(), T(1)); // Check function call operator

    CHECK_EQ(std::is_same< typename IntegralConstant< T, 1 >::value_type, T >::value, true);
    CHECK_EQ(std::is_same< typename IntegralConstant< T, 1 >::value_type, bool >::value, false);

    CHECK_EQ(std::is_same< typename IntegralConstant< T, 1 >::type, IntegralConstant< T, 1 > >::value, true);
    CHECK_EQ(std::is_same< typename IntegralConstant< T, 1 >::type, IntegralConstant< T, 0 > >::value, false);
}

// Test BoolConstant type.
TEST_CASE("BoolConstant")
{
    using Urho3D::BoolConstant;
    using Urho3D::TrueType;
    using Urho3D::FalseType;

    CHECK_EQ(BoolConstant< true >::value, true);
    CHECK_EQ(BoolConstant< false >::value, false);

    CHECK_EQ(std::is_same< typename BoolConstant< true >::value_type, bool >::value, true);
    CHECK_EQ(std::is_same< typename BoolConstant< false >::value_type, bool >::value, true);

    CHECK_EQ(std::is_same< typename BoolConstant< true >::type, BoolConstant< true > >::value, true);
    CHECK_EQ(std::is_same< typename BoolConstant< false >::type, BoolConstant< false > >::value, true);

    CHECK_EQ(TrueType::value, true);
    CHECK_EQ(FalseType::value, false);

    CHECK_EQ(std::is_same< typename TrueType::value_type, bool >::value, true);
    CHECK_EQ(std::is_same< typename FalseType::value_type, bool >::value, true);

    CHECK_EQ(std::is_same< typename TrueType::type, TrueType >::value, true);
    CHECK_EQ(std::is_same< typename FalseType::type, FalseType >::value, true);
}

// Test declval function.
TEST_CASE("declval")
{
    struct NonDefault
    {
        NonDefault() = delete;
        int test() const { return 1; }
    };

    CHECK_EQ(std::is_same< decltype(Urho3D::declval< NonDefault >().test()), int >::value, true);
    CHECK_EQ(std::is_same< decltype(Urho3D::declval< NonDefault >().test()), float >::value, false);
}

TEST_SUITE_END();
