#pragma once

#include <array>

namespace Xivi
{

  template <typename T,
            std::size_t N>
  using array = std::array<T, N>;

}
