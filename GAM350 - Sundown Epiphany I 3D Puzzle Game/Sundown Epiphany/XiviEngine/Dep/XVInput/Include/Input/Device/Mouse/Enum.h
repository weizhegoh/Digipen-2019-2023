#pragma once

namespace Xivi::Input
{

inline static constexpr auto max_button = 8;

enum class MouseButton : u8
{
  None = max_button -1,
  Left = 0,
  Middle = 1,
  Right = 2,
  Forward = 3,
  Backward = 4,
};
}