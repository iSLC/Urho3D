//
// Copyright (c) 2008-2022 the Urho3D project.
//

/*
 * Extended type traits and type utilities.
 * Anything that doesn't make it into main TypeTraits header
 * and is common enough to be used in more than one place.
*/

#pragma once

#include "../Base/TypeTraits.h"

/*
 * Forward declarations.
*/
namespace Urho3D {

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
template < class... > struct PackSize;
template < class... > struct CollectiveSize;
template < int64_t V > struct SignedValueFit;
template < uint64_t V > struct UnsignedValueFit;
template < class, class > struct IsBinaryCompatible;
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
/// Helper of \ref SignedValueFit used to avoid having to explicitly accessing the `type` member.
template < int64_t V > using SignedValueFit_t = typename SignedValueFit< V >::type;
/// Helper of \ref UnsignedValueFit used to avoid having to explicitly accessing the `type` member.
template < uint64_t V > using UnsignedValueFit_t = typename UnsignedValueFit< V >::type;
/// Helper of \ref IsReferenceCounted used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsReferenceCounted_v = IsReferenceCounted< T >::value;

/*
 * Implementation helpers.
*/
namespace Impl {

// Helper used to check if a signed integral value can fit in a 32-bit integral type.
template < int64_t V, bool B = (V > INT32_MAX || V < INT32_MIN) > struct Helper_SignedValueFit32 { typedef int32_t type; };
// Specialization of the %Helper_SignedValueFit32 type for the case where the value needs a type larger than 32-bit.
template < int64_t V > struct Helper_SignedValueFit32< V, true > { typedef int64_t type; };

// Helper used to check if a signed integral value can fit in a 16-bit integral type.
template < int64_t V, bool B = (V > INT16_MAX || V < INT16_MIN) > struct Helper_SignedValueFit16 { typedef int16_t type; };
// Specialization of the %Helper_SignedValueFit16 type for the case where the value needs a type larger than 16-bit.
template < int64_t V > struct Helper_SignedValueFit16< V, true > : public Helper_SignedValueFit32< V > { };

// Helper used to check if a signed integral value can fit in an 8-bit integral type.
template < int64_t V, bool B = (V > INT8_MAX || V < INT8_MIN) > struct Helper_SignedValueFit8 { typedef int8_t type; };
// Specialization of the %Helper_SignedValueFit8 type for the case where the value needs a type larger than 16-bit.
template < int64_t V > struct Helper_SignedValueFit8< V, true > : public Helper_SignedValueFit16< V > { };

// Helper used to check if an unsigned integral value can fit in a 32-bit integral type.
template < uint64_t V, bool B = (V > UINT32_MAX) > struct Helper_UnsignedValueFit32 { typedef uint32_t type; };
// Specialization of the %Helper_UnsignedValueFit32 type for the case where the value needs a type larger than 32-bit.
template < uint64_t V > struct Helper_UnsignedValueFit32< V, true > { typedef uint64_t type; };

// Helper used to check if an unsigned integral value can fit in a 16-bit integral type.
template < uint64_t V, bool B = (V > UINT16_MAX) > struct Helper_UnsignedValueFit16 { typedef uint16_t type; };
// Specialization of the %Helper_UnsignedValueFit16 type for the case where the value needs a type larger than 16-bit.
template < uint64_t V > struct Helper_UnsignedValueFit16< V, true > : public Helper_UnsignedValueFit32< V > { };

// Helper used to check if an unsigned integral value can fit in an 8-bit integral type.
template < uint64_t V, bool B = (V > UINT8_MAX) > struct Helper_UnsignedValueFit8 { typedef uint8_t type; };
// Specialization of the %Helper_UnsignedValueFit8 type for the case where the value needs a type larger than 16-bit.
template < uint64_t V > struct Helper_UnsignedValueFit8< V, true > : public Helper_UnsignedValueFit16< V > { };

} // Namespace:: Impl

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

// // Resolves to the more efficient of `const T` or `const T&`, in the context of returning a const-qualified value of type T. \remark Not a standard feature.
// template < class T > using ValueOrReferenceReturn_t = Conditional_t< sizeof(T) < (2 * UH_POINTER_BYTES) && IsTriviallyConstructible_v< T >, const T, const T & >;

// template < class T, class = void > struct IsComparableToNullptr : FalseType { };

// template < class T > struct IsComparableToNullPtr<
//     T, EnableIf_t<
//         IsConvertible_v< decltype(declval< T >() != nullptr), bool>
//     >
// > : TrueType { };

} // Namespace:: Urho3D
