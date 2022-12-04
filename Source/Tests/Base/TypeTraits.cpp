#include <doctest/doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <array>
#include <utility>
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include <Urho3D/Base/TypeTraits.h>

TEST_SUITE_BEGIN("TypeTraits");

// MSVC has issues if used a conditional macro in the variadic macro parameter list
#ifdef UH_HAS_CHAR8_TYPE
    using XChar8_t = char8_t;
#else
    using XChar8_t = unsigned char;
#endif

struct Dummy {
    int m;
    int f() { return 0; }
};
enum DummyEnum { };
enum class DummyEnumCls { E };
using NullPtr_t = decltype(nullptr);

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
    unsigned long long,
    XChar8_t
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

// Test IsVoid type-trait.
TEST_CASE("IsVoid")
{
    CHECK_EQ(Urho3D::IsVoid< void >::value, std::is_void< void >::value);
    CHECK_EQ(Urho3D::IsVoid< bool >::value, std::is_void< bool >::value);
    CHECK_EQ(Urho3D::IsVoid< void >::value, Urho3D::IsVoid_v< void >);
    CHECK_EQ(Urho3D::IsVoid< bool >::value, Urho3D::IsVoid_v< bool >);
}

// Test IsNullPointer type-trait.
TEST_CASE("IsNullPointer")
{
    CHECK_EQ(Urho3D::IsNullPointer< decltype(nullptr) >::value, std::is_null_pointer< decltype(nullptr) >::value);
    CHECK_EQ(Urho3D::IsNullPointer< int * >::value, std::is_null_pointer< int * >::value);
    CHECK_EQ(Urho3D::IsNullPointer< decltype(nullptr) >::value, Urho3D::IsNullPointer_v< decltype(nullptr) >);
    CHECK_EQ(Urho3D::IsNullPointer< int * >::value, Urho3D::IsNullPointer_v< int * >);
}

// Test IsIntegral type-trait.
TEST_CASE_TEMPLATE("IsIntegral", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsIntegral< T >::value, std::is_integral< T >::value);
    CHECK_EQ(Urho3D::IsIntegral< T * >::value, std::is_integral< T * >::value);
    CHECK_EQ(Urho3D::IsIntegral< T & >::value, std::is_integral< T & >::value);
    CHECK_EQ(Urho3D::IsIntegral< const T >::value, std::is_integral< const T >::value);
    CHECK_EQ(Urho3D::IsIntegral< T >::value, Urho3D::IsIntegral_v< T >);
}

// Test IsFloatingPoint type-trait.
TEST_CASE_TEMPLATE("IsFloatingPoint", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsFloatingPoint< T >::value, std::is_floating_point< T >::value);
    CHECK_EQ(Urho3D::IsFloatingPoint< T * >::value, std::is_floating_point< T * >::value);
    CHECK_EQ(Urho3D::IsFloatingPoint< T & >::value, std::is_floating_point< T & >::value);
    CHECK_EQ(Urho3D::IsFloatingPoint< const T >::value, std::is_floating_point< const T >::value);
    CHECK_EQ(Urho3D::IsFloatingPoint< T >::value, Urho3D::IsFloatingPoint_v< T >);
}

// Test IsArray type-trait.
TEST_CASE("IsArray")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsArray< A >::value, std::is_array< A >::value);
    CHECK_EQ(Urho3D::IsArray< A[] >::value, std::is_array< A[] >::value);
    CHECK_EQ(Urho3D::IsArray< A[3] >::value, std::is_array< A[3] >::value);
    CHECK_EQ(Urho3D::IsArray< int >::value, std::is_array< int >::value);
    CHECK_EQ(Urho3D::IsArray< int[] >::value, std::is_array< int[] >::value);
    CHECK_EQ(Urho3D::IsArray< int[3] >::value, std::is_array< int[3] >::value);
    CHECK_EQ(Urho3D::IsArray< float >::value, std::is_array< float >::value);
    CHECK_EQ(Urho3D::IsArray< float[] >::value, std::is_array< float[] >::value);
    CHECK_EQ(Urho3D::IsArray< float[3] >::value, std::is_array< float[3] >::value);
    CHECK_EQ(Urho3D::IsArray< Urho3D::Array< int, 0 > >::value, true); // Our IsArray also takes into account the Array container
    CHECK_EQ(Urho3D::IsArray< Urho3D::Array< int, 3 > >::value, true); // Our IsArray also takes into account the Array container
    CHECK_EQ(Urho3D::IsArray< int >::value, Urho3D::IsArray_v< int >);
    CHECK_EQ(Urho3D::IsArray< int[3] >::value, Urho3D::IsArray_v< int[3] >);
}

// Test IsCArray type-trait.
TEST_CASE("IsCArray")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsCArray< A >::value, false);
    CHECK_EQ(Urho3D::IsCArray< A[] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< A[3] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< int >::value, false);
    CHECK_EQ(Urho3D::IsCArray< int[] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< int[3] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< float >::value, false);
    CHECK_EQ(Urho3D::IsCArray< float[] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< float[3] >::value, true);
    CHECK_EQ(Urho3D::IsCArray< Urho3D::Array< int, 0 > >::value, false);
    CHECK_EQ(Urho3D::IsCArray< Urho3D::Array< int, 3 > >::value, false);
    CHECK_EQ(Urho3D::IsCArray< int >::value, Urho3D::IsCArray_v< int >);
    CHECK_EQ(Urho3D::IsCArray< int[3] >::value, Urho3D::IsCArray_v< int[3] >);
}

// Test IsArrayC type-trait.
TEST_CASE("IsArrayC")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsArrayC< A >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< A[] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< A[3] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< int >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< int[] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< int[3] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< float >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< float[] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< float[3] >::value, false);
    CHECK_EQ(Urho3D::IsArrayC< Urho3D::Array< int, 0 > >::value, true);
    CHECK_EQ(Urho3D::IsArrayC< Urho3D::Array< int, 3 > >::value, true);
    CHECK_EQ(Urho3D::IsArrayC< Urho3D::Array< int, 0 > >::value, Urho3D::IsArrayC_v< Urho3D::Array< int, 0 > >);
    CHECK_EQ(Urho3D::IsArrayC< Urho3D::Array< int, 3 > >::value, Urho3D::IsArrayC_v< Urho3D::Array< int, 3 > >);
}

// Test IsEnum type-trait.
TEST_CASE("IsEnum")
{
    struct A {
        enum E { };
    };
    enum E { };
    enum class Ec : int { };

    CHECK_EQ(Urho3D::IsEnum< A >::value, std::is_enum< A >::value);
    CHECK_EQ(Urho3D::IsEnum< E >::value, std::is_enum< E >::value);
    CHECK_EQ(Urho3D::IsEnum< A::E >::value, std::is_enum< A::E >::value);
    CHECK_EQ(Urho3D::IsEnum< Ec >::value, std::is_enum< Ec >::value);
    CHECK_EQ(Urho3D::IsEnum< int >::value, std::is_enum< int >::value);
    CHECK_EQ(Urho3D::IsEnum< E >::value, Urho3D::IsEnum_v< E >);
}

// Test IsUnion type-trait.
TEST_CASE("IsUnion")
{
    using A = Dummy;
    typedef union {
        int a;
        float b;
    } B;
    struct C {
        B d;
    };

    CHECK_EQ(Urho3D::IsUnion< A >::value, std::is_union< A >::value);
    CHECK_EQ(Urho3D::IsUnion< B >::value, std::is_union< B >::value);
    CHECK_EQ(Urho3D::IsUnion< const B >::value, std::is_union< const B >::value);
    CHECK_EQ(Urho3D::IsUnion< B * >::value, std::is_union< B * >::value);
    CHECK_EQ(Urho3D::IsUnion< C >::value, std::is_union< C >::value);
    CHECK_EQ(Urho3D::IsUnion< int >::value, std::is_union< int >::value);
    CHECK_EQ(Urho3D::IsUnion< B >::value, Urho3D::IsUnion_v< B >);
}

// Test IsClass type-trait.
TEST_CASE("IsClass")
{
    struct A { };
    class B { };
    enum class E { };
    union U {
        class UC { };
    };

    CHECK_EQ(Urho3D::IsClass< A >::value, std::is_class< A >::value);
    CHECK_EQ(Urho3D::IsClass< B >::value, std::is_class< B >::value);
    CHECK_EQ(Urho3D::IsClass< B & >::value, std::is_class< B & >::value);
    CHECK_EQ(Urho3D::IsClass< B * >::value, std::is_class< B * >::value);
    CHECK_EQ(Urho3D::IsClass< const B >::value, std::is_class< const B  >::value);
    CHECK_EQ(Urho3D::IsClass< E >::value, std::is_class< E  >::value);
    CHECK_EQ(Urho3D::IsClass< int >::value, std::is_class< int  >::value);
    CHECK_EQ(Urho3D::IsClass< struct S >::value, std::is_class< struct S  >::value);
    CHECK_EQ(Urho3D::IsClass< class C >::value, std::is_class< class C  >::value);
    CHECK_EQ(Urho3D::IsClass< B >::value, Urho3D::IsClass_v< B >);
}

int IsFunction_Dummy(int i) { return i; } // function
template < class > struct IsFunction_PMT { };
template < class T, class U > struct IsFunction_PMT< U T:: * > { using member_type = U; };
// Test IsFunction type-trait.
TEST_CASE("IsFunction")
{
    struct A {
        int fun1();
        bool fun2() const;
        void fun3() noexcept;
        long fun4() volatile;
        float fun5() const &;
        static char fun6() { return 'A'; }
    };
    int(*b)(int) = IsFunction_Dummy; // pointer to function
    struct C { int operator()(int i){return i;} } c; // function-like class
    using fun5_t = IsFunction_PMT< decltype(&A::fun5) >::member_type; // T is int() const &

    CHECK_EQ(Urho3D::IsFunction< A >::value, std::is_function< A >::value);
    CHECK_EQ(Urho3D::IsFunction< int(int) >::value, std::is_function< int(int) >::value);
    CHECK_EQ(Urho3D::IsFunction< int(*)(int) >::value, std::is_function< int(*)(int) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(IsFunction_Dummy) >::value, std::is_function< decltype(IsFunction_Dummy) >::value);
    CHECK_EQ(Urho3D::IsFunction< int >::value, std::is_function< int >::value);
    CHECK_EQ(Urho3D::IsFunction< fun5_t >::value, std::is_function< fun5_t >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun1) >::value, std::is_function< decltype(&A::fun1) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun2) >::value, std::is_function< decltype(&A::fun2) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun3) >::value, std::is_function< decltype(&A::fun3) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun4) >::value, std::is_function< decltype(&A::fun4) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun5) >::value, std::is_function< decltype(&A::fun5) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(&A::fun6) >::value, std::is_function< decltype(&A::fun6) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(b) >::value, std::is_function< decltype(b) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(c) >::value, std::is_function< decltype(c) >::value);
    CHECK_EQ(Urho3D::IsFunction< C >::value, std::is_function< C >::value);
}

// Test IsPointer type-trait.
TEST_CASE("IsPointer")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsPointer< A >::value, std::is_pointer< A >::value);
    CHECK_EQ(Urho3D::IsPointer< A * >::value, std::is_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsPointer< A const * >::value, std::is_pointer< A const * >::value);
    CHECK_EQ(Urho3D::IsPointer< A const * volatile >::value, std::is_pointer< A const * >::value);
    CHECK_EQ(Urho3D::IsPointer< const A * >::value, std::is_pointer< const A * >::value);
    CHECK_EQ(Urho3D::IsPointer< A & >::value, std::is_pointer< A & >::value);
    CHECK_EQ(Urho3D::IsPointer< int >::value, std::is_pointer< int >::value);
    CHECK_EQ(Urho3D::IsPointer< int * >::value, std::is_pointer< int * >::value);
    CHECK_EQ(Urho3D::IsPointer< int ** >::value, std::is_pointer< int ** >::value);
    CHECK_EQ(Urho3D::IsPointer< int[10] >::value, std::is_pointer< int[10] >::value);
    CHECK_EQ(Urho3D::IsPointer< decltype(nullptr) >::value, std::is_pointer< decltype(nullptr)  >::value);
    CHECK_EQ(Urho3D::IsPointer< int(*)(int) >::value, std::is_pointer< int(*)(int)  >::value);
    CHECK_EQ(Urho3D::IsPointer< A >::value, Urho3D::IsPointer_v< A >);
    CHECK_EQ(Urho3D::IsPointer< A * >::value, Urho3D::IsPointer_v< A * >);
}

// Test IsLvalueReference type-trait.
TEST_CASE("IsLvalueReference")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsLvalueReference< A >::value, std::is_lvalue_reference< A >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< A & >::value, std::is_lvalue_reference< A & >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< const A & >::value, std::is_lvalue_reference< const A & >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< A && >::value, std::is_lvalue_reference< A && >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< A * >::value, std::is_lvalue_reference< A * >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< int >::value, std::is_lvalue_reference< int >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< int & >::value, std::is_lvalue_reference< int & >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< const int & >::value, std::is_lvalue_reference< const int & >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< int && >::value, std::is_lvalue_reference< int && >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< int * >::value, std::is_lvalue_reference< int * >::value);
    CHECK_EQ(Urho3D::IsLvalueReference< A >::value, Urho3D::IsLvalueReference_v< A >);
    CHECK_EQ(Urho3D::IsLvalueReference< A & >::value, Urho3D::IsLvalueReference_v< A & >);
}

// Test IsRvalueReference type-trait.
TEST_CASE("IsRvalueReference")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsRvalueReference< A >::value, std::is_rvalue_reference< A >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< A & >::value, std::is_rvalue_reference< A & >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< const A & >::value, std::is_rvalue_reference< const A & >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< A && >::value, std::is_rvalue_reference< A && >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< A * >::value, std::is_rvalue_reference< A * >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< int >::value, std::is_rvalue_reference< int >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< int & >::value, std::is_rvalue_reference< int & >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< const int & >::value, std::is_rvalue_reference< const int & >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< int && >::value, std::is_rvalue_reference< int && >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< int * >::value, std::is_rvalue_reference< int * >::value);
    CHECK_EQ(Urho3D::IsRvalueReference< A >::value, Urho3D::IsRvalueReference_v< A >);
    CHECK_EQ(Urho3D::IsRvalueReference< A & >::value, Urho3D::IsRvalueReference_v< A & >);
}

// Test IsMemberObjectPointer type-trait.
TEST_CASE("IsMemberObjectPointer")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsMemberObjectPointer< A * >::value, std::is_member_object_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int A::* >::value, std::is_member_object_pointer< int A::* >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, std::is_member_object_pointer< int(A::*) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, std::is_member_object_pointer< int(A::*)() >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< decltype(&A::m) >::value, std::is_member_object_pointer< decltype(&A::m) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, Urho3D::IsMemberObjectPointer_v< int(A::*) >);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, Urho3D::IsMemberObjectPointer_v< int(A::*)() >);
}

// Test IsMemberFunctionPointer type-trait.
TEST_CASE("IsMemberFunctionPointer")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsMemberFunctionPointer< A * >::value, std::is_member_function_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< int(A::*)() >::value, std::is_member_function_pointer< int(A::*)() >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< decltype(&A::m) >::value, std::is_member_function_pointer< decltype(&A::m) >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< A * >::value, Urho3D::IsMemberFunctionPointer_v< A * >);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< int(A::*)() >::value, Urho3D::IsMemberFunctionPointer_v< int(A::*)() >);
}

// Test IsFundamental type-trait.
TEST_CASE_TEMPLATE("IsFundamental", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsFundamental< T >::value, std::is_fundamental< T >::value);
    CHECK_EQ(Urho3D::IsFundamental< T * >::value, std::is_fundamental< T * >::value);
    CHECK_EQ(Urho3D::IsFundamental< T & >::value, std::is_fundamental< T & >::value);
    CHECK_EQ(Urho3D::IsFundamental< T && >::value, std::is_fundamental< T && >::value);
    CHECK_EQ(Urho3D::IsFundamental< const T >::value, std::is_fundamental< const T >::value);
    CHECK_EQ(Urho3D::IsFundamental< const T * >::value, std::is_fundamental< const T * >::value);
    CHECK_EQ(Urho3D::IsFundamental< const T & >::value, std::is_fundamental< const T & >::value);
    CHECK_EQ(Urho3D::IsFundamental< T >::value, Urho3D::IsFundamental_v< T >);
}

// Test IsArithmetic type-trait.
TEST_CASE_TEMPLATE("IsArithmetic", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsArithmetic< T >::value, std::is_arithmetic< T >::value);
    CHECK_EQ(Urho3D::IsArithmetic< T * >::value, std::is_arithmetic< T * >::value);
    CHECK_EQ(Urho3D::IsArithmetic< T & >::value, std::is_arithmetic< T & >::value);
    CHECK_EQ(Urho3D::IsArithmetic< T && >::value, std::is_arithmetic< T && >::value);
    CHECK_EQ(Urho3D::IsArithmetic< const T >::value, std::is_arithmetic< const T >::value);
    CHECK_EQ(Urho3D::IsArithmetic< const T * >::value, std::is_arithmetic< const T * >::value);
    CHECK_EQ(Urho3D::IsArithmetic< const T & >::value, std::is_arithmetic< const T & >::value);
    CHECK_EQ(Urho3D::IsArithmetic< T >::value, Urho3D::IsArithmetic_v< T >);
}

// Test IsScalar type-trait.
TEST_CASE_TEMPLATE("IsScalar", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t,
    decltype(&Dummy::m),
    decltype(&Dummy::f),
    decltype(DummyEnumCls::E)
) {
    CHECK_EQ(Urho3D::IsScalar< T >::value, std::is_scalar< T >::value);
    CHECK_EQ(Urho3D::IsScalar< T * >::value, std::is_scalar< T * >::value);
    CHECK_EQ(Urho3D::IsScalar< T & >::value, std::is_scalar< T & >::value);
    CHECK_EQ(Urho3D::IsScalar< T && >::value, std::is_scalar< T && >::value);
    CHECK_EQ(Urho3D::IsScalar< const T >::value, std::is_scalar< const T >::value);
    CHECK_EQ(Urho3D::IsScalar< const T * >::value, std::is_scalar< const T * >::value);
    CHECK_EQ(Urho3D::IsScalar< const T & >::value, std::is_scalar< const T & >::value);
    CHECK_EQ(Urho3D::IsScalar< T >::value, Urho3D::IsScalar_v< T >);
}

// Test IsObject type-trait.
TEST_CASE_TEMPLATE("IsObject", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsObject< T >::value, std::is_object< T >::value);
    CHECK_EQ(Urho3D::IsObject< T * >::value, std::is_object< T * >::value);
    CHECK_EQ(Urho3D::IsObject< T & >::value, std::is_object< T & >::value);
    CHECK_EQ(Urho3D::IsObject< T && >::value, std::is_object< T && >::value);
    CHECK_EQ(Urho3D::IsObject< const T >::value, std::is_object< const T >::value);
    CHECK_EQ(Urho3D::IsObject< const T * >::value, std::is_object< const T * >::value);
    CHECK_EQ(Urho3D::IsObject< const T & >::value, std::is_object< const T & >::value);
    CHECK_EQ(Urho3D::IsObject< T >::value, Urho3D::IsObject_v< T >);
}

// Test IsCompound type-trait.
TEST_CASE_TEMPLATE("IsCompound", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsCompound< T >::value, std::is_compound< T >::value);
    CHECK_EQ(Urho3D::IsCompound< T * >::value, std::is_compound< T * >::value);
    CHECK_EQ(Urho3D::IsCompound< T & >::value, std::is_compound< T & >::value);
    CHECK_EQ(Urho3D::IsCompound< T && >::value, std::is_compound< T && >::value);
    CHECK_EQ(Urho3D::IsCompound< const T >::value, std::is_compound< const T >::value);
    CHECK_EQ(Urho3D::IsCompound< const T * >::value, std::is_compound< const T * >::value);
    CHECK_EQ(Urho3D::IsCompound< const T & >::value, std::is_compound< const T & >::value);
    CHECK_EQ(Urho3D::IsCompound< T >::value, Urho3D::IsCompound_v< T >);
}

// Test IsReference type-trait.
TEST_CASE_TEMPLATE("IsReference", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t
) {
    CHECK_EQ(Urho3D::IsReference< T >::value, std::is_reference< T >::value);
    CHECK_EQ(Urho3D::IsReference< T * >::value, std::is_reference< T * >::value);
    CHECK_EQ(Urho3D::IsReference< T & >::value, std::is_reference< T & >::value);
    CHECK_EQ(Urho3D::IsReference< T *& >::value, std::is_reference< T *& >::value);
    CHECK_EQ(Urho3D::IsReference< T *&& >::value, std::is_reference< T *&& >::value);
    CHECK_EQ(Urho3D::IsReference< const T >::value, std::is_reference< const T >::value);
    CHECK_EQ(Urho3D::IsReference< const T * >::value, std::is_reference< const T * >::value);
    CHECK_EQ(Urho3D::IsReference< const T & >::value, std::is_reference< const T & >::value);
    CHECK_EQ(Urho3D::IsReference< T >::value, Urho3D::IsReference_v< T >);
}

// Test IsMemberObjectPointer type-trait.
TEST_CASE("IsMemberObjectPointer")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsMemberObjectPointer< A * >::value, std::is_member_object_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int A::* >::value, std::is_member_object_pointer< int A::* >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, std::is_member_object_pointer< int(A::*) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, std::is_member_object_pointer< int(A::*)() >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< decltype(&A::m) >::value, std::is_member_object_pointer< decltype(&A::m) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< decltype(&A::f) >::value, std::is_member_object_pointer< decltype(&A::f) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, Urho3D::IsMemberObjectPointer_v< int(A::*) >);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, Urho3D::IsMemberObjectPointer_v< int(A::*)() >);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< A * >::value, Urho3D::IsMemberObjectPointer_v< A * >);
}

// Test IsConst type-trait.
TEST_CASE("IsConst")
{
    CHECK_EQ(Urho3D::IsConst< int >::value, std::is_const< int >::value);
    CHECK_EQ(Urho3D::IsConst< const int >::value, std::is_const< const int >::value);
    CHECK_EQ(Urho3D::IsConst< const int * >::value, std::is_const< const int * >::value);
    CHECK_EQ(Urho3D::IsConst< int * const >::value, std::is_const< int * const >::value);
    CHECK_EQ(Urho3D::IsConst< const int & >::value, std::is_const< const int & >::value);
    CHECK_EQ(Urho3D::IsConst< int >::value, Urho3D::IsConst_v< int >);
    CHECK_EQ(Urho3D::IsConst< const int >::value, Urho3D::IsConst_v< const int >);
}

// Test IsVolatile type-trait.
TEST_CASE("IsVolatile")
{
    CHECK_EQ(Urho3D::IsVolatile< int >::value, std::is_volatile< int >::value);
    CHECK_EQ(Urho3D::IsVolatile< volatile int >::value, std::is_volatile< volatile int >::value);
    CHECK_EQ(Urho3D::IsVolatile< volatile int * >::value, std::is_volatile< volatile int * >::value);
    CHECK_EQ(Urho3D::IsVolatile< int * volatile >::value, std::is_volatile< int * volatile >::value);
    CHECK_EQ(Urho3D::IsVolatile< int >::value, Urho3D::IsVolatile_v< int >);
    CHECK_EQ(Urho3D::IsVolatile< volatile int >::value, Urho3D::IsVolatile_v< volatile int >);
}

// Test IsTrivial type-trait.
TEST_CASE("IsTrivial")
{
    using A = Dummy;
    struct B {
        B() { }
    };
    struct C : B { };
    struct D { virtual void fn() { } };

    CHECK_EQ(Urho3D::IsTrivial< A >::value, std::is_trivial< A >::value);
    CHECK_EQ(Urho3D::IsTrivial< B >::value, std::is_trivial< B >::value);
    CHECK_EQ(Urho3D::IsTrivial< C >::value, std::is_trivial< C >::value);
    CHECK_EQ(Urho3D::IsTrivial< D >::value, std::is_trivial< D >::value);
    CHECK_EQ(Urho3D::IsTrivial< A >::value, Urho3D::IsTrivial_v< A >);
    CHECK_EQ(Urho3D::IsTrivial< B >::value, Urho3D::IsTrivial_v< B >);
}

// Test IsTriviallyCopyable type-trait.
TEST_CASE("IsTriviallyCopyable")
{
    using A = Dummy;
    struct B {
        B(B const&) { } // copy ctor
    };
    struct C {
        virtual void fn() { }
    };
    struct D {
        int m;
        D(D const&) = default; // -> trivially copyable
        D(int x): m(x + 1) { }
    };

    CHECK_EQ(Urho3D::IsTriviallyCopyable< A >::value, std::is_trivially_copyable< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyable< B >::value, std::is_trivially_copyable< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyable< C >::value, std::is_trivially_copyable< C >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyable< D >::value, std::is_trivially_copyable< D >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyable< A >::value, Urho3D::IsTriviallyCopyable_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyCopyable< C >::value, Urho3D::IsTriviallyCopyable_v< C >);
}

// Test IsStandardLayout type-trait.
TEST_CASE("IsStandardLayout")
{
    using A = Dummy;
    struct B {
        int m1;
    private:
        int m2;
    };
    struct C : A {
        int x;
    };
    struct D {
        virtual void fn() { };
    };
    CHECK_EQ(Urho3D::IsStandardLayout< A >::value, std::is_standard_layout< A >::value);
    CHECK_EQ(Urho3D::IsStandardLayout< B >::value, std::is_standard_layout< B >::value);
    CHECK_EQ(Urho3D::IsStandardLayout< C >::value, std::is_standard_layout< C >::value);
    CHECK_EQ(Urho3D::IsStandardLayout< D >::value, std::is_standard_layout< D >::value);
    CHECK_EQ(Urho3D::IsStandardLayout< A >::value, Urho3D::IsStandardLayout_v< A >);
    CHECK_EQ(Urho3D::IsStandardLayout< B >::value, Urho3D::IsStandardLayout_v< B >);
}

// Test IsPOD type-trait.
TEST_CASE("IsPOD")
{
    using A = Dummy;
    struct B {
        int m1;
    private:
        int m2;
    };
    struct C : A { };
    struct D : A {
        int x;
    };
    struct E : A {
        E() { }
    };
    struct F {
        virtual void fn() { };
    };
    CHECK_EQ(Urho3D::IsPOD< A >::value, std::is_pod< A >::value);
    CHECK_EQ(Urho3D::IsPOD< B >::value, std::is_pod< B >::value);
    CHECK_EQ(Urho3D::IsPOD< C >::value, std::is_pod< C >::value);
    CHECK_EQ(Urho3D::IsPOD< D >::value, std::is_pod< D >::value);
    CHECK_EQ(Urho3D::IsPOD< E >::value, std::is_pod< E >::value);
    CHECK_EQ(Urho3D::IsPOD< F >::value, std::is_pod< F >::value);
    CHECK_EQ(Urho3D::IsPOD< A >::value, Urho3D::IsPOD_v< A >);
    CHECK_EQ(Urho3D::IsPOD< F >::value, Urho3D::IsPOD_v< F >);
}

// Test IsLiteralType type-trait.
TEST_CASE("IsLiteralType")
{
    using A = Dummy;
    struct B {
        virtual ~B() { };
    };
#ifdef UH_GNUC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(UH_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    CHECK_EQ(Urho3D::IsLiteralType< A >::value, std::is_literal_type< A >::value);
    CHECK_EQ(Urho3D::IsLiteralType< B >::value, std::is_literal_type< B >::value);
    CHECK_EQ(Urho3D::IsLiteralType< int >::value, std::is_literal_type< int >::value);
    CHECK_EQ(Urho3D::IsLiteralType< int & >::value, std::is_literal_type< int & >::value);
    CHECK_EQ(Urho3D::IsLiteralType< int * >::value, std::is_literal_type< int * >::value);
#ifdef UH_GNUC
    #pragma GCC diagnostic pop
#elif defined(UH_CLANG)
    #pragma clang diagnostic pop
#endif
    CHECK_EQ(Urho3D::IsLiteralType< A >::value, Urho3D::IsLiteralType_v< A >);
    CHECK_EQ(Urho3D::IsLiteralType< B >::value, Urho3D::IsLiteralType_v< B >);
}

struct IsEmpty_Dummy {
    static int m;
};
// Test IsEmpty type-trait.
TEST_CASE("IsEmpty")
{
    struct A { };
    struct B {
        int m;
    };
    struct D {
        virtual ~D() { }
    };
    union E { };
#if UH_CPP_STANDARD >= UH_CPP20_STANDARD
    struct F {
        [[no_unique_address]] E e;
    };
#endif
    struct G {
        int:0;  
        // C++ standard allow "as a special case, an unnamed bit-field with a width of zero 
        // specifies alignment of the next bit-field at an allocation unit boundary.
        // Only when declaring an unnamed bit-field may the width be zero."
    };

    CHECK_EQ(Urho3D::IsEmpty< A >::value, std::is_empty< A >::value);
    CHECK_EQ(Urho3D::IsEmpty< B >::value, std::is_empty< B >::value);
    CHECK_EQ(Urho3D::IsEmpty< IsEmpty_Dummy >::value, std::is_empty< IsEmpty_Dummy >::value);
    CHECK_EQ(Urho3D::IsEmpty< D >::value, std::is_empty< D >::value);
    CHECK_EQ(Urho3D::IsEmpty< E >::value, std::is_empty< E >::value);
#if UH_CPP_STANDARD >= UH_CPP20_STANDARD
    CHECK_EQ(Urho3D::IsEmpty< F >::value, std::is_empty< F >::value);
#endif
    CHECK_EQ(Urho3D::IsEmpty< G >::value, std::is_empty< G >::value);
    CHECK_EQ(Urho3D::IsEmpty< A >::value, Urho3D::IsEmpty_v< A >);
    CHECK_EQ(Urho3D::IsEmpty< B >::value, Urho3D::IsEmpty_v< B >);
}

// Test IsPolymorphic type-trait.
TEST_CASE("IsPolymorphic")
{
    using A = Dummy;
    struct B {
        virtual void fn() { }
    };
    struct C : B { };
    struct D {
        virtual ~D() = default;
    };

    CHECK_EQ(Urho3D::IsPolymorphic< int >::value, std::is_polymorphic< int >::value);
    CHECK_EQ(Urho3D::IsPolymorphic< A >::value, std::is_polymorphic< A >::value);
    CHECK_EQ(Urho3D::IsPolymorphic< B >::value, std::is_polymorphic< B >::value);
    CHECK_EQ(Urho3D::IsPolymorphic< C >::value, std::is_polymorphic< C >::value);
    CHECK_EQ(Urho3D::IsPolymorphic< D >::value, std::is_polymorphic< D >::value);
    CHECK_EQ(Urho3D::IsPolymorphic< A >::value, Urho3D::IsPolymorphic_v< A >);
    CHECK_EQ(Urho3D::IsPolymorphic< B >::value, Urho3D::IsPolymorphic_v< B >);
}

// Test IsAbstract type-trait.
TEST_CASE("IsAbstract")
{
    using A = Dummy;
    struct B {
        virtual void fn() { };
    };
    struct C {
        virtual void fn() = 0;
    };
    struct D : C { };

    CHECK_EQ(Urho3D::IsAbstract< int >::value, std::is_abstract< int >::value);
    CHECK_EQ(Urho3D::IsAbstract< A >::value, std::is_abstract< A >::value);
    CHECK_EQ(Urho3D::IsAbstract< B >::value, std::is_abstract< B >::value);
    CHECK_EQ(Urho3D::IsAbstract< C >::value, std::is_abstract< C >::value);
    CHECK_EQ(Urho3D::IsAbstract< D >::value, std::is_abstract< D >::value);
    CHECK_EQ(Urho3D::IsAbstract< A >::value, Urho3D::IsAbstract_v< A >);
    CHECK_EQ(Urho3D::IsAbstract< C >::value, Urho3D::IsAbstract_v< C >);
}

// Test IsFinal type-trait.
TEST_CASE("IsFinal")
{
    using A = Dummy;
    class B final {};

    CHECK_EQ(Urho3D::IsFinal< int >::value, std::is_final< int >::value);
    CHECK_EQ(Urho3D::IsFinal< A >::value, std::is_final< A >::value);
    CHECK_EQ(Urho3D::IsFinal< B >::value, std::is_final< B >::value);
    CHECK_EQ(Urho3D::IsFinal< A >::value, Urho3D::IsFinal_v< A >);
    CHECK_EQ(Urho3D::IsFinal< B >::value, Urho3D::IsFinal_v< B >);
}

// Test IsAggregate type-trait.
TEST_CASE("IsAggregate")
{
    struct A { int x, y; };
    struct B {
        B(int, const char *) { }
    };

    CHECK_EQ(Urho3D::IsAggregate< int >::value, std::is_aggregate< int >::value);
    CHECK_EQ(Urho3D::IsAggregate< A >::value, std::is_aggregate< A >::value);
    CHECK_EQ(Urho3D::IsAggregate< B >::value, std::is_aggregate< B >::value);
    CHECK_EQ(Urho3D::IsAggregate< A >::value, Urho3D::IsAggregate_v< A >);
    CHECK_EQ(Urho3D::IsAggregate< B >::value, Urho3D::IsAggregate_v< B >);
}

enum DummyEnumInt : int { };
enum DummyEnumUint : unsigned { };
enum class DummyEnumClsInt : int { E };
enum class DummyEnumClsUInt : unsigned { E };

// Test IsSigned type-trait.
TEST_CASE_TEMPLATE("IsSigned", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t,
    DummyEnum,
    DummyEnumCls,
    DummyEnumInt,
    DummyEnumUint,
    DummyEnumClsInt,
    DummyEnumClsUInt
) {
    CHECK_EQ(Urho3D::IsSigned< T >::value, std::is_signed< T >::value);
    CHECK_EQ(Urho3D::IsSigned< T * >::value, std::is_signed< T * >::value);
    CHECK_EQ(Urho3D::IsSigned< T & >::value, std::is_signed< T & >::value);
    CHECK_EQ(Urho3D::IsSigned< const T >::value, std::is_signed< const T >::value);
    CHECK_EQ(Urho3D::IsSigned< const T * >::value, std::is_signed< const T * >::value);
    CHECK_EQ(Urho3D::IsSigned< const T & >::value, std::is_signed< const T & >::value);
    CHECK_EQ(Urho3D::IsSigned< T >::value, Urho3D::IsSigned_v< T >);
}

// Test IsUnsigned type-trait.
TEST_CASE_TEMPLATE("IsUnsigned", T,
    Dummy,
    bool,
    char,
    signed char,
    unsigned char,
    wchar_t,
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
    long double,
    XChar8_t,
    NullPtr_t,
    DummyEnum,
    DummyEnumCls,
    DummyEnumInt,
    DummyEnumUint,
    DummyEnumClsInt,
    DummyEnumClsUInt
) {
    CHECK_EQ(Urho3D::IsUnsigned< T >::value, std::is_unsigned< T >::value);
    CHECK_EQ(Urho3D::IsUnsigned< T * >::value, std::is_unsigned< T * >::value);
    CHECK_EQ(Urho3D::IsUnsigned< T & >::value, std::is_unsigned< T & >::value);
    CHECK_EQ(Urho3D::IsUnsigned< const T >::value, std::is_unsigned< const T >::value);
    CHECK_EQ(Urho3D::IsUnsigned< const T * >::value, std::is_unsigned< const T * >::value);
    CHECK_EQ(Urho3D::IsUnsigned< const T & >::value, std::is_unsigned< const T & >::value);
    CHECK_EQ(Urho3D::IsUnsigned< T >::value, Urho3D::IsUnsigned_v< T >);
}

// Test IsBoundedArray type-trait.
TEST_CASE("IsBoundedArray")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsBoundedArray< A >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< A >::value, false));
    CHECK_EQ(Urho3D::IsBoundedArray< A[] >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< A[] >::value, false));
    CHECK_EQ(Urho3D::IsBoundedArray< A[3] >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< A[3] >::value, true));
    CHECK_EQ(Urho3D::IsBoundedArray< float >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< float >::value, false));
    CHECK_EQ(Urho3D::IsBoundedArray< int >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< int >::value, false));
    CHECK_EQ(Urho3D::IsBoundedArray< int[] >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< int[] >::value, false));
    CHECK_EQ(Urho3D::IsBoundedArray< int[3] >::value, UH_LEAST_CPP20_OR(std::is_bounded_array< int[3] >::value, true));
    CHECK_EQ(Urho3D::IsBoundedArray< A >::value, Urho3D::IsBoundedArray_v< A >);
    CHECK_EQ(Urho3D::IsBoundedArray< A[3] >::value, Urho3D::IsBoundedArray_v< A[3] >);
}

// Test IsUnboundedArray type-trait.
TEST_CASE("IsUnboundedArray")
{
    using A = Dummy;

    CHECK_EQ(Urho3D::IsUnboundedArray< A >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< A >::value, false));
    CHECK_EQ(Urho3D::IsUnboundedArray< A[] >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< A[] >::value, true));
    CHECK_EQ(Urho3D::IsUnboundedArray< A[3] >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< A[3] >::value, false));
    CHECK_EQ(Urho3D::IsUnboundedArray< float >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< float >::value, false));
    CHECK_EQ(Urho3D::IsUnboundedArray< int >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< int >::value, false));
    CHECK_EQ(Urho3D::IsUnboundedArray< int[] >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< int[] >::value, true));
    CHECK_EQ(Urho3D::IsUnboundedArray< int[3] >::value, UH_LEAST_CPP20_OR(std::is_unbounded_array< int[3] >::value, false));
    CHECK_EQ(Urho3D::IsUnboundedArray< A >::value, Urho3D::IsUnboundedArray_v< A >);
    CHECK_EQ(Urho3D::IsUnboundedArray< A[3] >::value, Urho3D::IsUnboundedArray_v< A[3] >);
}

class IsConstructible_t {
    int i;
    double d;
public:
    IsConstructible_t(int n) : i(n), d() { }
    IsConstructible_t(int n, double f) noexcept : i(n), d(f) { }
};

// Test IsConstructible type-trait.
TEST_CASE("IsConstructible")
{
    using A = IsConstructible_t;

    CHECK_EQ(Urho3D::IsConstructible< int, int >::value, std::is_constructible< int, int >::value);
    CHECK_EQ(Urho3D::IsConstructible< A, int >::value, std::is_constructible< A, int >::value);
    CHECK_EQ(Urho3D::IsConstructible< A, float >::value, std::is_constructible< A, float >::value);
    CHECK_EQ(Urho3D::IsConstructible< A, int, double >::value, std::is_constructible< A, int, double >::value);
    CHECK_EQ(Urho3D::IsConstructible< A, const A & >::value, std::is_constructible< A, const A & >::value);
    CHECK_EQ(Urho3D::IsConstructible< A, int >::value, Urho3D::IsConstructible_v< A, int >);
    CHECK_EQ(Urho3D::IsConstructible< A, float >::value, Urho3D::IsConstructible_v< A, float >);
    CHECK_EQ(Urho3D::IsConstructible< A, int, double >::value, Urho3D::IsConstructible_v< A, int, double >);
    CHECK_EQ(Urho3D::IsConstructible< A, const A & >::value, Urho3D::IsConstructible_v< A, const A & >);
}

// Test IsTriviallyConstructible type-trait.
TEST_CASE("IsTriviallyConstructible")
{
    using A = IsConstructible_t;

    CHECK_EQ(Urho3D::IsTriviallyConstructible< int, int >::value, std::is_trivially_constructible< int, int >::value);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, int >::value, std::is_trivially_constructible< A, int >::value);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, float >::value, std::is_trivially_constructible< A, float >::value);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, int, double >::value, std::is_trivially_constructible< A, int, double >::value);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, const A & >::value, std::is_trivially_constructible< A, const A & >::value);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, int >::value, Urho3D::IsTriviallyConstructible_v< A, int >);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, float >::value, Urho3D::IsTriviallyConstructible_v< A, float >);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, int, double >::value, Urho3D::IsTriviallyConstructible_v< A, int, double >);
    CHECK_EQ(Urho3D::IsTriviallyConstructible< A, const A & >::value, Urho3D::IsTriviallyConstructible_v< A, const A & >);
}

// Test IsNoThrowConstructible type-trait.
TEST_CASE("IsNoThrowConstructible")
{
    using A = IsConstructible_t;

    CHECK_EQ(Urho3D::IsNoThrowConstructible< int, int >::value, std::is_nothrow_constructible< int, int >::value);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, int >::value, std::is_nothrow_constructible< A, int >::value);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, float >::value, std::is_nothrow_constructible< A, float >::value);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, int, double >::value, std::is_nothrow_constructible< A, int, double >::value);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, const A & >::value, std::is_nothrow_constructible< A, const A & >::value);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, int >::value, Urho3D::IsNoThrowConstructible_v< A, int >);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, float >::value, Urho3D::IsNoThrowConstructible_v< A, float >);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, int, double >::value, Urho3D::IsNoThrowConstructible_v< A, int, double >);
    CHECK_EQ(Urho3D::IsNoThrowConstructible< A, const A & >::value, Urho3D::IsNoThrowConstructible_v< A, const A & >);
}

// Test IsDefaultConstructible type-trait.
TEST_CASE("IsDefaultConstructible")
{
    struct A { };
    struct B {
        B() { }
    };
    struct C {
        C(int) { }
    };

    CHECK_EQ(Urho3D::IsDefaultConstructible< int >::value, std::is_default_constructible< int >::value);
    CHECK_EQ(Urho3D::IsDefaultConstructible< A >::value, std::is_default_constructible< A >::value);
    CHECK_EQ(Urho3D::IsDefaultConstructible< B >::value, std::is_default_constructible< B >::value);
    CHECK_EQ(Urho3D::IsDefaultConstructible< C >::value, std::is_default_constructible< C >::value);
    CHECK_EQ(Urho3D::IsDefaultConstructible< A >::value, Urho3D::IsDefaultConstructible_v< A >);
    CHECK_EQ(Urho3D::IsDefaultConstructible< C >::value, Urho3D::IsDefaultConstructible_v< C >);
}

// Test IsTriviallyDefaultConstructible type-trait.
TEST_CASE("IsTriviallyDefaultConstructible")
{
    struct A { };
    struct B {
        B() { }
    };
    struct C {
        C(int) { }
        virtual void fn() { }
    };

    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< int >::value, std::is_trivially_default_constructible< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< A >::value, std::is_trivially_default_constructible< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< B >::value, std::is_trivially_default_constructible< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< C >::value, std::is_trivially_default_constructible< C >::value);
    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< A >::value, Urho3D::IsTriviallyDefaultConstructible_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyDefaultConstructible< C >::value, Urho3D::IsTriviallyDefaultConstructible_v< C >);
}

// Test IsNoThrowDefaultConstructible type-trait.
TEST_CASE("IsNoThrowDefaultConstructible")
{
    struct A { };
    struct B {
        B() { }
    };
    struct C {
        C() noexcept { }
    };

    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< int >::value, std::is_nothrow_default_constructible< int >::value);
    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< A >::value, std::is_nothrow_default_constructible< A >::value);
    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< B >::value, std::is_nothrow_default_constructible< B >::value);
    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< C >::value, std::is_nothrow_default_constructible< C >::value);
    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< A >::value, Urho3D::IsNoThrowDefaultConstructible_v< A >);
    CHECK_EQ(Urho3D::IsNoThrowDefaultConstructible< B >::value, Urho3D::IsNoThrowDefaultConstructible_v< B >);
}

// Test IsCopyConstructible type-trait.
TEST_CASE("IsCopyConstructible")
{
    struct A { };
    struct B {
        B(B &&) { }
    };
    struct C {
        C(const C &){ }
    };

    CHECK_EQ(Urho3D::IsCopyConstructible< int >::value, std::is_copy_constructible< int >::value);
    CHECK_EQ(Urho3D::IsCopyConstructible< A >::value, std::is_copy_constructible< A >::value);
    CHECK_EQ(Urho3D::IsCopyConstructible< B >::value, std::is_copy_constructible< B >::value);
    CHECK_EQ(Urho3D::IsCopyConstructible< C >::value, std::is_copy_constructible< C >::value);
    CHECK_EQ(Urho3D::IsCopyConstructible< A >::value, Urho3D::IsCopyConstructible_v< A >);
    CHECK_EQ(Urho3D::IsCopyConstructible< B >::value, Urho3D::IsCopyConstructible_v< B >);
}

// Test IsTriviallyCopyConstructible type-trait.
TEST_CASE("IsTriviallyCopyConstructible")
{
    struct A { };
    struct B {
        B(const B &) { }
    };
    struct C {
        virtual void fn() { }
    };

    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< int >::value, std::is_trivially_copy_constructible< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< A >::value, std::is_trivially_copy_constructible< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< B >::value, std::is_trivially_copy_constructible< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< C >::value, std::is_trivially_copy_constructible< C >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< A >::value, Urho3D::IsTriviallyCopyConstructible_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyCopyConstructible< B >::value, Urho3D::IsTriviallyCopyConstructible_v< B >);
}

// Test IsNothrowCopyConstructible type-trait.
TEST_CASE("IsNothrowCopyConstructible")
{
    struct A { };
    struct B {
        B(const B &){ }
    };
    struct C {
        C(const C &) noexcept { }
    };

    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< int >::value, std::is_nothrow_copy_constructible< int >::value);
    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< A >::value, std::is_nothrow_copy_constructible< A >::value);
    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< B >::value, std::is_nothrow_copy_constructible< B >::value);
    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< C >::value, std::is_nothrow_copy_constructible< C >::value);
    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< A >::value, Urho3D::IsNothrowCopyConstructible_v< A >);
    CHECK_EQ(Urho3D::IsNothrowCopyConstructible< B >::value, Urho3D::IsNothrowCopyConstructible_v< B >);
}

// Test IsMoveConstructible type-trait.
TEST_CASE("IsMoveConstructible")
{
    struct A { };
    struct B {
        B(B &&) = delete;
    };

    CHECK_EQ(Urho3D::IsMoveConstructible< int >::value, std::is_move_constructible< int >::value);
    CHECK_EQ(Urho3D::IsMoveConstructible< A >::value, std::is_move_constructible< A >::value);
    CHECK_EQ(Urho3D::IsMoveConstructible< B >::value, std::is_move_constructible< B >::value);
    CHECK_EQ(Urho3D::IsMoveConstructible< A >::value, Urho3D::IsMoveConstructible_v< A >);
    CHECK_EQ(Urho3D::IsMoveConstructible< B >::value, Urho3D::IsMoveConstructible_v< B >);
}

// Test IsTriviallyMoveConstructible type-trait.
TEST_CASE("IsTriviallyMoveConstructible")
{
    struct A { };
    struct B {
        B(B &&) { }
    };
    struct C {
        virtual void fn() { }
    };

    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< int >::value, std::is_trivially_move_constructible< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< A >::value, std::is_trivially_move_constructible< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< B >::value, std::is_trivially_move_constructible< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< C >::value, std::is_trivially_move_constructible< C >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< A >::value, Urho3D::IsTriviallyMoveConstructible_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyMoveConstructible< C >::value, Urho3D::IsTriviallyMoveConstructible_v< C >);
}

// Test IsNoThrowMoveConstructible type-trait.
TEST_CASE("IsNoThrowMoveConstructible")
{
    struct A { };
    struct B {
        B(B &&) { }
    };
    struct C {
        C(C &&) noexcept { }
    };

    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< int >::value, std::is_nothrow_move_constructible< int >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< A >::value, std::is_nothrow_move_constructible< A >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< B >::value, std::is_nothrow_move_constructible< B >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< C >::value, std::is_nothrow_move_constructible< C >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< A >::value, Urho3D::IsNoThrowMoveConstructible_v< A >);
    CHECK_EQ(Urho3D::IsNoThrowMoveConstructible< C >::value, Urho3D::IsNoThrowMoveConstructible_v< C >);
}

// Test IsAssignable type-trait.
TEST_CASE("IsAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (const A &) { return *this; }
    };

    CHECK_EQ(Urho3D::IsAssignable< int, int >::value, std::is_assignable< int, int >::value);
    CHECK_EQ(Urho3D::IsAssignable< int &, int >::value, std::is_assignable< int &, int >::value);
    CHECK_EQ(Urho3D::IsAssignable< int, double >::value, std::is_assignable< int, double >::value);
    CHECK_EQ(Urho3D::IsAssignable< int&, double >::value, std::is_assignable< int&, double >::value);
    CHECK_EQ(Urho3D::IsAssignable< A, B >::value, std::is_assignable< A, B >::value);
    CHECK_EQ(Urho3D::IsAssignable< B, A >::value, std::is_assignable< B, A >::value);
    CHECK_EQ(Urho3D::IsAssignable< A &, const A & >::value, std::is_assignable< A &, const A & >::value);
    CHECK_EQ(Urho3D::IsAssignable< int, int >::value, Urho3D::IsAssignable_v< int, int >);
    CHECK_EQ(Urho3D::IsAssignable< int &, int >::value, Urho3D::IsAssignable_v< int &, int >);
}

// Test IsTriviallyAssignable type-trait.
TEST_CASE("IsTriviallyAssignable")
{
    using A = Dummy;
    struct B : A { };
    struct C {
        C & operator = (const A &) { return *this; }
    };

    CHECK_EQ(Urho3D::IsTriviallyAssignable< int, int >::value, std::is_trivially_assignable< int, int >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< int &, int >::value, std::is_trivially_assignable< int &, int >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< int, double >::value, std::is_trivially_assignable< int, double >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< int&, double >::value, std::is_trivially_assignable< int&, double >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< A, A >::value, std::is_trivially_assignable< A, A >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< A, B >::value, std::is_trivially_assignable< A, B >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< B, A >::value, std::is_trivially_assignable< B, A >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< C, A >::value, std::is_trivially_assignable< C, A >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< A &, const A & >::value, std::is_trivially_assignable< A &, const A & >::value);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< int, int >::value, Urho3D::IsTriviallyAssignable_v< int, int >);
    CHECK_EQ(Urho3D::IsTriviallyAssignable< int &, int >::value, Urho3D::IsTriviallyAssignable_v< int &, int >);
}

// Test IsNoThrowAssignable type-trait.
TEST_CASE("IsNoThrowAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (const A &) noexcept { return *this; }
        B & operator = (const B &) { return *this; }
    };

    CHECK_EQ(Urho3D::IsNoThrowAssignable< int, int >::value, std::is_nothrow_assignable< int, int >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< int &, int >::value, std::is_nothrow_assignable< int &, int >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< int, double >::value, std::is_nothrow_assignable< int, double >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< int&, double >::value, std::is_nothrow_assignable< int&, double >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< A, B >::value, std::is_nothrow_assignable< A, B >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< B, A >::value, std::is_nothrow_assignable< B, A >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< B, B >::value, std::is_nothrow_assignable< B, B >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< A &, const A & >::value, std::is_nothrow_assignable< A &, const A & >::value);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< int, int >::value, Urho3D::IsNoThrowAssignable_v< int, int >);
    CHECK_EQ(Urho3D::IsNoThrowAssignable< int &, int >::value, Urho3D::IsNoThrowAssignable_v< int &, int >);
}

// Test IsCopyAssignable type-trait.
TEST_CASE("IsCopyAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (const B &) = delete;
    };

    CHECK_EQ(Urho3D::IsCopyAssignable< int >::value, std::is_copy_assignable< int >::value);
    CHECK_EQ(Urho3D::IsCopyAssignable< int[2] >::value, std::is_copy_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsCopyAssignable< A >::value, std::is_copy_assignable< A >::value);
    CHECK_EQ(Urho3D::IsCopyAssignable< B >::value, std::is_copy_assignable< B >::value);
    CHECK_EQ(Urho3D::IsCopyAssignable< A >::value, Urho3D::IsCopyAssignable_v< A >);
    CHECK_EQ(Urho3D::IsCopyAssignable< B >::value, Urho3D::IsCopyAssignable_v< B >);
}

// Test IsTriviallyCopyAssignable type-trait.
TEST_CASE("IsTriviallyCopyAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (const B &) { return *this; }
    };

    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< int >::value, std::is_trivially_copy_assignable< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< int[2] >::value, std::is_trivially_copy_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< A >::value, std::is_trivially_copy_assignable< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< B >::value, std::is_trivially_copy_assignable< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< A >::value, Urho3D::IsTriviallyCopyAssignable_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyCopyAssignable< B >::value, Urho3D::IsTriviallyCopyAssignable_v< B >);
}

// Test IsNoThrowCopyAssignable type-trait.
TEST_CASE("IsNoThrowCopyAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (const B &) { return *this; }
    };
    struct C {
        C & operator = (const C &) noexcept { return *this; }
    };

    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< int >::value, std::is_nothrow_copy_assignable< int >::value);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< int[2] >::value, std::is_nothrow_copy_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< A >::value, std::is_nothrow_copy_assignable< A >::value);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< B >::value, std::is_nothrow_copy_assignable< B >::value);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< C >::value, std::is_nothrow_copy_assignable< C >::value);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< A >::value, Urho3D::IsNoThrowCopyAssignable_v< A >);
    CHECK_EQ(Urho3D::IsNoThrowCopyAssignable< C >::value, Urho3D::IsNoThrowCopyAssignable_v< C >);
}

// Test IsMoveAssignable type-trait.
TEST_CASE("IsMoveAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (B &&) = delete;
    };

    CHECK_EQ(Urho3D::IsMoveAssignable< int >::value, std::is_move_assignable< int >::value);
    CHECK_EQ(Urho3D::IsMoveAssignable< int[2] >::value, std::is_move_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsMoveAssignable< A >::value, std::is_move_assignable< A >::value);
    CHECK_EQ(Urho3D::IsMoveAssignable< B >::value, std::is_move_assignable< B >::value);
    CHECK_EQ(Urho3D::IsMoveAssignable< A >::value, Urho3D::IsMoveAssignable_v< A >);
    CHECK_EQ(Urho3D::IsMoveAssignable< B >::value, Urho3D::IsMoveAssignable_v< B >);
}

// Test IsTriviallyMoveAssignable type-trait.
TEST_CASE("IsTriviallyMoveAssignable")
{
    using A = Dummy;
    struct B {
        B & operator = (B &&) { return *this; }
    };

    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< int >::value, std::is_trivially_move_assignable< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< int[2] >::value, std::is_trivially_move_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< A >::value, std::is_trivially_move_assignable< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< B >::value, std::is_trivially_move_assignable< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< A >::value, Urho3D::IsTriviallyMoveAssignable_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyMoveAssignable< B >::value, Urho3D::IsTriviallyMoveAssignable_v< B >);
}

// Test IsNoThrowMoveAssignable type-trait.
TEST_CASE("IsNoThrowMoveAssignable")
{
    using A = Dummy;
#if defined(UH_MSC) && !defined(UH_CLANG)
    #pragma warning(disable: 26439) // This kind of function may not throw. Declare it 'noexcept'.
#endif
    struct B {
        B & operator = (B &&) { return *this; }
    };
    struct C {
        C & operator = (C &&) noexcept { return *this; }
    };

    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< int >::value, std::is_nothrow_move_assignable< int >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< int[2] >::value, std::is_nothrow_move_assignable< int[2] >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< A >::value, std::is_nothrow_move_assignable< A >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< B >::value, std::is_nothrow_move_assignable< B >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< C >::value, std::is_nothrow_move_assignable< C >::value);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< A >::value, Urho3D::IsNoThrowMoveAssignable_v< A >);
    CHECK_EQ(Urho3D::IsNoThrowMoveAssignable< C >::value, Urho3D::IsNoThrowMoveAssignable_v< C >);
}

// Test IsDestructible type-trait.
TEST_CASE("IsDestructible")
{
    using A = Dummy;
    struct B {
        ~B() = delete;
    };
#if defined(UH_MSC) && !defined(UH_CLANG)
    #pragma warning(disable: 4624) // 'derived class' : destructor was implicitly defined as deleted because a base class destructor is inaccessible or deleted.
#endif
    struct C : B {};

    CHECK_EQ(Urho3D::IsDestructible< int >::value, std::is_destructible< int >::value);
    CHECK_EQ(Urho3D::IsDestructible< int[2] >::value, std::is_destructible< int[2] >::value);
    CHECK_EQ(Urho3D::IsDestructible< A >::value, std::is_destructible< A >::value);
    CHECK_EQ(Urho3D::IsDestructible< B >::value, std::is_destructible< B >::value);
    CHECK_EQ(Urho3D::IsDestructible< C >::value, std::is_destructible< C >::value);
    CHECK_EQ(Urho3D::IsDestructible< A >::value, Urho3D::IsDestructible_v< A >);
    CHECK_EQ(Urho3D::IsDestructible< B >::value, Urho3D::IsDestructible_v< B >);
}

// Test IsTriviallyDestructible type-trait.
TEST_CASE("IsTriviallyDestructible")
{
    using A = Dummy;
    struct B {
        ~B() { }
    };

    CHECK_EQ(Urho3D::IsTriviallyDestructible< int >::value, std::is_trivially_destructible< int >::value);
    CHECK_EQ(Urho3D::IsTriviallyDestructible< int[2] >::value, std::is_trivially_destructible< int[2] >::value);
    CHECK_EQ(Urho3D::IsTriviallyDestructible< A >::value, std::is_trivially_destructible< A >::value);
    CHECK_EQ(Urho3D::IsTriviallyDestructible< B >::value, std::is_trivially_destructible< B >::value);
    CHECK_EQ(Urho3D::IsTriviallyDestructible< A >::value, Urho3D::IsTriviallyDestructible_v< A >);
    CHECK_EQ(Urho3D::IsTriviallyDestructible< B >::value, Urho3D::IsTriviallyDestructible_v< B >);
}

// Test IsNoThrowDestructible type-trait.
TEST_CASE("IsNoThrowDestructible")
{
    using A = Dummy;
    struct B {
        ~B() { }
    };
    struct C {
        ~C() noexcept(false) { }
    };

    CHECK_EQ(Urho3D::IsNoThrowDestructible< int >::value, std::is_nothrow_destructible< int >::value);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< int[2] >::value, std::is_nothrow_destructible< int[2] >::value);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< A >::value, std::is_nothrow_destructible< A >::value);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< B >::value, std::is_nothrow_destructible< B >::value);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< C >::value, std::is_nothrow_destructible< C >::value);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< A >::value, Urho3D::IsNoThrowDestructible_v< A >);
    CHECK_EQ(Urho3D::IsNoThrowDestructible< C >::value, Urho3D::IsNoThrowDestructible_v< C >);
}

// Test HasVirtualDestructor type-trait.
TEST_CASE("HasVirtualDestructor")
{
    using A = Dummy;
    struct B {
        virtual ~B() { }
    };
    struct C : B { };

    CHECK_EQ(Urho3D::HasVirtualDestructor< int >::value, std::has_virtual_destructor< int >::value);
    CHECK_EQ(Urho3D::HasVirtualDestructor< int[2] >::value, std::has_virtual_destructor< int[2] >::value);
    CHECK_EQ(Urho3D::HasVirtualDestructor< A >::value, std::has_virtual_destructor< A >::value);
    CHECK_EQ(Urho3D::HasVirtualDestructor< B >::value, std::has_virtual_destructor< B >::value);
    CHECK_EQ(Urho3D::HasVirtualDestructor< C >::value, std::has_virtual_destructor< C >::value);
    CHECK_EQ(Urho3D::HasVirtualDestructor< A >::value, Urho3D::HasVirtualDestructor_v< A >);
    CHECK_EQ(Urho3D::HasVirtualDestructor< C >::value, Urho3D::HasVirtualDestructor_v< C >);
}

// Test IsSwappableWith type-trait.
TEST_CASE("IsSwappableWith")
{
    //...
}

// Test IsSwappableWith type-trait.
TEST_CASE("IsSwappable")
{
    //...
}

TEST_SUITE_END();
