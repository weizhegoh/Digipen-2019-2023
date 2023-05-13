#pragma once

namespace Xivi::Types::Details
{
  template <typename... ListTypes, typename... InputListTypes>
  constexpr auto Concatenate(Typelist<ListTypes...>, Typelist<InputListTypes...>)
  {
    return Typelist<ListTypes..., InputListTypes...>{};
  }

  template <typename... ListTypes, typename... InputListTypes>
  constexpr auto Concatenate(Typelist<InputListTypes...>)
  {
    return Typelist<ListTypes..., InputListTypes...>{};
  }
}