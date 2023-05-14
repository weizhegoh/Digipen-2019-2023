#pragma once

#include "../Core.h"
#include "../Log.h"

#ifdef XV_DEBUG
  #define XV_ENABLE_ASSERTS
#endif

#ifdef XV_ENABLE_ASSERTS

  #define XV_ASSERT(expr)                                                 \
    if (!(expr)) {                                                        \
    XV_ERROR("Assertion failed at {}: Line - {}", __FILE__, __LINE__);    \
    XV_BREAK();                                                      \
    }

  #define XV_ASSERT_MSG(expr, ...)                                        \
    if (!(expr)) {                                                        \
    XV_ERROR("Assertion failed at {}: Line - {}", __FILE__, __LINE__);    \
    XV_ERROR(__VA_ARGS__);                                                \
    XV_BREAK();                                                           \
    }

  #define XV_CORE_ASSERT(expr)                                                 \
    if (!(expr)) {                                                             \
    XV_CORE_ERROR("Assertion failed at {}: Line - {}", __FILE__, __LINE__);    \
    XV_BREAK();                                                                \
    }

  #define XV_CORE_ASSERT_MSG(expr, ...)                                        \
    if (!(expr)) {                                                             \
    XV_CORE_ERROR("Assertion failed at {}: Line - {}", __FILE__, __LINE__);    \
    XV_CORE_ERROR(__VA_ARGS__);                                                \
    XV_BREAK();                                                                \
    }
#else
#define XV_ASSERT(...)
#define XV_ASSERT_MSG(...)
#define XV_CORE_ASSERT(...)
#define XV_CORE_ASSERT_MSG(...)
#endif