#pragma once

#include <forward_list>

namespace Xivi
{

  template <typename T,
            typename Allocator = std::allocator<T>>
  using forward_list = std::forward_list<T, Allocator>;

}