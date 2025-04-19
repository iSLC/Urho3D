#include "Urho3DBase.h"

#include <assert.h>

void Urho3D_AbortProgram()
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
void Urho3D_AssertHandler(UH_GNUC_ATTR(__attribute__((unused))) const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * msg,
                            UH_GNUC_ATTR(__attribute__((unused))) const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * src,
                            UH_GNUC_ATTR(__attribute__((unused))) unsigned loc)
{
#ifdef UH_IS_DEBUG
    UH_MSC_COMPAT_OR(_wassert, UH_WINDOWS_UNICODE_OR(_wassert, _assert))(msg, src, loc);
#elif !defined(UH_HAS_GNUC_ATTR)
    (void)msg;
    (void)src;
    (void)loc;
#endif
}
