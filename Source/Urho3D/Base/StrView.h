//
// Copyright (c) 2008-2020 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Urho3DConfig.h"

#include <cstring>

namespace Urho3D
{


/// Attempt to determine whether the characters of a NULL-terminated string are known at compile time.
/// This only works on compilers that provide the means necessary to identify such thing. MSVC is not one of them.
template < class T > [[nodiscard]] U3D_INLINE constexpr bool IsConstantString(const T * s [[maybe_unused]]) noexcept
{
#if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
    // In constexpr contexts all strings should be constant.
    return __builtin_is_constant_evaluated();
#elif defined(U3D_HAVE_BUILTIN_CONSTANT_P)
    while (__builtin_constant_p(*s) && *s) s++;
    return __builtin_constant_p(*s);
#else
    return false; // Make no assumptions
#endif
}

/// Attempt to determine whether the characters of a character array are known at compile time.
/// This only works on compilers that provide the means necessary to identify such thing. MSVC is not one of them.
template < class T >
[[nodiscard]] U3D_INLINE constexpr bool IsConstantCharArray(const T * a [[maybe_unused]], size_t n [[maybe_unused]]) noexcept
{
#if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
    // In constexpr contexts all character arrays should be constant.
    return __builtin_is_constant_evaluated();
#elif defined(U3D_HAVE_BUILTIN_CONSTANT_P)
    size_t i = 0;
    while (i < n && __builtin_constant_p(a[i])) i++;
    return i == n;
#else
    return false; // Make no assumptions
#endif
}

/// Object that can refer to a constant contiguous sequence of `char`-like objects with the first element of the
/// sequence at position zero. Similar to `std::basic_string_view` but less generic and more realistic on usage.
struct StrView
{
    static_assert(sizeof(char) == 1, "Code in this implementation expects a character to occupy a single byte");

    /// Type used to represent a single element in the viewed memory.
    using ValueType = char;
    /// Same as `ValueType` but with a reference qualifier.
    using Reference = const ValueType &;
    /// Same as `Reference` but with a const qualifier.
    using ConstReference = const ValueType &;
    /// Same as `ValueType` but with a pointer qualifier.
    using Pointer = const ValueType *;
    /// Same as `Pointer` but with a const qualifier.
    using ConstPointer = const ValueType *;
    /// Unsigned integer type used to represent quantities and/or lengths.
    using SizeType = size_t;

    /// String iterator.
    using Iterator = ValueType *;
    /// Constant string iterator.
    using ConstIterator = const ValueType *;

    /// Index outside the range of the largest string possible.
    static constexpr inline size_t NPOS = ~size_t(0);

    /// Default constructor. Initializes to an empty view.
    /// After construction, \ref Data() is equal to nullptr, and \ref Size() is equal to 0.
    constexpr StrView() noexcept = default;

    /// Explicit constructor. Initializes to a specific view. Behavior is undefined if [\p s, \p s + \p n) is not a valid range.
    /// After construction, \ref Data() is equal to \p s, and \ref Size() is equal to \p n.
    constexpr StrView(ConstPointer s, SizeType n) noexcept
        : m_Data(s), m_Size(n)
    { }

    /// C string constructor. Initializes the view to a null terminated array of characters.
    /// After construction, \ref Data() is equal to \p s, and \ref Size() is equal to position of the null character.
    constexpr StrView(ConstPointer s) noexcept // NOLINT(google-explicit-constructor)
        : m_Data(s), m_Size(Count(s))
    { }

    /// String constructor. Initializes from a heap string.
    /// After construction, \ref Data() is equal to `s.Data()`, and \ref Size() is equal to `s.Size()`.
    explicit StrView(const String & s) noexcept;

    /// Copy constructor. Initializes as a copy of another view.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal to `o.Size()`.
    constexpr StrView(const StrView & o) noexcept = default;

    /// Move constructor. Initializes as a copy of another view.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal to `o.Size()`.
    constexpr StrView(StrView && o) noexcept = default;

    /// Copy assignment operator. Assigns as a copy of another view.
    /// After assignment, \ref Data() is equal to `o.Data()`, and \ref Size() is equal to `o.Size()`.
    constexpr StrView & operator = (const StrView & o) noexcept = default;

    /// Move assignment operator. Assigns as a copy of another view.
    /// After assignment, \ref Data() is equal to `o.Data()`, and \ref Size() is equal to `o.Size()`.
    constexpr StrView & operator = (StrView && o) noexcept = default;

    /// Retrieve a constant reference to the character at location specified by \p i.
    [[nodiscard]] constexpr ConstReference At(SizeType i) const noexcept
    {
        U3D_ASSERT(i < m_Size)
        U3D_ASSERT(m_Size > 0)
        return m_Data[i];
    }

    /// Retrieve a constant reference to the character at location specified by \p i.
    [[nodiscard]] constexpr ConstReference  operator [] (SizeType i) const noexcept { return At(i); }

    /// Retrieve a constant iterator to the first character in the view.
    [[nodiscard]] constexpr ConstIterator Begin() const noexcept { return ConstIterator(m_Data); }
    /// Retrieve a constant iterator to the character following the last character in the view.
    [[nodiscard]] constexpr ConstIterator End() const noexcept { return ConstIterator(m_Data + m_Size); }

    /// Retrieve a constant iterator to the first character in the view. Alias of \ref Begin().
    [[nodiscard]] constexpr ConstIterator begin() const noexcept { return Begin(); }
    /// Retrieve a constant iterator to the character following the last character in the view. Alias of \ref End().
    [[nodiscard]] constexpr ConstIterator end() const noexcept { return End(); }

    /// Retrieve a constant iterator to a specific character in the view.
    [[nodiscard]] constexpr ConstIterator Iat(SizeType i) const noexcept { U3D_ASSERT(i <= m_Size) return ConstIterator(m_Data + i); }

    /// Retrieve a constant reference to the first character in the view.
    [[nodiscard]] constexpr ConstReference  Front() const noexcept { U3D_ASSERT(m_Size > 0) return m_Data[0]; }

    /// Retrieve a constant reference to the last character in the view.
    [[nodiscard]] constexpr ConstReference Back() const noexcept { U3D_ASSERT(m_Size > 0) return m_Data[m_Size - 1]; }

    /// Retrieve a constant pointer to the viewed array of characters.
    [[nodiscard]] constexpr ConstPointer Data() const noexcept { return m_Data; }

    /// Retrieve a constant pointer to the viewed array of characters starting at location specified by \p i.
    [[nodiscard]] constexpr ConstPointer DataFrom(SizeType i) const noexcept { U3D_ASSERT(m_Size >= i) return m_Data + i; }

    /// Retrieve the number of characters that the view is currently looking at.
    template < class T = SizeType > [[nodiscard]] constexpr T Size() const noexcept { return static_cast< T >(m_Size); }

    /// Retrieve the number of characters that the view is currently looking at. Alias of \ref Size().
    template < class T = SizeType > [[nodiscard]] constexpr T Length() const noexcept { return static_cast< T >(m_Size); }

    /// Retrieve the maximum number of characters that can be viewed at once on current platform and implementation.
    [[nodiscard]] static constexpr SizeType Msize_t() noexcept { return NPOS; }

    /// Check if the view has is currently looking at any characters.
    [[nodiscard]] constexpr bool Empty() const noexcept { return m_Size == 0; }

    /// Move the start of the view forward by \p n characters. The behavior is undefined if \p n > \ref Size().
    constexpr void RemovePrefix(SizeType n) noexcept
    {
        U3D_ASSERT(n <= m_Size)
        m_Data += n, m_Size -= n;
    }
    /// Move the end of the view back by \p n characters. The behavior is undefined if \p n > \ref Size().
    constexpr void RemoveSuffix(SizeType n) noexcept
    {
        U3D_ASSERT(n <= m_Size)
        m_Size -= n;
    }

    /// Exchange the view with that of \p o.
    constexpr void Swap(StrView & o) noexcept
    {
        const SizeType s = m_Size;
        m_Size = o.m_Size;
        o.m_Size = s;
        const ConstPointer d = m_Data;
        m_Data = o.m_Data;
        o.m_Data = d;
    }

    /// Copy the sub-string [\p pos, \p pos + `rcount`) to the character string pointed to by \p dest.
    /// `rcount` is the smaller of \p count and \ref Size() - \p pos.
    constexpr SizeType Copy(ValueType * dest, SizeType count, SizeType pos = 0) const noexcept
    {
        U3D_ASSERT(pos <= m_Size)
        // Do we need to copy anything?
        if (count == 0)
        {
            return 0; // False alarm
        }
        // Number of characters that SHOULD be copied
        const SizeType rcount = (m_Size - pos) >= count ? count : m_Size - pos;
        // Where the copying should start at
        ConstPointer source = m_Data + pos;
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        // Can the operation be performed at compile time?
        if (__builtin_is_constant_evaluated())
        {
    #endif
            // Perform a direct character by character copy
            for (size_t i = 0; i < rcount; ++i)
            {
                dest[i] = source[i];
            }
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        }
        else
        {
            // Fall back to a built-in "run-time" copy
            memcpy(dest, source, rcount);
        }
    #endif
        // Return the number of copied characters
        return rcount;
    }

    /// Obtain a view of the sub-string [\p pos, \p pos + \p rcount).
    /// `rcount` is the smaller of \p count and \ref Size() - \p pos.
    [[nodiscard]] constexpr StrView SubStr(SizeType pos = 0, SizeType count = NPOS) const noexcept
    {
        U3D_ASSERT(pos <= m_Size)
        return StrView(m_Data + pos, (m_Size - pos) >= count ? count : (m_Size - pos));
    }

    /// Compare this view against another view \p v.
    [[nodiscard]] constexpr int Compare(StrView v) const noexcept
    {
        // Attempt to compare the viewed character sequence first 
        if (const int r = Compare(m_Data, v.m_Data, m_Size >= v.m_Size ? m_Size : v.m_Size); r != 0)
        {
            return r;
        }
        // Try the view size if previous result was inconclusive 
        return (m_Size == v.m_Size ? 0 : (m_Size < v.m_Size ? -1 : 1));
    }

    /// Compare a portion of this view against another view \p v.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType count, StrView v) const noexcept
    {
        return SubStr(pos, count).Compare(v);
    }
    /// Compare a portion of this view against a portion of another view \p v.
    [[nodiscard]] constexpr int Compare(SizeType pos1, SizeType count1, StrView v, SizeType pos2, SizeType count2) const noexcept
    {
        return SubStr(pos1, count1).Compare(v.SubStr(pos2, count2));
    }
    /// Compare this view against a null-terminated character string.
    [[nodiscard]] constexpr int Compare(const ValueType * s) const noexcept
    {
        return Compare(StrView(s));
    }
    /// Compare a portion of this view against a null-terminated character string.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType count, const ValueType * s) const noexcept
    {
        return SubStr(pos, count).Compare(StrView(s));
    }
    /// Compare a portion of this view against a character array.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType count1, const ValueType * s, SizeType count2) const noexcept
    {
        return SubStr(pos, count1).Compare(StrView(s, count2));
    }

    /// Check if the string view begins with the given prefix, where the prefix is another string view.
    [[nodiscard]] constexpr bool StartsWith(StrView v) const noexcept
    {
        return m_Size >= v.m_Size && Compare(0, v.m_Size, v) == 0;
    }
    /// Check if the string view begins with the given prefix, where the prefix is a single character.
    [[nodiscard]] constexpr bool StartsWith(ValueType c) const noexcept
    {
        return m_Size > 0 && Locate(m_Data, m_Size, c) == m_Data;
    }
    /// Check if the string view begins with the given prefix, where the prefix is a null-terminated character string.
    [[nodiscard]] constexpr bool StartsWith(const ValueType * s) const noexcept
    {
        return StartsWith(StrView(s));
    }

    /// Check if the string view ends with the given suffix, where the suffix is another string view \p v.
    [[nodiscard]] constexpr bool EndsWith(StrView v) const noexcept
    {
        return m_Size >= v.m_Size && Compare(m_Size - v.m_Size, NPOS, v) == 0;
    }
    /// Check if the string view ends with the given suffix, where the suffix is a single character \p c.
    [[nodiscard]] constexpr bool EndsWith(ValueType c) const noexcept
    {
        return m_Size > 0 && Locate(m_Data, m_Size, c) == (m_Data + m_Size);
    }
    /// Check if the string view ends with the given suffix, where the suffix is a null-terminated character string \p s.
    [[nodiscard]] constexpr bool EndsWith(const ValueType * s) const noexcept
    {
        return EndsWith(StrView(s));
    }

    /// Find the first occurrence of another string view \p v in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(StrView v, SizeType pos = 0) const noexcept
    {
        return Find(v.m_Data, pos, v.m_Size);
    }

    /// Find the first occurrence of a single character \p c in this view, starting at position \p pos.
    /// If the character is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(ValueType c, SizeType pos = 0) const noexcept
    {
        // Is the starting point within range?
        if (pos <= m_Size)
        {
            // Keep looking while there are unchecked characters ranges
            if (auto p = Locate(m_Data + pos, m_Size - pos, c); p != nullptr)
            {
                return static_cast< SizeType >(p - m_Data);
            }
        }
        // Unable to locate such character
        return NPOS;
    }

    /// Find the first occurrence of null-terminated character string \p s in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // If there's nothing to search for then there's no point in trying
        if (U3D_UNLIKELY(count == 0))
        {
            // Is the starting point within range?
            if (U3D_LIKELY(pos <= m_Size))
            {
                return pos; // That's where the nothing can be found
            }
        }
        // Can the sub-string even fit this view?
        else if (count <= m_Size)
        {
            for (SizeType end = m_Size - count; pos <= end; ++pos)
            {
                // If one character matches then see if the rest match as well
                if (m_Data[pos] == s[0] && Compare(m_Data + pos + 1, s + 1, count - 1) == 0)
                {
                    return pos; // Sub-string found at this location
                }
            }
        }
        // Unable to locate such sub-string
        return NPOS;
    }

    /// Find the first occurrence of string \p s in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(const ValueType * s, SizeType pos = 0) const noexcept
    {
        return Find(s, pos, Count(s));
    }

    /// Find the last occurrence of another string view \p v in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned. 
    [[nodiscard]] constexpr SizeType RFind(StrView v, SizeType pos = NPOS) const noexcept
    {
        return RFind(v.m_Data, pos, v.m_Size);
    }

    /// Find the last occurrence of a single character \p c in this view, starting at position \p pos.
    /// If the character is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(ValueType c, SizeType pos = NPOS) const noexcept
    {
        // Is there a string to search into?
        if (SizeType size = m_Size; U3D_LIKELY(size > 0))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters
            for (++size; size-- > 0; )
            {
            // Is this the character we're looking for?
                if (m_Data[size] == c)
                {
                    return size; // Character found at this location
                }
            }
        }
        // No such character
        return NPOS;
    }

    /// Find the last occurrence of null-terminated character string \p s in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // Can the sub-string even fit this view?
        if (U3D_LIKELY(count <= m_Size))
        {
            // Rectify starting point, if necessary
            pos = (m_Size - count) >= pos ? pos : (m_Size - count);
            // Look for the sub-string in reverse
            do {
                if (Compare(m_Data + pos, s, count) == 0)
                {
                    return pos; // Sub-string found at this location
                }
            } while (pos-- > 0);
        }
        // No such sub-string
        return NPOS;
    }

    /// Find the last occurrence of string \p s in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(const ValueType * s, SizeType pos = NPOS) const noexcept
    {
        return RFind(s, pos, Count(s));
    }

    /// Finds the first occurrence of any of the characters of \p v in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindFirstOf(StrView v, SizeType pos = 0) const noexcept
    {
        return FindFirstOf(v.m_Data, pos, v.m_Size);
    }

    /// Finds the first character equal to the given character.
    [[nodiscard]] constexpr SizeType FindFirstOf(ValueType c, SizeType pos = 0) const noexcept
    {
        return Find(c, pos);
    }

    /// Finds the first character equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindFirstOf(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // Keep looking while there are unchecked characters ranges
        for (; count && pos < m_Size; ++pos)
        {
            // Look for the specified sub-string in this range
            if (Locate(s, count, m_Data[pos]) != nullptr)
            {
                return pos; // Sub-string found at this location
            }
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the first character equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindFirstOf(const ValueType * s, SizeType pos = 0) const noexcept
    {
        return FindFirstOf(s, pos, Count(s));
    }

    /// Finds the last occurrence of any of the characters of \p v in this view, ending at position \p pos.
    [[nodiscard]] constexpr SizeType FindLastOf(StrView v, SizeType pos = NPOS) const noexcept
    {
        return FindLastOf(v.m_Data, pos, v.m_Size);
    }

    /// Finds the last character equal to the given character.
    [[nodiscard]] constexpr SizeType FindLastOf(ValueType c, SizeType pos = NPOS) const noexcept
    {
        return RFind(c, pos);
    }

    /// Finds the last character equal to one of characters in the given character array.
    [[nodiscard]] constexpr SizeType FindLastOf(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // Is there a string to search into or even search for?
        if (SizeType size = m_Size; U3D_LIKELY(size && count))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters ranges
            do {
                // Look for the specified sub-string in this range
                if (Locate(s, count, m_Data[size]) != nullptr)
                {
                    return size; // Sub-string found at this location
                }
            } while (size-- != 0);
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the last character equal to one of characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindLastOf(const ValueType * s, SizeType pos = NPOS) const noexcept
    {
        return FindLastOf(s, pos, Count(s));
    }

    /// Finds the first character not equal to any of the characters of \p v in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(StrView v, SizeType pos = 0) const noexcept 
    {
        return FindFirstNotOf(v.m_Data, pos, v.m_Size);
    }

    /// Finds the first character not equal to the given character.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(ValueType c, SizeType pos = 0) const noexcept
    {
        // Keep looking while there are unchecked characters
        for (; pos < m_Size; ++pos)
        {
            // Is this the character we're looking for?
            if (m_Data[pos] != c)
            {
                return pos; // Character found at this location
            }
        }
        // No such character
        return NPOS;
    }

    /// Finds the first character not equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // Keep looking while there are unchecked characters ranges
        for (; pos < m_Size; ++pos)
        {
            // Look for the specified sub-string in this range
            if (Locate(s, count, m_Data[pos]) == nullptr)
            {
                return pos; // Sub-string found at this location
            }
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the first character not equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(const ValueType * s, SizeType pos = 0) const noexcept
    {
        return FindFirstNotOf(s, pos, Count(s));
    }

    /// Finds the last character not equal to any of the characters of \p v in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindLastNotOf(StrView v, SizeType pos = NPOS) const noexcept
    {
        return FindLastNotOf(v.m_Data, pos, v.m_Size);
    }

    /// Finds the last character not equal to the given character.
    [[nodiscard]] constexpr SizeType FindLastNotOf(ValueType c, SizeType pos = NPOS) const noexcept
    {
        // Is there a string to search into?
        if (SizeType size = m_Size; U3D_LIKELY(size))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters
            do {
                // Is this the character we're looking for?
                if (m_Data[size] != c)
                {
                    return size; // Character found at this location
                }
            } while (size--);
        }
        // No such character
        return NPOS;
    }

    /// Finds the last character not equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindLastNotOf(const ValueType * s, SizeType pos, SizeType count) const noexcept
    {
        // Is there a string to search into?
        if (SizeType size = m_Size; U3D_LIKELY(size))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters ranges
            do {
                // Look for the specified sub-string in this range
                if (!Locate(s, count, m_Data[size]))
                {
                    return size; // Sub-string found at this location
                }
            } while (size--);
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the last character not equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindLastNotOf(const ValueType * s, SizeType pos = NPOS) const noexcept
    {
        return FindLastNotOf(s, pos, Count(s));
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a string view.
    [[nodiscard]] constexpr bool Contains(StrView v) const noexcept
    {
        return Find(v) != NPOS;
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a single character.
    [[nodiscard]] constexpr bool Contains(ValueType c) const noexcept
    {
        return Find(c) != NPOS;
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a null-terminated character string.
    [[nodiscard]] constexpr bool Contains(const ValueType * s) const noexcept
    {
        return Find(s) != NPOS;
    }
protected:

    /// Compare two character sequences. Performs the comparison at compile time if possible or efficiently at run-time.
    [[nodiscard]] static U3D_INLINE constexpr int Compare(const ValueType * a, const ValueType * b, size_t n) noexcept
    {
        // Is there anything to be compared?
        if (U3D_UNLIKELY(n == 0))
        {
            return 0; // Strings are considered equal
        }
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        // Can the comparison be performed at compile time?
        if (__builtin_constant_p(n) && IsConstantCharArray(a, n) && IsConstantCharArray(b, n))
        {
    #endif
            // Perform a direct character by character comparison
            for (size_t i = 0; i < n; ++i)
            {
                if (a[i] < b[i])
                {
                    return -1;
                }
                else if (b[i] < a[i])
                {
                    return 1;
                }
            }
            // All characters compared equal so far
            return 0;
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in "run-time" comparison
        return memcmp(a, b, n);
    #endif
    }

    /// Count the length of a null-terminated character string.
    [[nodiscard]] static U3D_INLINE constexpr size_t Count(const char * s) noexcept
    {
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) || defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        // Can the operation be performed at compile time?
        if (IsConstantString(s))
        {
    #endif
            size_t i = 0;
            while (s[i])
            {
                ++i;
            }
            return i;
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) || defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in "run-time" comparison
        return strlen(s);
    #endif
    }

    /// Locate a character in a character array.
    [[nodiscard]] static U3D_INLINE constexpr const char * Locate(const char * s, size_t n, const char & c) noexcept
    {
        // Is there anything to search in?
        if (U3D_UNLIKELY(n == 0))
        {
            return nullptr; // Lookup aborted
        }
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        // Can the lookup be performed at compile time?
        if (__builtin_constant_p(n) && __builtin_constant_p(c) && IsConstantCharArray(s, n))
        {
    #endif
            // Perform a direct character by character lookup
            for (size_t i = 0; i < n; ++i)
            {
                if (s[i] == c)
                {
                    return s + i;
                }
            }
            // No such character
            return nullptr;
    #if defined(U3D_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(U3D_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in "run-time" comparison
        return static_cast< const char * >(memchr(s, c, n));
    #endif
    }
private:

    /// The start of the viewed string.
    ConstPointer    m_Data{nullptr};
    /// The length of the viewed string.
    SizeType        m_Size{0};
};

/// Equality comparison between two views.
U3D_INLINE constexpr bool operator == (const StrView & a, const StrView & b) noexcept { return a.Compare(b) == 0; }
/// Inequality comparison between two views.
U3D_INLINE constexpr bool operator != (const StrView & a, const StrView & b) noexcept { return a.Compare(b) != 0; }
/// Less than comparison between two views.
U3D_INLINE constexpr bool operator < (const StrView & a, const StrView & b) noexcept { return a.Compare(b) < 0; }
/// Greater than comparison between two views.
U3D_INLINE constexpr bool operator > (const StrView & a, const StrView & b) noexcept { return a.Compare(b) > 0; }
/// Less than or equal comparison between two views.
U3D_INLINE constexpr bool operator <= (const StrView & a, const StrView & b) noexcept { return a.Compare(b) <= 0; }
/// Greater than or equal comparison between two views.
U3D_INLINE constexpr bool operator >= (const StrView & a, const StrView & b) noexcept { return a.Compare(b) >= 0; }

namespace Literals {

/// Forms a string view of a character literal.
U3D_INLINE constexpr StrView operator "" _sv(const char * str, size_t len) noexcept
{
    return StrView{str, len};
}

} // Namespace:: Literals

/// Check whether the given string view contains absolutely no character (except whitespace).
[[nodiscard]] U3D_INLINE constexpr bool IsActuallyEmptyString(const StrView & s) noexcept
{
    for (const auto & c : s)
    {
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') return true;
    }
    return false;
}

}
