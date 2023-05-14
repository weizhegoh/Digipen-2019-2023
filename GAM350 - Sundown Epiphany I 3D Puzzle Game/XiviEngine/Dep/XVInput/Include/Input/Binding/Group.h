#pragma once

namespace Xivi
{

  enum class InputActionBindingGroup : u8
  {
    Default = 0,
    Keyboard = 1,
    Mouse = 1 << 1,
    Gamepad = 1 << 2
  };

  enum class InputActionBindingGroupMask : u8
  {
    Default = 0,
    Keyboard = static_cast<u8>(InputActionBindingGroup::Keyboard),
    KeyboardMouse = static_cast<u8>(InputActionBindingGroup::Keyboard) | static_cast<int>(InputActionBindingGroup::Mouse),
    Gamepad = static_cast<u8>(InputActionBindingGroup::Gamepad)
  };

}