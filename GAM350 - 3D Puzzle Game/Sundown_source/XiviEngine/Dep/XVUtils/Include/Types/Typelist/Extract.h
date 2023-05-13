#pragma once

namespace Xivi::Types::Details
{
  template <size_t Curr, size_t Dest, typename Type, typename... Types>
  static constexpr auto Extract()
  {
    if constexpr (Curr == Dest)
      return Type{};
    else
      return Extract<Curr + 1, Dest, Types...>();
  }

  template <size_t Dest, typename... Types>
  static constexpr auto Extract()
  {
    static_assert(Dest < sizeof...(Types));
    return Extract<0, Dest, Types...>();
  }
}