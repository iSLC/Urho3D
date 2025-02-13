//
// Copyright (c) 2008-2022 the Urho3D project.
//

#include "../Base/Foundation.h"

#include <cassert>

namespace Urho3D
{

} // Namespace:: Urho3D

[[noreturn]] inline void Urho3D_AbortProgram() noexcept
{
#if defined(UH_GNUC) || defined(UH_CLANG)
    __builtin_abort();
#elif defined(UH_MSC)
    abort();
#else
    static_assert(false);
#endif
}

// MSVC has only _wassert. MinGW switches between _wassert and _assert when _UNICODE macro definition is present (or not).
void Urho3D_AssertHandler([[maybe_unused]] const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * msg,
                   [[maybe_unused]] const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * src,
                   [[maybe_unused]] unsigned loc)
{
#ifdef UH_IS_DEBUG
    UH_MSC_COMPAT_OR(_wassert, UH_WINDOWS_UNICODE_OR(_wassert, _assert))(msg, src, loc);
#endif
}
