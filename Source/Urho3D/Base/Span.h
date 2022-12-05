//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"
#include "../Base/Iterator.h"

namespace Urho3D {

/// Constant of type \ref size_t that is used to differentiate \ref Span of static and dynamic extent.
inline constexpr size_t DynamicExtent = static_cast< size_t >(-1);

namespace Impl {

/// Helper structure with the most optimal layout necessary to store a fixed size span.
template < class T, size_t S > struct SpanStorage
{
    /// Whatever was given in template parameter \p S.
    static inline constexpr size_t SIZE = S;

    /// Pointer to the beginning of the sequence of elements.
    T * data_{nullptr};

    /// Default constructor. Initializes to an empty span.
    constexpr SpanStorage() noexcept = default;
    /// Copy constructor. Initializes to another span.
    constexpr SpanStorage(const SpanStorage & o) noexcept = default;
    /// Explicit constructor. Initializes to a specific span.
    constexpr SpanStorage(T * p, size_t n [[maybe_unused]]) noexcept
        : data_(p)
    { }

    /// Copy assignment operator. Assigns another span.
    constexpr SpanStorage & operator = (const SpanStorage & o) noexcept = default;

    /// Retrieve a pointer to the beginning of the sequence of elements.
    [[nodiscard]] constexpr T * Data() const noexcept { return data_; };
    /// Retrieve the number of elements in the sequence.
    [[nodiscard]] constexpr size_t Size() const noexcept { return SIZE; };

    /// Initializes to a specific span.
    constexpr void Set(T * p, size_t n [[maybe_unused]]) noexcept { data_ = p; }

    /// Retrieve a reference to the element at location specified by \p i.
    [[nodiscard]] constexpr T & At(size_t i) const noexcept
    {
        static_assert(SIZE != 0);
        UH_ASSERT(SIZE > 0)
        UH_ASSERT(i < SIZE)
        return data_[i];
    }
};

/// Specialization of \ref SpanStorage that can be used to store spans with dynamic size.
template < class T > struct SpanStorage< T, DynamicExtent >
{
    /// Pointer to the beginning of the sequence of elements.
    T * data_{nullptr};
    /// Number of elements in the sequence.
    size_t size_{0};

    /// Default constructor. Initializes to an empty span.
    constexpr SpanStorage() noexcept = default;
    /// Copy constructor. Initializes to another span.
    constexpr SpanStorage(const SpanStorage & o) noexcept = default;
    /// Explicit constructor. Initializes to a specific span.
    constexpr SpanStorage(T * p, size_t n) noexcept
        : data_(p), size_(n)
    { }

    /// Copy assignment operator. Assigns another span.
    constexpr SpanStorage & operator = (const SpanStorage & o) noexcept = default;

    /// Retrieve a pointer to the beginning of the sequence of elements.
    [[nodiscard]] constexpr T * Data() const noexcept { return data_; };
    /// Retrieve the number of elements in the sequence.
    [[nodiscard]] constexpr size_t Size() const noexcept { return size_; };

    /// Initializes to a specific span.
    constexpr void Set(T * p, size_t n) noexcept { data_ = p, size_ = n; }

    /// Retrieve a reference to the element at location specified by \p i.
    [[nodiscard]] constexpr T & At(size_t i) const noexcept
    {
        UH_ASSERT(size_ > 0)
        UH_ASSERT(i < size_)
        return data_[i];
    }
};

/// Used to check if elements of a contiguous container are convertible to a specific type.
template < class, class, class = void > struct IsContainerElementTypeCompatible : FalseType { };
/// Specialization of \ref IsContainerElementTypeCompatible which recognizes if elements of two arrays are compatible.
template < class T, class E > struct IsContainerElementTypeCompatible< T, E, Void_t< decltype(Data(declval< T >())) > >
    : IsConvertible< RemovePointer_t< decltype(Data(declval< T >())) > (*)[], E (*)[]> { };
/// Utility used to check if a container does not have a specialized \ref Span constructor but can still be used with.
/// For a container to be unspecialized it must not be a \ref Span or \ref Array type.
/// For a container to be valid, `Data(declval< C >())` and `Size(declval< C >())` must be well-formed expressions.
template < class C, class U = RemoveCV_t< RemoveReference_t< C > > > struct IsUnspecializedSpanContainer
    : public BoolConstant< (!IsSpan_v< U > && !IsArray_v< U > && UH_MSC_ONLY(!!)HasSizeAndData_v< C >) > { };

/// Utility used to select the proper extent for the span returned from \ref SubSpan methods.
template < size_t Extent, size_t Offset, size_t Count > [[nodiscard]] static constexpr size_t SubSpanExtent() noexcept
{
    if constexpr (Count != DynamicExtent)
    {
        return Count;
    }
    else if constexpr (Extent != DynamicExtent)
    {
        return (Extent - Offset);
    }
    else
    {
        return DynamicExtent;
    }
}

} // Namespace:: Impl

/// Class template %Span describes an object that can refer to a contiguous sequence of objects with the first element
/// of the sequence at position zero. A span can either have a static extent, in which case the number of elements
/// in the sequence is known and encoded in the type, or a dynamic extent.
template < class T, size_t E > struct Span
{
    static_assert(IsObject_v< T >, "A span's element type `T` must be an object type (not a reference type or void)");
    // TODO: figure out a way to implement this.
    //static_assert(IsComplete_v< T >, "A span's element type `T` must be a complete type (not a forward declaration)");
    static_assert(!IsAbstract_v< T >, "A span's element type `T` cannot be an abstract class type");

    /// Specialized type used internally to store the span.
    using StorageType = Impl::SpanStorage< T, E >;

    /// Whatever was given via template parameter \p T.
    using ElementType = T;
    /// Same as \ref ElementType but const or volatile qualifiers stripped away.
    using ValueType = RemoveCV_t< T >;
    /// Same as \ref ElementType but with a pointer qualifier.
    using Pointer = ElementType *;
    /// Same as \ref Pointer but with a const qualifier.
    using ConstPointer = const ElementType *;
    /// Same as \ref ElementType but with a reference qualifier.
    using Reference = ElementType &;
    /// Same as \ref Reference but with a const qualifier.
    using ConstReference = ElementType &;
    /// Element iterator (currently same as \ref Pointer).
    using Iterator = Pointer;
    /// Constant element iterator (currently same as \ref ConstPointer).
    using ConstIterator = ConstPointer;
    /// Unsigned integer type (usually an alias of \ref size_t).
    using SizeType = size_t;
    /// Signed integer type (usually an alias of \ref ptrdiff_t).
    using DifferenceType = ptrdiff_t;

    ///  Whatever was given via template parameter \p E.
    static inline constexpr SizeType Extent = E;

    /// Utility used in SFINAE checks to make sure this span has a dynamic extent.
    using IsDynamic = BoolConstant< (E == DynamicExtent) >;
    /// Utility used in SFINAE checks to make sure this span has a specific extent or is dynamic.
    template < size_t Ex > using IsExtentOrDynamic = BoolConstant< (Extent == Ex || Extent == DynamicExtent) >;

    /// Default constructor. Initializes to an empty span.
    /// After construction, \ref Data() is equal to nullptr, and \ref Size() is equal to 0.
    template < size_t Ex = Extent, EnableIf_t<
        ((Ex + 1u) <= 1u), int
    > = 0 > constexpr Span() noexcept
        : storage_()
    {
    }

    /// Construct to a specific view. The behavior is undefined if [\p data, \p data + \p count) is not a valid range.
    /// After construction, the span is a view over the range [\p data, \p data + \p count).
    constexpr Span(Pointer data, SizeType size) noexcept
        : storage_(data, size)
    {
        if constexpr (Extent != DynamicExtent)
        {
            UH_ASSERT(size == Extent)
        }
    }

    /// Construct to a specific view. The behavior is undefined if [\p first, \p last) is not a valid range.
    /// After construction, the span is a view over the range [\p first, \p first + (\p last - \p first)).
    constexpr Span(Pointer first, Pointer last) noexcept
        : storage_(first, last - first)
    {
        if constexpr (Extent != DynamicExtent)
        {
            UH_ASSERT(Distance(first, last) == static_cast< DifferenceType >(Extent))
        }
    }

    /// Constructs a span that is a view over the array \p arr.
    /// After construction, \ref Data() is equal to \ref Data(arr), and \ref Size() is equal to \p N.
    template < size_t N, EnableIf_t<
        IsExtentOrDynamic< N >::value && IsConvertible< ValueType(*)[], ElementType(*)[] >::value
    , int > = 0 > constexpr Span(ElementType (&arr)[N]) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(arr), static_cast< SizeType >(N))
    { }

    /// Constructs a span that is a view over the array \p arr.
    /// After construction, \ref Data() is equal to \ref Data(arr), and \ref Size() is equal to \p N.
    template < size_t N, EnableIf_t<
        IsExtentOrDynamic< N >::value && IsConvertible< ValueType(*)[], ElementType(*)[] >::value
    , int > = 0 > constexpr Span(Array< ValueType, N > & arr) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(arr.Data()), static_cast< SizeType >(N))
    { }

    /// Constructs a span that is a view over the array \p arr.
    /// After construction, \ref Data() is equal to \ref Data(arr), and \ref Size() is equal to \p N.
    template < size_t N, EnableIf_t<
        IsExtentOrDynamic< N >::value && IsConvertible< ValueType(*)[], ElementType(*)[] >::value
    , int > = 0 > constexpr Span(const Array< ValueType, N > & arr) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(arr.Data()), static_cast< SizeType >(N))
    { }

    /// Constructs a span that is a view over the container \p c.
    /// After construction, \ref Data() is equal to \ref Data(c), and \ref Size() is equal to \ref Size(c).
    template < class C, EnableIf_t<
        IsDynamic::value &&
        Impl::IsUnspecializedSpanContainer< C >::value &&
        Impl::IsContainerElementTypeCompatible< C &, ElementType >::value
    , int > = 0 > constexpr Span(C & c) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(Data(c)), static_cast< SizeType >(Size(c)))
    { }

    /// Constructs a span that is a view over the container \p c.
    /// After construction, \ref Data() is equal to \ref Data(c), and \ref Size() is equal to \ref Size(c).
    template < class C, EnableIf_t<
        IsDynamic::value &&
        Impl::IsUnspecializedSpanContainer< C >::value &&
        Impl::IsContainerElementTypeCompatible< const C &, ElementType >::value
    , int > = 0 > constexpr Span(const C & c) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(Data(c)), static_cast< SizeType >(Size(c)))
    { }

    /// Construct a span from another span \p o.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    constexpr Span(const Span & o) noexcept = default;

    /// Construct a span from another span \p o.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    constexpr Span(Span && o) noexcept = default;

    /// Construct a span from another span \p o.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    template < class OT, size_t OE, EnableIf_t<
        IsExtentOrDynamic< OE >::value && IsConvertible_v< OT (*)[], ElementType (*)[] >
    , int > = 0 > constexpr Span(Span< OT, OE > & o) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(o.Data()), o.Size())
    {
        if constexpr (Extent != DynamicExtent)
        {
            UH_ASSERT(o.Size() == Extent)
        }
    }

    /// Construct a span from another span \p o.
    /// After construction, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    template < class OT, size_t OE, EnableIf_t<
        IsExtentOrDynamic< OE >::value && IsConvertible_v< OT (*)[], ElementType (*)[] >
    , int > = 0 > constexpr Span(const Span< OT, OE > & o) noexcept // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : storage_(static_cast< Pointer >(o.Data()), o.Size())
    {
        if constexpr (Extent != DynamicExtent)
        {
            UH_ASSERT(o.Size() == Extent)
        }
    }

    /// Destructor.
    ~Span() noexcept = default;

    /// Assign a span that is a copy of another span \p o.
    /// After assignment, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    constexpr Span & operator = (const Span & o) noexcept = default;

    /// Assign a span that is a copy of another span \p o.
    /// After assignment, \ref Data() is equal to `o.Data()`, and \ref Size() is equal t `o.Size()`.
    template < class OT, size_t OE > constexpr auto operator = (const Span< OT, OE > & o) noexcept // NOLINT(misc-unconventional-assign-operator)
        -> EnableIf_t< IsExtentOrDynamic< OE >::value && IsConvertible_v< OT (*)[], ElementType (*)[] >, Span< T, E > > &
    {
        if constexpr (Extent != DynamicExtent)
        {
            UH_ASSERT(o.Size() == Extent)
        }
        storage_.Set(static_cast< Pointer >(o.Data()), o.Size());
        return *this;
    }

    /// Retrieve a reference to the element at location specified by \p i.
    [[nodiscard]] constexpr Reference At(SizeType i) const noexcept { storage_.At(i); }
    /// Retrieve a reference to the element at location specified by \p i.
    [[nodiscard]] constexpr Reference  operator [] (SizeType i) const noexcept { return storage_.At(i); }

    /// Retrieve a iterator to the first element in the span.
    [[nodiscard]] constexpr Iterator Begin() const noexcept { return ConstIterator(Data()); }
    /// Retrieve a iterator to the element following the last element in the span.
    [[nodiscard]] constexpr Iterator End() const noexcept { return ConstIterator(Data() + Size()); }

    /// Retrieve a iterator to the first element in the span. Alias of \ref Begin().
    [[nodiscard]] constexpr Iterator begin() const noexcept { return Begin(); }
    /// Retrieve a iterator to the element following the last element in the span. Alias of \ref End().
    [[nodiscard]] constexpr Iterator end() const noexcept { return End(); }

    /// Retrieve a iterator to a specific element in the span.
    [[nodiscard]] constexpr Iterator Iat(SizeType i) const noexcept { UH_ASSERT(i <= Size()) return ConstIterator(Data() + i); }

    /// Retrieve a reference to the first element in the span.
    [[nodiscard]] constexpr Reference  Front() const noexcept
    {
        static_assert(Extent != 0);
        UH_ASSERT(!Empty())
        return Data()[0];
    }

    /// Retrieve a reference to the last element in the span.
    [[nodiscard]] constexpr Reference Back() const noexcept
    {
        static_assert(Extent != 0);
        UH_ASSERT(!Empty())
        return Data()[Size() - 1];
    }

    /// Retrieve a pointer to the beginning of the sequence.
    [[nodiscard]] constexpr Pointer Data() const noexcept { return storage_.Data(); }

    /// Retrieve a pointer to the sequence of elements, starting at location specified by \p i.
    [[nodiscard]] constexpr Pointer DataFrom(SizeType i) const noexcept { UH_ASSERT(Size() >= i) return Data() + i; }

    /// Retrieve the number of elements in the span.
    [[nodiscard]] constexpr SizeType Size() const noexcept { return storage_.Size(); }
    /// Retrieve the number of elements in the span. Alias of \ref Size().
    [[nodiscard]] constexpr SizeType Length() const noexcept { return storage_.Size(); }

    /// Retrieve the size of the sequence in bytes.
    [[nodiscard]] constexpr SizeType SizeBytes() const noexcept { return Size() * sizeof(ElementType); }

    /// Check if the span is empty.
    [[nodiscard]] constexpr bool Empty() const noexcept { return Size() == 0; }

    /// Obtains a span that is a view over the first \p Count elements of this span.
    /// The program is ill-formed if \p Count > \ref Extent. The behavior is undefined if \p Count > \ref Size().
    template < size_t Count > [[nodiscard]] constexpr Span< ElementType, Count > First() const noexcept
    {
        if constexpr (Extent == DynamicExtent)
        {
            UH_ASSERT(Count <= Size())
        }
        else
        {
            static_assert(Count <= Extent);
        }
        return Span< ElementType, Count >{Data(), Count};
    }

    /// Obtains a span that is a view over the first \p count elements of this span.
    /// The program is ill-formed if \p count > \ref Extent. The behavior is undefined if \p count > \ref Size().
    [[nodiscard]] constexpr Span< ElementType, DynamicExtent > First(SizeType count) const noexcept
    {
        UH_ASSERT(count <= Size())
        return {Data(), count};
    }

    /// Obtains a span that is a view over the last \p Count elements of this span.
    /// The program is ill-formed if \p Count > \ref Extent. The behavior is undefined if \p Count > \ref Size().
    template < size_t Count > [[nodiscard]] constexpr Span< ElementType, Count > Last() const noexcept
    {
        if constexpr (Extent == DynamicExtent)
        {
            UH_ASSERT(Count <= Size())
        }
        else
        {
            static_assert(Count <= Extent);
        }
        return Span< ElementType, Count >{Data() + (Size() - Count), Count};
    }

    /// Obtains a span that is a view over the last \p count elements of this span.
    /// The program is ill-formed if \p count > \ref Extent. The behavior is undefined if \p count > \ref Size().
    [[nodiscard]] constexpr Span< ElementType, DynamicExtent > Last(SizeType count) const noexcept
    {
        UH_ASSERT(count <= Size())
        return {Data() + (Size() - count), count};
    }

    /// Obtains a span that is a view over the \p Count elements of this span starting at offset \p Offset.
    /// If Count is DynamicExtent, the number of elements in the sub-span is \ref Size() - \p Offset.
    template < size_t Offset, size_t Count = DynamicExtent > [[nodiscard]] constexpr auto SubSpan() const noexcept
        -> Span< ElementType, Impl::SubSpanExtent< Extent, Offset, Count >() >
    {
        if constexpr (Extent == DynamicExtent)
        {
            UH_ASSERT(Offset <= Size())
        }
        else
        {
            static_assert(Offset <= Extent);
        }

        using S = Span< ElementType, Impl::SubSpanExtent< Extent, Offset, Count >()>;

        if constexpr (Count == DynamicExtent)
        {
            return S{Data() + Offset, Size() - Offset};
        }
        else
        {
            if constexpr (Extent == DynamicExtent)
            {
                UH_ASSERT(Count <= Size())
                UH_ASSERT(Count <= (Size() - Offset))
            }
            else
            {
                static_assert(Count <= Extent);
                static_assert(Count <= (Extent - Offset));
            }
            return S{Data() + Offset, Count};
        }
    }

    /// Obtains a span that is a view over the \p count elements of this span starting at offset \p offset.
    /// If count is DynamicExtent, the number of elements in the sub-span is \ref Size() - \p offset.
    [[nodiscard]] constexpr Span< ElementType, DynamicExtent > SubSpan(SizeType offset, SizeType count = DynamicExtent) const noexcept
    {
        UH_ASSERT(offset <= Size())
        if (count == DynamicExtent)
        {
            count = Size() - offset;
        }
        else
        {
            UH_ASSERT(count <= Size())
            UH_ASSERT(offset + count <= Size())
        }
        return {Data() + offset, count};
    }

private:

    /// Specialized internal span storage.
    StorageType storage_{};
};

/// Deduction guide for fixed size C arrays.
template < class T, size_t N > Span(T (&)[N]) -> Span< T, N >;
/// Deduction guide for fixed size array wrappers.
template < class T, size_t N > Span(Array< T, N > &) -> Span< T, N >;
/// Deduction guide for constant fixed size array wrappers.
template < class T, size_t N > Span(const Span< T, N > &) -> Span< const T, N >;
/// Deduction guide for iterator range.
template < class T > Span(T *, T *) -> Span< T, DynamicExtent >;
/// Deduction guide for constant iterator range.
template < class T > Span(const T *, const T *) -> Span< const T, DynamicExtent >;
/// Deduction guide for special containers.
template < class C > Span(C &) -> Span< typename C::ValueType, DynamicExtent >;
/// Deduction guide for constant special containers.
template < class C > Span(const C &) -> Span< const typename C::ValueType, DynamicExtent >;

/// Obtains a view to the object representation of the elements of the span \p s.
/// If N is \ref DynamicExtent, the extent of the returned span is also \ref DynamicExtent; otherwise it is sizeof(\p T) * \p N.
template < class T, size_t E >
[[nodiscard]] inline Span< const uint8_t, (E == DynamicExtent ? DynamicExtent : E * sizeof(T)) > AsBytes(Span< T, E > s) noexcept
{
    auto data = reinterpret_cast< const uint8_t * >(s.Data());
    auto size = s.SizeBytes();
    constexpr auto Extent = E == DynamicExtent ? DynamicExtent : E * sizeof(T);
    return Span< const uint8_t, Extent >{data, size};
}

/// Obtains a view to the object representation of the elements of the span \p s.
/// If N is \ref DynamicExtent, the extent of the returned span is also \ref DynamicExtent; otherwise it is sizeof(\p T) * \p N.
template < class T, size_t E >
[[nodiscard]] inline Span< uint8_t, (E == DynamicExtent ? DynamicExtent : E * sizeof(T)) > AsWritableBytes(Span< T, E > s) noexcept
{
    auto data = reinterpret_cast< uint8_t * >(s.Data());
    auto size = s.SizeBytes();
    constexpr auto Extent = E == DynamicExtent ? DynamicExtent : E * sizeof(T);
    return Span< uint8_t, Extent >{data, size};
}

} // Namespace:: Urho3D
