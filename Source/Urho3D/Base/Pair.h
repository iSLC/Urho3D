//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"
#include "../Base/Concepts.h"
#include "../Container/Hash.h"

namespace Urho3D {

/// Structure holding two objects of arbitrary type.
template < class T1, class T2 > struct Pair
{
    typedef T1 FirstType;    ///< The first bound type.
    typedef T2 SecondType;   ///< The second bound type.

    T1 first_{}; ///< A copy of the first object.
    T2 second_{}; ///< A copy of the second object.

    /// Default constructor.
    constexpr Pair() = default;

    template < class U1 = T1, class U2 = T2 >
    requires std::constructible_from< T1, const U1 & > && std::constructible_from< T2, const U2 & >
    constexpr Pair(const U1 & first, const U2 & second)
        : first_(first), second_(second)
    {
    }

    /// Value constructor.
    template < class U1 = T1, class U2 = T2 >
    requires std::constructible_from< T1, U1 && > && std::constructible_from< T2, const U2 & >
    constexpr Pair(U1 && first, const U2 & second)
        : first_(std::forward< U1 >(first)), second_(second)
    {
    }

    /// Value constructor.
    template < class U1 = T1, class U2 = T2 >
    requires std::constructible_from< T1, const U1 & > && std::constructible_from< T2, U2 && >
    constexpr Pair(const U1 & first, U2 && second)
        : first_(first), second_(std::forward< U2 >(second))
    {
    }

    /// Value constructor.
    template < class U1 = T1, class U2 = T2 >
    requires std::constructible_from< T1, U1 && > && std::constructible_from< T2, U2 && >
    constexpr Pair(U1 && first, U2 && second)
        : first_(std::forward< U1 >(first)), second_(std::forward< U2 >(second))
    {
    }

    /// Copy constructor.
    constexpr Pair(const Pair & o) = default;
    /// Move constructor.
    constexpr Pair(Pair && o) = default;

    /// Copy constructor.
    template < class U1, class U2 >
    requires std::constructible_from< T1, const U1 & > && std::constructible_from< T2, const U2 & >
    constexpr Pair(const Pair< U1, U2 > & o)
        : first_(o.first_), second_(o.second_)
    {
    }

    /// Move constructor.
    template < class U1, class U2 >
    requires std::constructible_from< T1, U1 && > && std::constructible_from< T2, U2 && >
    explicit constexpr Pair(Pair< U1, U2 > && o)
        : first_(std::forward< U1 >(o.first_)), second_(std::forward< U2 >(o.second_))
    {
    }

    /// Copy assignment operator.
    Pair & operator = (const Pair & o) requires std::assignable_from< T1, const T1 & > && std::assignable_from< T2, const T2 & >
    {
        first_ = o.first_;
        second_ = o.second_;
        return *this;
    }

    /// Move assignment operator.
    Pair & operator = (Pair && o) requires std::assignable_from< T1, T1 && > && std::assignable_from< T2, T2 && >
    {
        first_ = std::forward< FirstType >(o.first_);
        second_ = std::forward< SecondType >(o.second_);
        return *this;
    }

    /// Copy assignment operator.
    template < class U1, class U2 >
    requires std::assignable_from< T1 &, const U1 & > && std::assignable_from< T2 &, const U2 & >
    Pair & operator = (const Pair< U1, U2 > & o)
    {
        first_ = o.first_;
        second_ = o.second_;
        return *this;
    }

    /// Move assignment operator.
    template < class U1, class U2 >
    requires std::assignable_from< T1 &, U1 && > && std::assignable_from< T2 &, U2 && >
    Pair & operator = (Pair< U1, U2 > && o)
    {
        first_ = std::forward< U1 >(o.first_);
        second_ = std::forward< U2 >(o.second_);
        return *this;
    }

    /// Swap contents.
    void Swap(Pair & o)
    {
        std::swap(first_, o.first_);
        std::swap(second_, o.second_);
    }

    /// Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (MakeHash(first_) & 0xffff) | (MakeHash(second_) << 16); }
};

/// Pair deduction guide.
template < class T1, class T2 > Pair(T1, T2) -> Pair< T1, T2 >;

/// Equality comparison operator.
template < class T1, class T2 > constexpr bool operator == (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return (x.first_ == y.first_) && (x.second_ == y.second_);
}

/// Inequality comparison operator.
template < class T1, class T2 > constexpr bool operator != (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return !(x == y);
}

/// Less than comparison operator.
template < class T1, class T2 > constexpr bool operator < (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return (x.first_ < y.first_) || (!(y.first_ < x.first_) && x.second_ < y.second_);
}

/// Greater than comparison operator.
template < class T1, class T2 > constexpr bool operator > (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return y < x;
}

/// Less than or equal comparison operator.
template < class T1, class T2 > constexpr bool operator <= (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return !(y < x);
}

/// Greater than or equal comparison operator.
template < class T1, class T2 > constexpr bool operator >= (const Pair< T1, T2 > & x, const Pair< T1, T2 > & y)
{
    return !(x < y);
}

/// Swap overload for pairs. Swaps the contents of \p x and \p y. Equivalent to x.Swap(y). See \ref Pair::Swap().
template < class T1, class T2 > constexpr std::enable_if_t<
    std::is_swappable_v< T1 > && std::is_swappable_v< T2 >
> Swap(Pair< T1, T2 > & x, Pair< T1, T2 > & y) { x.Swap(y); }

/// Swap overload for pairs. Deleted since one or both elements are not do not support swap operation.
template < class T1, class T2 > std::enable_if_t<
    !std::is_swappable_v< T1 > || !std::is_swappable_v< T2 >
> Swap(Pair< T1, T2 > & x, Pair< T1, T2 > & y) = delete;

/// Creates a \ref Pair object of type, defined by the argument type.
template < class T1, class T2 >
constexpr Pair< std::unwrap_ref_decay_t< T1 >, std::unwrap_ref_decay_t< T2 > > MakePair(T1 && x, T2 && y)
{
    return Pair< std::unwrap_ref_decay_t< T1 >, std::unwrap_ref_decay_t< T2 > >(std::forward< T1 >(x), std::forward< T2 >(y));
}

/// Specialization of \ref begin() for \ref Pair. Returns `first_` member.
template < class T > T begin(Pair< T, T > & p) { return p.first_; }
/// Specialization of \ref end() for \ref Pair. Returns `second_` member.
template < class T > T end(Pair< T, T > & p) { return p.second_; }

/// Specialization of \ref begin() for const \ref Pair. Returns `first_` member.
template < class T > T begin(const Pair< T, T > & p) { return p.first_; }
/// Specialization of \ref end() for const \ref Pair. Returns `second_` member.
template < class T > T end(const Pair< T, T > & p) { return p.second_; }

} // Namespace:: Urho3D
