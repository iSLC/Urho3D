//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include <type_traits>

namespace Urho3D {


/*
 * Forward declarations.
*/
struct StrView;
template < class, size_t > struct Span;
template < class, size_t > struct Array;

/*
 * Forward type-traits declarations.
*/
template < class > struct IsSpan;
template < class > struct IsArray;
template < class > struct IsStrView;
template < class, class = void > struct HasSizeAndData;

/// Helper of \ref IsSpan used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsSpan_v = IsSpan< T >::value;
/// Helper of \ref IsArray used to avoid having to explicitly access the `value` member.
template < class T > inline constexpr auto IsArray_v = IsArray< T >::value;
/// Helper of \ref IsStrView used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto IsStrView_v = IsStrView< T >::value;

/// Helper of \ref HasSizeAndData used to avoid having to explicitly accessing the `value` member.
template < class T > inline constexpr auto HasSizeAndData_v = HasSizeAndData< T >::value;

/// Check whether `T` is a \ref Span type.
template < class > struct IsSpan : std::false_type { };
/// Partial specialization for the case when `T` is a \ref Span type.
template < class T, size_t S > struct IsSpan< Span< T, S > > : std::true_type { };

/// Check whether `T` is an Array container.
template < class T > struct IsArray : public std::false_type { };
/// Partial specialization for the case when `T` is an Array container.
template < class T, size_t N > struct IsArray< Array< T, N > > : public std::true_type { };

/// Check whether `T` is a \ref StrView type.
template < class > struct IsStrView : std::false_type { };
/// Partial specialization for the case when `T` is a \ref StrView type.
template < > struct IsStrView< StrView > : std::true_type { };

namespace Concept {
    /// Constrains \p T to be a \p Span type.
    template < class T > concept DataView = IsSpan_v< T >;
    /// Constrains \p T to be a \p StrView type.
    template < class T > concept CharView = IsStrView_v< T >;
    /// Constrains \p T to be a \p Array type.
    template < class T > concept ArrayContainer = IsArray_v< T >;
    /// Constrains \p T to implement Data(c) and Size(c) functionality. 
    template < class T > concept DataAndSize = requires(T c) {
        { Data(c) } -> std::contiguous_iterator;
        { Size(c) } -> std::integral;
    };
} // Namespace:: Concept


} // Namespace:: Urho3D
