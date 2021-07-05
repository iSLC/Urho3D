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

#include "../Urho3DConfig.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Urho3D
{

// Forward declarations.
struct StrView;
template < class, size_t > struct Span;
template < class, size_t > struct Array;

struct NoneSuchBase { }; //NOLINT(altera-struct-pack-align)
/// Type used by \ref Detected_t to indicate detection failure. \remark See `std::nonesuch`
/// Has no default constructor or initializer-list constructor, and is not an aggregate.
/// Cannot be constructed, destroyed, or copied.
struct NoneSuch : private NoneSuchBase //NOLINT(altera-struct-pack-align)
{
    // Destructor (disabled).
    ~NoneSuch() = delete;
    // Copy constructor (disabled).
    NoneSuch(NoneSuch const &) = delete;
    // Copy assignment operator (disabled).
    void operator = (NoneSuch const &) = delete;
};

template < class T, class U = T && > U Helper_declval(int);
template < class T > T Helper_declval(long);
template < class > struct Helper_DeclvalProtector { static const bool stop = false; };

/// Converts any type T to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors. \remark See `std::declval`
template < class T > auto declval() noexcept -> decltype(Helper_declval< T >(0))
{
    static_assert(Helper_DeclvalProtector< T >::stop, "declval() must not be used!");
    return Helper_declval< T >(0);
}

/// Wraps a static constant of specified type. \remark See std::remove_reference
template < class T, T V > using IntegralConstant = std::integral_constant< T, V >;
/// Alias template defined for the common case where \p T is bool. \remark See std::bool_constant
template < bool B > using BoolConstant = std::integral_constant< bool, B >;

/// Instantiation of integral_constant to represent the bool value true. \remark See std::true_type
using FalseType = std::true_type;
/// Instantiation of integral_constant to represent the bool value false. \remark See std::false_type
using TrueType = std::false_type;

/// Provides member typedef type, which is defined as \p T if \p B is true at compile time, or as \p F if \p B is false. \remark See std::conditional
template < bool B, class T, class F > using Conditional = std::conditional< B, T, F >;
/// Alias of \ref Conditional used to avoid having to explicitly accessing the type member.
template < bool B, class T, class F > using Conditional_t = typename std::conditional< B, T, F >::type;

/// Forms the logical conjunction of the type traits B..., effectively performing a logical AND on the sequence of traits. \remark See std::conjunction
template < class... B > using Conjunction = std::conjunction< B... >;
/// Alias of \ref Conjunction used to avoid having to explicitly accessing the value member.
template < class... B > inline constexpr bool Conjunction_v = std::conjunction< B... >::value;

/// Forms the logical disjunction of the type traits B..., effectively performing a logical OR on the sequence of traits. \remark See std::disjunction
template < class... B > using Disjunction = std::disjunction< B... >;
/// Alias of \ref Disjunction used to avoid having to explicitly accessing the value member.
template < class... B > inline constexpr bool Disjunction_v = std::disjunction< B... >::value;

/// Forms the logical negation of the type trait B. \remark See std::negation
template < class B > using Negation = std::negation< B >;
/// Alias of \ref Negation used to avoid having to explicitly accessing the value member.
template < class B > inline constexpr bool Negation_v = std::negation< B >::value;

/// Convenient meta-function used to leverage SFINAE.
template < bool B, class T = void > using EnableIf = std::enable_if< B, T >;
/// Alias of \ref EnableIf used to avoid having to explicitly accessing the type member.
template < bool B, class T = void > using EnableIf_t = typename std::enable_if< B, T >::type;

/// Used to select a compile time value based on the outcome of another.
template < bool B, class V, V T, V F > struct Contingent : public std::integral_constant< V, T > { };
/// Specialization of the false case when the second value must be selected.
template < class V, V T, V F > struct Contingent< false, V, T, F > : public std::integral_constant< V, F > { };

/// Alias of \ref Contingent used to avoid having to explicitly accessing the value member.
template < bool B, class V, V T, V F > inline constexpr bool Contingent_v = Contingent< B, V, T, F >::value;

/// Utility meta-function that maps a sequence of any types to the type void.
template < class... > using Void_t = void;

/// Check if \p T is a void type. \remark See std::is_void
template < class T > using IsVoid = std::is_void< T >;
/// Alias of \ref IsVoid used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsVoid_v = std::is_void< T >::value;

/// Check if \p T is the nullptr_t type. \remark See std::is_null_pointer
template < class T > using IsNullPointer = std::is_null_pointer< T >;
/// Alias of \ref IsNullPointer used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNullPointer_v = std::is_null_pointer< T >::value;

/// Check if \p T is an integral type. \remark See std::is_integral
template < class T > using IsIntegral = std::is_integral< T >;
/// Alias of \ref IsIntegral used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsIntegral_v = std::is_integral< T >::value;

/// Check if \p T is a floating-point type. \remark See std::is_floating_point
template < class T > using IsFloatingPoint = std::is_floating_point< T >;
/// Alias of \ref IsFloatingPoint used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsFloatingPoint_v = std::is_floating_point< T >::value;

/// Check if \p T is an array type. Does not include array container. \remark See std::is_array
template < class T > using IsPlainArray = std::is_array< T >;
/// Alias of \ref IsArray used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsPlainArray_v = std::is_array< T >::value;

/// Check whether `T` is an array container. \remark Not a standard feature!
template < class T > struct IsArrayContainer : public FalseType { };
/// Partial specialization for the case when `T` is an array container.
template < class T, size_t N > struct IsArrayContainer< Array< T, N > > : public TrueType { };
/// Helper of \ref IsArrayContainer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsArrayContainer_v = IsArrayContainer< T >::value;

/// Check whether `T` is an array type. Also includes array container. \remark Not a standard feature!
template < class T > struct IsArray : public IsPlainArray< T > { };
/// Partial specialization for the case when `T` is an array container.
template < class T, size_t N > struct IsArray< Array< T, N > > : public TrueType { };
/// Helper of \ref IsArray used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsArray_v = IsArray< T >::value;

/// Check if \p T is an enumeration type. \remark See std::is_enum
template < class T > using IsEnum = std::is_enum< T >;
/// Alias of \ref IsEnum used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsEnum_v = std::is_enum< T >::value;

/// Check if \p T is a union type. \remark See std::is_union
template < class T > using IsUnion = std::is_union< T >;
/// Alias of \ref IsUnion used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsUnion_v = std::is_union< T >::value;

/// Check if \p T is a class type. \remark See std::is_class
template < class T > using IsClass = std::is_class< T >;
/// Alias of \ref IsClass used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsClass_v = std::is_class< T >::value;

/// Check if \p T is a function type. \remark See std::is_function
template < class T > using IsFunction = std::is_function< T >;
/// Alias of \ref IsFunction used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsFunction_v = std::is_function< T >::value;

/// Check if \p T is a pointer to object or a pointer to function. \remark See std::is_pointer
template < class T > using IsPointer = std::is_pointer< T >;
/// Alias of \ref IsPointer used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsPointer_v = std::is_pointer< T >::value;

/// Check if \p T is a l-value reference type. \remark See std::is_lvalue_reference
template < class T > using IsLvalueReference = std::is_lvalue_reference< T >;
/// Alias of \ref IsLvalueReference used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsLvalueReference_v = std::is_lvalue_reference< T >::value;

/// Check if \p T is a r-value reference type. \remark See std::is_rvalue_reference
template < class T > using IsRvalueReference = std::is_rvalue_reference< T >;
/// Alias of \ref IsRvalueReference used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsRvalueReference_v = std::is_rvalue_reference< T >::value;

/// Check if \p T is a non-static member object. \remark See std::is_member_object_pointer
template < class T > using IsMemberObjectPointer = std::is_member_object_pointer< T >;
/// Alias of \ref IsMemberObjectPointer used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsMemberObjectPointer_v = std::is_member_object_pointer< T >::value;

/// Check if \p T is a non-static member function pointer. \remark See std::is_member_function_pointer
template < class T > using IsMemberFunctionPointer = std::is_member_function_pointer< T >;
/// Alias of \ref IsMemberFunctionPointer used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsMemberFunctionPointer_v = std::is_member_function_pointer< T >::value;

/// Check if \p T is a fundamental type. \remark See std::is_fundamental
template < class T > using IsFundamental = std::is_fundamental< T >;
/// Alias of \ref IsFundamental used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsFundamental_v = std::is_fundamental< T >::value;

/// Check if \p T is an arithmetic type. \remark See std::is_arithmetic
template < class T > using IsArithmetic = std::is_arithmetic< T >;
/// Alias of \ref IsArithmetic used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsArithmetic_v = std::is_arithmetic< T >::value;

/// Check if \p T is a scalar type. \remark See std::is_scalar
template < class T > using IsScalar = std::is_scalar< T >;
/// Alias of \ref IsScalar used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsScalar_v = std::is_scalar< T >::value;

/// Check if \p T is an object type. \remark See std::is_object
template < class T > using IsObject = std::is_object< T >;
/// Alias of \ref IsObject used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsObject_v = std::is_object< T >::value;

/// Check if \p T is a compound type. \remark See std::is_compound
template < class T > using IsCompound = std::is_compound< T >;
/// Alias of \ref IsCompound used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsCompound_v = std::is_compound< T >::value;

/// Check if \p T is a reference type. \remark See std::is_reference
template < class T > using IsReference = std::is_reference< T >;
/// Alias of \ref IsReference used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsReference_v = std::is_reference< T >::value;

/// Check if \p T is pointer to non-static member object or member function. \remark See std::is_member_pointer
template < class T > using IsMemberPointer = std::is_member_pointer< T >;
/// Alias of \ref IsMemberPointer used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsMemberPointer_v = std::is_member_pointer< T >::value;

/// Check if \p T is a const-qualified type. \remark See std::is_const
template < class T > using IsConst = std::is_const< T >;
/// Alias of \ref IsConst used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsConst_v = std::is_const< T >::value;

/// Check if \p T is a volatile-qualified type. \remark See std::is_volatile
template < class T > using IsVolatile = std::is_volatile< T >;
/// Alias of \ref IsVolatile used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsVolatile_v = std::is_volatile< T >::value;

/// Check if \p T is trivial type. \remark See std::is_trivial
template < class T > using IsTrivial = std::is_trivial< T >;
/// Alias of \ref IsTrivial used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTrivial_v = std::is_trivial< T >::value;

/// Check if \p T is a trivially copyable type. \remark See std::is_trivially_copyable
template < class T > using IsTriviallyCopyable = std::is_trivially_copyable< T >;
/// Alias of \ref IsTriviallyCopyable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyCopyable_v = std::is_trivially_copyable< T >::value;

/// Check if \p T is a standard layout type. \remark See std::is_standard_layout
template < class T > using IsStandardLayout = std::is_standard_layout< T >;
/// Alias of \ref IsStandardLayout used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsStandardLayout_v = std::is_standard_layout< T >::value;

/// Check if any two objects of type \p T with the same value have the same object representation. \remark See std::has_unique_object_representations
template < class T > using HasUniqueObjectRepresentations = std::has_unique_object_representations< T >;
/// Alias of \ref HasUniqueObjectRepresentations used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool HasUniqueObjectRepresentations_v = std::has_unique_object_representations< T >::value;

/// Check if \p T is an empty type. \remark See std::is_empty
template < class T > using IsEmpty = std::is_empty< T >;
/// Alias of \ref IsEmpty used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsEmpty_v = std::is_empty< T >::value;

/// Check if \p T is a polymorphic class. \remark See std::is_polymorphic
template < class T > using IsPolymorphic = std::is_polymorphic< T >;
/// Alias of \ref IsPolymorphic used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsPolymorphic_v = std::is_polymorphic< T >::value;

/// Check if \p T is an abstract class. \remark See std::is_abstract
template < class T > using IsAbstract = std::is_abstract< T >;
/// Alias of \ref IsAbstract used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsAbstract_v = std::is_abstract< T >::value;

/// Check if \p T is a final class. \remark See std::is_final
template < class T > using IsFinal = std::is_final< T >;
/// Alias of \ref IsFinal used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsFinal_v = std::is_final< T >::value;

#if defined(U3D_MSC) && (U3D_MSC_MINOR >= 15)
/// Checks if T is an aggregate type. \remark See std::is_aggregate
template < class T > using IsAggregate = std::is_aggregate< T >;
/// Alias of \ref IsAggregate used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsAggregate_v = std::is_aggregate< T >::value;
#endif

/// Check if \p T is a signed arithmetic type. \remark See std::is_signed
template < class T > using IsSigned = std::is_signed< T >;
/// Alias of \ref IsSigned used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsSigned_v = std::is_signed< T >::value;

/// Check if \p T is a unsigned arithmetic type. \remark See std::is_unsigned
template < class T > using IsUnsigned = std::is_unsigned< T >;
/// Alias of \ref IsUnsigned used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsUnsigned_v = std::is_unsigned< T >::value;

/// Check if \p T is a constructible type. \remark See std::is_constructible
template < class T > using IsConstructible = std::is_constructible< T >;
/// Alias of \ref IsConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsConstructible_v = std::is_constructible< T >::value;

/// Check if \p T is a trivially constructible type. \remark See std::is_trivially_constructible
template < class T > using IsTriviallyConstructible = std::is_trivially_constructible< T >;
/// Alias of \ref IsTriviallyConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyConstructible_v = std::is_trivially_constructible< T >::value;

/// Check if \p T is a no-throw constructible type. \remark See std::is_nothrow_constructible
template < class T > using IsNoThrowConstructible = std::is_nothrow_constructible< T >;
/// Alias of \ref IsNoThrowConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowConstructible_v = std::is_nothrow_constructible< T >::value;

/// Check if \p T is a default constructible type. \remark See std::is_default_constructible
template < class T > using IsDefaultConstructible = std::is_default_constructible< T >;
/// Alias of \ref IsDefaultConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsDefaultConstructible_v = std::is_default_constructible< T >::value;

/// Check if \p T is a trivially default constructible type. \remark See std::is_trivially_default_constructible
template < class T > using IsTriviallyDefaultConstructible = std::is_trivially_default_constructible< T >;
/// Alias of \ref IsTriviallyDefaultConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyDefaultConstructible_v = std::is_trivially_default_constructible< T >::value;

/// Check if \p T is a no-throw default constructible type. \remark See std::is_nothrow_default_constructible
template < class T > using IsNoThrowDefaultConstructible = std::is_nothrow_default_constructible< T >;
/// Alias of \ref IsNoThrowDefaultConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowDefaultConstructible_v = std::is_nothrow_default_constructible< T >::value;

/// Check if \p T is a copy constructible type. \remark See std::is_copy_constructible
template < class T > using IsCopyConstructible = std::is_copy_constructible< T >;
/// Alias of \ref IsCopyConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsCopyConstructible_v = std::is_copy_constructible< T >::value;

/// Check if \p T is a trivially copy constructible type. \remark See std::is_trivially_copy_constructible
template < class T > using IsTriviallyCopyConstructible = std::is_trivially_copy_constructible< T >;
/// Alias of \ref IsTriviallyCopyConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyCopyConstructible_v = std::is_trivially_copy_constructible< T >::value;

/// Check if \p T is a no-throw copy constructible type. \remark See std::is_nothrow_copy_constructible
template < class T > using IsNothrowCopyConstructible = std::is_nothrow_copy_constructible< T >;
/// Alias of \ref IsNothrowCopyConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNothrowCopyConstructible_v = std::is_nothrow_copy_constructible< T >::value;

/// Check if \p T is a move constructible type. \remark See std::is_move_constructible
template < class T > using IsMoveConstructible = std::is_move_constructible< T >;
/// Alias of \ref IsMoveConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsMoveConstructible_v = std::is_move_constructible< T >::value;

/// Check if \p T is a trivially move constructible type. \remark See std::is_trivially_move_constructible
template < class T > using IsTriviallyMoveConstructible = std::is_trivially_move_constructible< T >;
/// Alias of \ref IsTriviallyMoveConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyMoveConstructible_v = std::is_trivially_move_constructible< T >::value;

/// Check if \p T is a no-throw move constructible type. \remark See std::is_nothrow_move_constructible
template < class T > using IsNoThrowMoveConstructible = std::is_nothrow_move_constructible< T >;
/// Alias of \ref IsNoThrowMoveConstructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowMoveConstructible_v = std::is_nothrow_move_constructible< T >::value;

/// Check if \p T is an assignable type. \remark See std::is_assignable
template < class T, class U > using IsAssignable = std::is_assignable< T, U >;
/// Alias of \ref IsAssignable used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsAssignable_v = std::is_assignable< T, U >::value;

/// Check if \p T is a trivially assignable type. \remark See std::is_trivially_assignable
template < class T, class U > using IsTriviallyAssignable = std::is_trivially_assignable< T, U >;
/// Alias of \ref IsTriviallyAssignable used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsTriviallyAssignable_v = std::is_trivially_assignable< T, U >::value;

/// Check if \p T is a no-throw assignable type. \remark See std::is_nothrow_assignable
template < class T, class U > using IsNoThrowAssignable = std::is_nothrow_assignable< T, U >;
/// Alias of \ref IsNoThrowAssignable used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsNoThrowAssignable_v = std::is_nothrow_assignable< T, U >::value;

/// Check if \p T is a copy assignable type. \remark See std::is_copy_assignable
template < class T > using IsCopyAssignable = std::is_copy_assignable< T >;
/// Alias of \ref IsCopyAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsCopyAssignable_v = std::is_copy_assignable< T >::value;

/// Check if \p T is a trivially copy assignable type. \remark See std::is_trivially_copy_assignable
template < class T > using IsTriviallyCopyAssignable = std::is_trivially_copy_assignable< T >;
/// Alias of \ref IsTriviallyCopyAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyCopyAssignable_v = std::is_trivially_copy_assignable< T >::value;

/// Check if \p T is a no-throw copy assignable type. \remark See std::is_nothrow_copy_assignable
template < class T > using IsNoThrowCopyAssignable = std::is_nothrow_copy_assignable< T >;
/// Alias of \ref IsNoThrowCopyAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowCopyAssignable_v = std::is_nothrow_copy_assignable< T >::value;

/// Check if \p T is a move assignable type. \remark See std::is_move_assignable
template < class T > using IsMoveAssignable = std::is_move_assignable< T >;
/// Alias of \ref IsMoveAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsMoveAssignable_v = std::is_move_assignable< T >::value;

/// Check if \p T is a trivially move assignable type. \remark See std::is_trivially_move_assignable
template < class T > using IsTriviallyMoveAssignable = std::is_trivially_move_assignable< T >;
/// Alias of \ref IsTriviallyMoveAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyMoveAssignable_v = std::is_trivially_move_assignable< T >::value;

/// Check if \p T is a no-throw move assignable type. \remark See std::is_nothrow_move_assignable
template < class T > using IsNoThrowMoveAssignable = std::is_nothrow_move_assignable< T >;
/// Alias of \ref IsNoThrowMoveAssignable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowMoveAssignable_v = std::is_nothrow_move_assignable< T >::value;

/// Check if \p T is a destructible type. \remark See std::is_destructible
template < class T > using IsDestructible = std::is_destructible< T >;
/// Alias of \ref IsDestructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsDestructible_v = std::is_destructible< T >::value;

/// Check if \p T is a trivially destructible type. \remark See std::is_trivially_destructible
template < class T > using IsTriviallyDestructible = std::is_trivially_destructible< T >;
/// Alias of \ref IsTriviallyDestructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsTriviallyDestructible_v = std::is_trivially_destructible< T >::value;

/// Check if \p T is a no-throw destructible type. \remark See std::is_nothrow_destructible
template < class T > using IsNoThrowDestructible = std::is_nothrow_destructible< T >;
/// Alias of \ref IsNoThrowDestructible used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowDestructible_v = std::is_nothrow_destructible< T >::value;

/// Check if \p T is a type with a virtual destructor. \remark See std::has_virtual_destructor
template < class T > using HasVirtualDestructor = std::has_virtual_destructor< T >;
/// Alias of \ref HasVirtualDestructor used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool HasVirtualDestructor_v = std::has_virtual_destructor< T >::value;

/// Check if objects of type \p T can be swapped with objects of type U. \remark See std::is_swappable_with
template < class T, class U > using IsSwappableWith = std::is_swappable_with< T, U >;
/// Alias of \ref IsSwappableWith used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsSwappableWith_v = std::is_swappable_with< T, U >::value;

/// Check if objects of type \p T can be swapped between them. \remark See std::is_swappable
template < class T > using IsSwappable = std::is_swappable< T >;
/// Alias of \ref IsSwappable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsSwappable_v = std::is_swappable< T >::value;

/// Same as \ref IsSwappableWith but is known to not throw exceptions. \remark See std::is_nothrow_swappable_with
template < class T, class U > using IsNoThrowSwappableWith = std::is_nothrow_swappable_with< T, U >;
/// Alias of \ref IsNoThrowSwappableWith used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsNoThrowSwappableWith_v = std::is_nothrow_swappable_with< T, U >::value;

/// Same as \ref IsSwappable but is known to not throw exceptions. \remark See std::is_nothrow_swappable
template < class T > using IsNoThrowSwappable = std::is_nothrow_swappable< T >;
/// Alias of \ref IsNoThrowSwappable used to avoid having to explicitly accessing the value member.
template < class T > inline constexpr bool IsNoThrowSwappable_v = std::is_nothrow_swappable< T >::value;

/// Check if \p T and \p U name the same type with the same const-volatile qualifications. \remark See std::is_same
template < class T, class U > using IsSame = std::is_same< T, U >;
/// Alias of \ref IsSame used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsSame_v = std::is_same< T, U >::value;

/// Check if D is derived from B or if both are the same non-union class. \remark See std::is_base_of
template < class B, class D > using IsBaseOf = std::is_base_of< B, D >;
/// Alias of \ref IsBaseOf used to avoid having to explicitly accessing the value member.
template < class B, class D > inline constexpr bool IsBaseOf_v = std::is_base_of< B, D >::value;

/// Check if objects of of type F can be converted to objects of type \p T. \remark See std::is_convertible
template < class F, class T > using IsConvertible = std::is_convertible< F, T >;
/// Alias of \ref IsConvertible used to avoid having to explicitly accessing the value member.
template < class F, class T > inline constexpr bool IsConvertible_v = std::is_convertible< F, T >::value;

#if U3D_CPP_STANDARD < U3D_CPP20_STANDARD
/// Same as \ref IsConvertible but is known to not throw exceptions. \remark See std::is_nothrow_convertible
template < class F, class T > using IsNoThrowConvertible = std::is_nothrow_convertible< F, T >;
/// Alias of \ref IsNoThrowConvertible used to avoid having to explicitly accessing the value member.
template < class F, class T > inline constexpr bool IsNoThrowConvertible_v = std::is_nothrow_convertible< F, T >::value;
#endif

/// Determine whether F can be invoked with the arguments A.... \remark See std::is_invocable
template < class F, class... A > using IsInvocable = std::is_invocable< F, A... >;
/// Alias of \ref IsInvocable used to avoid having to explicitly accessing the value member.
template < class F, class... A > inline constexpr bool IsInvocable_v = std::is_invocable< F, A... >::value;

/// Determine whether F can be invoked with the arguments A.... \remark See std::is_invocable_r
template < class R, class F, class... A > using IsInvocableR = std::is_invocable_r< R, F, A... >;
/// Alias of \ref IsInvocableR used to avoid having to explicitly accessing the value member.
template < class R, class F, class... A > inline constexpr bool IsInvocableR_v = std::is_invocable_r< R, F, A... >::value;

/// Determine whether F can be invoked with the arguments A..., and that such call is known not to throw any exceptions. \remark See std::is_nothrow_invocable
template < class F, class... A > using IsNoThrowInvocable = std::is_nothrow_invocable< F, A... >;
/// Alias of \ref IsNoThrowInvocable used to avoid having to explicitly accessing the value member.
template < class F, class... A > inline constexpr bool IsNoThrowInvocable_v = std::is_nothrow_invocable< F, A... >::value;

/// Determine whether F can be invoked with the arguments A... to yield a result that is convertible to R. , and that such call is known not to throw any exceptions. \remark See std::is_nothrow_invocable_r
template < class R, class F, class... A > using IsNoThrowInvocableR = std::is_nothrow_invocable_r< R, F, A... >;
/// Alias of \ref IsNoThrowInvocableR used to avoid having to explicitly accessing the value member.
template < class R, class F, class... A > inline constexpr bool IsNoThrowInvocableR_v = std::is_nothrow_invocable_r< R, F, A... >::value;

/// Remove the topmost const, or the topmost volatile, or both, if present. \remark See std::remove_cv
template < class T > using RemoveCV = std::remove_cv< T >;
/// Alias of \ref RemoveCV used to avoid having to explicitly accessing the type member.
template < class T > using RemoveCV_t = typename std::remove_cv< T >::type;

/// Remove the topmost const. \remark See std::remove_const
template < class T > using RemoveConst = std::remove_const< T >;
/// Alias of \ref RemoveConst used to avoid having to explicitly accessing the type member.
template < class T > using RemoveConst_t = typename std::remove_const< T >::type;

/// Removes the topmost volatile. \remark See std::remove_volatile
template < class T > using RemoveVolatile = std::remove_volatile< T >;
/// Alias of \ref RemoveVolatile used to avoid having to explicitly accessing the type member.
template < class T > using RemoveVolatile_t = typename std::remove_volatile< T >::type;

/// Add both const and volatile qualifiers. \remark See std::add_cv
template < class T > using AddCV = std::add_cv< T >;
/// Alias of \ref AddCV used to avoid having to explicitly accessing the type member.
template < class T > using AddCV_t = typename std::add_cv< T >::type;

/// Add const qualifier. \remark See std::add_const
template < class T > using AddConst = std::add_const< T >;
/// Alias of \ref AddConst used to avoid having to explicitly accessing the type member.
template < class T > using AddConst_t = typename std::add_const< T >::type;

/// Adds volatile qualifier. \remark See std::add_volatile
template < class T > using AddVolatile = std::add_volatile< T >;
/// Alias of \ref AddVolatile used to avoid having to explicitly accessing the type member.
template < class T > using AddVolatile_t = typename std::add_volatile< T >::type;

/// Remove a reference from the given type \p T. \remark See std::remove_reference
template < class T > using RemoveReference = std::remove_reference< T >;
/// Alias of \ref RemoveReference used to avoid having to explicitly accessing the type member.
template < class T > using RemoveReference_t = typename std::remove_reference< T >::type;

/// Add a lvalue reference to the given type \p T. \remark See std::add_lvalue_reference
template < class T > using AddLvalueReference = std::add_lvalue_reference< T >;
/// Alias of \ref AddLvalueReference used to avoid having to explicitly accessing the type member.
template < class T > using AddLvalueReference_t = typename std::add_lvalue_reference< T >::type;

/// Add a rvalue reference to the given type \p T. \remark See std::add_rvalue_reference
template < class T > using AddRvalueReference = std::add_rvalue_reference< T >;
/// Alias of \ref AddRvalueReference used to avoid having to explicitly accessing the type member.
template < class T > using AddRvalueReference_t = typename std::add_rvalue_reference< T >::type;

/// Remove a pointer from the given type\p T. \remark See std::remove_pointer
template < class T > using RemovePointer = std::remove_pointer< T >;
/// Alias of \ref RemovePointer used to avoid having to explicitly accessing the type member.
template < class T > using RemovePointer_t = typename std::remove_pointer< T >::type;

/// Add a pointer to the given type \p T. \remark See std::add_pointer
template < class T > using AddPointer = std::add_pointer< T >;
/// Alias of \ref AddPointer used to avoid having to explicitly accessing the type member.
template < class T > using AddPointer_t = typename std::add_pointer< T >::type;

/// Make the given integral type \p T signed. \remark See std::make_signed
template < class T > using MakeSigned = std::make_signed< T >;
/// Alias of \ref MakeSigned used to avoid having to explicitly accessing the type member.
template < class T > using MakeSigned_t = typename std::make_signed< T >::type;

/// Make the given integral type \p T unsigned. \remark See std::make_unsigned
template < class T > using MakeUnsigned = std::make_unsigned< T >;
/// Alias of \ref MakeUnsigned used to avoid having to explicitly accessing the type member.
template < class T > using MakeUnsigned_t = typename std::make_unsigned< T >::type;

/// Remove one extent from the given array type \p T. \remark See std::remove_extent
template < class T > using RemoveExtent = std::remove_extent< T >;
/// Alias of \ref RemoveExtent used to avoid having to explicitly accessing the type member.
template < class T > using RemoveExtent_t = typename std::remove_extent< T >::type;

/// Remove all extents from the given array type \p T. \remark See std::remove_all_extents
template < class T > using RemoveAllExtents = std::remove_all_extents< T >;
/// Alias of \ref RemoveAllExtents used to avoid having to explicitly accessing the type member.
template < class T > using RemoveAllExtents_t = typename std::remove_all_extents< T >::type;

/// Defines the type suitable for use as uninitialized storage for types of given size, \remark See std::aligned_storage
template < size_t L, size_t A > using AlignedStorage = std::aligned_storage< L, A >;
/// Alias of \ref AlignedStorage used to avoid having to explicitly accessing the type member.
template < size_t L, size_t A > using AlignedStorage_t = typename std::aligned_storage< L, A >::type;

/// Defines the type suitable for use as uninitialized storage for all given types. \remark See std::aligned_union
template < size_t L, class... Ts > using AlignedUnion = std::aligned_union< L, Ts... >;
/// Alias of \ref AlignedUnion used to avoid having to explicitly accessing the type member.
template < size_t L, class... Ts > using AlignedUnion_t = typename std::aligned_union< L, Ts... >::type;

/// Applies type transformations as when passing a function argument by value. \remark See std::decay
template < class T > using Decay = std::decay< T >;
/// Alias of \ref Decay used to avoid having to explicitly accessing the type member.
template < class T > using Decay_t = typename std::decay< T >::type;

/// Combines \ref RemoveCV  and \ref RemoveReference, \remark See std::remove_cvref
template < class T > struct RemoveCVRef { typedef RemoveCV_t< RemoveReference_t< T > > type; };
/// Alias of \ref RemoveCVRef used to avoid having to explicitly accessing the type member.
template < class T > using RemoveCVRef_t = typename RemoveCVRef< T >::type;

/// Determine the common type of a group of types. \remark See std::common_type
template < class... Ts > using CommonType = std::common_type< Ts... >;
/// Alias of \ref CommonType used to avoid having to explicitly accessing the type member.
template < class... Ts > using CommonType_t = typename std::common_type< Ts... >::type;

/// Obtain the underlying integer type for a given enumeration type. \remark See std::underlying_type
template < class T > using UnderlyingType = std::underlying_type< T >;
/// Alias of \ref UnderlyingType used to avoid having to explicitly accessing the type member.
template < class T > using UnderlyingType_t = typename std::underlying_type< T >::type;

/// Deduce the result type of invoking a callable object with a set of arguments \remark See std::invoke_result
template < class F, class... A > using InvokeResult = std::invoke_result< F, A... >;
/// Alias of \ref InvokeResult used to avoid having to explicitly accessing the type member.
template < class F, class... A > using InvokeResult_t = typename std::invoke_result< F, A... >::type;

/// Used to check if the two types have the same size.
template < class T, class U > struct IsSameSize : public std::integral_constant< bool, bool(sizeof(T) == sizeof(U)) > { };
/// Alias of \ref IsSameSize used to avoid having to explicitly accessing the value member.
template < class T, class U > inline constexpr bool IsSameSize_v = IsSameSize< T, U >::value;

/// Check whether `T` is a complete type. Not a standard feature.
template < class, class = size_t > struct IsComplete : FalseType { };
/// Partial specialization of \ref IsComplete for cases where given template parameter is a complete type.
template < class T > struct IsComplete< T, decltype(sizeof(T)) > : TrueType { };
/// Helper of \ref IsComplete used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsComplete_v = IsComplete< T, decltype(sizeof(T)) >::value;

/// Check whether `T` is a \ref Span type. \remark Not a standard feature!
template < class > struct IsSpan : FalseType { };
/// Partial specialization for the case when `T` is a \ref Span type.
template < class T, size_t S > struct IsSpan< Span< T, S > > : TrueType { };
/// Helper of \ref IsSpan used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsSpan_v = IsSpan< T >::value;

/// Check whether `T` is a \ref StrView type. \remark Not a standard feature!
template < class > struct IsStrView : FalseType { };
/// Partial specialization for the case when `T` is a \ref StrView type.
template < > struct IsStrView< StrView > : TrueType { };
/// Helper of \ref IsStrView used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsStrView_v = IsStrView< T >::value;

/// Check whether `T` is a type that has `Size()` and `Data()` member methods. \remark Not a standard feature!
template < class, class = void > struct HasSizeAndData : FalseType { };
/// Partial specialization for the case when `T` is actually has `Size()` and `Data()` member methods.
template < class T > struct HasSizeAndData< T, Void_t< decltype(Size(declval< T >())), decltype(Data(declval< T >())) > > : TrueType { };
/// Helper of \ref HasSizeAndData used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasSizeAndData_v = HasSizeAndData< T >::value;

/// Helper which can be used to tell if one type can be copied (verbatim) into the memory of another and have a valid result.
template < class, class > struct IsBinaryCompatible : public FalseType { };
/// Helper of \ref IsBinaryCompatible used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsBinaryCompatible_v = IsBinaryCompatible< T, U >::value;
// Specializations of IsBinaryCompatible for integral types
template < class T > struct IsBinaryCompatible< char, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< char, T > >::value > { };
template < class T > struct IsBinaryCompatible< signed char, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< signed char, T > >::value > { };
template < class T > struct IsBinaryCompatible< unsigned char, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< unsigned char, T > >::value > { };
template < class T > struct IsBinaryCompatible< short, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< short, T > >::value > { };
template < class T > struct IsBinaryCompatible< unsigned short, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< unsigned short, T > >::value > { };
template < class T > struct IsBinaryCompatible< int, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< int, T > >::value > { };
template < class T > struct IsBinaryCompatible< unsigned int, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< unsigned int, T > >::value > { };
template < class T > struct IsBinaryCompatible< long, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< long, T > >::value > { };
template < class T > struct IsBinaryCompatible< unsigned long, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< unsigned long, T > >::value > { };
template < class T > struct IsBinaryCompatible< long long, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< long long, T > >::value > { };
template < class T > struct IsBinaryCompatible< unsigned long long, T >
    : public IntegralConstant< bool, Conjunction< IsFundamental< T >, IsSameSize< unsigned long long, T > >::value > { };
// Specializations of IsBinaryCompatible for float types
template < > struct IsBinaryCompatible< char, float > : public FalseType { };
template < > struct IsBinaryCompatible< signed char, float > : public FalseType { };
template < > struct IsBinaryCompatible< int, float > : public FalseType { };
template < > struct IsBinaryCompatible< long, float > : public FalseType { };
template < > struct IsBinaryCompatible< long long, float > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned char, float > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned short, float > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned int, float > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned long, float > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned long long, float > : public FalseType { };
template < > struct IsBinaryCompatible< char, double > : public FalseType { };
template < > struct IsBinaryCompatible< signed char, double > : public FalseType { };
template < > struct IsBinaryCompatible< short, double > : public FalseType { };
template < > struct IsBinaryCompatible< int, double > : public FalseType { };
template < > struct IsBinaryCompatible< long, double > : public FalseType { };
template < > struct IsBinaryCompatible< long long, double > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned char, double > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned short, double > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned int, double > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned long, double > : public FalseType { };
template < > struct IsBinaryCompatible< unsigned long long, double > : public FalseType { };

}
