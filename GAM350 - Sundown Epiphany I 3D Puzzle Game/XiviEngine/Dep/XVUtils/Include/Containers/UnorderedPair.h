#pragma once

#include <Types/Base.h>
#include <Utils/Assert.h>

namespace Xivi
{

  template <typename T>
  struct UnorderedPair
  {
    T first;
    T second;

    UnorderedPair() = default;
    UnorderedPair(T f, T s)
        : first(f), second(s)
    {
    }

    auto operator[](u32 index) const
    {
      XV_CORE_ASSERT(index < 2);
      return index == 0 ? first : second;
    }

    bool operator==(const UnorderedPair &rhs) const
    {
      return (first == rhs.first && second == rhs.second) ||
             (first == rhs.second && second == rhs.first);
    }
  };

}