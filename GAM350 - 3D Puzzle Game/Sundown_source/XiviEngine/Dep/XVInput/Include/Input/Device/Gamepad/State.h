#pragma once

namespace Xivi::Input
{
  struct GamepadState
  {
    std::array<bool, max_pad_button> m_prev_button{false};
    std::array<bool, max_pad_button> m_curr_button{false};
    Vec2f m_left_stick{0.0f, 0.0f};
    Vec2f m_right_stick{0.0f, 0.0f};
    f32 m_left_trigger{0.0f};
    f32 m_right_trigger{0.0f};
  };

}