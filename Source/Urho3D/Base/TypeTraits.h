//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Limits.h"

/*
 * Forward declarations.
*/
namespace Urho3D {

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

} // Namespace:: Urho3D
