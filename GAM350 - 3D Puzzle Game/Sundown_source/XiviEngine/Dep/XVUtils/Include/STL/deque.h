#pragma once

#include <deque>

namespace Xivi
{

  template <typename T,
            typename Allocator = std::allocator<T>>
  using deque = std::deque<T, Allocator>;

}