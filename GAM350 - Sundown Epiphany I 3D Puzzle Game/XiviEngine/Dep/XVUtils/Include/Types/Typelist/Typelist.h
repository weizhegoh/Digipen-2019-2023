#pragma once

#include <type_traits>
#include "../Traits/Specialised.h"

namespace Xivi
{
  template <typename... Types>
  struct Typelist;


  template <typename List>
  concept IsTypelist = Traits::IsSpecialised<Typelist, List>();

  struct NullType
  {
  };

  template <typename T>
  concept IsNullType = std::is_same_v<NullType,T>;

  template <>
  struct Typelist<>
  {
    template <typename... AppendingTypes>
    using Append = Typelist<AppendingTypes...>;

    template <IsTypelist InputList>
    using Extend = InputList;

    template <template <typename> typename Predicate>
    using Find = NullType;

    template <template <typename> typename Predicate>
    using Remove = Typelist<>;

    static constexpr size_t Size = 0;
    static constexpr bool Empty = true;
    static constexpr int Index = -1;
  };

}

#include "Append.h"
#include "Concatenate.h"
#include "Find.h"
#include "Remove.h"
#include "Index.h"
#include "Extract.h"
#include "Unique.h"

namespace Xivi
{
  template <typename... Types>
  struct Typelist
  {
    template <typename... AppendingTypes>
    using Append = Typelist<Types..., AppendingTypes...>;

    template <IsTypelist InputList>
    using Extend = decltype(Xivi::Types::Details::Concatenate<Types...>(InputList{}));

    template <template <typename> typename Predicate>
    using Find = decltype(Xivi::Types::Details::Find<Predicate>(Types{}...));

    template <template <typename> typename Predicate>
    using Remove = decltype(Xivi::Types::Details::Remove<Predicate>(Types{}...));

    template <size_t Dest>
    using Extract = decltype(Xivi::Types::Details::Extract<Dest, Types...>());

    template <IsNullType T = NullType>
    using Unique = decltype(Xivi::Types::Details::Unique(T{}, Types{}...));

    static constexpr size_t Size = sizeof...(Types);
    static constexpr bool Empty = Size == 0 ? true : false;

    template <typename CheckType>
    static constexpr int Index = Xivi::Types::Details::Index<0, CheckType, Types...>();
  };



  template <template <typename> typename Predicate, typename... Types>
  using TypesRemove = decltype(Xivi::Types::Details::Remove<Predicate>(Typelist<>{}, Typelist<Types...>{}));

  template <IsTypelist List, typename... Types>
  using ListAppend = typename List::template Append<Types...>;

  template <IsTypelist List, IsTypelist InputList>
  using ListConcatenate = typename List::template Extend<InputList>;

  template <IsTypelist List, template <typename> typename Predicate>
  using ListFind = decltype(Xivi::Types::Details::Find<Predicate>(List{}));

  template <IsTypelist List, template <typename> typename Predicate>
  using ListRemove = decltype(Xivi::Types::Details::Remove<Predicate>(Typelist<>{}, List{}));
  template <IsTypelist List, size_t Dest>
  using ListExtract = List::template Extract<Dest>;

  template <IsTypelist List>
  using ListUnique = decltype(Xivi::Types::Details::Unique(List{}));

  template <IsTypelist List>
  static constexpr auto ListSize = List::Size;

  template <IsTypelist List>
  static constexpr auto ListEmpty = List::Empty;

  template <IsTypelist List, typename CheckType>
  static constexpr int ListIndex = List::template Index<CheckType>;

  template <typename T>
  struct IsNullTypePred : std::bool_constant < std::is_same_v<NullType, T> > {};
}