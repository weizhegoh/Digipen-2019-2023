#pragma once

#include <stack>

namespace Xivi
{

  template <typename T,
            typename Container = std::deque<T>>
  using stack = std::stack<T, Container>;

}