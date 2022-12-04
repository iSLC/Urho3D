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
    Urho3D::Undefined_t,
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
    XChar8_t
) {
    CHECK_EQ(Urho3D::IsIntegral< T >::value, std::is_integral< T >::value);
    CHECK_EQ(Urho3D::IsIntegral< T * >::value, std::is_integral< T * >::value);
    CHECK_EQ(Urho3D::IsIntegral< T & >::value, std::is_integral< T & >::value);
    CHECK_EQ(Urho3D::IsIntegral< const T >::value, std::is_integral< const T >::value);
    CHECK_EQ(Urho3D::IsIntegral< T >::value, Urho3D::IsIntegral_v< T >);
}

// Test IsFloatingPoint type-trait.
TEST_CASE_TEMPLATE("IsFloatingPoint", T,
    Urho3D::Undefined_t,
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
    XChar8_t
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
    struct A { };

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
    struct A { };

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
    struct A { };

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
    struct A { };
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
    struct A {};
    class B {};
    enum class E {};
    union U {
        class UC {};
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

int IsFunctin_Test(int i) { return i; } // function
template < class > struct IsFunctin_PMT {};
template < class T, class U > struct IsFunctin_PMT< U T:: * > { using member_type = U; };
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
    int(*b)(int) = IsFunctin_Test; // pointer to function
    struct C { int operator()(int i){return i;} } c; // function-like class
    using fun5_t = IsFunctin_PMT< decltype(&A::fun5) >::member_type; // T is int() const &

    CHECK_EQ(Urho3D::IsFunction< A >::value, std::is_function< A >::value);
    CHECK_EQ(Urho3D::IsFunction< int(int) >::value, std::is_function< int(int) >::value);
    CHECK_EQ(Urho3D::IsFunction< int(*)(int) >::value, std::is_function< int(*)(int) >::value);
    CHECK_EQ(Urho3D::IsFunction< decltype(IsFunctin_Test) >::value, std::is_function< decltype(IsFunctin_Test) >::value);
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
    struct A { };

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
    struct A { };

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
    struct A { };

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
    struct A {
        int x;
    };

    CHECK_EQ(Urho3D::IsMemberObjectPointer< A * >::value, std::is_member_object_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int A::* >::value, std::is_member_object_pointer< int A::* >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, std::is_member_object_pointer< int(A::*) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, std::is_member_object_pointer< int(A::*)() >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< decltype(&A::x) >::value, std::is_member_object_pointer< decltype(&A::x) >::value);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*) >::value, Urho3D::IsMemberObjectPointer_v< int(A::*) >);
    CHECK_EQ(Urho3D::IsMemberObjectPointer< int(A::*)() >::value, Urho3D::IsMemberObjectPointer_v< int(A::*)() >);
}

// Test IsMemberFunctionPointer type-trait.
TEST_CASE("IsMemberFunctionPointer")
{
    struct A {
        void m() { }
    };

    CHECK_EQ(Urho3D::IsMemberFunctionPointer< A * >::value, std::is_member_function_pointer< A * >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< void(A::*)() >::value, std::is_member_function_pointer< void(A::*)() >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< decltype(&A::m) >::value, std::is_member_function_pointer< decltype(&A::m) >::value);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< A * >::value, Urho3D::IsMemberFunctionPointer_v< A * >);
    CHECK_EQ(Urho3D::IsMemberFunctionPointer< void(A::*)() >::value, Urho3D::IsMemberFunctionPointer_v< void(A::*)() >);
}

TEST_SUITE_END();
