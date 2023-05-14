#pragma once

#include <vector>

namespace Xivi
{
  template <typename T,
            typename Allocator = std::allocator<T>>
  using vector = std::vector<T, Allocator>;
}