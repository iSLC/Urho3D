//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"

namespace Urho3D {
namespace Impl
{

/// Helper type used as storage by \ref TupleBase.
template < size_t I, class T > struct TupleStorage
{
    /// Alias of template parameter \p T.
    using ValueType = T;

    /// Whatever was given via template parameter \p I.
    static inline constexpr size_t Index = I;

    /// Default constructor.
    constexpr TupleStorage() = default;

    /// Construct with a copy of the specified value.
    template < class U = T, EnableIf_t< IsConstructible_v< T, const U & >, bool > = true >
    constexpr explicit TupleStorage(const U & v)
        : value_(v)
    {
    }
    /// Construct by taking ownership of the specified value.
    template < class U = T, EnableIf_t< IsConstructible_v< T, U && >, bool > = true >
    constexpr explicit TupleStorage(U && v) noexcept // NOLINT(bugprone-forwarding-reference-overload)
        : value_(Move(v))
    {
    }

    /// Copy constructor.
    constexpr TupleStorage(const TupleStorage & o) = default;
    /// Move constructor.
    constexpr TupleStorage(TupleStorage && o) = default;

    /// Copy assignment operator.
    constexpr TupleStorage & operator = (Conditional_t< IsCopyAssignable_v< T >, const TupleStorage &, const NoneSuch & > o)
    {
        value_ = o.value_;
        return *this;
    }

    /// Move assignment operator.
    constexpr TupleStorage & operator = (Conditional_t< IsMoveAssignable_v< T >, TupleStorage &&, NoneSuch && > o)
    {
        value_ = Forward< T >(o.value_);
        return *this;
    }

    /// Equality comparison operator.
    [[nodiscard]] constexpr bool operator == (const T & v) const { return (value_ == v); }
    /// Inequality comparison operator.
    [[nodiscard]] constexpr bool operator != (const T & v) const { return (value_ != v); }

    /// Retrieve a reference to the stored type.
    [[nodiscard]] constexpr T & Get() { return value_; }
    /// Retrieve a constant reference to the stored type.
    [[nodiscard]] constexpr const T & Get() const { return value_; }

protected:

    /// Stored value.
    ValueType value_{};
};

/// Primary template of \ref TupleBase which acts as a break when types are left in the parameter pack.
template < size_t I, class... Ts > struct TupleBase
{
    /// Default constructor.
    constexpr TupleBase() = default;

    /// Copy constructor.
    constexpr TupleBase(const TupleBase & o) = default;
    /// Move constructor.
    constexpr TupleBase(TupleBase && o) noexcept = default;

    /// Copy assignment operator.
    constexpr TupleBase & operator = (const TupleBase & o) = default;
    /// Move assignment operator.
    constexpr TupleBase & operator = (TupleBase && o) = default;

    /// Invoke the specified callback with the current parameter pack.
    template < class F, class... Args > constexpr auto Apply(F && fn, Args &&... args)
    {
        return fn(Forward< Args >(args)...);
    }
};

/// Partial specialization of \ref TupleBase which unpacks the specified parameter pack and provides storage for each element.
template < size_t I, class T, class... Ts > struct TupleBase< I, T, Ts... >
    : public TupleStorage< I, T >
    , public TupleBase< I + 1, Ts... >
{
    /// Base type used as storage for current type.
    using Storage = TupleStorage< I, T >;\
    /// Base type implementing the next type in the parameter pack.
    using Next = TupleBase< I + 1, Ts... >;

    /// Construct with a copy of the current value.
    template < class U, class... Args, EnableIf_t<
        IsConstructible_v< T, const U & >
    , bool > = true > constexpr TupleBase(const U & arg, Args &&... args) // NOLINT(google-explicit-constructor)
        : Storage(arg), Next(Forward< Args >(args)...)
    {
    }

    /// Construct by taking ownership of the current value.
    template < class U, class... Args, EnableIf_t< IsConstructible_v< T, U && >, bool > = true >
    constexpr TupleBase(U && arg, Args &&... args) // NOLINT(google-explicit-constructor)
        : Storage(Move(arg)), Next(Forward< Args >(args)...)
    {
    }

    /// Copy constructor.
    constexpr TupleBase(const TupleBase & o) = default;
    /// Move constructor.
    constexpr TupleBase(TupleBase && o) noexcept = default;

    /// Copy assignment operator.
    constexpr TupleBase & operator = (const TupleBase & o) = default;
    /// Move assignment operator.
    constexpr TupleBase & operator = (TupleBase && o) = default;

    /// Invoke the specified callback with the current parameter pack.
    template < class F, class... Args > constexpr auto Apply(F && fn, Args &&... args)
    {
        return Next::Apply(Forward< F >(fn), Forward< Args >(args)..., Storage::Get());
    }
};

} // Namespace:: Impl

/// Fixed-size collection of heterogeneous values. It is a generalization of \ref Pair. \remark See `std::tuple`
/// This is a minimal, oversimplified implementation of `std::tuple` with limited functionality.
template < class T, class... Ts > struct Tuple : public Impl::TupleBase< 0, T, Ts... >
{
    /// Base type which provides the actual implementation.
    using Base = Impl::TupleBase< 0, T, Ts... >;

    /// Base constructor.
    template < class... Args > constexpr Tuple(Args &&... args) // NOLINT(google-explicit-constructor)
        : Base(Forward<Args>(args)...)
    {
    }

    /// Copy constructor.
    constexpr Tuple(const Tuple & o) = default;
    /// Move constructor.
    constexpr Tuple(Tuple && o) noexcept = default;

    /// Copy assignment operator.
    constexpr Tuple & operator = (const Tuple & o) = default;
    /// Move assignment operator.
    constexpr Tuple & operator = (Tuple && o) = default;
};

/// Template deduction guides.
template < class... Ts > Tuple(Ts... args) -> Tuple< Ts... >;

/// Retrieve the value at index \p I from tuple \p t.
template < size_t I, class... Ts > [[nodiscard]] constexpr auto & Get(Tuple< Ts... > & t)
{
    // Cast the tuple to the base class that corresponds to the index and type for the given index
    return static_cast< Impl::TupleStorage< I, ExtractTypeAt_t< I, Ts... > > & >(t).Get();
}
/// Retrieve the value at index \p I from tuple \p t.
template < size_t I, class... Ts > [[nodiscard]] constexpr const auto & Get(const Tuple< Ts... > & t)
{
    // Cast the tuple to the base class that corresponds to the index and type for the given index
    return static_cast< Impl::TupleStorage< I, ExtractTypeAt_t< I, Ts... > > & >(t).Get();
}

/// Compare two tuples for equality.
template < size_t I, class... Ts > constexpr bool CompareTuple(const Tuple< Ts... > & a, Tuple< Ts... > & b)
{
    if constexpr (I == 0)
    {
        return Get< 0 >(a) == Get< 0 >(b);
    }
    else
    {
        return Get< I >(a) == Get< I >(b) && CompareTuple< I - 1 >(a, b);
    }
}

/// Equality comparison operator.
template < class... Ts > constexpr bool operator == (const Tuple< Ts... > & a, const Tuple< Ts... > & b)
{
    return CompareTuple< sizeof...(Ts) - 1 >(a, b);
}
/// Inequality comparison operator.
template < class... Ts > constexpr bool operator != (const Tuple< Ts... > & a, const Tuple< Ts... > & b)
{
    return !CompareTuple< sizeof...(Ts) - 1 >(a, b);
}

} // Namespace:: Urho3D
