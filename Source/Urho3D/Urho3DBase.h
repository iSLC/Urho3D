//
// Copyright (c) 2008-2025 the Urho3D project.
//

#pragma once

#include "Urho3DConfig.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Make sure C++23 standard is available. Superficial check as it doesn't take into account partial implementations.
 * Some compilers may cheat on standard support. They will be "blocked" later on in compiler detection.
*/

#ifdef __cplusplus
  #ifndef UH_SUPPORTS_CPP23
        #pragma message "Value of __cplusplus is: " UH_STRINGIZEWRAP(__cplusplus)
        #warning This compiler does not support or have enabled the C++23 standard.
  #endif
#endif

/// Causes abnormal program termination unless SIGABRT is being caught by a signal handler and the handler does not return.
void __attribute__((noreturn)) Urho3D_AbortProgram();

/// Custom assert handler fired by the UH_ASSERT... macros. It includes the source and location where it 
void Urho3D_AssertHandler(const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * msg, const UH_MSC_COMPAT_OR(wchar_t, UH_WINDOWS_UNICODE_OR(wchar_t, char)) * src, unsigned loc);

/*
 * Assertion handler. Allow the assert handler to be customized.
*/

#if !defined(UH_ASSERT_HANDLER)
    // Expects Urho3D_AssertHandler to be implemented and available.
    #define UH_ASSERT_HANDLER(MSG_, SRC_, LOC_) Urho3D_AssertHandler(MSG_, SRC_, LOC_)
#endif

/*
 * Assertion macros, if required and allowed.
*/

#if defined(UH_IS_DEBUG) && !defined(UH_NO_ASSERT)
    // MSVC has only _wassert. MinGW switches between _wassert and _assert when _UNICODE macro definition is present (or not).
    #ifdef UH_MSC
        // Invoke the assert handler only if the expression `e` yields a false value
        #define UH_ASSERT(e) (void)((!!(e)) || (UH_ASSERT_HANDLER(UH_CONCAT_(L, #e), UH_CONCAT_(L, __FILE__), __LINE__), 0));
        // Invoke the assert handler only if both the primary expression `e` and exclusion expression `ex` yield a false value (allows to include an exception case to the primary expression)
        #define UH_ASSERT_EX(e, ex) (void)((!!((e) || (ex))) || (UH_ASSERT_HANDLER(UH_CONCAT_(L, #e), UH_CONCAT_(L, __FILE__), __LINE__), 0));
        // Forcefully invoke the assert handler without deppending the expression to trigger it
        #define UH_ASSERT_NOW(e) UH_ASSERT_HANDLER(UH_CONCAT_(L, #e), UH_CONCAT_(L, __FILE__), __LINE__);
    #else
        // Invoke the assert handler only if the expression `e` yields a false value
        #define UH_ASSERT(e) (UH_LIKELY(!!(e)) ? void(0) : []() { UH_ASSERT_HANDLER(UH_WAPIS(#e), UH_WAPIS(__FILE__), __LINE__); }());
        // Invoke the assert handler only if both the primary expression `e` and exclusion expression `ex` yield a false value (allows to include an exception case to the primary expression)
        #define UH_ASSERT_EX(e, ex) (UH_LIKELY(!!((e) || (ex))) ? void(0) : []() { UH_ASSERT_HANDLER(UH_WAPIS(#e), UH_WAPIS(__FILE__), __LINE__); }());
        // Forcefully invoke the assert handler without deppending the expression to trigger it
        #define UH_ASSERT_NOW(e) UH_ASSERT_HANDLER(UH_WAPIS(#e), UH_WAPIS(__FILE__), __LINE__);
    #endif
    // Evaluate the specified expression `e`
    #define UH_ASSERT_ONLY(e) e
    // Does nothing
    #define UH_EXCEPT_ASSERT(e)
    // Indicates that assert is enabled
    #define UH_ASSERT_ENABLED 1
    // Create the variable `x` of type `t`
    #define UH_ASSERT_VAR_DEFINE(t, x) t x;
    // Create the variable `x` of type `t` and store the value from `v` into it
    #define UH_ASSERT_VAR_CREATE(t, x, v) t x [[maybe_unused]] = v;
    // Invoke the constructor of a variable `x` of type `t` with the specified arguments.
    #define UH_ASSERT_VAR_CONSTRUCT(t, x, ...) t x(##__VA_ARGS__) [[maybe_unused]];
    // Generate the value `v` then create and store it in the specified variable `x` of type `t`
    // Difference between this and UH_ASSERT_VAR_CREATE is that this one generates the value in non-debug mode.
    #define UH_ASSERT_VALUE(t, x, v) UH_ASSERT_VAR_CREATE(t, x, v)
    // Generate the value `v` and store it in the specified variable `x`
    #define UH_ASSERT_STORE(x, v) x = v;
    // Create an isolated scope for the following operations to not worry about naming conflicts
    // Generate the value `v` then create and store it in the specified variable `x` of type `t`
    // Finally, run the specified assert expression `e` and assert on the result
    #define UH_ASSERT_SCOPE(t, x, e, v) { UH_ASSERT_VAR_CREATE(t, x, v) UH_ASSERT(e) }
    // Provide the statement `s`. Closing semicolon added automatically.
    #define UH_ASSERT_STMT(s) s;
    // Assert on the specified expression `e`
    #define UH_ASSERT_OR(e, s) UH_ASSERT(e)
    // Assert on the specified expression `e`
    #define UH_ASSERT_OR_LEAVE(e, v) UH_ASSERT(e)
    // Assert on the specified expression `e`
    #define UH_ASSERT_OR_YIELD(e, v) UH_ASSERT(e)
    // Assert on the specified expression `e`
    #define UH_ASSERT_OR_ABORT(e) UH_ASSERT(e)
    // Generate the value `v` then create and store it in the specified variable `x` of type `t`
    // Run the specified assert expression `e` and assert on the result
    // If the expression `e` did not trigger an assert, return the value stored in `x` and returned
    #define UH_ASSERT_AND_RETURN(t, x, e, v) UH_ASSERT_VALUE(t, x, v) UH_ASSERT(e) return x;
#else
    // Does nothing
    #define UH_ASSERT(e)
    // Does nothing
    #define UH_ASSERT_EX(e, ex)
    // Does nothing
    #define UH_ASSERT_NOW(e)
    // Does nothing
    #define UH_ASSERT_ONLY(e)
    // Evaluate the specified expression `e`
    #define UH_EXCEPT_ASSERT(e) e
    // Indicates that assert is disabled
    #undef UH_ASSERT_ENABLED
    // Does nothing
    #define UH_ASSERT_VAR_DEFINE(t, x)
    // Does nothing
    #define UH_ASSERT_VAR_CREATE(t, x, v)
    // Does nothing
    #define UH_ASSERT_VAR_CONSTRUCT(t, x, ...)
    // Only generate the value `v` but do not bother storing it in anything
    #define UH_ASSERT_VALUE(t, x, v) v;
    // Only generate the value `v` but do not bother storing it in anything
    #define UH_ASSERT_STORE(x, v) v;
    // Only generate the value `v` but do not bother storing it in anything
    #define UH_ASSERT_SCOPE(t, x, e, v) v;
    // Don't provide the specified statement `s`
    #define UH_ASSERT_STMT(s)
    // Process the statement `s` if the expression `s` would've triggered an assertion
    #define UH_ASSERT_OR(e, s) if (UH_UNLIKELY(!(e))) s;
    // Return from the current function if the expression `s` would've triggered an assertion
    #define UH_ASSERT_OR_LEAVE(e) if (UH_UNLIKELY(!(e))) return;
    // Return a value from the current function if the expression `s` would've triggered an assertion
    #define UH_ASSERT_OR_YIELD(e, v) if (UH_UNLIKELY(!(e))) return v;
    // Abort the program if the expression `s` would've triggered an assertion
    #define UH_ASSERT_OR_ABORT(e) if (UH_UNLIKELY(!(e))) AbortProgram();
    // Simply return a value from the current function.
    #define UH_ASSERT_AND_RETURN(t, x, e, v) return v;
#endif

/*
 * Unreachable code. Mark code paths that are known to not be reached.
*/

// GCC 4.8+, Clang, Intel and other compilers compatible with GCC
#if defined(UH_GNUC) || defined(UH_CLANG)
    inline __attribute__((always_inline, noreturn)) void Urho3D_Unreachable() { __builtin_unreachable(); }
    #define UH_UNREACHABLE Urho3D_Unreachable();
// MSVC
#elif defined(UH_MSC)
    __forceinline __declspec(noreturn) void Urho3D_Unreachable() { __assume(false); }
    #define UH_UNREACHABLE Urho3D_Unreachable();
// Failsafe
#else
    #define UH_UNREACHABLE UH_ASSERT(0) // TODO: do a better job that works in release mode as well
#endif

/*
 * Case-insensitive string comparison. This may differ in name depending on the platform.
*/

#ifdef UH_WINDOWS
   #define UH_STRICMP(a,b) stricmp(a,b)
   #define UH_STRNICMP(a,b,n) strnicmp(a,b,n)
#else
   #define UH_STRICMP(a,b) strcasecmp(a,b)
   #define UH_STRNICMP(a,b,n) strncasecmp(a,b,n)
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
