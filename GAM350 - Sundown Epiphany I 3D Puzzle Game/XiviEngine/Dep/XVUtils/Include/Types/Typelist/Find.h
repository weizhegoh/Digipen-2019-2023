#pragma once

namespace Xivi::Types::Details
{
  template <template <typename> typename Predicate, typename Type>
  constexpr auto Find(Typelist<Type>)
  {
    if constexpr (Predicate<Type>::value)
      return Type{};
    else
      return NullType{};
  }

  template <template <typename> typename Predicate, typename Type, typename... Types>
  constexpr auto Find(Typelist<Type, Types...>)
  {
    if constexpr (Predicate<Type>::value)
      return Type{};
    else
      return Find<Predicate>(Typelist<Types...>{});
  }

  template <template <typename> typename Predicate, typename... Types>
  constexpr auto Find(Types...)
  {
    return Find<Predicate>(Typelist<Types...>{});
  }
}
