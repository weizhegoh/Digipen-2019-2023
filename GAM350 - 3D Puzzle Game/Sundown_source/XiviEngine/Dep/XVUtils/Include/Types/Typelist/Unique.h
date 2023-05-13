#pragma once

namespace Xivi::Types::Details
{

  template <typename CheckType, typename... Types>
  constexpr bool Has(Typelist<Types...>)
  {
    if constexpr (std::disjunction_v<std::is_same<CheckType, Types>...>)
      return true;
    else
      return false;
  }

  template <IsTypelist List>
  constexpr auto Unique(List list, Typelist<>)
  {
    return list;
  }

  template <IsTypelist List, typename Type, typename... Types>
  constexpr auto Unique(List list, Typelist<Type, Types...>)
  {
    if constexpr (Has<Type>(list))
      return Unique(list, Typelist<Types...>{});
    else
      return Unique(Concatenate(list, Typelist<Type>{}), Typelist<Types...>{});
  }

  template <typename... Types>
  constexpr auto Unique(Typelist<Types...>)
  {
    return Unique(Typelist<>{}, Typelist<Types...>{});
  }

  template <typename Type, typename... Types>
  constexpr auto Unique(Type, Types...)
  {
    return Unique(Typelist<>{}, Typelist<Types...>{});
  }

}