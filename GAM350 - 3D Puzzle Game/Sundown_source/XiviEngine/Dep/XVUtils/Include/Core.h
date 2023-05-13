#pragma once

#ifdef XV_DEBUG
  #define XV_INLINE
  #define XV_FORCEINLINE
  #define XV_ENABLE_ASSERTS
#else
  #define XV_INLINE inline
  #define XV_FORCEINLINE __forceinline
#endif

#define XV_EXPAND(x) x

#define XV_CONCAT(x,y) x ## y
#define XV_STRING(x) #x
#define XV_STRINGIFY(x) XV_STRING(x)

#define BIT(x) (1ULL << x)

// Platform Specifics
#ifdef XV_PLATFORM_WINDOWS
  #ifdef XV_DEBUG
    #define XV_BREAK() __debugbreak()
    #include <cstdlib>
    #include <crtdbg.h>
    #define _CRTDBG_MAP_ALLOC
    #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
  #else
    #define XV_BREAK()
    #define DBG_NEW new
  #endif
  #define NOMINMAX
  #include <Windows.h>
#else
  #error "Not Supported!"
#endif