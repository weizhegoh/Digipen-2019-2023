#pragma once

#include <list>

namespace Xivi
{

  template <typename T,
            typename Allocator = std::allocator<T>>
  using list = std::list<T, Allocator>;
}