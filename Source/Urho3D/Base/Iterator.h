//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/TypeTraits.h"

namespace Urho3D {

/// Marks an iterator that can read from the pointed-to element.
struct InputIteratorTag { };
/// Marks an iterator that can write to the pointed-to element.
struct OutputIteratorTag { };
/// Marks an iterator that can read data from the pointed-to element.
struct ForwardIteratorTag : public InputIteratorTag { };
/// Marks an iterator that can be moved in both directions (i.e. incremented and decremented).
struct BidirectionalIteratorTag : public ForwardIteratorTag { };
/// Marks an iterator that can be moved to point to any element in constant time.
struct RandomAccessIteratorTag : public BidirectionalIteratorTag { };
/// Marks an iterator whose logically adjacent elements are also physically adjacent in memory.
struct ContiguousIteratorTag : public RandomAccessIteratorTag { };

/// Type trait utility that provides uniform interface to the properties of iterator types.
template < class Iterator > struct IteratorTraits
{
    /// The type of the values that can be obtained by dereferencing the iterator. This type is void for output iterators.
    using ValueType         = typename Iterator::ValueType;
    /// Signed integer type that can be used to identify distance between iterators.
    using DifferenceType    = typename Iterator::DifferenceType;
    /// Defines a pointer to the type iterated over (\ref ValueType).
    using Pointer           = typename Iterator::Pointer;
    /// Defines a reference to the type iterated over (\ref ValueType).
    using Reference         = typename Iterator::Reference;
    /// The category of the iterator. Must be one of iterator category tags.
    using IteratorCategory  = typename Iterator::IteratorCategory;
};

/// Specialization of \ref IteratorTraits for pointers.
template < class T > struct IteratorTraits< T * >
{
    /// Whatever was given via template parameter \p T.
    using ValueType         = RemoveCV_t< T >;
    /// Signed integer type that can be used to identify distance between iterators.
    using DifferenceType    = ptrdiff_t;
    /// Same as \ref ValueType but with a pointer qualifier.
    using Pointer           = T *;
    /// Same as \ref ValueType but with a reference qualifier.
    using Reference         = T &;
    /// The category of the iterator. In this case \ref RandomAccessIteratorTag.
    using IteratorCategory  = RandomAccessIteratorTag;
};
/// Specialization of \ref IteratorTraits for const pointers.
template < class T > struct IteratorTraits< const T * >
{
    /// Whatever was given via template parameter \p T.
    using ValueType         = T;
    /// Signed integer type that can be used to identify distance between iterators.
    using DifferenceType    = ptrdiff_t;
    /// Same as \ref ValueType but with a const and pointer qualifier.
    using Pointer           = const T *;
    /// Same as \ref ValueType but with a const and reference qualifier.
    using Reference         = const T &;
    /// The category of the iterator. In this case \ref RandomAccessIteratorTag.
    using IteratorCategory  = RandomAccessIteratorTag;
};
/// Specialization of \ref IteratorTraits for void pointers.
template < > struct IteratorTraits< void * >
{
    /// The type of the values that can be obtained by dereferencing the iterator. In this case \ref uint8_t.
    using ValueType         = uint8_t;
    /// Signed integer type that can be used to identify distance between iterators.
    using DifferenceType    = ptrdiff_t;
    /// Same as \ref ValueType but with a pointer qualifier.
    using Pointer           = ValueType *;
    /// Same as \ref ValueType but with a reference qualifier.
    using Reference         = ValueType &;
    /// The category of the iterator. In this case \ref RandomAccessIteratorTag.
    using IteratorCategory  = RandomAccessIteratorTag;
};
/// Specialization of \ref IteratorTraits for const void pointers.
template < > struct IteratorTraits< const void * >
{
    /// The type of the values that can be obtained by dereferencing the iterator. In this case \ref uint8_t.
    using ValueType         = uint8_t;
    /// Signed integer type that can be used to identify distance between iterators.
    using DifferenceType    = ptrdiff_t;
    /// Same as \ref ValueType but with a const and pointer qualifier.
    using Pointer           = const ValueType *;
    /// Same as \ref ValueType but with a const and reference qualifier.
    using Reference         = const ValueType &;
    /// The category of the iterator. In this case \ref RandomAccessIteratorTag.
    using IteratorCategory  = RandomAccessIteratorTag;
};

/// Advance an iterator by a specific distance.
template < class It, class Distance > constexpr void Advance(It & it, Distance n) noexcept
{
    using Category = typename IteratorTraits< It >::IteratorCategory;
    static_assert(IsBaseOf_v< InputIteratorTag, Category >);
    // Cast the given distance to the proper type
    auto dist = typename IteratorTraits< It >::DifferenceType(n);
    // Is this a random access iterator?
    if constexpr (IsBaseOf_v< RandomAccessIteratorTag, Category >)
    {
        it += dist;
    }
    else
    {
        // Do we need to go forward?
        while (dist > 0)
        {
            --dist, ++it;
        }
        // Can this iterator go backwards?
        if constexpr (IsBaseOf_v< BidirectionalIteratorTag, Category >)
        {
            // Do we need to go backward? (obviously we do at this point. but not if `dist` is zero)
            while (dist < 0)
            {
                ++dist, --it;
            }
        }
    }
}
/// Specialization of \ref Advance for void pointers.
template < class Distance > constexpr void Advance(void *& it, Distance n) noexcept
{
    it = static_cast< typename IteratorTraits< void * >::Pointer >(it) + typename IteratorTraits< void * >::DifferenceType(n);
}
/// Specialization of \ref Advance for const void pointers.
template < class Distance > constexpr void Advance(const void *& it, Distance n) noexcept
{
    it = static_cast< typename IteratorTraits< const void * >::Pointer >(it) + typename IteratorTraits< const void * >::DifferenceType(n);
}
/// Returns the distance between two iterators.
template < class It > [[nodiscard]] constexpr typename IteratorTraits< It >::DifferenceType Distance(It first, It last) noexcept
{
    using Category = typename IteratorTraits< It >::IteratorCategory;
    static_assert(IsBaseOf_v< InputIteratorTag, Category >);
    // Is this a random access iterator?
    if constexpr (IsBaseOf_v< RandomAccessIteratorTag, Category >)
    {
        return (last - first);
    }
    else
    {
        typename IteratorTraits< It >::DifferenceType result = 0;
        // Iterate over all elements to find out how many are
        while (first != last)
        {
            ++first, ++result;
        }
        // Return the count
        return result;
    }
}
/// Specialization of \ref Distance for void pointers.
template < > [[nodiscard]] constexpr typename IteratorTraits< void * >::DifferenceType Distance(void * first, void * last) noexcept
{
    using Pointer = typename IteratorTraits< void * >::Pointer;
    using DifferenceType = typename IteratorTraits< void * >::DifferenceType;
    return static_cast< DifferenceType >(static_cast< Pointer >(last) - static_cast< Pointer >(first));
}
/// Specialization of \ref Distance for const void pointers.
template < > [[nodiscard]] constexpr typename IteratorTraits< void * >::DifferenceType Distance(const void * first, const void * last) noexcept
{
    using Pointer = typename IteratorTraits< const void * >::Pointer;
    using DifferenceType = typename IteratorTraits< const void * >::DifferenceType;
    return static_cast< DifferenceType >(static_cast< Pointer >(last) - static_cast< Pointer >(first));
}
/// Returns the distance between two iterators.
template < class It > [[nodiscard]] constexpr typename IteratorTraits< It >::DifferenceType AbsDistance(It first, It last) noexcept
{
    return static_cast< typename IteratorTraits< It >::DifferenceType >(Abs(Distance(first, last)));
}
/// Return the \p n 'th successor of iterator \p it.
template < class It > [[nodiscard]] constexpr It Next(It it, typename IteratorTraits< It >::DifferenceType n = 1) noexcept
{
    Advance(it, n);
    return it;
}
/// Return the \p n 'th predecessor of iterator \p it.
template < class It > [[nodiscard]] constexpr It Prev(It it, typename IteratorTraits< It >::DifferenceType n = 1) noexcept
{
    Advance(it, -n);
    return it;
}
/// Returns exactly `c.Begin()`, which is typically an iterator to the beginning of the sequence represented by \p c.
template < class T > [[nodiscard]] constexpr auto Begin(T & c) -> decltype(c.Begin()) { return c.Begin(); }
/// Returns exactly `c.Begin()`, which is typically a constant iterator to the beginning of the sequence represented by \p c.
template < class T > [[nodiscard]] constexpr auto Begin(const T & c) -> decltype(c.Begin()) { return c.Begin(); }
/// Returns a pointer to the beginning of the array \p c.
template < class T, size_t N > [[nodiscard]] constexpr T * Begin(T (&c)[N]) { return &c[0]; }
/// Returns a const pointer to the beginning of the array \p c.
template < class T, size_t N > [[nodiscard]] constexpr const T * Begin(const T (&c)[N]) { return &c[0]; }
/// Returns exactly `Begin(c)`, with \p c always treated as const-qualified.
template< class C > [[nodiscard]] constexpr auto CBegin(const C & c ) -> decltype(Begin(c)) { return Begin(c); }

/// Returns exactly `c.End()`, which is typically an iterator one past the end of the sequence represented by \p c.
template < class T > [[nodiscard]] constexpr auto End(T & c) -> decltype(c.End()) { return c.End(); }
/// Returns exactly `c.End()`, which is typically a constant iterator one past the end of the sequence represented by \p c.
template < class T > [[nodiscard]] constexpr auto End(const T & c) -> decltype(c.End()) { return c.End(); }
/// Returns a pointer to the end of the array \p c.
template < class T, size_t N > [[nodiscard]] constexpr T * End(T (&c)[N]) { return &c[N]; }
/// Returns a const pointer to the end of the array \p c.
template < class T, size_t N > [[nodiscard]] constexpr const T * End(const T (&c)[N]) { return &c[N]; }
/// Returns exactly `End(c)`, with \p c always treated as const-qualified.
template< class C > [[nodiscard]] constexpr auto CEnd(const C & c ) -> decltype(End(c)) { return End(c); }

/// Return exactly `c.Size()`.
template < class C > [[nodiscard]] constexpr auto Size(const C & c) -> decltype(c.Size()) { return c.Size(); }
/// Return exactly `c.Size()` converted to a signed type.
template < class C > [[nodiscard]] constexpr auto SSize(const C & c) -> MakeSigned_t< decltype(c.Size()) >
{
    return static_cast< MakeSigned_t< decltype(c.Size()) > >(c.Size());
}
/// Returns the size of the array. In this case the \p N parameter.
template < class T, size_t N > [[nodiscard]] constexpr size_t Size(const T (&c)[N]) { return N; }
/// Returns the size of the array. In this case the \p N parameter.
template < class T, ptrdiff_t N > [[nodiscard]] constexpr ptrdiff_t SSize(const T (&c)[N]) { return N; }

/// Return whether the given container is empty. In this case, exactly the result of `c.Empty()`.
template < class C > [[nodiscard]] constexpr auto Empty(const C & c) -> decltype(c.Empty()) { return c.Empty(); }
/// Returns whether the given array is empty. In this case, whether parameter \p N is zero.
template < class T, size_t N> [[nodiscard]] constexpr bool Empty(const T (&a [[maybe_unused]])[N]) { return (N == 0); }

/// Returns a pointer to the block of memory containing the elements of the container.
template < class C > [[nodiscard]] constexpr auto Data(C & c) -> decltype(c.Data()) {  }
/// Returns a const pointer to the block of memory containing the elements of the container.
template < class C > [[nodiscard]] constexpr auto Data(const C& c) -> decltype(c.Data()) {  }
/// Returns a pointer to the beginning array itself.
template <class T, size_t N > [[nodiscard]] constexpr T * Data(T (&a)[N]) { return a; }
/// Returns a const pointer to the beginning array itself.
template <class T, size_t N > [[nodiscard]] constexpr const T * Data(const T (&a)[N]) { return a; }

/// Iterator adaptor that reverses the direction of a given iterator.
template < class Iterator > struct ReverseIterator
{
    using ValueType         = typename IteratorTraits< Iterator >::ValueType;
    using DifferenceType    = typename IteratorTraits< Iterator >::DifferenceType;
    using Pointer           = typename IteratorTraits< Iterator >::Pointer  ;
    using Reference         = typename IteratorTraits< Iterator >::Reference;
    using IteratorCategory  = typename IteratorTraits< Iterator >::IteratorCategory;
    using IteratorType      = Iterator;

    /// Default constructor. The underlying iterator is value-initialized.
    /// Operations on the resulting iterator have defined behavior if and only if the corresponding operations on a value-initialized \p Iterator also have defined behavior.
    constexpr ReverseIterator() noexcept = default;

    /// The underlying iterator is initialized with \p itr.
    constexpr explicit ReverseIterator(Iterator itr) noexcept
        : itr_(itr)
    { }

    /// The underlying iterator is initialized with \p itr.
    /// This overload participates in overload resolution only if \p U is not the same type as \p Iterator and \ref IsConvertible< const U &, Iterator > is modeled.
    template < class U, EnableIf_t<
        Negation_v< IsSame< U, Iterator > > && IsConvertible_v< const U &, Iterator >
    , int > = 0 > constexpr ReverseIterator(const ReverseIterator< U > & o) noexcept
        : itr_(o.Base())
    { }

    /// The underlying iterator is assigned the value of the underlying iterator of \p o, i.e. o.Base().
    /// This overload participates in overload resolution only if \p U is not the same type as \p Iterator and \ref IsConvertible< const U &, Iterator > and \ref IsAssignable< Iterator &, const U & > are modeled.
    template < class U > constexpr auto operator = (const ReverseIterator< U > & o) noexcept
        -> EnableIf_t<
            Negation_v< IsSame< U, Iterator > > && IsConvertible_v< const U &, Iterator > && IsAssignable_v< Iterator &, const U & >
        , ReverseIterator< U > > &
    {
        itr_ = o.Base();
        return *this;
    }

    /// Returns the underlying base iterator.
    constexpr Iterator Base() const { return itr_; }

    /// Returns a reference to the element previous to current.
    constexpr Reference operator * () const { Iterator prev(itr_); return *(--prev); }
    /// Returns a pointer to the element previous to current.
    constexpr Pointer operator -> () const { Iterator prev(itr_); return AddressOf(*(--prev)); }

    ///
    constexpr ReverseIterator & operator ++ () { --itr_; return *this; }
    ///
    constexpr ReverseIterator & operator -- () { ++itr_; return *this; }
    ///
    constexpr ReverseIterator operator ++ (int) { ReverseIterator prev(*this); return --prev; } // NOLINT(cert-dcl21-cpp)
    ///
    constexpr ReverseIterator operator -- (int) { ReverseIterator prev(*this); return ++prev; } // NOLINT(cert-dcl21-cpp)
    ///
    constexpr ReverseIterator & operator += (size_t n) { itr_ -= n; return *this; }
    ///
    constexpr ReverseIterator & operator -= (size_t n) { itr_ += n; return *this; }
    ///
    constexpr ReverseIterator operator + (size_t n) const { return ReverseIterator(itr_ - n); }
    ///
    constexpr ReverseIterator operator - (size_t n) const { return ReverseIterator(itr_ + n); }
    ///
    constexpr Reference operator [] (size_t n) const { return *(*this + n); }
    ///
    constexpr DifferenceType operator - (const ReverseIterator & i) const { return Distance(i.itr_, itr_); }
protected:

    ///
    Iterator itr_{};
};


/// Perform an equality comparison on the underlying iterators. i.e. lhs.base() == rhs.base()
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator == (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() == rhs.Base();
}

/// Perform an inequality comparison on the underlying iterators. i.e. lhs.Base() != rhs.Base();
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator != (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() != rhs.Base();
}

/// Perform a less than comparison on the underlying iterators.
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator < (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() > rhs.Base();
}

/// Perform a greater than comparison on the underlying iterators. i.e. lhs.Base() > rhs.Base();
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator <= (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() >= rhs.Base();
}

/// Perform a less than or equal comparison on the underlying iterators. i.e. lhs.Base() < rhs.Base();
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator > (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() < rhs.Base();
}

/// Perform a greater than or equal comparison on the underlying iterators. i.e. lhs.Base() <= rhs.Base();
/// Inverse comparisons are applied in order to take into account that the iterator order is reversed.
template< class Iterator1, class Iterator2 >
constexpr bool operator >= (const ReverseIterator< Iterator1 > & lhs, const ReverseIterator< Iterator2 > & rhs)
{
    return lhs.Base() <= rhs.Base();
}

/// Deduction guide for \ref ReverseIterator.
template < class Iterator > ReverseIterator(Iterator) -> ReverseIterator< Iterator >;

/// Convenience function template that constructs a \ref ReverseIterator for the given iterator \p i with the type deduced from the type of the argument.
template < class Iterator > inline constexpr ReverseIterator< Iterator > MakeReverseIterator(Iterator i)
{
    return ReverseIterator< Iterator >(i);
}

///
template < class Iterator > struct MoveIterator
{

    using ValueType         = typename IteratorTraits< Iterator >::ValueType;
    using DifferenceType    = typename IteratorTraits< Iterator >::DifferenceType;
    using Pointer           = typename IteratorTraits< Iterator >::Pointer  ;
    using Reference         = AddRvalueReference_t< ValueType >;
    using IteratorCategory  = typename IteratorTraits< Iterator >::IteratorCategory;

    ///
    MoveIterator () noexcept = default;
    ///
    explicit MoveIterator(Iterator itr) noexcept
        : itr_(Move(itr))
    { }

    ///
    constexpr bool operator == (const MoveIterator & o) const noexcept { return (itr_ == o.itr_); }
    ///
    constexpr bool operator < (const MoveIterator & o) const noexcept { return (itr_ < o.itr_); }
    ///
    constexpr Iterator Base() const noexcept { return itr_; }
    ///
    constexpr Reference operator * () const noexcept { return Move(*itr_); }
    ///
    constexpr Pointer operator -> () const noexcept { return &(*itr_); }
    ///
    constexpr MoveIterator & operator ++ () noexcept { ++itr_; return *this; }
    ///
    constexpr MoveIterator & operator -- () noexcept { --itr_; return *this; }
    ///
    constexpr MoveIterator operator ++ (int) noexcept { MoveIterator r(*this); return ++itr_; } // NOLINT(cert-dcl21-cpp)
    ///
    constexpr MoveIterator operator -- (int) noexcept { MoveIterator r(*this); return --itr_; } // NOLINT(cert-dcl21-cpp)
    ///
    constexpr MoveIterator & operator += (size_t n) noexcept { itr_ += n; return *this; }
    ///
    constexpr MoveIterator & operator -= (size_t n) noexcept { itr_ -= n; return *this; }
    ///
    constexpr MoveIterator operator + (size_t n) const noexcept { return MoveIterator(itr_ - n); }
    ///
    constexpr MoveIterator operator - (size_t n) const noexcept { return MoveIterator(itr_ + n); }
    ///
    constexpr Reference operator [] (size_t n) const noexcept { return Move(*(*this + n)); }
    ///
    constexpr DifferenceType operator - (const MoveIterator & i) const noexcept { return Distance(itr_, i.itr_); }
protected:

    ///
    Iterator itr_{};
};

///
template < class Iterator > inline constexpr MoveIterator< Iterator > MakeMoveIterator(Iterator i) noexcept
{
    return MoveIterator< Iterator >(i);
}

///
template < typename Container > struct InsertIterator
{
    using ValueType         = typename Container::ValueType;
    using DifferenceType    = typename Container::DifferenceType;
    using Pointer           = typename Container::Pointer;
    using Reference         = typename Container::Reference;
    using Iterator          = typename Container::Iterator;
    using IteratorCategory  = OutputIteratorTag;

    ///
    constexpr explicit InsertIterator(Container & out, Iterator itr) noexcept
        : out_(out)
        , itr_(itr)
    { }

    ///
    constexpr InsertIterator &  operator = (typename Container::ConstReference v) noexcept
    {
        itr_ = out_.Insert(itr_, v);
        return *this;
    }
    ///
    constexpr InsertIterator &  operator * () noexcept  { return *this; }
    ///
    constexpr InsertIterator &  operator ++ () noexcept { ++itr_; return *this; }
    ///
    constexpr InsertIterator    operator ++ (int) noexcept  { InsertIterator prev(*this); ++itr_; return prev; } // NOLINT(cert-dcl21-cpp)

protected:
    ///
    Container & out_;
    ///
    Iterator    itr_;
};

///
template < class Container >
inline constexpr InsertIterator< Container > Inserter(Container & out, typename Container::Iterator itr) noexcept
{
    return InsertIterator< Container >(out, itr);
}

///
template < class Container > struct BackInsertIterator
{
    using ValueType         = typename Container::ValueType;
    using DifferenceType    = typename Container::DifferenceType;
    using Pointer           = typename Container::Pointer;
    using Reference         = typename Container::Reference;
    using IteratorCategory  = OutputIteratorTag;

    ///
    constexpr explicit BackInsertIterator(Container & out) noexcept
        : out_(out)
    { }

    ///
    constexpr BackInsertIterator &  operator = (typename Container::ConstReference v) noexcept { out_.PushBack(v); return *this; }
    ///
    constexpr BackInsertIterator &  operator * () noexcept { return *this; }
    ///
    constexpr BackInsertIterator &  operator ++ () noexcept { return *this; }
    ///
    constexpr BackInsertIterator operator ++ (int) noexcept { return *this; } // NOLINT(cert-dcl21-cpp)
protected:

    ///
    Container & out_;
};

///
template < class Container > inline constexpr BackInsertIterator< Container > BackInserter(Container & out) noexcept
{
    return BackInsertIterator< Container >(out);
}

///
template < class Container > struct FrontInsertIterator
{
    using ValueType         = typename Container::ValueType;
    using DifferenceType    = typename Container::DifferenceType;
    using Pointer           = typename Container::Pointer;
    using Reference         = typename Container::Reference;
    using IteratorCategory  = OutputIteratorTag;

    ///
    constexpr explicit FrontInsertIterator(Container & out) noexcept
        : out_(out)
    { }

    ///
    constexpr FrontInsertIterator & operator = (typename Container::ConstReference v) noexcept { out_.PushFront(v); return *this; }
    ///
    constexpr FrontInsertIterator & operator * () noexcept { return *this; }
    ///
    constexpr FrontInsertIterator & operator ++ () noexcept { return *this; }
    ///
    constexpr FrontInsertIterator operator ++ (int) noexcept { return *this; } // NOLINT(cert-dcl21-cpp)
protected:

    ///
    Container & out_;
};

///
template < class Container > inline constexpr FrontInsertIterator< Container > FrontInserter(Container & ctr) noexcept
{
    return FrontInsertIterator< Container >(ctr);
}

} // Namespace:: Urho3D
