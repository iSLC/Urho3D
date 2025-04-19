//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include <utility>
#include <type_traits>

namespace Urho3D {

/// Forms lvalue reference to const type of \p t.
template < class T > [[nodiscard]] constexpr std::add_const_t< T > & AsConst(T & t) { return t; }
/// Const r-value reference overload is deleted to disallow r-value arguments.
template < class T > void AsConst(const T &&) = delete;

} // Namespace:: Urho3D
