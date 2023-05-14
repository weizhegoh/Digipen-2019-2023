#pragma once

namespace Xivi::Types::Details
{
  template <typename... ListTypes, typename... AppendingTypes>
  constexpr auto Append(Typelist<ListTypes...>, AppendingTypes...)
  {
    return Typelist<ListTypes..., AppendingTypes...>{};
  }

  template <typename... ListTypes, typename... AppendingTypes>
  constexpr auto Append(ListTypes..., AppendingTypes...)
  {
    return Typelist<ListTypes..., AppendingTypes...>{};
  }
}