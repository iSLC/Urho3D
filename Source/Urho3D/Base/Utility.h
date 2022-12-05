//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/TypeTraits.h"

namespace Urho3D {

/// Returns the actual address of the object or function referenced by \p r, even in the presence of an overloaded operator &. \remark See std::addressof
template < class T > [[nodiscard]]  constexpr T * AddressOf(T & r) noexcept
{
    return __builtin_addressof(r); // MSVC supports this, apparently
}
/// R-value overload is deleted to prevent taking the address of const r-values.
template< class T > [[nodiscard]] const T * AddressOf(const T &&) = delete;

/// Forward an l-value. This function is used to implement "perfect forwarding". \remark See std::forward
template < class T > [[nodiscard]] constexpr T && Forward(RemoveReference_t< T > & t) noexcept
{
    return static_cast< T && >(t);
}

// Forward an r-value. This function is used to implement "perfect forwarding". \remark See std::forward
template < class T > [[nodiscard]] constexpr T && Forward(RemoveReference_t< T > && t) noexcept
{
    static_assert(IsLvalueReference_v< T >, "Template argument substituting T is an l-value reference type");
    return static_cast< T && >(t);
}

/// Convert a value to an r-value. It is exactly equivalent to a static_cast to an r-value reference type. \remark See std::move
template < class T > [[nodiscard]] constexpr RemoveReference_t< T > && Move(T && t) noexcept
{
    return static_cast< RemoveReference_t< T > && >(t);
}

/// Conditionally convert a value to an r-value. \remark See std::move_if_noexcept
/// Same as \ref Move unless the type's move constructor could throw and the type is copyable, in which case an l-value-reference is returned instead.
template < class T>
[[nodiscard]] constexpr Conditional_t< NeedsUnsafeMoveConstructorAlternative< T >::value, const T &, T && > MoveIfNoexcept(T & t) noexcept
{
    return Move(t);
}

/// Replaces the value of \p o with \p v and returns the old value of \p o. \remark See std::exchange
template < class T, class U = T > [[nodiscard]] constexpr T Exchange(T & o, U && v)
{
    T old = Move(o);
    o = Forward< U >(v);
    return old;
}

/// Swaps two values. \remark See std::swap
template < class T > constexpr EnableIf_t<
    IsMoveConstructible_v< T > && IsMoveAssignable_v< T >
> Swap(T & a, T & b) noexcept(Conjunction_v< IsNoThrowMoveConstructible< T >, IsNoThrowMoveAssignable< T > >)
{
    T t = Move(a);
    a = Move(b);
    b = Move(t);
}

/// Swap the contents of two arrays. \remark See std::swap
template < class T, size_t N > constexpr EnableIf_t<
    IsSwappable_v< T >
> Swap(T (&a)[N], T (&b)[N]) noexcept(IsNoThrowSwappable_v< T >)
{
    for (size_t n = 0; n < N; ++n)
    {
        Swap(a[n], b[n]);
    }
}

/// Forms lvalue reference to const type of \p t.
template < class T > [[nodiscard]] constexpr AddConst_t< T > & AsConst(T & t) { return t; }
/// Const r-value reference overload is deleted to disallow r-value arguments.
template < class T > void AsConst(const T &&) = delete;

} // Namespace:: Urho3D
