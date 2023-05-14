#pragma once

// isocpp
// https://www.reedbeta.com/blog/python-like-enumerate-in-cpp17/

#include <tuple>

namespace Xivi
{
  template <typename Container,
            typename CIterator = decltype(std::begin(std::declval<Container>())),
            typename = decltype(std::end(std::declval<Container>()))>
  inline constexpr auto Enumerate(Container &&iterable)
  {
    struct Iterator
    {
      size_t index;
      CIterator iter;
      constexpr bool operator!=(const Iterator &rhs) const { return iter != rhs.iter; }
      constexpr void operator++()
      {
        ++index;
        ++iter;
      }
      constexpr auto operator*() const { return std::tie(index, *iter); }
    };

    struct IteratorWrapper
    {
      Container iterable;
      constexpr auto begin() { return Iterator{0, std::begin(iterable)}; }
      constexpr auto end() { return Iterator{0, std::end(iterable)}; }
    };

    return IteratorWrapper{std::forward<Container>(iterable)};
  }

  template <typename Container,
            typename CIterator = decltype(std::rbegin(std::declval<Container>())),
            typename = decltype(std::rend(std::declval<Container>()))>
  inline constexpr auto REnumerate(Container &&iterable)
  {
    struct Iterator
    {
      size_t index;
      CIterator iter;
      constexpr bool operator!=(const Iterator &rhs) const { return iter != rhs.iter; }
      constexpr void operator++()
      {
        --index;
        ++iter;
      }
      constexpr auto operator*() const { return std::tie(index, *iter); }
    };

    struct IteratorWrapper
    {
      Container iterable;
      constexpr auto begin() { return Iterator{std::size(iterable) - 1, std::rbegin(iterable)}; }
      constexpr auto end() { return Iterator{std::size(iterable) - 1, std::rend(iterable)}; }
    };

    return IteratorWrapper{std::forward<Container>(iterable)};
  }
}