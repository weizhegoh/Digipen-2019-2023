#pragma once

#include <queue>

namespace Xivi
{

  template <typename T,
            typename Container = std::deque<T>>
  using queue = std::queue<T, Container>;

  template <typename T,
            typename Container = std::vector<T>,
            typename Compare = std::less<typename Container::value_type>>
  using priority_queue = std::priority_queue<T, Container, Compare>;

}