#pragma once

#include <type_traits>
#include <array>
#include <vector>
#include <span>
#include <string_view>

namespace Xivi::Traits
{
  template <typename T>
  struct IsArrayType : std::is_array<T>
  {
  };

  template <typename T>
  struct IsArrayType<std::vector<T>> : std::true_type
  {
  };

  template <typename T, size_t N>
  struct IsArrayType<std::span<T, N>> : std::true_type
  {
  };

  template <typename T, size_t N>
  struct IsArrayType<std::array<T, N>> : std::true_type
  {
  };
}

namespace Xivi::Traits::Concepts
{
  template <typename T>
  concept StringType = std::is_convertible_v<T, std::string_view>;

  template <typename T>
  concept ArrayType = IsArrayType<T>::value;
}