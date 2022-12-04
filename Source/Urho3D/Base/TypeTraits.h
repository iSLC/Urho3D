//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Limits.h"

/*
 * Forward declarations.
*/
namespace Urho3D {

template < class > struct InPlaceType_t;
template < class > struct ReferenceWrapper;
template < class, class > struct Pair;
template < class, size_t > class Span;
template < class, size_t > struct Array;
template < class > struct Vector;
struct ReferenceCounted;
struct StrView;

/// Wraps a static constant of specified type. It is the base class for the C++ type traits. \remark See `std::integral_constant`
// One of the few exceptions that need to be defined before anything else.
template < class T, T V > struct IntegralConstant
{
    /// Wrapped constant value (declaration).
    static constexpr T value = V;
    /// Type of wrapped constant.
    using value_type = T;
    /// Type of this integral constant.
    using type = IntegralConstant< T, V >;
    /// Is implicitly convertible. Returns the wrapped constant.
    constexpr operator T() const noexcept { return value; } // NOLINT
    /// Is callable. Returns the wrapped constant.
    constexpr T operator () () const noexcept { return value; }
};
/// Definition of the wrapped constant in \ref IntegralConstant.
//template < class T, T V > constexpr T IntegralConstant< T, V >::value;

/// Helper alias template for the common case where `T` is `bool`. \remark See `std::bool_constant`
template < bool V > using BoolConstant = IntegralConstant< bool, V >;

/// Common case where T is `bool` and the constant is `true`. \remark See `std::true_type`
using TrueType = BoolConstant< true >;
/// Common case where T is `bool` and the constant is `false`. \remark See `std::false_type`
using FalseType = BoolConstant< false >;

/// Utility type used to provide an undefined type.
struct Undefined_t;

/// Utility meta-function that maps a sequence of any types to the type `void`. \remark See `std::void_t`
template < class... > using Void_t = void;

// declval implementation helpers
template < class T, class U = T && > U Helper_declval(int);
template < class T > T Helper_declval(long);
template < class > struct Helper_DeclvalProtector { static const bool stop = false; };

/// Converts any type T to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors. \remark See `std::declval`
template < class T > auto declval() noexcept -> decltype(Helper_declval< T >(0))
{
    static_assert(Helper_DeclvalProtector< T >::stop, "declval() must not be used!");
    return Helper_declval< T >(0);
}

// Miscellaneous.
template < class > struct TypeIdentity;

// Forward declarations of type traits for primary type categories.
template < class > struct IsVoid;
template < class > struct IsNullPointer;
template < class > struct IsIntegral;
template < class > struct IsFloatingPoint;
template < class > struct IsCArray;
template < class > struct IsArrayC;
template < class > struct IsArray;
template < class > struct IsEnum;
template < class > struct IsUnion;
template < class > struct IsClass;
template < class > struct IsFunction;
template < class > struct IsPointer;
template < class > struct IsLvalueReference;
template < class > struct IsRvalueReference;
template < class > struct IsMemberObjectPointer;
template < class > struct IsMemberFunctionPointer;
// Forward declarations of type traits for composite type categories.
template < class > struct IsFundamental;
template < class > struct IsArithmetic;
template < class > struct IsScalar;
template < class > struct IsObject;
template < class > struct IsCompound;
template < class > struct IsReference;
template < class > struct IsMemberPointer;
// Forward declarations of type traits for type properties.
template < class > struct IsConst;
template < class > struct IsVolatile;
template < class > struct IsTrivial;
template < class > struct IsTriviallyCopyable;
template < class > struct IsStandardLayout;
template < class > struct IsPOD;
template < class > struct IsLiteralType;
template < class > struct IsEmpty;
template < class > struct IsPolymorphic;
template < class > struct IsAbstract;
template < class > struct IsFinal;
template < class > struct IsAggregate;
template < class > struct IsSigned;
template < class > struct IsUnsigned;
template < class > struct IsBoundedArray;
template < class > struct IsUnboundedArray;
// Forward declarations of type traits for supported operations.
template < class, class... > struct IsConstructible;
template < class, class... > struct IsTriviallyConstructible;
template < class, class... > struct IsNoThrowConstructible;
template < class > struct IsDefaultConstructible;
template < class > struct IsTriviallyDefaultConstructible;
template < class > struct IsNoThrowDefaultConstructible;
template < class > struct IsCopyConstructible;
template < class > struct IsTriviallyCopyConstructible;
template < class > struct IsNothrowCopyConstructible;
template < class > struct IsMoveConstructible;
template < class > struct IsTriviallyMoveConstructible;
template < class > struct IsNoThrowMoveConstructible;
template < class, class > struct IsAssignable;
template < class, class > struct IsTriviallyAssignable;
template < class, class > struct IsNoThrowAssignable;
template < class > struct IsCopyAssignable;
template < class > struct IsTriviallyCopyAssignable;
template < class > struct IsNoThrowCopyAssignable;
template < class > struct IsMoveAssignable;
template < class > struct IsTriviallyMoveAssignable;
template < class > struct IsNoThrowMoveAssignable;
template < class > struct IsDestructible;
template < class > struct IsTriviallyDestructible;
template < class > struct IsNoThrowDestructible;
template < class > struct HasVirtualDestructor;
template < class, class > struct IsSwappableWith;
template < class > struct IsSwappable;
template < class, class > struct IsNoThrowSwappableWith;
template < class > struct IsNoThrowSwappable;
// Forward declarations of type traits for property queries.
template < class > struct AlignmentOf;
template < class > struct Rank;
template < class, unsigned > struct Extent;
// Forward declarations of type traits for type relationships.
template < class, class > struct IsSame;
template < class, class > struct IsBaseOf;
template < class, class > struct IsConvertible;
template < class, class > struct IsNoThrowConvertible;
// Forward declarations of type traits for const-volatility specifiers.
template < class > struct RemoveCV;
template < class > struct RemoveConst;
template < class > struct RemoveVolatile;
template < class > struct AddCV;
template < class > struct AddConst;
template < class > struct AddVolatile;
// Forward declarations of type traits for references.
template < class > struct RemoveReference;
template < class > struct AddLvalueReference;
template < class > struct AddRvalueReference;
// Forward declarations of type traits for pointers.
template < class > struct RemovePointer;
template < class > struct AddPointer;
// Forward declarations of type traits for sign modifiers.
template < class > struct MakeSigned;
template < class > struct MakeUnsigned;
// Forward declarations of type traits for arrays.
template < class > struct RemoveExtent;
template < class > struct RemoveAllExtents;
// Forward declarations of type traits for miscellaneous transformations.
template < size_t, size_t > struct AlignedStorage;
template < class > struct Decay;
template < class > struct RemoveCvRef;
template < bool, class > struct EnableIf;
template < bool, class, class > struct Conditional;
// Forward declarations of type traits for operations on traits.
template < class... > struct Disjunction;
template < class... > struct Conjunction;
template < class > struct Negation;
// Forward declarations of non-standard (helper) type traits.
template < class T, T, T > struct ComparisonEQ;
template < class T, T, T > struct ComparisonNE;
template < class T, T, T > struct ComparisonLT;
template < class T, T, T > struct ComparisonGT;
template < class T, T, T > struct ComparisonLE;
template < class T, T, T > struct ComparisonGE;
template < bool, class V, V, V > struct Contingent;
template < class, class > struct IsSameSize;
template < class, class > struct IsGreaterSize;
template < class, class > struct IsSmallerSize;
template < class > struct FitsInPointer;
template < class, class > struct FitsInSameMemory;
template < class > struct TypeSize;
template < class... > struct CollectiveSize;
template < class, class > struct IsComplete;
template < class > struct IsReferenceable;
template < class T > struct DecayAndStrip;
template < class, class > struct MatchCV;
template < class > struct IsInPlaceType;
template < class > struct IsImplicitlyDefaultConstructible;
template < class > struct NeedsUnsafeMoveConstructorAlternative;
template < int64_t V > struct SignedValueFit;
template < uint64_t V > struct UnsignedValueFit;
template < class, class > struct IsBinaryCompatible;
template < class > struct IsReferenceCounted;
template < class > struct IsSpan;
template < class > struct IsStrView;
template < class > struct ViewOfType;
template < class > struct IsRecycledType;
template < class > struct IsViewableType;
template < class > struct IsIntegralPrimitive;
template < class > struct IsIntegralPair;
template < class > struct IsPointerPair;
template < class > struct IsFloatingPointPair;
template < class > struct IsFundamentalPair;
template < class > struct IsIntegralPrimitivePair;
template < class > struct IsIntegralOnly;
template < class > struct IsPointerOnly;
template < class > struct IsFloatingPointOnly;
template < class > struct IsIntegralPrimitiveOnly;
template < class > struct IsFundamentalOnly;
template < class, class = void > struct HasSizeAndData;
template < class > struct GetFirstArg;
template < class, class > struct ReplaceFirstArg;
template < size_t, class, class... > struct ExtractTypeAt;
// Forward declarations of type traits for property queries.
template < class, class = Void_t< > > struct HasTypeMemberType;
template < class, class = Void_t< > > struct HasValueMemberType;
template < class, class = Void_t< > > struct HasReferenceMemberType;
template < class, class = Void_t< > > struct HasPointerMemberType;
template < class, class = Void_t< > > struct HasSizeMemberType;
template < class, class = Void_t< > > struct HasDifferenceMemberType;


/// Helper of \ref TypeIdentity used to avoid having to explicitly accessing the `type` member.
template < class T > using TypeIdentity_t = typename TypeIdentity< T >::type;

/// Helper of \ref IsVoid used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsVoid_v = IsVoid< T >::value;
/// Helper of \ref IsNullPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsNullPointer_v = IsNullPointer< T >::value;
/// Helper of \ref IsIntegral used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegral_v = IsIntegral< T >::value;
/// Helper of \ref IsFloatingPoint used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFloatingPoint_v = IsFloatingPoint< T >::value;
/// Helper of \ref IsCArray used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsCArray_v = IsCArray< T >::value;
/// Helper of \ref IsArrayC used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsArrayC_v = IsArrayC< T >::value;
/// Helper of \ref IsArray used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsArray_v = IsArray< T >::value;
/// Helper of \ref IsEnum used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsEnum_v = IsEnum< T >::value;
/// Helper of \ref IsUnion used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsUnion_v = IsUnion< T >::value;
/// Helper of \ref IsClass used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsClass_v = IsClass< T >::value;
/// Helper of \ref IsFunction used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFunction_v = IsFunction< T >::value;
/// Helper of \ref IsPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsPointer_v = IsPointer< T >::value;
/// Helper of \ref IsLvalueReference used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsLvalueReference_v = IsLvalueReference< T >::value;
/// Helper of \ref IsRvalueReference used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsRvalueReference_v = IsRvalueReference< T >::value;
/// Helper of \ref IsMemberObjectPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsMemberObjectPointer_v = IsMemberObjectPointer< T >::value;
/// Helper of \ref IsMemberFunctionPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsMemberFunctionPointer_v = IsMemberFunctionPointer< T >::value;

/// Helper of \ref IsFundamental used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFundamental_v = IsFundamental< T >::value;
/// Helper of \ref IsArithmetic used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsArithmetic_v = IsArithmetic< T >::value;
/// Helper of \ref IsScalar used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsScalar_v = IsScalar< T >::value;
/// Helper of \ref IsObject used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsObject_v = IsObject< T >::value;
/// Helper of \ref IsCompound used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsCompound_v = IsCompound< T >::value;
/// Helper of \ref IsReference used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsReference_v = IsReference< T >::value;
/// Helper of \ref IsMemberPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsMemberPointer_v = IsMemberPointer< T >::value;

/// Helper of \ref IsConst used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsConst_v = IsConst< T >::value;
/// Helper of \ref IsVolatile used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsVolatile_v = IsVolatile< T >::value;
/// Helper of \ref IsTrivial used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsTrivial_v = IsTrivial< T >::value;
/// Helper of \ref IsTriviallyCopyable used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsTriviallyCopyable_v = IsTriviallyCopyable< T >::value;
/// Helper of \ref IsStandardLayout used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsStandardLayout_v = IsStandardLayout< T >::value;
/// Helper of \ref IsPOD used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsPOD_v = IsPOD< T >::value;
/// Helper of \ref IsLiteralType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsLiteralType_v = IsLiteralType< T >::value;
/// Helper of \ref IsEmpty used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsEmpty_v = IsEmpty< T >::value;
/// Helper of \ref IsPolymorphic used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsPolymorphic_v = IsPolymorphic< T >::value;
/// Helper of \ref IsAbstract used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsAbstract_v = IsAbstract< T >::value;
/// Helper of \ref IsFinal used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFinal_v = IsFinal< T >::value;
/// Helper of \ref IsAggregate used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsAggregate_v = IsAggregate< T >::value;
/// Helper of \ref IsSigned used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsSigned_v = IsSigned< T >::value;
/// Helper of \ref IsUnsigned used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsUnsigned_v = IsUnsigned< T >::value;
/// Helper of \ref IsBoundedArray used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsBoundedArray_v = IsBoundedArray< T >::value;
/// Helper of \ref IsUnboundedArray used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsUnboundedArray_v = IsUnboundedArray< T >::value;

/// Helper of \ref IsConstructible used to avoid having to explicitly access the `value` member.
template < class T, class... Ts > inline constexpr auto IsConstructible_v = IsConstructible< T, Ts... >::value;
/// Helper of \ref IsTriviallyConstructible used to avoid having to explicitly access the `value` member.
template < class T, class... Ts > inline constexpr auto IsTriviallyConstructible_v = IsTriviallyConstructible< T, Ts... >::value;
/// Helper of \ref IsNoThrowConstructible used to avoid having to explicitly access the `value` member.
template < class T, class... Ts > inline constexpr auto IsNoThrowConstructible_v = IsNoThrowConstructible< T, Ts... >::value;
/// Helper of \ref IsDefaultConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsDefaultConstructible_v = IsDefaultConstructible< T >::value;
/// Helper of \ref IsTriviallyDefaultConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyDefaultConstructible_v = IsTriviallyDefaultConstructible< T >::value;
/// Helper of \ref IsNoThrowDefaultConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowDefaultConstructible_v = IsNoThrowDefaultConstructible< T >::value;
/// Helper of \ref IsCopyConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsCopyConstructible_v = IsCopyConstructible< T >::value;
/// Helper of \ref IsTriviallyCopyConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyCopyConstructible_v = IsTriviallyCopyConstructible< T >::value;
/// Helper of \ref IsNothrowCopyConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNothrowCopyConstructible_v = IsNothrowCopyConstructible< T >::value;
/// Helper of \ref IsMoveConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsMoveConstructible_v = IsMoveConstructible< T >::value;
/// Helper of \ref IsTriviallyMoveConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyMoveConstructible_v = IsTriviallyMoveConstructible< T >::value;
/// Helper of \ref IsNoThrowMoveConstructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowMoveConstructible_v = IsNoThrowMoveConstructible< T >::value;
/// Helper of \ref IsAssignable used to avoid having to explicitly access the `value` member.
template < class T, class U > inline constexpr auto IsAssignable_v = IsAssignable< T, U >::value;
/// Helper of \ref IsTriviallyAssignable used to avoid having to explicitly access the `value` member.
template < class T, class U > inline constexpr auto IsTriviallyAssignable_v = IsTriviallyAssignable< T, U >::value;
/// Helper of \ref IsNoThrowAssignable used to avoid having to explicitly access the `value` member.
template < class T, class U > inline constexpr auto IsNoThrowAssignable_v = IsNoThrowAssignable< T, U >::value;
/// Helper of \ref IsCopyAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsCopyAssignable_v = IsCopyAssignable< T >::value;
/// Helper of \ref IsTriviallyCopyAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyCopyAssignable_v = IsTriviallyCopyAssignable< T >::value;
/// Helper of \ref IsNoThrowCopyAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowCopyAssignable_v = IsNoThrowCopyAssignable< T >::value;
/// Helper of \ref IsMoveAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsMoveAssignable_v = IsMoveAssignable< T >::value;
/// Helper of \ref IsTriviallyMoveAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyMoveAssignable_v = IsTriviallyMoveAssignable< T >::value;
/// Helper of \ref IsNoThrowMoveAssignable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowMoveAssignable_v = IsNoThrowMoveAssignable< T >::value;
/// Helper of \ref IsDestructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsDestructible_v = IsDestructible< T >::value;
/// Helper of \ref IsTriviallyDestructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsTriviallyDestructible_v = IsTriviallyDestructible< T >::value;
/// Helper of \ref IsNoThrowDestructible used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowDestructible_v = IsNoThrowDestructible< T >::value;
/// Helper of \ref HasVirtualDestructor used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto HasVirtualDestructor_v = HasVirtualDestructor< T >::value;
///  Helper of \ref IsSwappableWith used to avoid having to explicitly access the `value` member.
template < class T, class U > inline constexpr auto IsSwappableWith_v = IsSwappableWith< T, U >::value;
///  Helper of \ref IsSwappable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsSwappable_v = IsSwappable< T >::value;
///  Helper of \ref IsNoThrowSwappableWith used to avoid having to explicitly access the `value` member.
template < class T, class U > inline constexpr auto IsNoThrowSwappableWith_v = IsNoThrowSwappableWith< T, U >::value;
///  Helper of \ref IsNoThrowSwappable used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsNoThrowSwappable_v = IsNoThrowSwappable< T >::value;

/// Helper of \ref AlignmentOf used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr size_t AlignmentOf_v = AlignmentOf< T >::value;
/// Helper of \ref Rank used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto Rank_v = Rank< T >::value;
/// Helper of \ref Extent used to avoid having to explicitly accessing the `value` member.
template < class T, unsigned N = 0 > inline constexpr auto Extent_v = Extent< T, N >::value;

/// Helper of \ref IsSame used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsSame_v = IsSame< T, U >::value;
/// Helper of \ref IsBaseOf used to avoid having to explicitly accessing the `value` member.
template < class B, class D > inline constexpr auto IsBaseOf_v = IsBaseOf< B, D >::value;
/// Helper of \ref IsConvertible used to avoid having to explicitly accessing the `value` member.
template < class F, class T > inline constexpr auto IsConvertible_v = IsConvertible< F, T >::value;
/// Helper of \ref IsNoThrowConvertible used to avoid having to explicitly accessing the `value` member.
template < class F, class T > inline constexpr auto IsNoThrowConvertible_v = IsNoThrowConvertible< F, T >::value;

/// Helper of \ref RemoveCV used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveCV_t = typename RemoveCV< T >::type;
/// Helper of \ref RemoveConst used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveConst_t = typename RemoveConst< T >::type;
/// Helper of \ref RemoveVolatile used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveVolatile_t = typename RemoveVolatile< T >::type;
/// Helper of \ref AddConst used to avoid having to explicitly accessing the `type` member.
template < class T > using AddCV_t = typename AddCV< T >::type;
/// Helper of \ref AddConst used to avoid having to explicitly accessing the `type` member.
template < class T > using AddConst_t = typename AddConst< T >::type;
/// Helper of \ref AddVolatile used to avoid having to explicitly accessing the `type` member.
template < class T > using AddVolatile_t = typename AddVolatile< T >::type;

/// Helper of \ref RemoveReference used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveReference_t = typename RemoveReference< T >::type;
/// Helper of \ref AddLvalueReference used to avoid having to explicitly accessing the `type` member.
template < class T > using AddLvalueReference_t = typename AddLvalueReference< T >::type;
/// Helper of \ref AddRvalueReference used to avoid having to explicitly accessing the `type` member.
template < class T > using AddRvalueReference_t = typename AddRvalueReference< T >::type;

/// Helper of \ref RemovePointer used to avoid having to explicitly accessing the `type` member.
template < class T > using RemovePointer_t = typename RemovePointer< T >::type;
/// Helper of \ref AddPointer used to avoid having to explicitly accessing the `type` member.
template < class T > using AddPointer_t = typename AddPointer< T >::type;

/// Helper of \ref MakeSigned used to avoid having to explicitly accessing the `type` member.
template < class T > using MakeSigned_t = typename MakeSigned< T >::type;
/// Helper of \ref MakeUnsigned used to avoid having to explicitly accessing the `type` member.
template < class T > using MakeUnsigned_t = typename MakeUnsigned< T >::type;

/// Helper of \ref RemoveExtent used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveExtent_t = typename RemoveExtent< T >::type;
/// Helper of \ref RemoveAllExtents used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveAllExtents_t = typename RemoveAllExtents< T >::type;

/// Helper of \ref AlignedStorage used to avoid having to explicitly accessing the `type` member.
template < size_t N, size_t A > using AlignedStorage_t = typename AlignedStorage< N, A >::type;
/// Helper of \ref Decay used to avoid having to explicitly accessing the `type` member.
template < class T > using Decay_t = typename Decay< T >::type;
/// Helper of \ref RemoveCvRef used to avoid having to explicitly accessing the `type` member.
template < class T > using RemoveCvRef_t = typename RemoveCvRef< T >::type;
/// Helper of \ref EnableIf used to avoid having to explicitly access the `type` member.
template < bool C, class T = void > using EnableIf_t = typename EnableIf< C, T >::type;
/// Helper of \ref Conditional used to avoid having to explicitly access the `type` member.
template < bool B, class T, class F > using Conditional_t = typename Conditional< B, T, F >::type;

/// Helper of \ref Disjunction used to avoid having to explicitly access the `value` member.
template < class... B > inline constexpr auto Disjunction_v = Disjunction< B... >::value;
/// Helper of \ref Disjunction used to avoid having to explicitly accessing the `type` member.
template < class... B > using Disjunction_t = typename Disjunction< B... >::type;
/// Helper of \ref Conjunction used to avoid having to explicitly access the `value` member.
template < class... B > inline constexpr auto Conjunction_v = Conjunction< B... >::value;
/// Helper of \ref Conjunction used to avoid having to explicitly accessing the `type` member.
template < class... B > using Conjunction_t = typename Conjunction< B... >::type;
/// Helper of \ref Negation used to avoid having to explicitly access the `value` member.
template < class B > inline constexpr auto Negation_v = Negation< B >::value;
/// Helper of \ref Negation used to avoid having to explicitly accessing the `type` member.
template < class... B > using Negation_t = typename Negation< B... >::type;

/// Convenient meta-function used to leverage SFINAE which acts as the opposite of \ref EnableIf.
template < bool C, class T = void > using DisableIf = EnableIf< !C, T >;
/// Helper of \ref DisableIf used to avoid having to explicitly accessing the `type` member.
template < bool C, class T = void > using DisableIf_t = typename EnableIf< !C, T >::type;

/// Helper alias of \ref EnableIf where `T` is the `bool` type. Not a standard feature.
template < bool C > using BoolEnableIf = EnableIf< C, bool >;
/// Helper of \ref BoolEnableIf used to avoid having to explicitly accessing the `type` member.
template < bool C > using BoolEnableIf_t = typename EnableIf< C, bool >::type;

/// Helper alias of \ref DisableIf where `T` is the `bool` type. Not a standard feature.
template < bool C > using BooleanDisableIf = EnableIf< !C, bool >;
/// Helper of \ref BooleanDisableIf used to avoid having to explicitly accessing the `type` member.
template < bool C > using BooleanDisableIf_t = typename EnableIf< !C, bool >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and passes them to a \ref Conjunction operation.
template < class T, class... B > using Require = EnableIf< Conjunction_v< B... >, T >;
/// Helper of \ref Require used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using Require_t = typename EnableIf< Conjunction_v< B... >, T >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and passes them to a \ref Disjunction operation.
template < class T, class... B > using RequireAny = EnableIf< Disjunction_v< B... >, T >;
/// Helper of \ref RequireAny used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using RequireAny_t = typename EnableIf< Disjunction_v< B... >, T >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and wraps them in a \ref Negation operation then passes them to a \ref Conjunction operation.
template < class T, class... B > using RequireNot = EnableIf< Conjunction_v< Negation< B >... >, T >;
/// Helper of \ref RequireNot used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using RequireNot_t = typename EnableIf< Conjunction_v< Negation< B >... >, T >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and wraps them in a \ref Negation operation then passes them to a \ref Disjunction operation.
template < class T, class... B > using RequireAnyNot = EnableIf< Disjunction_v< Negation< B >... >, T >;
/// Helper of \ref RequireAnyNot used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using RequireAnyNot_t = typename EnableIf< Disjunction_v< Negation< B >... >, T >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and passes them to a \ref Conjunction operation then wraps the result in a \ref Negation operation.
template < class T, class... B > using ReverseRequire = EnableIf< Negation_v< Conjunction< B... > >, T >;
/// Helper of \ref ReverseRequire used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using ReverseRequire_t = typename EnableIf< Negation_v< Conjunction< B... > >, T >::type;

/// Variation of \ref EnableIf that accepts any number of conditions and passes them to a \ref Disjunction operation then wraps the result in a \ref Negation operation.
template < class T, class... B > using ReverseRequireAny = EnableIf< Negation_v< Disjunction< B... > >, T >;
/// Helper of \ref ReverseRequireAny used to avoid having to explicitly accessing the `type` member.
template < class T, class... B > using ReverseRequireAny_t = typename EnableIf< Negation_v< Disjunction< B... > >, T >::type;

/// Variation of \ref Conjunction where the result is passed through \ref Negation and then returned.
template < class... B > using NegateConjunction = Negation< Conjunction< B... > >;
/// Helper of \ref NegateConjunction used to avoid having to explicitly accessing the `type` member.
template < class... B > inline constexpr auto NegateConjunction_v = Negation< Conjunction< B... > >::value;
/// Variation of \ref Disjunction where the result is passed through  \ref Negation and then returned.
template < class... B > using NegateDisjunction = Negation< Disjunction< B... > >;
/// Helper of \ref NegateDisjunction used to avoid having to explicitly accessing the `type` member.
template < class... B > inline constexpr auto NegateDisjunction_v = Negation< Disjunction< B... > >::value;

/// Variation of \ref Conjunction where each template element is passed through \ref Negation and then tested.
template < class... B > using ReverseConjunction = Conjunction< Negation< B >... >;
/// Helper of \ref ReverseConjunction used to avoid having to explicitly accessing the `value` member.
template < class... B > inline constexpr auto ReverseConjunction_v = Conjunction< Negation< B >... >::value;
/// Variation of \ref Disjunction where each template element is passed through \ref Negation and then tested.
template < class... B > using ReverseDisjunction = Disjunction< Negation< B >... >;
/// Helper of \ref ReverseDisjunction used to avoid having to explicitly accessing the `type` member.
template < class... B > inline constexpr auto ReverseDisjunction_v = Disjunction< Negation< B >... >::value;

/// Helper of \ref Contingent used to avoid having to explicitly accessing the `value` member.
template < bool B, class V, V T, V F > inline constexpr V Contingent_v = Contingent< B, V, T, F >::value;

/// Helper of \ref IsSameSize used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsSameSize_v = IsSameSize< T, U >::value;
/// Helper of \ref IsGreaterSize used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsGreaterSize_v = IsGreaterSize< T, U >::value;
/// Helper of \ref IsSmallerSize used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsSmallerSize_v = IsSmallerSize< T, U >::value;
/// Helper of \ref FitsInPointer used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto FitsInPointer_v = FitsInPointer< T >::value;
/// Helper of \ref FitsInSameMemory used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto FitsInSameMemory_v = FitsInSameMemory< T, U >::value;
/// Helper of \ref TypeSize used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr size_t TypeSize_v = TypeSize< T >::value;
/// Helper of \ref PackSize used to avoid having to explicitly accessing the `value` member.
template < class... Ts > inline constexpr size_t PackSize_v = sizeof...(Ts);
/// Helper of \ref CollectiveSize used to avoid having to explicitly accessing the `value` member.
template < class... Ts > inline constexpr size_t CollectiveSize_v = CollectiveSize< Ts... >::value;
/// Helper of \ref IsComplete used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsComplete_v = IsComplete< T, decltype(sizeof(T)) >::value;

/// Helper of \ref ComparisonEQ used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonEQ_v = ComparisonEQ< T, A, B >::value;
/// Helper of \ref ComparisonNE used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonNE_v = ComparisonNE< T, A, B >::value;
/// Helper of \ref ComparisonLT used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonLT_v = ComparisonLT< T, A, B >::value;
/// Helper of \ref ComparisonGT used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonGT_v = ComparisonGT< T, A, B >::value;
/// Helper of \ref ComparisonLE used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonLE_v = ComparisonLE< T, A, B >::value;
/// Helper of \ref ComparisonGE used to avoid having to explicitly accessing the `value` member.
template < class T, T A, T B > inline constexpr auto ComparisonGE_v = ComparisonGE< T, A, B >::value;
/// Helper of \ref IsReferenceable used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsReferenceable_v = IsReferenceable< T >::value;
/// Helper of \ref DecayAndStrip used to avoid having to explicitly accessing the `type` member..
template < class T > using DecayAndStrip_t = typename DecayAndStrip< T >::type;
/// Helper of \ref MatchCV used to avoid having to explicitly accessing the `type` member.
template < class Q, class U > using MatchCV_t = typename MatchCV< Q, U >::type;
/// Helper of \ref IsInPlaceType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsInPlaceType_v = IsInPlaceType< T >::value;
/// Helper of \ref IsImplicitlyDefaultConstructible used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsImplicitlyDefaultConstructible_v = IsImplicitlyDefaultConstructible< T >::value;
/// Helper of \ref NeedsUnsafeMoveConstructorAlternative used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr bool NeedsUnsafeMoveConstructorAlternative_v = NeedsUnsafeMoveConstructorAlternative< T >::value;
/// Helper of \ref SignedValueFit used to avoid having to explicitly accessing the `type` member..
template < int64_t V > using SignedValueFit_t = typename SignedValueFit< V >::type;
/// Helper of \ref UnsignedValueFit used to avoid having to explicitly accessing the `type` member..
template < uint64_t V > using UnsignedValueFit_t = typename UnsignedValueFit< V >::type;
/// Helper of \ref IsReferenceCounted used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsReferenceCounted_v = IsReferenceCounted< T >::value;
/// Helper of \ref IsSpan used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsSpan_v = IsSpan< T >::value;
/// Helper of \ref IsStrView used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsStrView_v = IsStrView< T >::value;
/// Helper of \ref IsBinaryCompatible used to avoid having to explicitly accessing the `value` member.
template < class T, class U > inline constexpr auto IsBinaryCompatible_v = IsBinaryCompatible< T, U >::value;
/// Helper of \ref ViewOfType used to avoid having to explicitly accessing the `type` member.
template < class T > using ViewOfType_t = typename ViewOfType< T >::type;
/// Helper of \ref IsRecycledType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsRecycledType_v = IsRecycledType< T >::value;
/// Helper of \ref IsViewableType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsViewableType_v = IsViewableType< T >::value;
/// Helper of \ref IsIntegralPrimitive used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegralPrimitive_v = IsIntegralPrimitive< T >::value;
/// Helper of \ref IsIntegralPair used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegralPair_v = IsIntegralPair< T >::value;
/// Helper of \ref IsPointerPair used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsPointerPair_v = IsPointerPair< T >::value;
/// Helper of \ref IsFloatingPointPair used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFloatingPointPair_v = IsFloatingPointPair< T >::value;
/// Helper of \ref IsFundamentalPair used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFundamentalPair_v = IsFundamentalPair< T >::value;
/// Helper of \ref IsIntegralPrimitivePair used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegralPrimitivePair_v = IsIntegralPrimitivePair< T >::value;
/// Helper of \ref IsIntegralOnly used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegralOnly_v = IsIntegralOnly< T >::value;
/// Helper of \ref IsPointerOnly used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsPointerOnly_v = IsPointerOnly< T >::value;
/// Helper of \ref IsFloatingPointOnly used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFloatingPointOnly_v = IsFloatingPointOnly< T >::value;
/// Helper of \ref IsIntegralPrimitiveOnly used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsIntegralPrimitiveOnly_v = IsIntegralPrimitiveOnly< T >::value;
/// Helper of \ref IsFundamentalOnly used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsFundamentalOnly_v = IsFundamentalOnly< T >::value;
/// Helper of \ref HasSizeAndData used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasSizeAndData_v = HasSizeAndData< T >::value;
/// Helper of \ref GetFirstArg used to avoid having to explicitly accessing the `type` member.
template < class T > using GetFirstArg_t = typename GetFirstArg< T >::type;
/// Helper of \ref ReplaceFirstArg used to avoid having to explicitly accessing the `type` member.
template < class T, class U > using ReplaceFirstArg_t = typename ReplaceFirstArg< T, U >::type;
/// Helper of \ref ExtractTypeAt used to avoid having to explicitly accessing the `type` member.
template < size_t I, class... Ts > using ExtractTypeAt_t = typename ExtractTypeAt< I, Ts... >::type;
/// Helper of \ref HasTypeMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasTypeMemberType_v = HasTypeMemberType< T >::value;
/// Helper of \ref HasValueMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasValueMemberType_v = HasValueMemberType< T >::value;
/// Helper of \ref HasReferenceMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasReferenceMemberType_v = HasReferenceMemberType< T >::value;
/// Helper of \ref HasPointerMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasPointerMemberType_v = HasPointerMemberType< T >::value;
/// Helper of \ref HasSizeMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasSizeMemberType_v = HasSizeMemberType< T >::value;
/// Helper of \ref HasDifferenceMemberType used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasDifferenceMemberType_v = HasDifferenceMemberType< T >::value;

/// Makes sure the given type \p T is not `void` and if it is, \ref Undefined_t is used instead.
template < class T > using MakeNotVoid_t = Conditional_t< IsVoid_v< T >, Undefined_t, T >;

} // Namespace:: Urho3D

/*
 * Helpers.
*/
namespace Urho3D {
namespace Impl {

/// Helper used to detect whether a type is referenceable.
template < class T, class = void > struct Helper_IsReferenceable : public FalseType { };
// Specialization of %IsReferenceable for when the type is actually referenceable.
template < class T > struct Helper_IsReferenceable< T, Void_t< T & > > : public TrueType { };

/// Implementation of the detection idiom (negative case).
template < class Default, class AlwaysVoid, template < class... > class Op, class... Args > struct Helper_Detector
{
    using value_t = FalseType;
    using type = Default;
};

/// Implementation of the detection idiom (positive case).
template < typename Default, template < typename... > class Op, typename... Args > struct Helper_Detector<
    Default, Void_t< Op< Args... > >, Op, Args...
> {
    using value_t = TrueType;
    using type = Op< Args... >;
};

// Forward declaration of Swap function.
template < class T > constexpr EnableIf_t<
    IsMoveConstructible_v< T > && IsMoveAssignable_v< T >
> Swap(T & a, T & b) noexcept(Conjunction_v< IsNoThrowMoveConstructible< T >, IsNoThrowMoveAssignable< T > >);
template < class T, size_t N > constexpr EnableIf_t<
    IsSwappable_v< T >
> Swap(T (&a)[N], T (&b)[N]) noexcept(IsNoThrowSwappable_v< T >);

// Helper to select a signed integral type for on the specified size (in bytes).
template < size_t > struct Helper_SelectSigned;
template < > struct Helper_SelectSigned< 1 > { typedef int8_t type; };
template < > struct Helper_SelectSigned< 2 > { typedef int16_t type; };
template < > struct Helper_SelectSigned< 4 > { typedef int32_t type; };
template < > struct Helper_SelectSigned< 8 > { typedef int64_t type; };
#ifdef UH_HAS_INT128_TYPE
    template < > struct Helper_SelectSigned< 16 > { typedef UH_INT128_TYPE type; };
#endif

// Helper to select an unsigned integral type for on the specified size (in bytes).
template < size_t > struct Helper_SelectUnsigned;
template < > struct Helper_SelectUnsigned< 1 > { typedef uint8_t type; };
template < > struct Helper_SelectUnsigned< 2 > { typedef uint16_t type; };
template < > struct Helper_SelectUnsigned< 4 > { typedef uint32_t type; };
template < > struct Helper_SelectUnsigned< 8 > { typedef uint64_t type; };
#ifdef UH_HAS_UINT128_TYPE
    template < > struct Helper_SelectUnsigned< 16 > { typedef UH_UINT128_TYPE type; };
#endif

// Helper used to check if a signed integral value can fit in a 32-bit integral type.
template < int64_t V, bool B = (V > INT32_MAX || V < INT32_MIN) > struct Helper_SignedValueFit32 { typedef int32_t type; };
// Specialization of the %Helper_SignedValueFit32 type for the case where the value needs a type larger than 32-bit.
template < int64_t V > struct Helper_SignedValueFit32< V, true > { typedef int64_t type; };

// Helper used to check if a signed integral value can fit in a 16-bit integral type.
template < int64_t V, bool B = (V > INT16_MAX || V < INT16_MIN) > struct Helper_SignedValueFit16 { typedef int16_t type; };
// Specialization of the %Helper_SignedValueFit16 type for the case where the value needs a type larger than 16-bit.
template < int64_t V > struct Helper_SignedValueFit16< V, true > : public Helper_SignedValueFit32< V > { };

// Helper used to check if a signed integral value can fit in a 8-bit integral type.
template < int64_t V, bool B = (V > INT8_MAX || V < INT8_MIN) > struct Helper_SignedValueFit8 { typedef int8_t type; };
// Specialization of the %Helper_SignedValueFit8 type for the case where the value needs a type larger than 16-bit.
template < int64_t V > struct Helper_SignedValueFit8< V, true > : public Helper_SignedValueFit16< V > { };

// Helper used to check if a unsigned integral value can fit in a 32-bit integral type.
template < uint64_t V, bool B = (V > UINT32_MAX) > struct Helper_UnsignedValueFit32 { typedef uint32_t type; };
// Specialization of the %Helper_UnsignedValueFit32 type for the case where the value needs a type larger than 32-bit.
template < uint64_t V > struct Helper_UnsignedValueFit32< V, true > { typedef uint64_t type; };

// Helper used to check if a unsigned integral value can fit in a 16-bit integral type.
template < uint64_t V, bool B = (V > UINT16_MAX) > struct Helper_UnsignedValueFit16 { typedef uint16_t type; };
// Specialization of the %Helper_UnsignedValueFit16 type for the case where the value needs a type larger than 16-bit.
template < uint64_t V > struct Helper_UnsignedValueFit16< V, true > : public Helper_UnsignedValueFit32< V > { };

// Helper used to check if a unsigned integral value can fit in a 8-bit integral type.
template < uint64_t V, bool B = (V > UINT8_MAX) > struct Helper_UnsignedValueFit8 { typedef uint8_t type; };
// Specialization of the %Helper_UnsignedValueFit8 type for the case where the value needs a type larger than 16-bit.
template < uint64_t V > struct Helper_UnsignedValueFit8< V, true > : public Helper_UnsignedValueFit16< V > { };

// Helper used to check if a type is an integral type.
template < class > struct Helper_IsIntegral : public FalseType { };
template < > struct Helper_IsIntegral< bool > : public TrueType { };
template < > struct Helper_IsIntegral< char > : public TrueType { };
template < > struct Helper_IsIntegral< signed char > : public TrueType { };
template < > struct Helper_IsIntegral< unsigned char > : public TrueType { };
template < > struct Helper_IsIntegral< wchar_t > : public TrueType { };
#ifdef UH_HAS_CHAR8_TYPE
    template < > struct Helper_IsIntegral< char8_t > : public TrueType { };
#endif
template < > struct Helper_IsIntegral< char16_t > : public TrueType { };
template < > struct Helper_IsIntegral< char32_t > : public TrueType { };
template < > struct Helper_IsIntegral< short > : public TrueType { };
template < > struct Helper_IsIntegral< unsigned short > : public TrueType { };
template < > struct Helper_IsIntegral< int > : public TrueType { };
template < > struct Helper_IsIntegral< unsigned int > : public TrueType { };
template < > struct Helper_IsIntegral< long > : public TrueType { };
template < > struct Helper_IsIntegral< unsigned long > : public TrueType { };
template < > struct Helper_IsIntegral< long long > : public TrueType { };
template < > struct Helper_IsIntegral< unsigned long long > : public TrueType { };
#ifdef UH_HAS_INT128_TYPE
    template < > struct Helper_IsIntegral< UH_INT128_TYPE > : public TrueType { };
    template < > struct Helper_IsIntegral< UH_UINT128_TYPE > : public TrueType { };
#endif

// Helper used to check if a type is a floating point type.
template < class > struct Helper_IsFloatingPoint : public FalseType { };
template < > struct Helper_IsFloatingPoint< float > : public TrueType { };
template < > struct Helper_IsFloatingPoint< double > : public TrueType { };
template < > struct Helper_IsFloatingPoint< long double > : public TrueType { };
#ifdef UH_HAS_FLOAT128_TYPE
    template < > struct Helper_IsFloatingPoint< UH_FLOAT128_TYPE > : public TrueType { };
#endif

// Helper used to check if the specified type is a pointer type.
template < class > struct Helper_IsPointer : public FalseType { };
template < class T > struct Helper_IsPointer< T * > : public TrueType { };
// Helper used to check if the specified type is a pointer to a non-static member object.
template < class > struct Helper_IsMemberObjectPointer : public FalseType { };
template < class T, class C > struct Helper_IsMemberObjectPointer< T C::* > : public Negation_t< IsFunction< T > > { };
// Helper used to check if the specified type is a pointer to a non-static member function.
template < class> struct Helper_IsMemberFunctionPointer : public FalseType { };
template < class T, class C > struct Helper_IsMemberFunctionPointer< T C::* > : public IsFunction< T >::type { };
// Helper used to check if the specified type is a pointer to an non-static member function or object.
template < class T > struct Helper_IsMemberPointer : public FalseType { };
template < class T, class C > struct Helper_IsMemberPointer< T C::* > : public TrueType  { };
// Helper used to check if the specified type is a signed arithmetic type.
template < class T, bool = IsArithmetic_v< T > > struct Helper_IsSigned : public FalseType { };
template < class T > struct Helper_IsSigned< T, true > : public BoolConstant< bool(T(-1) < T(0)) > { };

// For when __is_nothrow_constructible builtin is not available.
#if defined(UH_GNUC) && (UH_GNUC_MAJOR < 11)
// Helper used to check if the specified type has a constructor for specific arguments.
template < class T, class... Args > struct Helper_IsNoThrowConstructible : public BoolConstant< noexcept(T(declval< Args >()...)) > { };
template < class T, class Arg > struct Helper_IsNoThrowConstructible< T, Arg > : public BoolConstant< noexcept(static_cast< T >(declval< Arg >())) > { };
template < class T > struct Helper_IsNoThrowConstructible< T > : public IsNoThrowDefaultConstructible< T > { };
// Helper used to check if the specified type has non-throwing default constructor.
template < class T > struct Helper_IsNoThrowDefaultConstructibleAtom : public BoolConstant< noexcept(T()) > { };
template < class T, bool = IsCArray_v< T > > struct Helper_IsNoThrowDefaultConstructible;
template < class T > struct Helper_IsNoThrowDefaultConstructible< T, true >
    : public Conjunction< IsBoundedArray< T >, Helper_IsNoThrowDefaultConstructibleAtom< RemoveAllExtents_t< T > > > { };
template < class T > struct Helper_IsNoThrowDefaultConstructible< T, false > : public Helper_IsNoThrowDefaultConstructibleAtom< T > { };
#endif
// Helper used to check if the specified type has copy constructor.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsCopyConstructible;
template < class T > struct Helper_IsCopyConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsCopyConstructible< T, true > : public BoolConstant< __is_constructible(T, const T &) > { };
// Helper used to check if the specified type has trivial copy constructor.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsTriviallyCopyConstructible;
template < class T > struct Helper_IsTriviallyCopyConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsTriviallyCopyConstructible< T, true >
    : public Conjunction< IsCopyConstructible< T >, BoolConstant< __is_trivially_constructible(T, const T &) > > { };
// Helper used to check if the specified type has non-throwing copy constructor.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsNoThrowCopyConstructible;
template < class T > struct Helper_IsNoThrowCopyConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsNoThrowCopyConstructible< T, true > : public IsNoThrowConstructible< T, const T & > { };
// Helper used to check if the specified type can be constructed from an rvalue reference.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsMoveConstructible;
template < class T > struct Helper_IsMoveConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsMoveConstructible< T, true > : public BoolConstant< __is_constructible(T, T&&) > { };
// Helper used to check if the specified type can be trivially constructed from an rvalue reference.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsTriviallyMoveConstructible;
template < class T > struct Helper_IsTriviallyMoveConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsTriviallyMoveConstructible< T, true >
    : public Conjunction< IsMoveConstructible< T >, BoolConstant< __is_trivially_constructible(T, T &&) > > { };
// Helper used to check if the specified type can be constructed from an rvalue reference without throwing exceptions.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsNoThrowMoveConstructible;
template < class T > struct Helper_IsNoThrowMoveConstructible< T, false > : public FalseType { };
template < class T > struct Helper_IsNoThrowMoveConstructible< T, true > : public IsNoThrowConstructible< T, T&& > { };
// Helper used to check if the specified type has a assignment operator for a specific argument.
template < class T, class U > struct Helper_IsNoThrowAssignable
// For when __is_nothrow_assignable builtin is not available.
#if defined(UH_GNUC) && (UH_GNUC_MAJOR < 11)
    : public BoolConstant< noexcept(declval< T >() = declval< U >()) >
#else
    : public BoolConstant< __is_nothrow_assignable(T, U) >
#endif
{ };
// Helper used to check if the specified type has a copy assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsCopyAssignable;
template < class T > struct Helper_IsCopyAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsCopyAssignable< T, true > : public BoolConstant< __is_assignable(T &, const T &) > { };
// Helper used to check if the specified type has a trivial copy assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsTriviallyCopyAssignable;
template < class T > struct Helper_IsTriviallyCopyAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsTriviallyCopyAssignable< T, true > : public BoolConstant< __is_trivially_assignable(T &, const T &) > { };
// Helper used to check if the specified type has a non-throwing copy assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsNoThrowCopyAssignable;
template < class T > struct Helper_IsNoThrowCopyAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsNoThrowCopyAssignable< T, true > : public IsNoThrowAssignable< T &, const T & > { };
// Helper used to check if the specified type has a move assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsMoveAssignable;
template < class T > struct Helper_IsMoveAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsMoveAssignable< T, true > : public BoolConstant< __is_assignable(T &, const T &&) > { };
// Helper used to check if the specified type has a trivial move assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsTriviallyMoveAssignable;
template < class T > struct Helper_IsTriviallyMoveAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsTriviallyMoveAssignable< T, true > : public BoolConstant< __is_trivially_assignable(T &, T &&) > { };
// Helper used to check if the specified type has a non-throwing move assignment operator.
template < class T, bool = IsReferenceable_v< T > > struct Helper_IsNoThrowMoveAssignable;
template < class T > struct Helper_IsNoThrowMoveAssignable< T, false > : public FalseType { };
template < class T > struct Helper_IsNoThrowMoveAssignable< T, true > : public IsNoThrowAssignable< T &, T && > { };

// MSVC has the __is_destructible builtin.
#ifndef UH_MSC
// Only necessary for non-MSVC compilers.
struct Helper_DoIsDestructible
{
    template < class T, class = decltype(declval< T & >().~T()) > static TrueType Test(int);
    template < class > static FalseType Test(...); //NOLINT
};
// Helper used to check if the specified type has a non-deleted destructor.
template < class T > struct Helper_IsDestructible : public Helper_DoIsDestructible { typedef decltype(Test< T >(0)) type; };

// More helpers to clean up the type before doing the destructor detection.
template < class T,
       bool = Disjunction_v< IsVoid< T >, IsBoundedArray< T >, IsFunction< T > >,
       bool = Disjunction_v< IsReference< T >, IsScalar< T > >
> struct Helper_IsDestructibleSafe;
template < class T> struct Helper_IsDestructibleSafe< T, false, false >
    : public Helper_IsDestructible< RemoveAllExtents_t< T > >::type { };
template < class T > struct Helper_IsDestructibleSafe< T, true, false > : public FalseType { };
template < class T > struct Helper_IsDestructibleSafe< T, false, true > : public TrueType { };

// Only necessary for non-MSVC compilers.
struct Helper_DoIsNoThrowDestructible
{
    template < class T > static BoolConstant< noexcept(declval< T & >().~T()) > Test(int); //NOLINT
    template < class > static FalseType Test(...); //NOLINT
};
// Helper used to check if the specified type has a non-deleted non-throwing destructor.
template < class T > struct Helper_IsNoThrowDestructible : public Helper_DoIsNoThrowDestructible { typedef decltype(Test< T >(0)) type; };

// More helpers to clean up the type before doing the destructor detection.
template < class T,
       bool = Disjunction_v< IsVoid< T >, IsBoundedArray< T >, IsFunction< T > >,
       bool = Disjunction_v< IsReference< T >, IsScalar< T > >
> struct Helper_IsNoThrowDestructibleSafe;
template < class T > struct Helper_IsNoThrowDestructibleSafe< T, false, false >
    : public Helper_IsNoThrowDestructible< RemoveAllExtents_t< T > >::type { };
template < class T > struct Helper_IsNoThrowDestructibleSafe< T, true, false > : public FalseType { };
template < class T > struct Helper_IsNoThrowDestructibleSafe< T, false, true > : public TrueType { };
#endif

// Helper used to check if the specified type can be swapped with objects of same type.
struct Helper_DoIsSwappable
{
    template < class T, class = decltype(Swap(declval< T & >(), declval< T & >())) > static TrueType Test(int);
    template < class > static FalseType Test(...);
};
struct Helper_DoIsNoThrowSwappable
{
    template < class T > static BoolConstant< noexcept(Swap(declval< T & >(), declval< T & >())) > Test(int);
    template < class > static FalseType Test(...);
};

template < class T > struct Helper_IsSwappable : public Helper_DoIsSwappable { typedef decltype(Test< T >(0)) type; };
template < class T > struct Helper_IsNoThrowSwappable : public Helper_DoIsNoThrowSwappable { typedef decltype(Test< T >(0)) type; };

// Helper used to check if the specified type can be swapped with objects of same or different type.
struct Helper_DoIsSwappableWith
{
    template < class T, class U,
        class = decltype(Swap(declval< T >(), declval< U >())),
        class = decltype(Swap(declval< U >(), declval< T >()))
    > static TrueType Test(int);
    template < class, class > static FalseType Test(...);
};
struct Helper_DoIsNoThrowSwappableWith
{
    template < class T, class U > static BoolConstant<
        noexcept(Swap(declval< T >(), declval< U >())) && noexcept(Swap(declval< U >(), declval< T >()))
    > Test(int);
    template < class, class > static FalseType Test(...);
};

template < class T, class U > struct Helper_IsSwappableWith : public Helper_DoIsSwappableWith { typedef decltype(Test< T, U >(0)) type; };
// Optimization for the homogenous lvalue case, not required:
template < class T > struct Helper_IsSwappableWith< T &, T & > : public Helper_DoIsSwappable { typedef decltype(Test< T & >(0)) type; };

template < class T, class U > struct Helper_IsNoThrowSwappableWith : public Helper_DoIsNoThrowSwappableWith { typedef decltype(Test< T, U >(0)) type; };
// Optimization for the homogenous lvalue case, not required:
template < class T > struct Helper_IsNoThrowSwappableWith< T &, T & > : public Helper_DoIsNoThrowSwappable { typedef decltype(Test< T & >(0)) type; };

// MSVC has the __is_convertible_to builtin.
#ifndef UH_MSC
// Helper used to check if the specified type can be converted to another type.
template < class F, class T,
    bool = Disjunction_v< IsVoid< F >, IsFunction< T >, IsCArray< T > >
> struct Helper_IsConvertible : public IsVoid< T > { };

template < class F, class T > class Helper_IsConvertible< F, T, false >
{
    template < class T1 > static void TestAux(T1) noexcept;
    template < class F1, class T1, class = decltype(TestAux< T1 >(declval< F1 >())) > static TrueType Test(int);
    template < class, class > static FalseType Test(...);
public:
    typedef decltype(Test< F, T >(0)) type;
};
#endif


// Helper used to check if the specified type can be converted to another type without throwing exceptions .
template < class F, class T,
    bool = Disjunction_v< IsVoid< F >, IsFunction< T >, IsCArray< T > >
> struct Helper_IsNoThrowConvertible : public IsVoid< T > { };

template < class F, class T > class Helper_IsNoThrowConvertible< F, T, false >
{
    template < class T1 > static void TestAux(T1) noexcept;
    template < class F1, class T1 > static BoolConstant< noexcept(TestAux< T1 >(declval< F1 >())) > Test(int);
    template < class, class > static FalseType Test(...);
public:
    using type = decltype(Test< F, T >(0));
};

// Helper used to add a lvalue reference to the specified type.
template < class T > auto Helper_TryAddLvalueReference(int) -> TypeIdentity< T& >;
template < class T > auto Helper_TryAddLvalueReference(...) -> TypeIdentity< T >;

// Helper used to add a rvalue reference to the specified type.
template < class T > auto Helper_TryAddRvalueReference(int) -> TypeIdentity< T&& >;
template < class T > auto Helper_TryAddRvalueReference(...) -> TypeIdentity< T >;

// Helper used to add a pointer to the specified type.
template < class T > auto Helper_TryAddPointer(int) -> TypeIdentity< RemoveReference_t< T > * >;
template < class T > auto Helper_TryAddPointer(...) -> TypeIdentity< T >;

// Helper used to make the specified type a signed while retaining properties and qualifiers.
template < class T, bool S = IsSigned_v< T > > struct Helper_MakeUnsigned { typedef T type; };
template < class T > struct Helper_MakeUnsigned< T, true > { typedef MatchCV_t< T, typename Helper_SelectUnsigned< sizeof(T) >::type > type; };
template < bool S > struct Helper_MakeUnsigned< bool, S >; // Leave boolean undefined.

// Helper used to make the specified type a signed while retaining properties and qualifiers.
template < class T, bool S = IsSigned_v< T > > struct Helper_MakeSigned { typedef T type; };
template < class T > struct Helper_MakeSigned< T, false > { typedef MatchCV_t< T, typename Helper_SelectSigned< sizeof(T) >::type > type; };
template < bool S > struct Helper_MakeSigned< bool, S >; // Leave boolean undefined.

// Helper used to provide the nested type type, which is a trivial standard-layout type
// suitable for use as uninitialized storage for any object whose size is at most N
// and whose alignment requirement is a divisor of A.
template < size_t N, size_t A > struct Helper_AlignedStorage
{
    UH_GNUC_OR(union, struct) type {
        UH_EXCEPT_GNUC(alignas(A)) unsigned char data[N];
#ifdef UH_GNUC
        struct __attribute__((__aligned__((A)))) { } align;
#endif
    };
};

// Helper used to select the proper decaying utility for the specified type.
template < class U, bool A = IsCArray_v< U >, bool F = IsFunction_v< U > > struct Helper_DecaySelector;
template < class U > struct Helper_DecaySelector< U, false, false > { typedef RemoveCV_t< U > type; };
template < class U > struct Helper_DecaySelector< U, true, false > { typedef RemoveExtent_t< U > * type; };
template < class U > struct Helper_DecaySelector< U, false, true > { typedef AddPointer_t< U > type; };

// Helper which adds a reference to a type when given a ReferenceWrapper
template < class T > struct Helper_StripReferenceWrapper { typedef T type; };
template < class T > struct Helper_StripReferenceWrapper< ReferenceWrapper< T > > { typedef T & type; };

// Helper used to apply specific qualifiers to the specified type.
template < class U, bool C, bool V > struct Helper_MatchCV;
template < class U > struct Helper_MatchCV< U, false, false > { typedef U type; };
template < class U > struct Helper_MatchCV< U, false, true > { typedef volatile U type; };
template < class U > struct Helper_MatchCV< U, true, false > { typedef const U type; };
template < class U > struct Helper_MatchCV< U, true, true > { typedef const volatile U type; };

// Helper used to check the the specified type is wrapped in %InPlaceType_t type.
template < class > struct Helper_IsInPlaceType : public FalseType { };
template < class T > struct Helper_IsInPlaceType< InPlaceType_t< T > > : public TrueType { };

// Helper used to check if the specified type has a implicit default constructor.
struct Helper_DoIsImplicitlyDefaultConstructible
{
    template < class T > static void Helper(const T &);
    template < class T > static TrueType Test(const T &, decltype(Helper< const T & >({}))* = 0);
    static FalseType Test(...);
};
template < class T > struct Helper_IsImplicitlyDefaultConstructible : public Helper_DoIsImplicitlyDefaultConstructible
{
    typedef decltype(Test(declval< T >())) type;
};
template < class T > struct Helper_IsImplicitlyDefaultConstructibleSafe : public Helper_IsImplicitlyDefaultConstructible< T >::type { };

} // Namespace:: Impl
} // Namespace:: Urho3D

/*
 * Implementation.
*/
namespace Urho3D {

struct NoneSuchBase { };

/// Type used by \ref Detected_t to indicate detection failure. \remark See `std::nonesuch`
/// Has no default constructor or initializer-list constructor, and is not an aggregate.
/// Cannot be constructed, destroyed, or copied.
struct NoneSuch : private NoneSuchBase
{
    // Destructor (disabled).
    ~NoneSuch() = delete;
    // Copy constructor (disabled).
    NoneSuch(NoneSuch const &) = delete;
    // Copy assignment operator (disabled).
    void operator = (NoneSuch const &) = delete;
};

/// An alias for %TrueType if the template-id `Op< Args... >` denotes a valid type; otherwise it is an alias for %FalseType. \remark See `std::is_detected`
template < template < class... > class Op, class... Args > using IsDetected = typename Impl::Helper_Detector< NoneSuch, void, Op, Args... >::value_t;
/// Alias of %IsDetected used to avoid having to explicitly access the `value` member.
template < template < class... > class Op, class... Args > inline constexpr auto IsDetected_v = IsDetected< Op, Args... >::value;

/// An alias for `Op< Args... >` if that template-id denotes a valid type; otherwise it is an alias for the class %NoneSuch. \remark See `std::detected_t`
template < template < class... > class Op, class... Args > using Detected_t = typename Impl::Helper_Detector< NoneSuch, void, Op, Args... >::type;

// Detect whether `Op< Args... >` is a valid type, use `Default` if not. \remark See `std::detected_or`
template < class Default, template < class... > class Op, class... Args > using DetectedOr = Impl::Helper_Detector< Default, void, Op, Args... >;
/// Alias of %DetectedOr used to avoid having to explicitly access the `type` member.
template < class Default, template < class... > class Op, class... Args > using DetectedOr_t = typename DetectedOr< Default, Op, Args... >::type;

/// An alias for %TrueType if the template-id `Op< Args... >` is of type `To`; otherwise it is an alias for %FalseType. \remark See `std::is_detected_exact`
template < class Expected, template < class... > class Op, class... Args > using IsDetectedExact = IsSame< Expected, Detected_t< Op, Args... > >;
/// Alias of %IsDetectedExact used to avoid having to explicitly access the `value` member.
template < class Expected, template < class... > class Op, class... Args > inline constexpr auto IsDetectedExact_v = IsDetectedExact< Expected, Op, Args... >::value;

/// An alias for %TrueType if the template-id `Op< Args... >` is convertible to type `To`; otherwise it is an alias for %FalseType. \remark See `std::is_detected_convertible`
template < class To, template < class... > class Op, class... Args > using IsDetectedConvertible = IsConvertible< Detected_t< Op, Args... >, To >;
/// Alias of %IsDetectedConvertible used to avoid having to explicitly access the `value` member.
template < class To, template < class... > class Op, class... Args > inline constexpr auto IsDetectedConvertible_v = IsDetectedConvertible< To, Op, Args... >::value;

/// Provides the member typedef `type` that names `T` (i.e., the identity transformation). \remark See `std::type_identity`
template < class T > struct TypeIdentity { using type = T; };

/// Check whether `T` is a `void` type. \remark See `std::is_void`
template < class T > struct IsVoid : IsSame< void, RemoveCV_t< T > > { };
/// Check whether `T` is the `nullptr_t` type. \remark See `std::is_null_pointer`
template < class T > struct IsNullPointer : IsSame< decltype(nullptr), RemoveCV_t< T > > { };

/// Check whether `T` is an integral type. \remark See `std::is_integral`
template < class T > struct IsIntegral : public Impl::Helper_IsIntegral< RemoveCV_t< T > >::type { };
/// Check whether `T` is a floating-point type. \remark See `std::is_floating_point`
template < class T > struct IsFloatingPoint : public Impl::Helper_IsFloatingPoint< RemoveCV_t< T > >::type { };

/// Check whether `T` is a C array type. \remark See `std::is_array`
template < class > struct IsCArray : public FalseType { };
/// Partial specialization for the case when `T` is an array of known size.
template < class T, size_t N > struct IsCArray< T[N] > : public TrueType { };
/// Partial specialization for the case when `T` is an array of unknown size.
template < class T > struct IsCArray< T[] > : public TrueType { };

/// Check whether `T` is an array container. \remark Not a standard feature!
template < class T > struct IsArrayC : public FalseType { };
/// Partial specialization for the case when `T` is an array container.
template < class T, size_t N > struct IsArrayC< Array< T, N > > : public TrueType { };

/// Check whether `T` is an array type. \remark Not a standard feature!
template < class T > struct IsArray : public IsCArray< T > { };
/// Partial specialization for the case when `T` is an array container.
template < class T, size_t N > struct IsArray< Array< T, N > > : public TrueType { };

/// Check whether `T` is an enumeration type. \remark See `std::is_enum`
template < class T > struct IsEnum : public BoolConstant< __is_enum(T) > { };
/// Check whether `T` is a union type. \remark See `std::is_union`
template < class T > struct IsUnion : public BoolConstant< __is_union(T) > { };
/// Check whether `T` is a class type. \remark See `std::is_class`
template < class T > struct IsClass : public BoolConstant< __is_class(T) > { };

// Deal with some MSC specific issues
#if defined(UH_MSC) && !defined(UH_CLANG)
    #pragma warning(disable: 4180) // qualifier applied to function type has no meaning; ignored
#endif
/// Check whether `T` is a function type. \remark See `std::is_function`
template < class T > struct IsFunction : public BoolConstant< !IsConst_v< const T > > { };
// Partial specialization of %IsFunction for lvalue type.
template < class T > struct IsFunction< T & > : public FalseType { };
// Partial specialization of %IsFunction for rvalue type.
template < class T > struct IsFunction< T && > : public FalseType { };

/// Check whether `T` is a pointer to object or a pointer to function. \remark See `std::is_pointer`
template < class T > struct IsPointer : public Impl::Helper_IsPointer< RemoveCV_t< T > >::type { };

/// Check whether `T` is a l-value reference type. \remark See `std::is_lvalue_reference`
template < class > struct IsLvalueReference : public FalseType { };
template < class T > struct IsLvalueReference< T & > : public TrueType { };

/// Check whether `T` is a r-value reference type. \remark See `std::is_rvalue_reference`
template < class > struct IsRvalueReference : public FalseType { };
template < class T > struct IsRvalueReference< T && > : public TrueType { };

/// Check whether `T` is a non-static member object. \remark See `std::is_member_object_pointer`
template < class T > struct IsMemberObjectPointer : public Impl::Helper_IsMemberObjectPointer< RemoveCV_t< T > >::type { };
/// Check whether `T` is a non-static member function pointer. \remark See `std::is_member_function_pointer`
template < class T > struct IsMemberFunctionPointer : public Impl::Helper_IsMemberFunctionPointer< RemoveCV_t< T > >::type { };

/// Check whether `T` is a fundamental type. \remark See `std::is_fundamental`
template < class T > struct IsFundamental : public Disjunction_t< IsArithmetic< T >, IsVoid< T >, IsNullPointer< T > > { };
/// Check whether `T` is an arithmetic type. \remark See `std::is_arithmetic`
template < class T > struct IsArithmetic : public Disjunction_t< IsIntegral< T >, IsFloatingPoint< T > > { };
/// Check whether `T` is a scalar type. \remark See `std::is_scalar`
template < class T > struct IsScalar : public Disjunction_t< IsArithmetic< T >, IsEnum< T >, IsPointer< T >, IsMemberPointer< T >, IsNullPointer< T > > { };
/// Check whether `T` is an object type. \remark See `std::is_object`
template < class T > struct IsObject : public Negation_t< Disjunction< IsFunction< T >, IsReference< T >, IsVoid< T > > > { };
/// Check whether `T` is a compound type. \remark See `std::is_compound`
template < class T > struct IsCompound : public Negation_t< IsFundamental< T > > { };
/// Check whether `T` is a reference type. \remark See `std::is_reference`
template < class T > struct IsReference : public Disjunction_t< IsLvalueReference< T >, IsRvalueReference< T > > { };
/// Check whether `T` is pointer to non-static member object or member function. \remark See `std::is_member_pointer`
template < class T > struct IsMemberPointer : public Impl::Helper_IsMemberPointer< RemoveCV_t< T > >::type { };

/// Check whether `T` is a const-qualified type. \remark See `std::is_const`
template < class > struct IsConst : public FalseType { };
template < class T > struct IsConst< T const > : public TrueType { };

/// Check whether `T` is a volatile-qualified type. \remark See `std::is_volatile`
template < class > struct IsVolatile : public FalseType { };
template < class T > struct IsVolatile< T volatile > : public TrueType { };

/// Check whether `T` is trivial type. \remark See `std::is_trivial`
template < class T > struct IsTrivial : public BoolConstant< __is_trivial(T) > { };
/// Check whether `T` is a trivially copyable type. \remark See `std::is_trivially_copyable`
template < class T > struct IsTriviallyCopyable : public BoolConstant< __is_trivially_copyable(T) > { };
/// Check whether `T` is a standard layout type. \remark See `std::is_standard_layout`
template < class T > struct IsStandardLayout : public BoolConstant< __is_standard_layout(T) > { };
/// Check whether `T` is a POD type ("plain old data type"). \remark See `std::is_pod`
template < class T > struct IsPOD : public BoolConstant< __is_pod(T) > { };
/// Check whether `T` satisfies all requirements of literal type. \remark See `std::is_literal_type`
template < class T > struct IsLiteralType : public BoolConstant< __is_literal_type(T) > { };
/// Check whether `T` is an empty type. \remark See `std::is_empty`
template < class T > struct IsEmpty : public BoolConstant< __is_empty(T) > { };
/// Check whether `T` is a polymorphic class. \remark See `std::is_polymorphic`
template < class T > struct IsPolymorphic : public BoolConstant< __is_polymorphic(T) > { };
/// Check whether `T` is an abstract class. \remark See `std::is_abstract`
template < class T > struct IsAbstract : public BoolConstant< __is_abstract(T) > { };
/// Check whether `T` is a final class. \remark See `std::is_final`
template < class T > struct IsFinal : public BoolConstant< __is_final(T) > { };
/// Checks if T is an aggregate type. \remark See `std::is_aggregate`
template < class T > struct IsAggregate : BoolConstant< __is_aggregate(RemoveCV_t< T >) > { };
/// Check whether `T` is a signed arithmetic type. \remark See `std::is_signed`
template < class T > struct IsSigned : public Impl::Helper_IsSigned< T >::type { };
/// Check whether `T` is a unsigned arithmetic type. \remark See `std::is_unsigned`
template < class T > struct IsUnsigned : public Conjunction_t< IsArithmetic< T >, Negation< IsSigned< T > > > { };

/// Checks whether T is an array type of known bound. \remark See `std::is_bounded_array`
template < class T > struct IsBoundedArray : FalseType { };
template < class T, size_t N > struct IsBoundedArray< T[N] > : TrueType { };

/// Checks whether T is an array type of unknown bound. \remark See `std::is_unbounded_array`
template < class T > struct IsUnboundedArray : FalseType { };
template < class T > struct IsUnboundedArray< T[] > : TrueType { };

/// Check whether `T` is a constructible type. \remark See `std::is_constructible`
template < class T, class... Args > struct IsConstructible : public BoolConstant< __is_constructible(T, Args...) > { };
/// Check whether `T` is a trivially constructible type. \remark See `std::is_trivially_constructible`
template < class T, class... Args > struct IsTriviallyConstructible : public BoolConstant< __is_trivially_constructible(T, Args...) > { };
/// Check whether `T` is a no-throw constructible type. \remark See `std::is_nothrow_constructible`
template < class T, class... Args > struct IsNoThrowConstructible
#if defined(UH_MSC) || (defined(UH_GNUC) && (UH_GNUC_MAJOR >= 11))
    : public BoolConstant< __is_nothrow_constructible(T, Args...) >
#else
    : public Conjunction< IsConstructible< T, Args... >, Impl::Helper_IsNoThrowConstructible< T, Args... > >
#endif
{ };

/// Check whether `T` is a default constructible type. \remark See `std::is_default_constructible`
template < class T > struct IsDefaultConstructible : public BoolConstant< __is_constructible(T) > { };
/// Check whether `T` is a trivially default constructible type. \remark See `std::is_trivially_default_constructible`
template < class T > struct IsTriviallyDefaultConstructible : public BoolConstant< __is_trivially_constructible(T) > { };
/// Check whether `T` is a no-throw default constructible type. \remark See `std::is_nothrow_default_constructible`
template < class T > struct IsNoThrowDefaultConstructible
#if defined(UH_MSC) || (defined(UH_GNUC) && (UH_GNUC_MAJOR >= 11))
    : public BoolConstant< __is_nothrow_constructible(T) >
#else
    : public Conjunction< IsDefaultConstructible< T >, Impl::Helper_IsNoThrowDefaultConstructible< T > >
#endif
{ };

/// Check whether `T` is a copy constructible type. \remark See `std::is_copy_constructible`
template < class T > struct IsCopyConstructible : public Impl::Helper_IsCopyConstructible< T > { };
/// Check whether `T` is a trivially copy constructible type. \remark See `std::is_trivially_copy_constructible`
template < class T > struct IsTriviallyCopyConstructible : public Impl::Helper_IsTriviallyCopyConstructible< T > { };
/// Check whether `T` is a no-throw copy constructible type. \remark See `std::is_nothrow_copy_constructible`
template < class T > struct IsNothrowCopyConstructible : public Impl::Helper_IsNoThrowCopyConstructible< T > { };

/// Check whether `T` is a move constructible type. \remark See `std::is_move_constructible`
template < class T > struct IsMoveConstructible : public Impl::Helper_IsMoveConstructible< T > { };
/// Check whether `T` is a trivially move constructible type. \remark See `std::is_trivially_move_constructible`
template < class T > struct IsTriviallyMoveConstructible : public Impl::Helper_IsTriviallyMoveConstructible< T > { };
/// Check whether `T` is a no-throw move constructible type. \remark See `std::is_nothrow_move_constructible`
template < class T > struct IsNoThrowMoveConstructible : public Impl::Helper_IsNoThrowMoveConstructible< T > { };

/// Check whether `T` is an assignable type. \remark See `std::is_assignable`
template < class T, class U > struct IsAssignable : public BoolConstant< __is_assignable(T, U) > { };
/// Check whether `T` is a trivially assignable type. \remark See `std::is_trivially_assignable`
template < class T, class U > struct IsTriviallyAssignable : public BoolConstant< __is_trivially_assignable(T, U) > { };
/// Check whether `T` is a no-throw assignable type. \remark See `std::is_nothrow_assignable`
template < class T, class U > struct IsNoThrowAssignable : public Conjunction< IsAssignable< T, U >, Impl::Helper_IsNoThrowAssignable< T, U > > { };

/// Check whether `T` is a copy assignable type. \remark See `std::is_copy_assignable`
template < class T > struct IsCopyAssignable : public Impl::Helper_IsCopyAssignable< T > { };
/// Check whether `T` is a trivially copy assignable type. \remark See `std::is_trivially_copy_assignable`
template < class T > struct IsTriviallyCopyAssignable : public Impl::Helper_IsTriviallyCopyAssignable< T > { };
/// Check whether `T` is a no-throw copy assignable type. \remark See `std::is_nothrow_copy_assignable`
template < class T > struct IsNoThrowCopyAssignable : public Impl::Helper_IsNoThrowCopyAssignable< T > { };

/// Check whether `T` is a move assignable type. \remark See `std::is_move_assignable`
template < class T > struct IsMoveAssignable : public Impl::Helper_IsMoveAssignable< T > { };
/// Check whether `T` is a trivially move assignable type. \remark See `std::is_trivially_move_assignable`
template < class T > struct IsTriviallyMoveAssignable : public Impl::Helper_IsTriviallyMoveAssignable< T > { };
/// Check whether `T` is a no-throw move assignable type. \remark See `std::is_nothrow_move_assignable`
template < class T > struct IsNoThrowMoveAssignable : public Impl::Helper_IsNoThrowMoveAssignable< T > { };

/// Check whether `T` is a destructible type. \remark See `std::is_destructible`
template < class T > struct IsDestructible
#ifdef UH_MSC
    : public BoolConstant< __is_destructible(T) >
#else
    : public Impl::Helper_IsDestructibleSafe< T >::type
#endif
{ };
/// Check whether `T` is a trivially destructible type. \remark See `std::is_trivially_destructible`
template < class T > struct IsTriviallyDestructible : public Conjunction< IsDestructible< T >, BoolConstant< __has_trivial_destructor(T) > > { };
/// Check whether `T` is a no-throw destructible type. \remark See `std::is_nothrow_destructible`
template < class T > struct IsNoThrowDestructible
#ifdef UH_MSC
    : public BoolConstant< __is_nothrow_destructible(T) >
#else
    : public Impl::Helper_IsNoThrowDestructibleSafe< T >::type
#endif
{ };

/// Check whether `T` is a type with a virtual destructor. \remark See `std::has_virtual_destructor`
template < class T > struct HasVirtualDestructor : public BoolConstant< __has_virtual_destructor(T) > { };

/// Check whether objects of type `T` can be swapped with objects of type `U`. \remark See `std::is_swappable_with`
template < class T, class U > struct IsSwappableWith : public Impl::Helper_IsSwappableWith< T, U >::type { };
/// Check whether objects of type `T` can be swapped between them. \remark See `std::is_swappable`
template < class T > struct IsSwappable : public Impl::Helper_IsSwappable<T>::type { };

/// Same as \ref IsSwappableWith but is known to not throw exceptions. \remark See `std::is_nothrow_swappable_with`
template < class T, class U > struct IsNoThrowSwappableWith : public Impl::Helper_IsNoThrowSwappableWith< T, U >::type { };
/// Same as \ref IsSwappable but is known to not throw exceptions. \remark See `std::is_nothrow_swappable`
template < class T > struct IsNoThrowSwappable : public Impl::Helper_IsNoThrowSwappable< T >::type { };

/// Provides the member constant value equal to the alignment requirement of the type `T`, as if obtained by an `alignof` expression. \remark See `std::alignment_of`
template < class T > struct AlignmentOf : public IntegralConstant< size_t, alignof(T) > { };

/// If `T` is an array type, provides the member constant value equal to the number of dimensions of the array. For any other type, value is 0. \remark See `std::rank`
template < class T > struct Rank : public IntegralConstant< size_t, 0 > { }; 
template < class T > struct Rank< T[] > : public IntegralConstant< size_t, Rank_v< T > + 1 > { }; 
template < class T, size_t N > struct Rank< T[N] > : public IntegralConstant< size_t, Rank_v< T > + 1 > { };

/// If `T` is an array type, provides the member constant value equal to the number of elements along the Nth dimension of the array. \remark See `std::extent`
template < class T, unsigned N = 0 > struct Extent : IntegralConstant< size_t, 0 > { };
template < class T > struct Extent< T[], 0 > : IntegralConstant< size_t, 0 > { };
template < class T, unsigned N > struct Extent< T[], N > : Extent< T, N-1 > { };
template < class T, size_t I > struct Extent< T[I], 0 > : IntegralConstant< size_t, I > { };
template < class T, size_t I, unsigned N > struct Extent< T[I], N > : Extent< T, N-1 > { };

/// Check whether `T` and `U` mName the same type with the same const-volatile qualifications. \remark See `std::is_same`
template < class, class > struct IsSame : public FalseType { };
template < class T > struct IsSame< T, T > : public TrueType { };

/// Check whether `D` is derived from `B` or if both are the same non-union class. \remark See `std::is_base_of`
template < class B, class D > struct IsBaseOf : public BoolConstant< __is_base_of(B, D) > { };

/// Check whether objects of of type `F` can be converted to objects of type `T`. \remark See `std::is_convertible`
template < class F, class T > struct IsConvertible
#ifdef UH_MSC
    : public BoolConstant< __is_convertible_to(F, T) >
#else
    : public Impl::Helper_IsConvertible< F, T >::type
#endif
{ };
/// Same as \ref IsConvertible but is known to not throw exceptions. \remark See `std::is_nothrow_convertible`
template < class F, class T > struct IsNoThrowConvertible : public Impl::Helper_IsNoThrowConvertible< F, T >::type { };

/// Provides the member typedef type which is the same as `T`, except that its topmost cv-qualifiers are removed. \remark See `std::remove_cv`
template < class T > struct RemoveCV { typedef RemoveConst_t< RemoveVolatile_t< T > > type; };
/// Provides the member typedef type which is the same as `T`, except that its topmost const-qualifier is removed. \remark See `std::remove_const`
template < class T > struct RemoveConst { typedef T type; };
/// Partial specialization for the case where `T` is actually const and that qualifier must be stripped away.
template < class T > struct RemoveConst< T const > { typedef T type; };
/// Provides the member typedef type which is the same as `T`, except that its topmost volatile-qualifier is removed. \remark See `std::remove_volatile`
template < class T > struct RemoveVolatile { typedef T type; };
/// Partial specialization for the case where `T` is actually volatile and that qualifier must be stripped away.
template < class T > struct RemoveVolatile< T volatile > { typedef T type; };

/// Provides the member typedef type which is the same as `T`, except it has a cv-qualifier added. \remark See `std::add_cv`
template < class T > struct AddCV { typedef AddConst_t< AddVolatile_t< T > > type; };
/// Provides the member typedef type which is the same as `T`, except it has a const-qualifier added. \remark See `std::add_const`
template < class T > struct AddConst { typedef T const type; };
/// Provides the member typedef type which is the same as `T`, except it has a volatile-qualifier added. \remark See `std::add_volatile`
template < class T > struct AddVolatile { typedef T volatile type; };

/// If the type `T` is a reference type, provides the member typedef type which is the type referred to by `T`. \remark See `std::remove_reference`
template < class T > struct RemoveReference { typedef T type; };
/// Partial specialization for the case where `T` is an l-value reference type.
template < class T > struct RemoveReference< T & > { typedef T type; };
/// Partial specialization for the case where `T` is an r-value reference type.
template < class T > struct RemoveReference< T && > { typedef T type; };

/// If `T` is an object type or a function type that has no cv- or ref- qualifier, provides a member typedef type which is `T&`. \remark See `std::add_lvalue_reference`
template < class T > struct AddLvalueReference : decltype(Impl::Helper_TryAddLvalueReference< T >(0)) { };
/// If `T` is an object type or a function type that has no cv- or ref- qualifier, provides a member typedef type which is `T&&`, otherwise type is `T`. \remark See `std::add_rvalue_reference`
template < class T > struct AddRvalueReference : decltype(Impl::Helper_TryAddRvalueReference< T >(0)) { };

/// Provides the member typedef type which is the type pointed to by `T`, or, if `T` is not a pointer, then type is the same as `T`. \remark See `std::remove_pointer`
template < class T > struct RemovePointer { typedef T type; };
template < class T > struct RemovePointer< T * > { typedef T type; };
template < class T > struct RemovePointer< T * const > { typedef T type; };
template < class T > struct RemovePointer< T * volatile > { typedef T type; };
template < class T > struct RemovePointer< T * const volatile > { typedef T type; };

/// If `T` is a reference type, then provides the member typedef type which is a pointer to the referred type. \remark See `std::add_pointer`
template < class T > struct AddPointer : decltype(Impl::Helper_TryAddPointer< T >(0)) { };

/// If `T` is an integral (except bool) or enumeration type, provides the member typedef type which is the signed integer type corresponding to `T`, with the same cv-qualifiers. \remark See `std::make_signed`
template < class T > struct MakeSigned { typedef typename Impl::Helper_MakeSigned< T >::type type; };
/// If `T` is an integral (except bool) or enumeration type, provides the member typedef type which is the unsigned integer type corresponding to `T`, with the same cv-qualifiers. \remark See `std::make_unsigned`
template < class T > struct MakeUnsigned { typedef typename Impl::Helper_MakeUnsigned< T >::type type; };

/// If `T` is an array of some type `X`, provides the member typedef type equal to `X`, otherwise type is `T`. Note that if `T` is a multidimensional array, only the first dimension is removed. \remark See `std::remove_extent`
template < class T > struct RemoveExtent { typedef T type; };
template < class T, size_t N > struct RemoveExtent< T[N] > { typedef T type; };
template < class T > struct RemoveExtent< T[] > { typedef T type; };

/// If `T` is a multidimensional array of some type `X`, provides the member typedef type equal to `X`, otherwise type is `T`. \remark See `std::remove_all_extents`
template < class T > struct RemoveAllExtents { typedef T type; };
template < class T, size_t N > struct RemoveAllExtents< T[N] > { typedef typename RemoveAllExtents< T >::type type; };
template < class T > struct RemoveAllExtents< T[] > { typedef typename RemoveAllExtents< T >::type type; };

/// Provides the nested `type` type, which is a trivial standard-layout type suitable for use as uninitialized storage for any object whose size is at most `L` and whose alignment requirement is a divisor of `A`. \remark See `std::aligned_storage`
template < size_t N, size_t A = alignof(max_align_t) > struct AlignedStorage { typedef typename Impl::Helper_AlignedStorage< N, A >::type type; };

/// Applies l-value-to-r-value, array-to-pointer, and function-to-pointer implicit conversions to the type `T`, removes cv-qualifiers, and defines the resulting type as the member typedef type. \remark See `std::decay`
template < class T > struct Decay { typedef typename Impl::Helper_DecaySelector< RemoveReference_t< T > >::type type; };
/// If the type T is a reference type, provides the member typedef type which is the type referred to by T with its topmost cv-qualifiers removed. Otherwise type is T with its topmost cv-qualifiers removed. \remark See `std::remove_cvref`
template < class T > struct RemoveCvRef { typedef RemoveCV_t< RemoveReference_t< T > > type; };

/// Convenient meta-function used to leverage SFINAE. \remark See `std::enable_if`
template < bool, class T = void > struct EnableIf { };
// Partial specialization for the case where `T` is `true`.
template < class T > struct EnableIf< true, T > { typedef T type; };

/// Provides member typedef `type`, which is defined as `T` if `B` is true at compile time, or as `F` if `B` is false. \remark See `std::conditional`
template < bool C, class T, class F > struct Conditional { typedef T type; };
/// Partial specialization of \ref Conditional for the case `C` is false.
template < class T, class F > struct Conditional< false, T, F > { typedef F type; };

/// Forms the logical conjunction of the type traits `B...`, effectively performing a logical `AND` on the sequence of traits. \remark See `std::conjunction`
template < class... > struct Conjunction;
/// Partial specialization of \ref Conjunction for the case where no parameters are given.
template < > struct Conjunction< > : public TrueType { };
/// Partial specialization of \ref Conjunction for the case where a single parameter is given.
template < class B1 > struct Conjunction< B1 > : public B1 { };
/// Partial specialization of \ref Conjunction for the case where two parameters are given.
template < class B1, class B2 > struct Conjunction< B1, B2 > : public Conditional< B1::value, B2, B1 >::type { };
/// Partial specialization of \ref Conjunction for the case where more than two parameters are given.
template < class B1, class B2, class B3, class... Bn>
    struct Conjunction< B1, B2, B3, Bn... > : public Conditional< B1::value, Conjunction< B2, B3, Bn... >, B1 >::type { };

/// Forms the logical conjunction of the type traits `B...`, effectively performing a logical `AND` on the sequence of traits. \remark See `std::conjunction`
template < class... > struct Disjunction;
/// Partial specialization of \ref Disjunction for the case where no parameters are given.
template < > struct Disjunction< > : public FalseType { };
/// Partial specialization of \ref Disjunction for the case where a single parameter is given.
template < class B1 > struct Disjunction< B1 > : public B1 { };
/// Partial specialization of \ref Disjunction for the case where two parameters are given.
template < class B1, class B2 > struct Disjunction< B1, B2 > : public Conditional< B1::value, B1, B2>::type { };
/// Partial specialization of \ref Disjunction for the case where more than two parameters are given.
template < class B1, class B2, class B3, class... Bn >
    struct Disjunction< B1, B2, B3, Bn... > : public Conditional< B1::value, B1, Disjunction< B2, B3, Bn... > >::type { };

/// Forms the logical negation of the type trait `B`. \remark See `std::negation`
template < class B > struct Negation : public BoolConstant< !bool(B::value) > { };

/// Used to perform a compile time equality comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonEQ : public BoolConstant< (A == B) > { };
/// Used to perform a compile time inequality comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonNE : public BoolConstant< (A != B) > { };
/// Used to perform a compile time less than comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonLT : public BoolConstant< (A < B) > { };
/// Used to perform a compile time greater than comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonGT : public BoolConstant< (A > B) > { };
/// Used to perform a compile time less than or equal comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonLE : public BoolConstant< (A <= B) > { };
/// Used to perform a compile time greater than or equal comparison between two values. Used mainly for SFINAE. Not a standard feature!
template < class T, T A, T B > struct ComparisonGE : public BoolConstant< (A >= B) > { };

/// Used to select a compile time value based on the outcome of another. The `true` case where `T` is selected. Not a standard feature.
template < bool B, class V, V T, V F > struct Contingent : public IntegralConstant< V, T > { };
/// Partial specialization of \ref Contingent for the `false` case when the `F` template parameter must be selected.
template < class V, V T, V F > struct Contingent< false, V, T, F > : public IntegralConstant< V, F > { };

/// Used to check if the two types have the same size. Not a standard feature.
template < class T, class U > struct IsSameSize : public BoolConstant< (sizeof(T) == sizeof(U)) > { };
/// Used to check if the size of one type is greater than another. Not a standard feature.
template < class T, class U > struct IsGreaterSize : public BoolConstant< (sizeof(T) > sizeof(U)) > { };
/// Used to check if the size of one type is smaller than another. Not a standard feature.
template < class T, class U > struct IsSmallerSize : public BoolConstant< (sizeof(T) < sizeof(U)) > { };

/// Used to check if a type is small enough to fit in a pointer. Not a standard feature.
template < class T > struct FitsInPointer : public BoolConstant< sizeof(T) <= UH_POINTER_BYTES > { };
/// Used to check if a type can fit in the same memory as another type. Not a standard feature.
template < class T, class U > struct FitsInSameMemory : public BoolConstant< (sizeof(T) <= sizeof(U)) && (alignof(T) <= alignof(U)) > { };

/// Used to extract the size of a type and turn it into an integral constant. Not a standard feature.
template < class T > struct TypeSize : public IntegralConstant< size_t, sizeof(T) > { };
/// Partial specialization of \ref TypeSize used to handle the case where the specified type is `void`.
template < > struct TypeSize< void > : public IntegralConstant< size_t, 0 > { };

/// Used to extract the number of elements in a parameter pack. Not a standard feature.
template < class... Ts > struct PackSize : public IntegralConstant< size_t, sizeof...(Ts) > { };

/// Used to compute the sum size of all elements in a parameter pack. Not a standard feature.
template < class... Ts > struct CollectiveSize;
/// Partial specialization of \ref CollectiveSize to handle empty parameter packs.
template < > struct CollectiveSize< > : IntegralConstant< size_t, 0 >{ };
/// Partial specialization of \ref CollectiveSize used to process the last (or only) type in the parameter pack.
template < class T > struct CollectiveSize< T > : IntegralConstant< size_t, TypeSize_v< T > >{ };
/// Partial specialization of \ref CollectiveSize used to iterate the parameter pack and process each element.
template < class T, class... Ts > struct CollectiveSize< T, Ts... > : IntegralConstant< size_t, TypeSize_v< T > + CollectiveSize< Ts... >::value >{ };

/// Check whether `T` is a complete type. Not a standard feature.
template < class, class = size_t > struct IsComplete : FalseType { };
/// Partial specialization of \ref IsComplete for cases where given template parameter is a complete type.
template < class T > struct IsComplete< T, decltype(sizeof(T)) > : TrueType { };

/// Utility to detect referenceable types. Not a standard feature.
template < class T > struct IsReferenceable : public Impl::Helper_IsReferenceable< T >::type { };

/// Utility for constructing identically cv-qualified types. Not a standard feature.
template < class Q, class U > struct MatchCV { typedef typename Impl::Helper_MatchCV< U, IsConst_v< Q >, IsVolatile_v< Q > >::type type; };

/// Used to decay a type and strip the \ref ReferenceWrapper type from it.
template < class T > struct DecayAndStrip { typedef typename Impl::Helper_StripReferenceWrapper< Decay_t< T > >::type type; };

/// Check whether `T` is actually a `std::in_place_type` type. Not a standard feature.
template < class T > struct IsInPlaceType : public Impl::Helper_IsInPlaceType< T > { };

/// Used to check if a type is implicitly default constructible.
template < class T > struct IsImplicitlyDefaultConstructible
    : public Conjunction< IsDefaultConstructible< T >, Impl::Helper_IsImplicitlyDefaultConstructibleSafe< T > > { };

/// Helper type-trait used to check if a type's copy constructor can be used when the move constructor could throw.
template < class T > struct NeedsUnsafeMoveConstructorAlternative
    : public Conjunction< Negation< IsNoThrowMoveConstructible< T > >, IsCopyConstructible< T > > { };

/// Helper type-trait used to identify what kind of signed integer is required to store the specified value. Not a standard feature.
template < int64_t V > struct SignedValueFit : public Impl::Helper_SignedValueFit8< V > { };

/// Helper type-trait used to identify what kind of unsigned integer is required to store the specified value. Not a standard feature.
template < uint64_t V > struct UnsignedValueFit : public Impl::Helper_UnsignedValueFit8< V > { };

/// Helper which can be used to tell if one type can be copied (verbatim) into the memory of another and have a valid result.
template < class, class > struct IsBinaryCompatible : public FalseType { };

// Specializations of %IsBinaryCompatible for fundamental types.
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

// Specializations of %IsBinaryCompatible for fundamental types.
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

/// Check whether `T` is reference counted. I.e. It inherits from ReferenceCounted. \remark Not a standard feature!
template < class T > struct IsReferenceCounted : public IsBaseOf< ReferenceCounted, T > { };

/// Check whether `T` is a \ref Span type. \remark Not a standard feature!
template < class > struct IsSpan : FalseType { };
/// Partial specialization for the case when `T` is a \ref Span type.
template < class T, size_t S > struct IsSpan< Span< T, S > > : TrueType { };

/// Check whether `T` is a \ref StrView type. \remark Not a standard feature!
template < class > struct IsStrView : FalseType { };
/// Partial specialization for the case when `T` is a \ref StrView type.
template < > struct IsStrView< StrView > : TrueType { };

/// Used to specify what kind of view can be used with a certain type.
template < class T > struct ViewOfType;

/// Used to specify that a type can be recycled and should not be destroyed if another is assigned into his place.
template < class T > struct IsRecycledType : public FalseType { };
/// Used to specify that a type can be viewed into.
template < class T > struct IsViewableType : public FalseType { };

/// Helper which can be used to tell if a type is considered an integral primitive, that is, integer or pointer. \remark Not a standard feature.
template < class T > struct IsIntegralPrimitive : public Disjunction< IsIntegral< T >, IsPointer< T > > { };

/// Check whether `Pair< T1, T2 >` is composed two integral types. \remark Not a standard feature.
template < class T > struct IsIntegralPair : public FalseType { };
/// Specialization of %IsIntegralPair for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsIntegralPair< Pair< T1, T2 > >
    : public Conjunction< IsIntegral< T1 >, IsIntegral< T2 > > { };

/// Check whether `Pair< T1, T2 >` is composed two pointer types. \remark Not a standard feature.
template < class T > struct IsPointerPair : public FalseType { };
/// Specialization of %IsPointerPair for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsPointerPair< Pair< T1, T2 > >
    : public Conjunction< IsPointer< T1 >, IsPointer< T2 > > { };

/// Check whether `Pair< T1, T2 >` is composed two floating point types. \remark Not a standard feature.
template < class T > struct IsFloatingPointPair : public FalseType { };
/// Specialization of %IsFloatingPointPair for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsFloatingPointPair< Pair< T1, T2 > >
    : public Conjunction< IsFloatingPoint< T1 >, IsFloatingPoint< T2 > > { };

/// Check whether `Pair< T1, T2 >` is composed two fundamental types. \remark Not a standard feature.
template < class T > struct IsFundamentalPair : public FalseType { };
/// Specialization of %IsFundamentalPair for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsFundamentalPair< Pair< T1, T2 > >
    : public Conjunction< IsFundamental< T1 >, IsFundamental< T2 > > { };

/// Check whether `Pair< T1, T2 >` is composed two integral primitive types. \remark Not a standard feature.
template < class T > struct IsIntegralPrimitivePair : public FalseType { };
/// Specialization of %IsIntegralPrimitivePair for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsIntegralPrimitivePair< Pair< T1, T2 > >
    : public Conjunction< IsIntegralPrimitive< T1 >, IsIntegralPrimitive< T2 > > { };

/// Helper which can be used to tell if a type is composed of only integral types. \remark Not a standard feature.
template < class T > struct IsIntegralOnly : public IsIntegral< T > {  };
/// Specialization of %IsIntegralOnly for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsIntegralOnly< Pair< T1, T2 > > : public IsIntegralPair< Pair< T1, T2 > > {  };

/// Helper which can be used to tell if a type is composed of pointer types. \remark Not a standard feature.
template < class T > struct IsPointerOnly : public IsPointer< T > {  };
/// Specialization of %IsPointerOnly for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsPointerOnly< Pair< T1, T2 > > : public IsPointerPair< Pair< T1, T2 > > {  };

/// Helper which can be used to tell if a type is composed of floating point types. \remark Not a standard feature.
template < class T > struct IsFloatingPointOnly : public IsFloatingPoint< T > {  };
/// Specialization of %IsFloatingPointOnly for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsFloatingPointOnly< Pair< T1, T2 > > : public IsFloatingPointPair< Pair< T1, T2 > > {  };

/// Helper which can be used to tell if a type is composed of integral primitives, that is, integer or pointer. \remark Not a standard feature.
template < class T > struct IsIntegralPrimitiveOnly : public IsIntegralPrimitive< T > {  };
/// Specialization of %IsIntegralPrimitiveOnly for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsIntegralPrimitiveOnly< Pair< T1, T2 > > : public IsIntegralPrimitivePair< Pair< T1, T2 > > {  };

/// Helper which can be used to tell if a type is composed of fundamental types. \remark Not a standard feature.
template < class T > struct IsFundamentalOnly : public IsFundamental< T > {  };
/// Specialization of %IsFundamentalOnly for %Pair type that checks both \ref Pair::FirstType and \ref Pair::SecondType.
template < class T1, class T2 > struct IsFundamentalOnly< Pair< T1, T2 > > : public IsFundamentalPair< Pair< T1, T2 > > {  };

/// Check whether `T` is a type that has `Size()` and `Data()` member methods. \remark Not a standard feature!
template < class, class > struct HasSizeAndData : FalseType { };
/// Partial specialization for the case when `T` is actually has `Size()` and `Data()` member methods.
template < class T > struct HasSizeAndData< T, Void_t< decltype(Size(declval< T >())), decltype(Data(declval< T >())) > > : TrueType { };

/// Given `Template< T, ... >` return \p T, otherwise invalid.
template < class T > struct GetFirstArg { using type = Undefined_t; };
/// Specialization of \ref GetFirstArg which meets the requirements and can extract \p T.
template < template < class, class... > class Template, class T, class... Types >
struct GetFirstArg< Template< T, Types... > > { using type = T; };

/// Given `Template< T, ... >` and \p U return `Template< U, ... >`, otherwise invalid.
template < class T, class U > struct ReplaceFirstArg { };
/// Specialization of \ref ReplaceFirstArg which meets the requirements and can replace \p T with \p U.
template < template < class, class... > class Template, class U, class T, class... Types >
struct ReplaceFirstArg< Template< T, Types... >, U> { using type = Template< U, Types... >; };


/// Helper type trait that can be used to extract the type at a certain index from a parameter pack. \remark Not a standard feature.
template < size_t I, class T, class... Ts > struct ExtractTypeAt
{
    static_assert(I <= sizeof...(Ts), "Index outside the range of parameter pack");
    using type = typename ExtractTypeAt< I - 1, Ts... >::type;
};
/// Specialization of \ref ExtractTypeAt which acts as the break and assumes the requested type was found.
/// When index reaches zero, the member `type` will be defined as template parameter \p T.
template < class T, class... Ts > struct ExtractTypeAt< 0, T, Ts... >
{
    using type = T;
};

/// Helper type trait that can be used to check if a type has a `Type` member typedef. \remark Not a standard feature.
template < class, class > struct HasTypeMemberType : public FalseType {};
/// Specialization of \ref HasTypeMemberType for when the specified type has the `Type` member typedef.
template < class T > struct HasTypeMemberType< T, Void_t< typename T::Type > > : TrueType {};

/// Helper type trait that can be used to check if a type has a `ValueType` member typedef. \remark Not a standard feature.
template < class, class > struct HasValueMemberType : public FalseType {};
/// Specialization of \ref HasValueMemberType for when the specified type has the `ValueType` member typedef.
template < class T > struct HasValueMemberType< T, Void_t< typename T::ValueType > > : TrueType {};

/// Helper type trait that can be used to check if a type has a `Reference` member typedef. \remark Not a standard feature.
template < class, class > struct HasReferenceMemberType : public FalseType {};
/// Specialization of \ref HasValueMemberType for when the specified type has the `Reference` member typedef.
template < class T > struct HasReferenceMemberType< T, Void_t< typename T::Reference > > : TrueType {};

/// Helper type trait that can be used to check if a type has a `Pointer` member typedef. \remark Not a standard feature.
template < class, class > struct HasPointerMemberType : public FalseType {};
/// Specialization of \ref HasValueMemberType for when the specified type has the `Pointer` member typedef.
template < class T > struct HasPointerMemberType< T, Void_t< typename T::Pointer > > : TrueType {};

/// Helper type trait that can be used to check if a type has a `SizeType` member typedef. \remark Not a standard feature.
template < class, class > struct HasSizeMemberType : public FalseType {};
/// Specialization of \ref HasValueMemberType for when the specified type has the `SizeType` member typedef.
template < class T > struct HasSizeMemberType< T, Void_t< typename T::SizeType > > : TrueType {};

/// Helper type trait that can be used to check if a type has a `DifferenceType` member typedef. \remark Not a standard feature.
template < class, class > struct HasDifferenceMemberType : public FalseType {};
/// Specialization of \ref HasValueMemberType for when the specified type has the `DifferenceType` member typedef.
template < class T > struct HasDifferenceMemberType< T, Void_t< typename T::DifferenceType > > : TrueType {};

} // Namespace:: Urho3D
