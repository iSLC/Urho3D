//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Foundation.h"

namespace Urho3D {

struct String;

/// Attempt to determine whether the characters of a NULL-terminated string are known at compile time.
/// This only works on compilers that provide the means necessary to identify such thing. MSVC is not one of them atm.
template < class T > [[nodiscard]] UH_INLINE constexpr bool IsConstantString(const T * s [[maybe_unused]]) noexcept
{
#if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
    // In constexpr contexts all strings should be constant.
    return __builtin_is_constant_evaluated();
#elif defined(UH_HAVE_BUILTIN_CONSTANT_P)
    while (__builtin_constant_p(*s) && *s) s++;
    return __builtin_constant_p(*s);
#else
    return false; // Make no assumptions
#endif
}

/// Attempt to determine whether the characters of a character array are known at compile time.
/// This only works on compilers that provide the means necessary to identify such thing. MSVC is not one of them atm.
template < class T >
[[nodiscard]] UH_INLINE constexpr bool IsConstantCharArray(const T * a [[maybe_unused]], size_t n [[maybe_unused]]) noexcept
{
#if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
    // In constexpr contexts all character arrays should be constant.
    return __builtin_is_constant_evaluated();
#elif defined(UH_HAVE_BUILTIN_CONSTANT_P)
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
    using Iterator = const ValueType *;
    /// Constant string iterator.
    using ConstIterator = const ValueType *;

    /// Index outside the range of the largest string possible.
    static constexpr inline SizeType NPOS = ~SizeType(0);

    /// Default constructor. Initializes to an empty view.
    /// After construction, \ref Data() is equal to nullptr, and \ref Size() is equal to 0.
    constexpr StrView() noexcept = default;

    /// Null pointer constructor. Initializes to an empty view.
    /// After construction, \ref Data() is equal to nullptr, and \ref Size() is equal to 0.
    constexpr StrView([[maybe_unused]] std::nullptr_t x)
        : StrView()
    { }

    /// C string constructor. Initializes the view to a null terminated array of characters.
    /// After construction, \ref Data() is equal to \p str, and \ref Size() is equal to position of the null character.
    constexpr StrView(ConstPointer str) noexcept // NOLINT(google-explicit-constructor)
        : data_(str), size_(Count(str))
    { }

    /// Explicit constructor. Initializes to a specific view. Behavior is undefined if [ \p ptr, \p ptr + \p len ) is not a valid range.
    /// After construction, \ref Data() is equal to \p ptr, and \ref Size() is equal to \p len.
    constexpr StrView(ConstPointer ptr, SizeType len) noexcept
        : data_(ptr), size_(len)
    { UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0)) }

    /// Iterator constructor. Initializes to a specific view. Behavior is undefined if \p r is not a valid range.
    /// After construction, \ref Data() is equal to \p p.begin, and \ref Size() is equal to size of the range.
    constexpr explicit StrView(const Range_t< ValueType * > & r) noexcept
        : data_(r.begin), size_(r.Size())
    { }

    /// Iterator constructor. Initializes to a specific view. Behavior is undefined if \p r is not a valid range.
    /// After construction, \ref Data() is equal to \p p.begin, and \ref Size() is equal to size of the range.
    constexpr explicit StrView(const Range_t< const ValueType * > & r) noexcept
        : data_(r.begin), size_(r.Size())
    { }

    /// String constructor. Initializes from a dynamic string.
    /// After construction, \ref Data() is equal to `s.Data()`, and \ref Size() is equal to `s.Size()`.
    constexpr explicit StrView(const String & s) noexcept;

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

    /// Retrieve a constant reference to the character at location specified by \p idx.
    [[nodiscard]] constexpr ConstReference At(SizeType idx) const noexcept
    {
        UH_ASSERT(idx < size_)
        UH_ASSERT(size_ > 0)
        return data_[idx];
    }

    /// Retrieve a constant reference to the character at location specified by \p idx.
    /// Has the same requirements (and assumptions) as the \ref At() member function.
    [[nodiscard]] constexpr ConstReference  operator [] (SizeType idx) const noexcept { return At(idx); }

    /// Retrieve a constant iterator to the first character in the view.
    [[nodiscard]] constexpr ConstIterator Begin() const noexcept { return ConstIterator(data_); }
    /// Retrieve a constant iterator to the character following the last character in the view.
    [[nodiscard]] constexpr ConstIterator End() const noexcept { return ConstIterator(data_ + size_); }
    /// Retrieve a constant iterator to the last character of the string.
    [[nodiscard]] constexpr ConstIterator Last() const noexcept { UH_ASSERT(size_ > 0) return ConstIterator(data_ + size_ - 1); }

    /// Retrieve a constant iterator to the first character in the view. Alias of \ref Begin().
    [[nodiscard]] constexpr ConstIterator begin() const noexcept { return Begin(); }
    /// Retrieve a constant iterator to the character following the last character in the view. Alias of \ref End().
    [[nodiscard]] constexpr ConstIterator end() const noexcept { return End(); }

    /// Retrieve a constant iterator to a specific character in the view.
    [[nodiscard]] constexpr ConstIterator Iat(SizeType idx) const noexcept { UH_ASSERT(idx <= size_) return ConstIterator(data_ + idx); }

    /// Retrieve a constant reference to the first character in the view.
    [[nodiscard]] constexpr ConstReference Front() const noexcept { UH_ASSERT(size_ > 0) return data_[0]; }

    /// Retrieve a constant reference to the last character in the view.
    [[nodiscard]] constexpr ConstReference Back() const noexcept { UH_ASSERT(size_ > 0) return data_[size_ - 1]; }

    /// Retrieve a constant pointer to the viewed array of characters.
    [[nodiscard]] constexpr ConstPointer Data() const noexcept { return data_; }

    /// Retrieve a constant pointer to the viewed array of characters starting at location specified by \p i.
    [[nodiscard]] constexpr ConstPointer DataFrom(SizeType idx) const noexcept { UH_ASSERT(size_ >= idx) return data_ + idx; }

    /// Retrieve the number of characters that the view is currently looking at.
    template < class T = SizeType > [[nodiscard]] constexpr T Size() const noexcept { return static_cast< T >(size_); }

    /// Retrieve the number of characters that the view is currently looking at. Alias of \ref Size().
    template < class T = SizeType > [[nodiscard]] constexpr T Length() const noexcept { return static_cast< T >(size_); }

    /// Retrieve the maximum number of characters that can be viewed at once on current platform and implementation.
    [[nodiscard]] static constexpr SizeType MaxSize() noexcept { return NPOS; }

    /// Check if the view has is currently looking at any characters.
    [[nodiscard]] constexpr bool Empty() const noexcept { return size_ == 0; }

    /// Move the start of the view forward by \p num characters. The behavior is undefined if \p num > \ref Size().
    constexpr void RemovePrefix(SizeType num) noexcept
    {
        UH_ASSERT(num <= size_)
        data_ += num, size_ -= num;
    }
    /// Move the end of the view back by \p num characters. The behavior is undefined if \p num > \ref Size().
    constexpr void RemoveSuffix(SizeType num) noexcept
    {
        UH_ASSERT(num <= size_)
        size_ -= num;
    }

    /// Exchange the view with that of \p o.
    constexpr void Swap(StrView & o) noexcept
    {
        const SizeType s = size_;
        size_ = o.size_;
        o.size_ = s;
        const ConstPointer d = data_;
        data_ = o.data_;
        o.data_ = d;
    }

    /// Copy the sub-string [ \p pos, \p pos + `count` ) to the character string pointed to by \p dest.
    /// `count` is the smaller of \p len and \ref Size() - \p pos.
    constexpr SizeType Copy(ValueType * dest, SizeType len = NPOS, SizeType pos = 0) const noexcept
    {
        UH_ASSERT(pos <= size_)
        // Do we need to copy anything?
        if (len == 0)
        {
            return 0; // False alarm
        }
        // Number of characters that SHOULD be copied
        len = (size_ - pos) >= len ? len : size_ - pos;
        // Where the copying should start at
        ConstPointer source = data_ + pos;
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        // Can the operation be performed at compile time?
        if (__builtin_is_constant_evaluated())
        {
    #endif
            // Perform a direct character by character copy
            for (size_t i = 0; i < len; ++i)
            {
                dest[i] = source[i];
            }
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED)
        }
        else
        {
            // Fall back to a built-in "run-time" copy
            MemCpy(dest, source, len);
        }
    #endif
        // Return the number of copied characters
        return len;
    }

    /// Obtain a view of the sub-string [ \p pos, \p pos + `count` ).
    /// `count` is the smaller of \p len and \ref Size() - \p pos.
    [[nodiscard]] constexpr StrView SubStr(SizeType pos = 0, SizeType len = NPOS) const noexcept
    {
        UH_ASSERT(pos <= size_)
        return {data_ + pos, (size_ - pos) >= len ? len : (size_ - pos)};
    }

    /// Compare this view against another view \p val.
    [[nodiscard]] constexpr int Compare(StrView val) const noexcept
    {
        // Are they at least the same size?
        if (size_ == val.size_)
        {
            return Compare(data_, val.data_, size_); // Compare the viewed character sequence
        }
        // Use the size of the viewed character sequence to reach a conclusion
        else if (size_ < val.size_)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    /// Compare a portion of this view against another view \p val.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType len, StrView val) const noexcept
    {
        return SubStr(pos, len).Compare(val);
    }
    /// Compare a portion of this view against a portion of another view \p val.
    [[nodiscard]] constexpr int Compare(SizeType pos1, SizeType len1, StrView val, SizeType pos2, SizeType len2) const noexcept
    {
        return SubStr(pos1, len1).Compare(val.SubStr(pos2, len2));
    }
    /// Compare this view against a null-terminated character string.
    /// NOTE: This is inefficient at runtime (atm) as it does a strlen on the string before comparing it.
    [[nodiscard]] constexpr int Compare(const ValueType * str) const noexcept
    {
        return Compare(StrView(str));
    }
    /// Compare a portion of this view against a null-terminated character string.
    /// NOTE: This is inefficient at runtime (atm) as it does a strlen on the string before comparing it.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType len, const ValueType * str) const noexcept
    {
        return SubStr(pos, len).Compare(StrView(str));
    }
    /// Compare a portion of this view against a character array.
    [[nodiscard]] constexpr int Compare(SizeType pos, SizeType len1, const ValueType * ptr, SizeType len2) const noexcept
    {
        return SubStr(pos, len1).Compare(StrView(ptr, len2));
    }

    /// Check if the string view begins with the given prefix, where the prefix is another string view.
    [[nodiscard]] constexpr bool StartsWith(StrView val) const noexcept
    {
        return size_ >= val.size_ && Compare(0, val.size_, val) == 0;
    }
    /// Check if the string view begins with the given prefix, where the prefix is a single character.
    [[nodiscard]] constexpr bool StartsWith(ValueType chr) const noexcept
    {
        return size_ > 0 && data_[0] == chr;
    }
    /// Check if the string view begins with the given prefix, where the prefix is a null-terminated character string.
    [[nodiscard]] constexpr bool StartsWith(const ValueType * str) const noexcept
    {
        return StartsWith(StrView(str));
    }

    /// Check if the string view ends with the given suffix, where the suffix is another string view \p val.
    [[nodiscard]] constexpr bool EndsWith(StrView val) const noexcept
    {
        return size_ >= val.size_ && Compare(size_ - val.size_, NPOS, val) == 0;
    }
    /// Check if the string view ends with the given suffix, where the suffix is a single character \p chr.
    [[nodiscard]] constexpr bool EndsWith(ValueType chr) const noexcept
    {
        return size_ > 0 && data_[size_ - 1] == chr;
    }
    /// Check if the string view ends with the given suffix, where the suffix is a null-terminated character string \p str.
    [[nodiscard]] constexpr bool EndsWith(const ValueType * str) const noexcept
    {
        return EndsWith(StrView(str));
    }

    /// Find the first occurrence of another string view \p val in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(StrView val, SizeType pos = 0) const noexcept
    {
        return Find(val.data_, pos, val.size_);
    }

    /// Find the first occurrence of a single character \p chr in this view, starting at position \p pos.
    /// If the character is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(ValueType chr, SizeType pos = 0) const noexcept
    {
        // Is the starting point within range?
        if (pos <= size_)
        {
            // Keep looking while there are unchecked characters ranges
            if (auto p = Locate(data_ + pos, size_ - pos, chr); p != nullptr)
            {
                return static_cast< SizeType >(p - data_);
            }
        }
        // Unable to locate such character
        return NPOS;
    }

    /// Find the first occurrence of a character array \p ptr in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // If there's nothing to search for then there's no point in trying
        if (UH_UNLIKELY(len == 0))
        {
            // Is the starting point within range?
            if (UH_LIKELY(pos <= size_))
            {
                return pos; // That's where the nothing can be found
            }
        }
        // Can the sub-string even fit this view?
        else if (pos < size_ && len <= size_)
        {
            for (SizeType end = size_ - len; pos <= end; ++pos)
            {
                // If one character matches then see if the rest match as well
                if (data_[pos] == ptr[0] && Compare(data_ + pos + 1, ptr + 1, len - 1) == 0)
                {
                    return pos; // Sub-string found at this location
                }
            }
        }
        // Unable to locate such sub-string
        return NPOS;
    }

    /// Find the first occurrence of string \p str in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType Find(const ValueType * str, SizeType pos = 0) const noexcept
    {
        return Find(str, pos, Count(str));
    }

    /// Find the last occurrence of another string view \p val in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(StrView val, SizeType pos = NPOS) const noexcept
    {
        return RFind(val.data_, pos, val.size_);
    }

    /// Find the last occurrence of a single character \p chr in this view, starting at position \p pos.
    /// If the character is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(ValueType chr, SizeType pos = NPOS) const noexcept
    {
        // Is there a string to search into?
        if (SizeType size = size_; UH_LIKELY(size > 0))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters
            for (++size; size-- > 0;)
            {
                // Is this the character we're looking for?
                if (data_[size] == chr)
                {
                    return size; // Character found at this location
                }
            }
        }
        // No such character
        return NPOS;
    }

    /// Find the last occurrence of null-terminated character string \p ptr in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // Can the sub-string even fit this view?
        if (UH_LIKELY(len <= size_))
        {
            // Skip checking characters that wouldn't fit in the view anyway
            pos = Min((size_ - len), pos);
            // Continuously check for a matching string while going back one character at a time
            do {
                if (Compare(data_ + pos, ptr, len) == 0)
                {
                    return pos; // Sub-string found at this location
                }
            } while (pos-- > 0);
        }
        // No such sub-string
        return NPOS;
    }

    /// Find the last occurrence of string \p str in this view, starting at position \p pos.
    /// If the string is not found, \ref NPOS is returned.
    [[nodiscard]] constexpr SizeType RFind(const ValueType * str, SizeType pos = NPOS) const noexcept
    {
        return RFind(str, pos, Count(str));
    }

    /// Finds the first occurrence of any of the characters of \p val in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindFirstOf(StrView val, SizeType pos = 0) const noexcept
    {
        return FindFirstOf(val.data_, pos, val.size_);
    }

    /// Finds the first character equal to the given character.
    [[nodiscard]] constexpr SizeType FindFirstOf(ValueType chr, SizeType pos = 0) const noexcept
    {
        return Find(chr, pos);
    }

    /// Finds the first character equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindFirstOf(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // Anything to search for?
        if (UH_LIKELY(len))
        {
            // Keep looking while there are unchecked characters ranges
            for (; pos < size_; ++pos)
            {
                // Perform a reverse search on the needle string instead of the viewed string
                // Probability of needle being smaller than the viewed string should be higher
                if (Locate(ptr, len, data_[pos]) != nullptr)
                {
                    return pos; // Sub-string found at this location
                }
            }
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the first character equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindFirstOf(const ValueType * str, SizeType pos = 0) const noexcept
    {
        return FindFirstOf(str, pos, Count(str));
    }

    /// Finds the last occurrence of any of the characters of \p val in this view, ending at position \p pos.
    [[nodiscard]] constexpr SizeType FindLastOf(StrView val, SizeType pos = NPOS) const noexcept
    {
        return FindLastOf(val.data_, pos, val.size_);
    }

    /// Finds the last character equal to the given character.
    [[nodiscard]] constexpr SizeType FindLastOf(ValueType chr, SizeType pos = NPOS) const noexcept
    {
        return RFind(chr, pos);
    }

    /// Finds the last character equal to one of characters in the given character array.
    [[nodiscard]] constexpr SizeType FindLastOf(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // Is there a string to search into or even search for?
        if (SizeType size = size_; UH_LIKELY(size && len))
        {
            // Should we omit some characters?
            if (--size; size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters ranges
            do {
                // Perform a reverse search on the needle string instead of the viewed string
                // Probability of needle being smaller than the viewed string should be higher
                if (Locate(ptr, len, data_[size]) != nullptr)
                {
                    return size; // Sub-string found at this location
                }
            } while (size-- != 0);
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the last character equal to one of characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindLastOf(const ValueType * str, SizeType pos = NPOS) const noexcept
    {
        return FindLastOf(str, pos, Count(str));
    }

    /// Finds the first character not equal to any of the characters of \p val in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(StrView val, SizeType pos = 0) const noexcept
    {
        return FindFirstNotOf(val.data_, pos, val.size_);
    }

    /// Finds the first character not equal to the given character.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(ValueType chr, SizeType pos = 0) const noexcept
    {
        // Keep looking while there are unchecked characters
        for (; pos < size_; ++pos)
        {
            // Is this the character we're looking for?
            if (data_[pos] != chr)
            {
                return pos; // Character found at this location
            }
        }
        // No such character
        return NPOS;
    }

    /// Finds the first character not equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // Keep looking while there are unchecked characters ranges
        for (; pos < size_; ++pos)
        {
            // Look for the specified sub-string in this range
            if (Locate(ptr, len, data_[pos]) == nullptr)
            {
                return pos; // Sub-string found at this location
            }
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the first character not equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindFirstNotOf(const ValueType * str, SizeType pos = 0) const noexcept
    {
        return FindFirstNotOf(str, pos, Count(str));
    }

    /// Finds the last character not equal to any of the characters of \p val in this view, starting at position \p pos.
    [[nodiscard]] constexpr SizeType FindLastNotOf(StrView val, SizeType pos = NPOS) const noexcept
    {
        return FindLastNotOf(val.data_, pos, val.size_);
    }

    /// Finds the last character not equal to the given character.
    [[nodiscard]] constexpr SizeType FindLastNotOf(ValueType chr, SizeType pos = NPOS) const noexcept
    {
        // Is there a string to search into?
        if (SizeType size = size_; UH_LIKELY(size))
        {
            // Should we skip some characters?
            if (--size; size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters
            do {
                // Is this the character we're looking for?
                if (data_[size] != chr)
                {
                    return size; // Character found at this location
                }
            } while (size--);
        }
        // No such character
        return NPOS;
    }

    /// Finds the last character not equal to any of the characters in the given character array.
    [[nodiscard]] constexpr SizeType FindLastNotOf(const ValueType * ptr, SizeType pos, SizeType len) const noexcept
    {
        UH_ASSERT((ptr == nullptr && len == 0) || (ptr != nullptr && len >= 0))
        // Is there a string to search into?
        if (SizeType size = size_; UH_LIKELY(size > 0))
        {
            // Should we skip some characters?
            if (--size > pos)
            {
                size = pos;
            }
            // Keep looking while there are unchecked characters ranges
            do {
                // Look for the specified sub-string in this range
                if (!Locate(ptr, len, data_[size]))
                {
                    return size; // Sub-string found at this location
                }
            } while (size--);
        }
        // No such sub-string
        return NPOS;
    }

    /// Finds the last character not equal to any of the characters in the given null-terminated character string.
    [[nodiscard]] constexpr SizeType FindLastNotOf(const ValueType * str, SizeType pos = NPOS) const noexcept
    {
        return FindLastNotOf(str, pos, Count(str));
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a string view.
    [[nodiscard]] constexpr bool Contains(StrView val) const noexcept
    {
        return Find(val) != NPOS;
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a single character.
    [[nodiscard]] constexpr bool Contains(ValueType chr) const noexcept
    {
        return Find(chr) != NPOS;
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a character array.
    [[nodiscard]] constexpr bool Contains(const ValueType * ptr, SizeType len) const noexcept
    {
        return Find(ptr, 0, len) != NPOS;
    }

    /// Check if the string view contains the given sub-string, where the sub-string is a null-terminated character string.
    [[nodiscard]] constexpr bool Contains(const ValueType * str) const noexcept
    {
        return Find(str) != NPOS;
    }

    /// Trim white-space from the beginning of the string view.
    constexpr void TrimLeft() noexcept
    {
        // Make sure there's something to trim
        if (UH_LIKELY(size_ > 0))
        {
            // Find the first non-space character
            do {
                // Is this a space character or should we stop?
                if (*data_ == ' ' || *data_ == '\t')
                {
                    ++data_; // Trim this character
                    --size_; // Update view length
                }
                else
                {
                    break; // Stop trimming
                }
            } while (size_ > 0);
        }
    }

    /// Trim white-space from the end of the string view.
    constexpr void TrimRight() noexcept
    {
        // Make sure there's something to trim
        if (UH_LIKELY(size_ > 0))
        {
            auto itr = Last(), end = Begin() - 1;
            // Find the first non-space character
            while (itr != end)
            {
                // Is this a space character or should we stop?
                if (*itr == ' ' || *itr == '\t')
                {
                    --itr; // Trim this character
                }
                else
                {
                    break; // Stop trimming
                }
            }
            // Find how many characters were left after trimming
            size_ = static_cast< SizeType >(static_cast< intptr_t >(itr - end));
        }
    }

    /// Trim white-space from both sides of the string view.
    /// \note Right side is trimmed first to not alter the start of the string if the entire string view is white-space only.
    constexpr void Trim() noexcept { TrimRight(); TrimLeft(); }

    /// Trim white-space from the beginning. Returns view with the trimmed white-space characters.
    [[nodiscard]] constexpr StrView TrimmedLeft() const noexcept { StrView s(*this); s.TrimLeft(); return s; }

    /// Trim white-space from the end. Returns view with the trimmed white-space characters.
    [[nodiscard]] constexpr StrView TrimmedRight() const noexcept { StrView s(*this); s.TrimRight(); return s; }

    /// Trim white-space from the beginning and end. Returns view with the trimmed white-space characters.
    /// \note Right side is trimmed first to not alter the start of the string if the entire string view is white-space only.
    [[nodiscard]] constexpr StrView Trimmed() const noexcept { StrView s(*this); s.Trim(); return s; }

    /// Trim characters specified by \p chars from the beginning of the string view.
    constexpr void TrimLeft(StrView chars) noexcept
    {
        // Make sure there's something to trim
        if (UH_LIKELY(size_ > 0))
        {
            // Find the first non-space character
            do {
                // Is this a space character or should we stop?
                if (Locate(chars.data_, chars.size_, *data_) != nullptr)
                {
                    ++data_; // Trim this character
                    --size_; // Update view length
                }
                else
                {
                    break; // Stop trimming
                }
            } while (size_ > 0);
        }
    }

    /// Trim characters specified by \p chars from the end of the string view.
    constexpr void TrimRight(StrView chars) noexcept
    {
        // Make sure there's something to trim
        if (UH_LIKELY(size_ > 0))
        {
            auto itr = Last(), end = Begin() - 1;
            // Find the first non-space character
            while (itr != end)
            {
                // Is this a space character or should we stop?
                if (Locate(chars.data_, chars.size_, *itr) != nullptr)
                {
                    --itr; // Trim this character
                }
                else
                {
                    break; // Stop trimming
                }
            }
            // Find how many characters were left after trimming
            size_ = static_cast< SizeType >(static_cast< intptr_t >(itr - end));
        }
    }

    /// Trim characters specified by \p chars from the both sides of the string view.
    constexpr void Trim(StrView chars) noexcept { TrimRight(chars); TrimLeft(chars); }

    /// Trim characters specified by \p chars from the beginning. Returns a view with the specified characters trimmed from start.
    [[nodiscard]] constexpr StrView TrimmedLeft(StrView chars) const noexcept { StrView s(*this); s.TrimLeft(chars); return s; }

    /// Trim characters specified by \p chars from the end. Returns a view with the specified characters trimmed from back.
    [[nodiscard]] constexpr StrView TrimmedRight(StrView chars) const noexcept { StrView s(*this); s.TrimRight(chars); return s; }

    /// Trim characters specified by \p chars from the beginning and end. Returns a view with the specified characters trimmed from both ends.
    [[nodiscard]] constexpr StrView Trimmed(StrView chars) const noexcept { StrView s(*this); s.Trim(chars); return s; }

protected:

    /// Retrieve the length of the C string from \p str. The length of a C string is determined by the terminating null-character.
    /// NOTE: This variant does not check if the specified pointer is null before attempting to access it.
    [[nodiscard]] static UH_INLINE constexpr SizeType Count(ConstPointer str, IsPure_t) noexcept
    {
        UH_ASSERT(str != nullptr)
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) || defined(UH_HAVE_BUILTIN_CONSTANT_P)
        // Can the operation be performed at compile time?
        if (IsConstantString(str))
        {
    #endif
            size_t i = 0;
            // Search for the null character
            while (str[i])
            {
                ++i;
            }
            return i;
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) || defined(UH_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in run-time comparison
        return StrLen(str);
    #endif
    }

    /// Retrieve the length of the C string from \p str. The length of a C string is determined by the terminating null-character.
    /// NOTE: This variant does check if the specified pointer is null before attempting to access it.
    [[nodiscard]] static UH_INLINE constexpr SizeType Count(ConstPointer str) noexcept
    {
        return UH_UNLIKELY(str == nullptr) ? 0 : Count(str, IsPure);
    }

    /// Compare two character sequences. Performs the comparison at compile time if possible or efficiently at run-time.
    [[nodiscard]] static UH_INLINE constexpr int Compare(ConstPointer a, ConstPointer b, SizeType len) noexcept
    {
        UH_ASSERT((a == nullptr && len == 0) || (a != nullptr && len >= 0))
        UH_ASSERT((b == nullptr && len == 0) || (b != nullptr && len >= 0))
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(UH_HAVE_BUILTIN_CONSTANT_P)
        // Can the comparison be performed at compile time?
        if (__builtin_constant_p(len) && IsConstantCharArray(a, len) && IsConstantCharArray(b, len))
        {
    #endif
            // Is there anything to be compared?
            if (UH_UNLIKELY(len == 0))
            {
                return 0; // Strings are considered equal
            }
            // Perform a direct character by character comparison
            for (size_t i = 0; i < len; ++i)
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
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(UH_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in run-time comparison
        return MemCmp(a, b, len);
    #endif
    }

    /// Search within the first \p len characters of the character array pointed by \p str for the first occurrence of \p value and return a pointer to it.
    /// \note It behaves like `memchr` but can identify a constexpr context and perform the search at compile time or fall-back to `memchr` for run-time.
    [[nodiscard]] static UH_INLINE constexpr ConstPointer Locate(ConstPointer str, SizeType len, ConstReference value) noexcept
    {
        UH_ASSERT((str == nullptr && len == 0) || (str != nullptr && len >= 0))
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(UH_HAVE_BUILTIN_CONSTANT_P)
        // Can the lookup be performed at compile time?
        if (__builtin_constant_p(len) && __builtin_constant_p(value) && IsConstantCharArray(str, len))
        {
    #endif
            // Is there anything to search in?
            if (UH_UNLIKELY(len == 0))
            {
                return nullptr; // Leave
            }
            // Perform a direct character by character lookup
            for (size_t i = 0; i < len; ++i)
            {
                if (str[i] == value)
                {
                    return str + i;
                }
            }
            // No such character
            return nullptr;
    #if defined(UH_HAVE_BUILTIN_IS_CONSTANT_EVALUATED) && defined(UH_HAVE_BUILTIN_CONSTANT_P)
        }
        // Fall back to a built-in "run-time" comparison
        return static_cast< ConstPointer >(MemChr(str, value, len));
    #endif
    }

private:

    /// The start of the viewed string.
    ConstPointer data_{nullptr};
    /// The length of the viewed string.
    SizeType size_{0};
};

/// Equality comparison between two views.
UH_INLINE constexpr bool operator == (const StrView & a, const StrView & b) noexcept { return a.Compare(b) == 0; }
/// Inequality comparison between two views.
UH_INLINE constexpr bool operator != (const StrView & a, const StrView & b) noexcept { return a.Compare(b) != 0; }
/// Less than comparison between two views.
UH_INLINE constexpr bool operator < (const StrView & a, const StrView & b) noexcept { return a.Compare(b) < 0; }
/// Greater than comparison between two views.
UH_INLINE constexpr bool operator > (const StrView & a, const StrView & b) noexcept { return a.Compare(b) > 0; }
/// Less than or equal comparison between two views.
UH_INLINE constexpr bool operator <= (const StrView & a, const StrView & b) noexcept { return a.Compare(b) <= 0; }
/// Greater than or equal comparison between two views.
UH_INLINE constexpr bool operator >= (const StrView & a, const StrView & b) noexcept { return a.Compare(b) >= 0; }

namespace Literals {

/// Forms a string view of a character literal.
UH_INLINE constexpr StrView operator "" _sv(const char * str, size_t len) noexcept
{
    return StrView{str, len};
}

} // Namespace:: Literals

/// Check whether the given string view contains absolutely no character (except whitespace).
[[nodiscard]] UH_INLINE constexpr bool IsActuallyEmptyString(const StrView & s) noexcept
{
    for (const auto & c : s) // NOLINT(readability-use-anyofallof)
    {
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') return false; // Non-whitespace character
    }
    // All whitespace characters
    return true;
}

} // Namespace:: Urho3D
