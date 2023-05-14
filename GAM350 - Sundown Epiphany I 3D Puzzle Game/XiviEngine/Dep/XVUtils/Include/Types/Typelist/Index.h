#pragma once

namespace Xivi::Types::Details
{
  template <int N, typename CheckType, typename Type, typename... Types>
  constexpr int Index()
  {
    if constexpr (std::is_same_v<CheckType, Type>)
      return N;

    if constexpr (sizeof...(Types) > 0)
      return Index<N + 1, CheckType, Types...>();
    else
      return -1;
  }
}