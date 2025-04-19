//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"
#include "../Base/Functors.h"

namespace Urho3D {

/// A container that encapsulates fixed size arrays.
template < class T, size_t N > struct Array
{
    /// Whatever was given via template parameter `T`.
    using ValueType = T;
    /// Same as `ValueType` but with a reference qualifier.
    using Reference = T &;
    /// Same as `Reference` but with a const qualifier.
    using ConstReference = const T &;
    /// Same as `ValueType` but with a pointer qualifier.
    using Pointer = T *;
    /// Same as `Pointer` but with a const qualifier.
    using ConstPointer = const T *;
    /// .
    using SizeType = size_t;

    /// Array iterator.
    using Iterator = T *;
    /// Constant array iterator.
    using ConstIterator = const T *;

    /// The data member is intentionally public to allow for aggregate initialization of the object.
    ValueType data_[N];

    /// Retrieve a reference to the element at specified location \p i.
    [[nodiscard]] constexpr Reference operator[] (SizeType i) noexcept { UH_ASSERT(i < N) return data_[i]; }
    /// Retrieve a constant reference to the element at specified location \p i.
    [[nodiscard]] constexpr ConstReference operator[] (SizeType i) const noexcept { UH_ASSERT(i < N) return data_[i]; }

    /// Retrieve a reference to the element at specified location \p i.
    [[nodiscard]] constexpr Reference At(SizeType i) noexcept { UH_ASSERT(i < N) return data_[i]; }
    /// Retrieve a constant reference to the element at specified location \p i.
    [[nodiscard]] constexpr ConstReference At(SizeType i) const noexcept { UH_ASSERT(i < N) return data_[i]; }

    /// Retrieve a reference to the first element in the container.
    [[nodiscard]] constexpr Reference Front() noexcept { return data_[0]; }
    /// Retrieve a constant reference to the first element in the container.
    [[nodiscard]] constexpr ConstReference Front() const noexcept { return data_[0]; }

    /// Retrieve a reference to the last element in the container.
    [[nodiscard]] constexpr Reference Back() noexcept { return data_[N - 1]; }
    /// Retrieve a constant reference to the last element in the container.
    [[nodiscard]] constexpr ConstReference Back() const noexcept { return data_[N - 1]; }

    /// Retrieve a pointer to the underlying array serving as element storage.
    [[nodiscard]] constexpr Pointer Data() noexcept { return data_; }
    /// Retrieve a constant pointer to the underlying array serving as element storage.
    [[nodiscard]] constexpr ConstPointer Data() const noexcept { return data_; }

    /// Retrieve an iterator to the first element of the container.
    [[nodiscard]] constexpr Iterator Begin() noexcept { return Iterator(data_); }
    /// Retrieve a constant iterator to the first element of the container.
    [[nodiscard]] constexpr ConstIterator Begin() const noexcept { return ConstIterator(data_); }
    /// Retrieve an iterator to the element following the last element of the container.
    [[nodiscard]] constexpr Iterator End() noexcept { return Iterator(data_ + N); }
    /// Retrieve a constant iterator to the element following the last element of the container.
    [[nodiscard]] constexpr ConstIterator End() const noexcept { return ConstIterator(data_ + N); }

    /// Retrieve an iterator to the first element of the container. Alias of \ref Begin().
    [[nodiscard]] constexpr Iterator begin() noexcept { return Begin(); }
    /// Retrieve a constant iterator to the first element of the container. Alias of \ref Begin().
    [[nodiscard]] constexpr ConstIterator begin() const noexcept { return Begin(); }
    /// Retrieve an iterator to the element following the last element of the container. Alias of \ref End().
    [[nodiscard]] constexpr Iterator end() noexcept { return End(); }
    /// Retrieve a constant iterator to the element following the last element of the container. Alias of \ref End().
    [[nodiscard]] constexpr ConstIterator end() const noexcept { return End(); }

    /// Retrieve the number of elements in the container.
    [[nodiscard]] constexpr SizeType Size() const noexcept { return N; }
    /// Retrieve the maximum number of elements the container is able to hold. For fixed arrays this is equivalent to `Size()`.
    [[nodiscard]] constexpr SizeType MaxSize() const noexcept { return N; }
    /// Check if the container has no elements, i.e. whether `N` is 0, such that `Begin() == End()`.
    [[nodiscard]] constexpr bool Empty() const noexcept { return false; }

    /// Assigns the given value value to all elements in the container.
    template < class U = T >
    requires std::assignable_from< T &, const U & >
    constexpr void Fill(const U & v) noexcept
    {
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        // Can we use a built-in MemCpy operation?
        if constexpr (IsBinaryCompatible_v< T, U > && sizeof(T) == 1)
        {
            // Can we do this at compile-time?
            if (__builtin_is_constant_evaluated())
            {
                for (size_t i = 0; i < N; ++i)
                {
                    data_[i] = v;
                }
            }
            else
            {
                MemSet(data_, v, N);
            }
        }
        else
        {
    #endif
            for (size_t i = 0; i < N; ++i)
            {
                data_[i] = v;
            }
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        }
    #endif
    }
    /// Exchanges the contents of the container with those of other.
    constexpr void Swap(Array & o) noexcept { std::swap(data_, o.data_); }
    /// Exchanges the contents of the container with those of a raw array of the same size.
    constexpr void Swap(T (&o)[N]) noexcept { std::swap(data_, o); }
};

/// Deduction guide.
template < class T, class... U > Array(T, U...) -> Array< std::enable_if_t< (std::is_same_v< T, U > && ...), T >, 1 + sizeof...(U) >;

namespace Impl {

/// Implementation of equality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool ArrayEqual(const T (&a)[N], const T (&b)[N]) noexcept
{
    // Can we get away with a bit-wise memory comparison?
    if constexpr (IsIntegralPrimitive_v< T >)
    {
        return MemCmp(a, b, N) == 0;
    }
    NotEqual< T, T > ne;
    // Fall back to the usual iteration with individual comparison
    for (size_t i = 0; i < N; ++i)
    {
        if (ne(a[i], b[i])) return false;
    }
    // All elements compared the same
    return true;
}

/// Implementation of less than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool ArrayLess(const T (&a)[N], const T (&b)[N]) noexcept
{
    // Can we get away with a bit-wise memory comparison?
    if constexpr (sizeof(T) == 1 && IsIntegral_v< T >)
    {
        return MemCmp(a, b, N) < 0;
    }
    Less< T, T > le;
    // Fall back to the usual iteration
    for (size_t i = 0; i < N; ++i)
    {
        if (!le(a[i], b[i])) return false;
    }
    // All elements compared less than
    return true;
}

} // Namespace:: Impl

/// Equality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator == (const Array< T, N > & a, const Array< T, N > & b) noexcept { return Impl::ArrayEqual(a.data_, b.data_); }
/// Inequality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator != (const Array< T, N > & a, const Array< T, N > & b) noexcept { return !Impl::ArrayEqual(a.data_, b.data_); }
/// Less than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator < (const Array< T, N > & a, const Array< T, N > & b) noexcept { return Impl::ArrayLess(a.data_, b.data_); }
/// Greater than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator > (const Array< T, N > & a, const Array< T, N > & b) noexcept { return Impl::ArrayLess(b.data_, a.data_); }
/// Less than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator <= (const Array< T, N > & a, const Array< T, N > & b) noexcept { return !Impl::ArrayLess(b.data_, a.data_); }
/// Greater than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator >= (const Array< T, N > & a, const Array< T, N > & b) noexcept { return !Impl::ArrayLess(a.data_, b.data_); }

/// Equality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator == (const Array< T, N > & a, const T (&b)[N]) noexcept { return Impl::ArrayEqual(a.data_, b); }
/// Inequality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator != (const Array< T, N > & a, const T (&b)[N]) noexcept { return !Impl::ArrayEqual(a.data_, b); }
/// Less than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator < (const Array< T, N > & a, const T (&b)[N]) noexcept { return Impl::ArrayLess(a.data_, b); }
/// Greater than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator > (const Array< T, N > & a, const T (&b)[N]) noexcept { return Impl::ArrayLess(b, a.data_); }
/// Less than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator <= (const Array< T, N > & a, const T (&b)[N]) noexcept { return !Impl::ArrayLess(b, a.data_); }
/// Greater than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator >= (const Array< T, N > & a, const T (&b)[N]) noexcept { return !Impl::ArrayLess(a.data_, b); }

/// Equality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator == (const T (&a)[N], const Array< T, N > & b) noexcept { return Impl::ArrayEqual(a, b.data_); }
/// Inequality comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator != (const T (&a)[N], const Array< T, N > & b) noexcept { return !Impl::ArrayEqual(a, b.data_); }
/// Less than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator < (const T (&a)[N], const Array< T, N > & b) noexcept { return Impl::ArrayLess(a, b.data_); }
/// Greater than comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator > (const T (&a)[N], const Array< T, N > & b) noexcept { return Impl::ArrayLess(b.data_, a); }
/// Less than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator <= (const T (&a)[N], const Array< T, N > & b) noexcept { return !Impl::ArrayLess(b.data_, a); }
/// Greater than or equal comparison between two arrays of the same type and size.
template < class T, size_t N > [[nodiscard]] constexpr bool operator >= (const T (&a)[N], const Array< T, N > & b) noexcept { return !Impl::ArrayLess(a, b.data_); }

/// A specialization of %Array for 0 length arrays. Zero length are allowed at the cost of undefined behavior.
/// This however is just a shell of an array where any attempts to access its data should yield a crash or assert.
template < class T > struct Array< T, 0 >
{
    ///< Whatever was given via template parameter `T`.
    using ValueType = T;
    ///< Same as `ValueType` but with a reference qualifier.
    using Reference = T &;
    ///< Same as `Reference` but with a const qualifier.
    using ConstReference = const T &;
    ///< Same as `ValueType` but with a pointer qualifier.
    using Pointer = T *;
    ///< Same as `Pointer` but with a const qualifier.
    using ConstPointer = const T *;
    ///< .
    using SizeType = size_t;

    ///< Array iterator.
    using Iterator = T *;
    ///< Constant array iterator.
    using ConstIterator = const T *;

    /// Retrieve a reference to the element at specified location \p i. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr Reference operator[] (SizeType i [[maybe_unused]]) noexcept { UH_ASSERT(0 /*i < 0*/) return *static_cast< Pointer >(nullptr); }
    /// Retrieve a constant reference to the element at specified location \p i. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr ConstReference operator[] (SizeType i [[maybe_unused]]) const noexcept { UH_ASSERT(0 /*i < 0*/) return *static_cast< ConstPointer >(nullptr); }

    /// Retrieve a reference to the element at specified location \p i. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr Reference At(SizeType i [[maybe_unused]]) noexcept { UH_ASSERT(0 /*i < 0*/) return *static_cast< Pointer >(nullptr); }
    /// Retrieve a constant reference to the element at specified location \p i. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr ConstReference At(SizeType i [[maybe_unused]]) const noexcept { UH_ASSERT(0 /*i < 0*/) return *static_cast< ConstPointer >(nullptr); }

    /// Retrieve a reference to the first element in the container. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr Reference Front() noexcept { UH_ASSERT(0 /*N > 0*/) return *static_cast< Pointer >(nullptr); }
    /// Retrieve a constant reference to the first element in the container. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr ConstReference Front() const noexcept { UH_ASSERT(0 /*N > 0*/) return *static_cast< ConstPointer >(nullptr); }

    /// Retrieve a reference to the last element in the container. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr Reference Back() noexcept { UH_ASSERT(0 /*N > 0*/) return *static_cast< Pointer >(nullptr); }
    /// Retrieve a constant reference to the last element in the container. Always yields undefined behavior on empty arrays.
    [[nodiscard]] constexpr ConstReference Back() const noexcept { UH_ASSERT(0 /*N > 0*/) return *static_cast< ConstPointer >(nullptr); }

    /// Retrieve a pointer to the underlying array serving as element storage. Pointer is not dereferenceable.
    [[nodiscard]] constexpr Pointer Data() noexcept { return static_cast< Pointer >(nullptr); }
    /// Retrieve a constant pointer to the underlying array serving as element storage. Pointer is not dereferenceable.
    [[nodiscard]] constexpr ConstPointer Data() const noexcept { return static_cast< ConstPointer >(nullptr); }

    /// Retrieve an iterator to the first element of the container. Equal to `End()` for empty arrays.
    [[nodiscard]] constexpr Iterator Begin() noexcept { return Iterator(nullptr); }
    /// Retrieve a constant iterator to the first element of the container. Equal to `End()` for empty arrays.
    [[nodiscard]] constexpr ConstIterator Begin() const noexcept { return ConstIterator(nullptr); }
    /// Retrieve an iterator to the element following the last element of the container. Undefined behavior if accessed.
    [[nodiscard]] constexpr Iterator End() noexcept { return Iterator(nullptr); }
    /// Retrieve a constant iterator to the element following the last element of the container. Undefined behavior if accessed.
    [[nodiscard]] constexpr ConstIterator End() const noexcept { return ConstIterator(nullptr); }

    /// Retrieve an iterator to the first element of the container. Alias of \ref Begin().
    [[nodiscard]] constexpr Iterator begin() noexcept { return Begin(); }
    /// Retrieve a constant iterator to the first element of the container. Alias of \ref Begin().
    [[nodiscard]] constexpr ConstIterator begin() const noexcept { return Begin(); }
    /// Retrieve an iterator to the element following the last element of the container. Alias of \ref End().
    [[nodiscard]] constexpr Iterator end() noexcept { return End(); }
    /// Retrieve a constant iterator to the element following the last element of the container. Alias of \ref End().
    [[nodiscard]] constexpr ConstIterator end() const noexcept { return End(); }

    /// Retrieve the number of elements in the container. Always 0 for empty arrays.
    [[nodiscard]] constexpr SizeType Size() const noexcept { return 0; }
    /// Retrieve the maximum number of elements the container is able to hold. Always 0 for empty arrays.
    [[nodiscard]] constexpr SizeType MaxSize() const noexcept { return 0; }
    /// Check if the container has no elements, i.e. whether `N` is 0, such that `Begin() == End()`. Always true.
    [[nodiscard]] constexpr bool Empty() const noexcept { return true; }

    /// Assigns the given value value to all elements in the container. Does nothing for empty arrays.
    template < class U = T >
    requires std::assignable_from< T &, const U & >
    constexpr void Fill(const U & v [[maybe_unused]]) noexcept { }
    /// Exchanges the contents of the container with those of other. Does nothing for empty arrays.
    constexpr void Swap(Array & o [[maybe_unused]]) noexcept { }
    /// Exchanges the contents of the container with those of a raw array of the same size. Does nothing for empty arrays.
    constexpr void Swap(T (&o [[maybe_unused]])[0]) noexcept { }
};

/// Equality comparison between two arrays of the same type and zero length. Empty arrays always compare equal.
template < class T > [[nodiscard]] constexpr bool operator == (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }
/// Inequality comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator != (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Less than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator < (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Greater than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator > (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Less than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator <= (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }
/// Greater than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator >= (const Array< T, 0 > & a [[maybe_unused]], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }

/// Equality comparison between two arrays of the same type and zero length. Empty arrays always compare equal.
template < class T > [[nodiscard]] constexpr bool operator == (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return true; }
/// Inequality comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator != (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return false; }
/// Less than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator < (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return false; }
/// Greater than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator > (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return false; }
/// Less than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator <= (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return true; }
/// Greater than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator >= (const Array< T, 0 > & a [[maybe_unused]], const T (&b [[maybe_unused]])[0]) noexcept { return true; }

/// Equality comparison between two arrays of the same type and zero length. Empty arrays always compare equal.
template < class T > [[nodiscard]] constexpr bool operator == (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }
/// Inequality comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator != (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Less than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator < (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Greater than comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator > (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return false; }
/// Less than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator <= (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }
/// Greater than or equal comparison between two arrays of the same type and zero length.
template < class T > [[nodiscard]] constexpr bool operator >= (const T (&a [[maybe_unused]])[0], const Array< T, 0 > & b [[maybe_unused]]) noexcept { return true; }

} // Namespace:: Urho3D
