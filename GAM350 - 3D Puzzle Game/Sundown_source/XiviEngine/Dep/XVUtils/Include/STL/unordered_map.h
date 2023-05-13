#pragma once

#include <unordered_map>

namespace Xivi
{

  template <typename Key,
            typename T,
            typename Hash = std::hash<Key>,
            typename KeyEqual = std::equal_to<Key>,
            typename Allocator = std::allocator<std::pair<const Key, T>>>
  using unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

  template <typename Key,
            typename T,
            typename Hash = std::hash<Key>,
            typename KeyEqual = std::equal_to<Key>,
            typename Allocator = std::allocator<std::pair<const Key, T>>>
  using unordered_multimap = std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>;

}