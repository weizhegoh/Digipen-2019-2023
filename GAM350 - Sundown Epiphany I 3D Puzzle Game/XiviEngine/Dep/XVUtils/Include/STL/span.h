#pragma once

#include <span>

namespace Xivi
{

  template <typename T,
            std::size_t Extent = std::dynamic_extent>
  using span = std::span<T, Extent>;

}