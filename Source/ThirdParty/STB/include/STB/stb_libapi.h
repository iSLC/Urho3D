#ifndef STB_INCLUDE_STB_LIB_API_H
#define STB_INCLUDE_STB_LIB_API_H

// Urho3D: Avoid possible undefined reference errors in tools when building engine as a shared library.
#ifndef STB_URHO_API
   // #if defined(_WIN32)
   //    #if defined(STB_DLL_EXPORTS) // We are using this library
   //       #define STB_URHO_API __declspec(dllexport)
   //    #elif defined(STB_DLL_IMPORTS) // We are using this library
   //       #define STB_URHO_API __declspec(dllimport)
   //    #else
   //       #define STB_URHO_API
   //    #endif
   // #elif defined(__GNUC__)
   //    #define STB_URHO_API __attribute__((visibility("default")))
   // #else
   //    #define STB_URHO_API 
   // #endif
   #ifdef __cplusplus
      #define STB_URHO_API extern "C" 
   #else
      #define STB_URHO_API extern 
   #endif
#endif

#endif // STB_INCLUDE_STB_LIB_API_H
