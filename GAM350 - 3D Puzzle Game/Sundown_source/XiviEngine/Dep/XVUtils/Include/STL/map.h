#pragma once

#include <map>

namespace Xivi
{

  template <typename Key,
            typename T,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<std::pair<const Key, T>>>
  using map = std::map<Key, T, Compare, Allocator>;

  template <typename Key,
            typename T,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<std::pair<const Key, T>>>
  using multimap = std::multimap<Key, T, Compare, Allocator>;

}