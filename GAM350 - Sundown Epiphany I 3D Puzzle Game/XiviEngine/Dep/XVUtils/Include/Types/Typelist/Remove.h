#pragma once

namespace Xivi::Types::Details
{
  template <template <typename> typename Predicate, IsTypelist List>
  constexpr auto Remove(List list, Typelist<>)
  {
    return list;
  }

  template <template <typename> typename Predicate, IsTypelist List, typename Type, typename... Types>
  constexpr auto Remove(List list, Typelist<Type, Types...>)
  {
    if constexpr (Predicate<Type>::value)
      return Remove<Predicate>(list, Typelist<Types...>{});
    else
      return Remove<Predicate>(Concatenate(list, Typelist<Type>{}), Typelist<Types...>{});
  }

  template <template <typename> typename Predicate, typename... Types>
  constexpr auto Remove(Types...)
  {
    return Remove<Predicate>(Typelist<>{}, Typelist<Types...>{});
  }

} // namespace Xivi::Types::Internal
