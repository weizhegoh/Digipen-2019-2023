#pragma once

#include <unordered_set>

namespace Xivi
{

  template <typename Key,
            typename Hash = std::hash<Key>,
            typename KeyEqual = std::equal_to<Key>,
            typename Allocator = std::allocator<Key>>
  using unordered_set = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

  template <typename Key,
            typename Hash = std::hash<Key>,
            typename KeyEqual = std::equal_to<Key>,
            typename Allocator = std::allocator<Key>>
  using unordered_multiset = std::unordered_multiset<Key, Hash, KeyEqual, Allocator>;

}