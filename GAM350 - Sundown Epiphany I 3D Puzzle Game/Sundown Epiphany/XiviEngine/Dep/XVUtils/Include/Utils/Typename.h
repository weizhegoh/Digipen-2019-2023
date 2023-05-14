#pragma once
#include <string_view>

namespace Xivi::Types::Details
{
  template <typename T>
  constexpr std::string_view Extractor() noexcept
  {
#if defined(_MSC_VER)
    constexpr auto prefix = std::string_view{"Extractor<struct "};
    constexpr auto suffix = std::string_view{">(void)"};
    constexpr auto function = std::string_view{__FUNCSIG__};
#elif defined(__GNUC__)
    constexpr auto prefix = std::string_view{"with T = "};
    constexpr auto suffix = std::string_view{";"};
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#else
#error Unsupported Compiler
#endif

    constexpr auto start = function.find(prefix) + prefix.size();
    constexpr auto end = function.rfind(suffix);

    static_assert(start < end);

    return std::string_view(function.substr(start, (end - start)));
  }

  template <typename T>
  struct Storage
  {
    static inline constexpr auto Value = Extractor<T>();
  };
}

namespace Xivi
{
  template <typename T>
  inline constexpr auto Typename() noexcept
  {
    return Types::Details::Storage<T>::Value;
  }

}