//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Urho3DConfig.h"

namespace Urho3D {

/// Fixed type function object for performing negation.
///    Unless specialized, invokes `operator-` on type `T`.
template < class T = void > struct Negate
{
    /// Calls `operator-` on the given instance and returns the result.
    UH_INLINE constexpr T operator () (const T & v) const noexcept
    {
        return -v;
    }
};
/// Dynamic type function object for performing negation.
/// Unless specialized, invokes `operator-` on type `T`.
template < > struct Negate< void >
{
    /// Calls `operator-` on the given instance and returns the result.
    template < class T > UH_INLINE constexpr auto operator () (const T & v) const noexcept
        -> decltype(-v)
    {
        return -v;
    }
};

/// Fixed type function object for performing bitwise NOT.
/// Unless specialized, invokes `operator~` on type `T`.
template < class T = void > struct BitNOT
{
    /// Calls `operator~` on the given instance and returns the result.
    UH_INLINE constexpr T operator () (const T & v) const noexcept
    {
        return ~v;
    }
};
/// Dynamic type function object for performing bitwise NOT.
/// Unless specialized, invokes `operator~` on type `T`.
template < > struct BitNOT< void >
{
    /// Calls `operator~` on the given instance and returns the result.
    template < class T > UH_INLINE constexpr auto operator () (const T & v) const noexcept
        -> decltype(~v)
    {
        return ~v;
    }
};

/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator==` on types `T` and `U`.
template < class T = void, class U = void > struct Equal
{
    /// Calls `operator==` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a == b)
    {
        return a == b;
    }
};
/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator!=` on types `T` and `U`.
template < class T = void, class U = void > struct NotEqual
{
    /// Calls `operator!=` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a != b)
    {
        return a != b;
    }
};
/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator<` on types `T` and `U`.
template < class T = void, class U = void > struct Less
{
    /// Calls `operator<` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a < b)
    {
        return a < b;
    }
};
/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator>` on types `T` and `U`.
template < class T = void, class U = void > struct Greater
{
    /// Calls `operator>` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a > b)
    {
        return a > b;
    }
};
/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator<=` on types `T` and `U`.
template < class T = void, class U = void > struct LessEqual
{
    /// Calls `operator<=` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a <= b)
    {
        return a <= b;
    }
};
/// Fixed type function object for performing comparisons.
/// Unless specialized, invokes `operator>=` on types `T` and `U`.
template < class T = void, class U = void > struct GreaterEqual
{
    /// Calls `operator>=` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a >= b)
    {
        return a >= b;
    }
};

/// Fixed type function object for performing bitwise AND.
/// Unless specialized, invokes `operator&` on types `T` and `U`.
template < class T = void, class U = void > struct BitAND
{
    /// Calls `operator&` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a & b)
    {
        return a & b;
    }
};
/// Fixed type function object for performing bitwise OR.
/// Unless specialized, invokes `operator|` on types `T` and `U`.
template < class T = void, class U = void > struct BitOR
{
    /// Calls `operator|` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a | b)
    {
        return a | b;
    }
};
/// Fixed type function object for performing bitwise XOR.
/// Unless specialized, invokes `operator^` on types `T` and `U`.
template < class T = void, class U = void > struct BitXOR
{
    /// Calls `operator^` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a ^ b)
    {
        return a ^ b;
    }
};

/// Fixed type function object for performing addition.
/// Unless specialized, invokes `operator+` on types `T` and `U`.
template < class T = void, class U = void > struct Addition
{
    /// Calls `operator+` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a + b)
    {
        return a + b;
    }
};
/// Fixed type function object for performing subtraction.
/// Unless specialized, invokes `operator-` on types `T` and `U`.
template < class T = void, class U = void > struct Subtraction
{
    /// Calls `operator-` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a - b)
    {
        return a - b;
    }
};
/// Fixed type function object for performing multiplication.
/// Unless specialized, invokes `operator*` on types `T` and `U`.
template < class T = void, class U = void > struct Multiplication
{
    /// Calls `operator*` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a * b)
    {
        return a * b;
    }
};
/// Fixed type function object for performing division.
/// Unless specialized, invokes `operator/` on types `T` and `U`.
template < class T = void, class U = void > struct Division
{
    /// Calls `operator/` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a / b)
    {
        return a / b;
    }
};
/// Fixed type function object for computing remainders of divisions.
/// Unless specialized, invokes `operator%` on types `T` and `U`.
template < class T = void, class U = void > struct Modulus
{
    /// Calls `operator%` on the given instances and returns the result.
    UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a % b)
    {
        return a % b;
    }
};

/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator==` on type `T` and any other given type.
template < class T > struct Equal< T, void >
{
    /// Calls `operator==` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a == b)
    {
        return a == b;
    }
};
/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator!=` on type `T` and any other given type.
template < class T > struct NotEqual< T, void >
{
    /// Calls `operator==` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a != b)
    {
        return a != b;
    }
};
/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator<` on type `T` and any other given type.
template < class T > struct Less< T, void >
{
    /// Calls `operator<` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a < b)
    {
        return a < b;
    }
};
/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator>` on type `T` and any other given type.
template < class T > struct Greater< T, void >
{
    /// Calls `operator>` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a > b)
    {
        return a > b;
    }
};
/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator<=` on type `T` and any other given type.
template < class T > struct LessEqual< T, void >
{
    /// Calls `operator<=` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a <= b)
    {
        return a <= b;
    }
};
/// Partially (right) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator>=` on type `T` and any other given type.
template < class T > struct GreaterEqual< T, void >
{
    /// Calls `operator>=` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a >= b)
    {
        return a >= b;
    }
};

/// Partially (right) type agnostic function object for performing bitwise AND.
/// Unless specialized, invokes `operator&` on type `T` and any other given type.
template < class T > struct BitAND< T, void >
{
    /// Calls `operator&` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a & b)
    {
        return a & b;
    }
};
/// Partially (right) type agnostic function object for performing bitwise OR.
/// Unless specialized, invokes `operator|` on type `T` and any other given type.
template < class T > struct BitOR< T, void >
{
    /// Calls `operator|` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a | b)
    {
        return a | b;
    }
};
/// Partially (right) type agnostic function object for performing bitwise XOR.
/// Unless specialized, invokes `operator^` on type `T` and any other given type.
template < class T > struct BitXOR< T, void >
{
    /// Calls `operator^` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a ^ b)
    {
        return a ^ b;
    }
};

/// Partially (right) type agnostic function object for performing addition.
/// Unless specialized, invokes `operator+` on type `T` and any other given type.
template < class T > struct Addition< T, void >
{
    /// Calls `operator+` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a + b)
    {
        return a + b;
    }
};
/// Partially (right) type agnostic function object for performing subtraction.
/// Unless specialized, invokes `operator-` on type `T` and any other given type.
template < class T > struct Subtraction< T, void >
{
    /// Calls `operator-` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a - b)
    {
        return a - b;
    }
};
/// Partially (right) type agnostic function object for performing multiplication.
/// Unless specialized, invokes `operator*` on type `T` and any other given type.
template < class T > struct Multiplication< T, void >
{
    /// Calls `operator*` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a * b)
    {
        return a * b;
    }
};
/// Partially (right) type agnostic function object for performing division.
/// Unless specialized, invokes `operator/` on type `T` and any other given type.
template < class T > struct Division< T, void >
{
    /// Calls `operator/` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a / b)
    {
        return a / b;
    }
};
/// Partially (right) type agnostic function object for computing remainders of divisions.
/// Unless specialized, invokes `operator%` on type `T` and any other given type.
template < class T > struct Modulus< T, void >
{
    /// Calls `operator%` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a % b)
    {
        return a % b;
    }
};

/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator==` on any given type and `T`.
template < class T > struct Equal< void, T >
{
    /// Calls `operator==` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a == b)
    {
        return a == b;
    }
};
/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator!=` on any given type and `T`.
template < class T > struct NotEqual< void, T >
{
    /// Calls `operator!=` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a != b)
    {
        return a != b;
    }
};
/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator<` on any given type and `T`.
template < class T > struct Less< void, T >
{
    /// Calls `operator<` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a < b)
    {
        return a < b;
    }
};
/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator>` on any given type and `T`.
template < class T > struct Greater< void, T >
{
    /// Calls `operator>` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a > b)
    {
        return a > b;
    }
};
/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator<=` on any given type and `T`.
template < class T > struct LessEqual< void, T >
{
    /// Calls `operator<=` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a <= b)
    {
        return a <= b;
    }
};
/// Partially (left) type agnostic function object for performing comparisons.
/// Unless specialized, invokes `operator>=` on any given type and `T`.
template < class T > struct GreaterEqual< void, T >
{
    /// Calls `operator>=` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a >= b)
    {
        return a >= b;
    }
};

/// Partially (left) type agnostic function object for performing bitwise AND.
/// Unless specialized, invokes `operator&` on any given type and `T`.
template < class T > struct BitAND< void, T >
{
    /// Calls `operator&` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a & b)
    {
        return a & b;
    }
};
/// Partially (left) type agnostic function object for performing bitwise OR.
/// Unless specialized, invokes `operator|` on any given type and `T`.
template < class T > struct BitOR< void, T >
{
    /// Calls `operator|` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a | b)
    {
        return a | b;
    }
};
/// Partially (left) type agnostic function object for performing bitwise XOR.
/// Unless specialized, invokes `operator^` on any given type and `T`.
template < class T > struct BitXOR< void, T >
{
    /// Calls `operator^` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a ^ b)
    {
        return a ^ b;
    }
};

/// Partially (left) type agnostic function object for performing addition.
/// Unless specialized, invokes `operator+` on any given type and `T`.
template < class T > struct Addition< void, T >
{
    /// Calls `operator+` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a + b)
    {
        return a + b;
    }
};
/// Partially (left) type agnostic function object for performing subtraction.
/// Unless specialized, invokes `operator-` on any given type and `T`.
template < class T > struct Subtraction< void, T >
{
    /// Calls `operator-` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a - b)
    {
        return a - b;
    }
};
/// Partially (left) type agnostic function object for performing multiplication.
/// Unless specialized, invokes `operator*` on any given type and `T`.
template < class T > struct Multiplication< void, T >
{
    /// Calls `operator*` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a * b)
    {
        return a * b;
    }
};
/// Partially (left) type agnostic function object for performing division.
/// Unless specialized, invokes `operator/` on any given type and `T`.
template < class T > struct Division< void, T >
{
    /// Calls `operator/` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a / b)
    {
        return a / b;
    }
};
/// Partially (left) type agnostic function object for computing remainders of divisions.
/// Unless specialized, invokes `operator%` on any given type and `T`.
template < class T > struct Modulus< void, T >
{
    /// Calls `operator%` on the given instances and returns the result.
    template < class U > UH_INLINE constexpr auto operator () (const U & a, const T & b) const noexcept
        -> decltype(a % b)
    {
        return a % b;
    }
};


/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator==` on any given type combination.
template < > struct Equal< void, void >
{
    /// Calls `operator==` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a == b)
    {
        return a == b;
    }
};
/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator!=` on any given type combination.
template < > struct NotEqual< void, void >
{
    /// Calls `operator!=` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a != b)
    {
        return a != b;
    }
};
/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator<` on any given type combination.
template < > struct Less< void, void >
{
    /// Calls `operator<` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a < b)
    {
        return a < b;
    }
};
/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator>` on any given type combination.
template < > struct Greater< void, void >
{
    /// Calls `operator>` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a > b)
    {
        return a > b;
    }
};
/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator<=` on any given type combination.
template < > struct LessEqual< void, void >
{
    /// Calls `operator<=` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a <= b)
    {
        return a <= b;
    }
};
/// Fully type agnostic function object for performing comparisons.
/// Specialization that invokes `operator>=` on any given type combination.
template < > struct GreaterEqual< void, void >
{
    /// Calls `operator>=` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a >= b)
    {
        return a >= b;
    }
};

/// Fully type agnostic function object for performing bitwise AND.
/// Specialization that invokes `operator&` on any given type combination.
template < > struct BitAND< void, void >
{
    /// Calls `operator&` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a & b)
    {
        return a & b;
    }
};
/// Fully type agnostic function object for performing bitwise OR.
/// Specialization that invokes `operator|` on any given type combination.
template < > struct BitOR< void, void >
{
    /// Calls `operator|` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a | b)
    {
        return a | b;
    }
};
/// Fully type agnostic function object for performing bitwise XOR.
/// Specialization that invokes `operator^` on any given type combination.
template < > struct BitXOR< void, void >
{
    /// Calls `operator^` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a ^ b)
    {
        return a ^ b;
    }
};

/// Fully type agnostic function object for performing addition.
/// Specialization that invokes `operator+` on any given type combination.
template < > struct Addition< void, void >
{
    /// Calls `operator+` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a + b)
    {
        return a + b;
    }
};
/// Fully type agnostic function object for performing subtraction.
/// Specialization that invokes `operator-` on any given type combination.
template < > struct Subtraction< void, void >
{
    /// Calls `operator-` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a - b)
    {
        return a - b;
    }
};
/// Fully type agnostic function object for performing multiplication.
/// Specialization that invokes `operator*` on any given type combination.
template < > struct Multiplication< void, void >
{
    /// Calls `operator*` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a * b)
    {
        return a * b;
    }
};
/// Fully type agnostic function object for performing division.
/// Specialization that invokes `operator/` on any given type combination.
template < > struct Division< void, void >
{
    /// Calls `operator/` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a / b)
    {
        return a / b;
    }
};
/// Fully type agnostic function object for computing remainders of divisions.
/// Specialization that invokes `operator%` on any given type combination.
template < > struct Modulus< void, void >
{
    /// Calls `operator%` on the given instances and returns the result.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a % b)
    {
        return a % b;
    }
};

/// Logical AND functor for values of the same type.
template < class T = void > struct LogicalAND
{
    /// Function call operator that performs the actual logic.
    UH_INLINE constexpr auto operator () (const T & a, const T & b) const noexcept
        -> decltype(a && b)
    {
        return a && b;
    }
};
/// Logical OR functor for values of the same type.
template < class T = void > struct LogicalOR
{
    /// Function call operator that performs the actual logic.
    UH_INLINE constexpr auto operator () (const T & a, const T & b) const noexcept
        -> decltype(a || b)
    {
        return a || b;
    }
};
/// Logical NOT functor for values of the same type.
template < class T = void > struct LogicalNOT
{
    /// Function call operator that performs the actual logic.
    UH_INLINE constexpr auto operator () (const T & v) const noexcept
        -> decltype(!v)
    {
        return !v;
    }
};

/// Logical AND functor for values of different types.
template < > struct LogicalAND< void >
{
    /// Function call operator that performs the actual logic.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a && b)
    {
        return a && b;
    }
};
/// Logical OR functor for values of different types.
template < > struct LogicalOR< void >
{
    /// Function call operator that performs the actual logic.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & a, const U & b) const noexcept
        -> decltype(a || b)
    {
        return a || b;
    }
};
/// Logical NOT functor for values of different types.
template < > struct LogicalNOT< void >
{
    /// Function call operator that performs the actual logic.
    template < class T, class U > UH_INLINE constexpr auto operator () (const T & v) const noexcept
        -> decltype(!v)
    {
        return !v;
    }
};

} // Namespace:: Urho3D
