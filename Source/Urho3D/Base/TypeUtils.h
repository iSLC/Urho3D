//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include <type_traits>

namespace Urho3D {

template < class, class = void > struct HasSizeAndData;

/// Helper of \ref HasSizeAndData used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasSizeAndData_v = HasSizeAndData< T >::value;

/// Check whether `T` is a type that has `Size()` and `Data()` member methods. \remark Not a standard feature!
template < class, class > struct HasSizeAndData : std::false_type { };
/// Partial specialization for the case when `T` is actually has `Size()` and `Data()` member methods.
template < class T > struct HasSizeAndData< T, std::void_t< decltype(Size(declval< T >())), decltype(Data(declval< T >())) > > : std::true_type { };

} // Namespace:: Urho3D
