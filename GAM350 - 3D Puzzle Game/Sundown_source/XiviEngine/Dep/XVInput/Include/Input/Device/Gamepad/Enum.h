#pragma once

namespace Xivi::Input
{

inline static constexpr auto max_pad_button = 32;

enum class GamepadButton : u8
{
  None = max_pad_button -1,
  A=0,
  B,
  X,
  Y,
  LeftShoulder,
  RightShoulder,
  Back,
  Start,
  Guide,
  LeftStick,
  RightStick,
  DPadUp,
  DPadRight,
  DPadDown,
  DPadLeft,
  Cross = A,
  Circle = B,
  Square = X,
  Triangle = Y,
  South = Cross,
  East = Circle,
  West = Square,
  North = Triangle
};

enum class GamepadAxis : u8
{
  LeftX,
  LeftY,
  RightX,
  RightY,
  LeftTrigger,
  RightTrigger
};
}