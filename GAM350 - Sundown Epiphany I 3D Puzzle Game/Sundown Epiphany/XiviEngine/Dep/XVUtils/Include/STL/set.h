#pragma once

#include <set>

namespace Xivi
{

  template <typename Key,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<Key>>
  using set = std::set<Key, Compare, Allocator>;

  template <typename Key,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<Key>>
  using multiset = std::multiset<Key, Compare, Allocator>;

}