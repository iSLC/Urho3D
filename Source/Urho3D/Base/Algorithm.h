//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"
#include "../Base/Iterator.h"
#include "../Base/Functors.h"
#include "../Base/TypeTraits.h"

namespace Urho3D
{

/// Returns an iterator pointing to the first element in the range [first, last) that is not less than value.
template <class TRandomAccessIterator, class T>
TRandomAccessIterator LowerBound(TRandomAccessIterator first, TRandomAccessIterator last, const T& value)
{
    unsigned count = last - first;

    while (count > 0)
    {
        const unsigned step = count / 2;
        const TRandomAccessIterator it = first + step;
        if (*it < value)
        {
            first = it + 1;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}

/// Returns an iterator pointing to the first element in the range [first, last) that is greater than value.
template <class TRandomAccessIterator, class T>
TRandomAccessIterator UpperBound(TRandomAccessIterator first, TRandomAccessIterator last, const T& value)
{
    unsigned count = last - first;

    while (count > 0)
    {
        const unsigned step = count / 2;
        const TRandomAccessIterator it = first + step;
        if (!(value < *it))
        {
            first = it + 1;
            count -= step + 1;
        }
        else
        {
            count = step;
        };
    }
    return first;
}

/// Swap the values of the elements the given iterators are pointing to.
template < typename It > constexpr void IterSwap(It a, It b) noexcept { Swap(*a, *b); }

/// Copies all elements in the range [\p first, \p last) starting from \p first and proceeding to \p last - 1.
/// The behavior is undefined if \p dest is within the range [\p first, \p last).
template < class InputIt, class OutputIt >
constexpr OutputIt Copy(InputIt first, InputIt last, OutputIt dest) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        *dest = *first;
    }
    return dest;
}

/// Same as \ref Copy but only copy the elements for which the predicate \p pred returns true.
/// The relative order of the elements that are copied is preserved.
template < class InputIt, class OutputIt, class UnaryPredicate >
constexpr OutputIt CopyIf(InputIt first, InputIt last, OutputIt dest, UnaryPredicate pred) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        if (pred(*first))
        {
            *dest = *first;
        }
    }
    return dest;
}

/// Copies exactly \p count values from the range beginning at \p first to the range beginning at \p dest.
template < class InputIt, class Size, class OutputIt >
constexpr OutputIt CopyN(InputIt first, Size count, OutputIt dest) noexcept
{
    for (; count; --count, ++dest, ++first)
    {
        *dest = *first;
    }
    return dest;
}

/// Copies the elements from the range, defined by [\p first, \p last), to another range ending at \p dest.
/// The elements are copied in reverse order (the last element is copied first), but their relative order is preserved.
template < class BiDirIt1, class BiDirIt2 >
constexpr BiDirIt2 CopyBackward(BiDirIt1 first, BiDirIt1 last, BiDirIt2 dest) noexcept
{
    while (first != last)
    {
        *(--dest) = *(--last);
    }
    return dest;
}

/// Applies the given function object \p f to the result of dereferencing every iterator in the range [\p first, \p last), in order.
template < class InputIt, class UnaryFunction >
constexpr UnaryFunction ForEach(InputIt first, InputIt last, UnaryFunction f) noexcept
{
    for (; first != last; ++first)
    {
        f(*first);
    }
    // Implicit move since C++11
    return f;
}

/// Applies the given function object \p f to the result of dereferencing every iterator in the range [\p first, \p first + \p count), in order.
template < class InputIt, class Size, class UnaryFunction >
constexpr InputIt ForEachN(InputIt first, Size count, UnaryFunction f) noexcept
{
    for (; count; --count, ++first)
    {
        f(*first);
    }
    return first;
}

/// Assigns the given \p value to the elements in the range [\p first, \p last).
template < class ForwardIt, class T >
constexpr void Fill(ForwardIt first, ForwardIt last, const T & value) noexcept
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}


/// Assigns the given \p value to the first \p count elements in the range beginning at \p first if \p count > 0.
// Does nothing otherwise.
template < class OutputIt, class Size, class T >
constexpr OutputIt FillN(OutputIt first, Size count, const T & value) noexcept
{
    for (; count; --count, ++first)
    {
        *first = value;
    }
    return first;
}

/// Exchanges elements between range [\p first1, \p last1) and another range starting at \p first2.
template < class ForwardIt1, class ForwardIt2 >
constexpr ForwardIt2 SwapRanges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) noexcept
{
    for (; first1 != last1; ++first1, ++first2)
    {
        IterSwap(first1, first2);
    }
    return first2;
}

/// Returns the first element in the range [\p first, \p last) that is equal to \p value.
template < class InputIt, class T >
[[nodiscard]] constexpr InputIt Find(InputIt first, InputIt last, const T & value) noexcept
{
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            return first;
        }
    }
    return last;
}

/// Returns the first element in the range [\p first, \p last) for which predicate \p p returns true.
template < class InputIt, class UnaryPredicate >
[[nodiscard]] constexpr InputIt FindIf(InputIt first, InputIt last, UnaryPredicate p) noexcept
{
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            return first;
        }
    }
    return last;
}

/// Returns the first element in the range [\p first, \p last) for which predicate \p q returns true.
template < class InputIt, class UnaryPredicate >
[[nodiscard]] constexpr InputIt FindIfNot(InputIt first, InputIt last, UnaryPredicate q) noexcept
{
    for (; first != last; ++first)
    {
        if (!q(*first))
        {
            return first;
        }
    }
    return last;
}

/// Searches the range [\p first, \p last) for two consecutive equal elements. Elements are compared using `operator ==`.
template < class ForwardIt >
[[nodiscard]] constexpr ForwardIt AdjacentFind(ForwardIt first, ForwardIt last) noexcept
{
    if (first != last)
    {
        for (ForwardIt prev = first; ++first != last; ++prev)
        {
            if (*prev == *first)
            {
                return prev;
            }
        }
    }
    return last;
}

/// Searches the range [\p first, \p last) for two consecutive equal elements. Elements are compared using the given binary predicate \p p.
template < class ForwardIt, class BinaryPredicate >
[[nodiscard]] constexpr ForwardIt AdjacentFind(ForwardIt first, ForwardIt last, BinaryPredicate p) noexcept
{
    if (first != last)
    {
        for (ForwardIt prev = first; ++first != last; ++prev)
        {
            if (p(*prev, *first))
            {
                return prev;
            }
        }
    }
    return last;
}

/// Find the first mismatching pair of elements from two ranges. Elements are compared using `operator==`.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p first2 + (\p last1 - \p first1)).
template < class InputIt1, class InputIt2 >
[[nodiscard]] constexpr Pair< InputIt1, InputIt2 > Mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2) noexcept
{
    while (first1 != last1 && *first1 == *first2)
    {
        ++first1, ++first2;
    }
    return MakePair(first1, first2);
}

/// Find the first mismatching pair of elements from two ranges. Elements are compared using the given binary predicate \p p.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p first2 + (\p last1 - \p first1)).
template < class InputIt1, class InputIt2, class BinaryPredicate >
[[nodiscard]] constexpr Pair< InputIt1, InputIt2 > Mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) noexcept
{
    while (first1 != last1 && p(*first1, *first2))
    {
        ++first1, ++first2;
    }
    return MakePair(first1, first2);
}

/// Find the first mismatching pair of elements from two ranges. Elements are compared using `operator==`.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p last2).
template < class InputIt1, class InputIt2 >
[[nodiscard]] constexpr Pair< InputIt1, InputIt2 > Mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) noexcept
{
    while (first1 != last1 && first2 != last2 && *first1 == *first2)
    {
        ++first1, ++first2;
    }
    return MakePair(first1, first2);
}

/// Find the first mismatching pair of elements from two ranges. Elements are compared using the given binary predicate \p p.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p last2).
template < class InputIt1, class InputIt2, class BinaryPredicate >
[[nodiscard]] constexpr Pair< InputIt1, InputIt2 > Mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p) noexcept
{
    while (first1 != last1 && first2 != last2 && p(*first1, *first2))
    {
        ++first1, ++first2;
    }
    return MakePair(first1, first2);
}

/// Check if the elements from two ranges are equal. Elements are compared using `operator==`.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p first2 + (\p last1 - \p first1)).
template < class InputIt1, class InputIt2 >
[[nodiscard]] constexpr bool Identical(InputIt1 first1, InputIt1 last1, InputIt2 first2) noexcept
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (!(*first1 == *first2))
        {
            return false;
        }
    }
    return true;
}

/// Check if the elements from two ranges are equal. Elements are compared using the given binary predicate \p p.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p first2 + (\p last1 - \p first1)).
template < class InputIt1, class InputIt2, class BinaryPredicate >
[[nodiscard]] constexpr bool Identical(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) noexcept
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (!p(*first1, *first2))
        {
            return false;
        }
    }
    return true;
}

/// Check if the elements from two ranges are equal. Elements are compared using `operator==`.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p last2).
template < class InputIt1, class InputIt2 >
[[nodiscard]] constexpr bool Identical(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) noexcept
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (!(*first1 == *first2))
        {
            return false;
        }
    }
    return true;
}

/// Check if the elements from two ranges are equal. Elements are compared using the given binary predicate \p p.
/// First range is defined by [\p first1, \p last1) and the other defined by [\p first2, \p last2).
template < class InputIt1, class InputIt2, class BinaryPredicate >
[[nodiscard]] constexpr bool Identical(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p) noexcept
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (!p(*first1, *first2))
        {
            return false;
        }
    }
    return true;
}

/// Count the number of elements in the range [\p first, \p last) that are equal to \p value.
template < class InputIt, class T >
[[nodiscard]] constexpr typename IteratorTraits< InputIt >::DifferenceType Count(InputIt first, InputIt last, const T & value) noexcept
{
    typename IteratorTraits< InputIt >::DifferenceType count = 0;
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            ++count;
        }
    }
    return count;
}

/// Count the number of elements in the range [\p first, \p last) for which predicate \p p returns true.
template < class InputIt, class UnaryPredicate >
[[nodiscard]] constexpr typename IteratorTraits< InputIt >::DifferenceType CountIf(InputIt first, InputIt last, UnaryPredicate p) noexcept
{
    typename IteratorTraits< InputIt >::DifferenceType count = 0;
    for (; first != last; ++first)
    {
        if (p(*first))
        {
            ++count;
        }
    }
    return count;
}

/// Apply the unary operation \p op to a range and store the result into another range.
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest + (\p last - \p first))
template < class InputIt, class OutputIt, class UnaryOperation >
constexpr OutputIt Transform(InputIt first, InputIt last, OutputIt dest, UnaryOperation op) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        *dest = op(*first);
    }
    return dest;
}

/// Apply the binary operation \p op to two ranges and store the result into another range. Both ranges must have same length.
/// First range is defined by [\p first1, \p last1) and second range is defined by [\p first2, \p first2 + (\p last1 - \p first1))
/// Destination range is defined by [\p dest, \p dest + (\p last1 - \p first1))
template < class InputIt1, class InputIt2, class OutputIt, class BinaryOperation >
constexpr OutputIt Transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt dest, BinaryOperation op) noexcept
{
    for (; first1 != last1; ++dest, ++first1, ++first2)
    {
        *dest = op(*first1, *first2);
    }
    return dest;
}

/// Replace all elements within a range that are equal to \p old_value with \p new_value.
/// Range is defined by [\p first, \p last)
template < class ForwardIt, class T >
constexpr void Replace(ForwardIt first, ForwardIt last, const T & old_value, const T & new_value) noexcept
{
    for (; first != last; ++first)
    {
        if (*first == old_value)
        {
            *first = new_value;
        }
    }
}

/// Replace all elements within a range for which predicate \p p returns true with \p new_value.
/// Range is defined by [\p first, \p last)
template < class ForwardIt, class UnaryPredicate, class T >
constexpr void ReplaceIf(ForwardIt first, ForwardIt last, UnaryPredicate p, const T & new_value) noexcept
{
    for (; first != last; ++first)
    {
        if(p(*first))
        {
            *first = new_value;
        }
    }
}

/// Copy all elements that are equal to \p old_value from one range to another. The source and destination ranges cannot overlap.
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest + (\p last - \p first))
template < class InputIt, class OutputIt, class T >
constexpr OutputIt ReplaceCopy(InputIt first, InputIt last, OutputIt dest, const T & old_value, const T & new_value) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        *dest = (*first == old_value) ? new_value : *first;
    }
    return dest;
}

/// Copy all elements for which predicate \p p returns true from one range to another. The source and destination ranges cannot overlap.
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest + (\p last - \p first))
template < class InputIt, class OutputIt, class UnaryPredicate, class T >
constexpr OutputIt ReplaceCopyIf(InputIt first, InputIt last, OutputIt dest, UnaryPredicate p, const T & new_value) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        *dest = p(*first) ? new_value : *first;
    }
    return dest;
}

/// Assign each element in range defined by [\p first, \p last) a value generated by the given function object \p g.
template < class ForwardIt, class Generator >
constexpr void Generate(ForwardIt first, ForwardIt last, Generator g) noexcept
{
    for (; first != last; ++first)
    {
        *first = g();
    }
}

/// Assign values, generated by given function object \p g, to the first \p count elements in the range beginning at \p first, if count > 0.
/// Does nothing otherwise.
template < class OutputIt, class Size, class Generator >
constexpr OutputIt GenerateN(OutputIt first, Size count, Generator g) noexcept
{
    for (; count; --count, ++first)
    {
        *first = g();
    }
    return first;
}

/// Reverse the order of elements in the range defined by [\p first, \p last).
template < class BiDirIt >
constexpr void Reverse(BiDirIt first, BiDirIt last) noexcept
{
    for (; Distance(first, --last) > 0; ++first)
    {
        IterSwap(first, last);
    }
}

/// Copy the elements from one range to another in such a way that the elements in the new range are in reverse order.
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest + (\p last - \p first))
template < class BiDirIt, class OutputIt >
constexpr OutputIt ReverseCopy(BiDirIt first, BiDirIt last, OutputIt dest) noexcept
{
    for (; first != last; ++dest)
    {
        *dest = *(--last);
    }
    return dest;
}

/// Moves the elements from one range to another. After this operation the elements in the moved-from range will still
/// contain valid values of the appropriate type, but not necessarily the same values as before the move.
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest + (\p last - \p first))
template < class InputIt, class OutputIt >
constexpr OutputIt Move(InputIt first, InputIt last, OutputIt dest) noexcept
{
    for (; first != last; ++dest, ++first)
    {
        *dest = Move(*first);
    }

    return dest;
}

/// Moves the elements from one range to another. The elements are moved in reverse order (the last element is moved first),
/// but their relative order is preserved. The behavior is undefined if \p dest is within (\p first, \p last].
/// Source range is defined by [\p first, \p last) and destination range is defined by [\p dest, \p dest - (\p last - \p first))
template < class BiDirIt1, class BiDirIt2 >
constexpr BiDirIt2 MoveBackward(BiDirIt1 first, BiDirIt1 last, BiDirIt2 dest) noexcept
{
    while (first != last)
    {
        *(--dest) = Move(*(--last));
    }
    return dest;
}

} // Namespace:: Urho3D
