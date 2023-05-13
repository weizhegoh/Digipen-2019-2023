#pragma once

namespace Xivi::Input
{
  struct MouseState
  {
    std::array<bool, max_button> m_prev_button{false};
    std::array<bool, max_button> m_curr_button{false};
    Vec2f m_position{0.0f, 0.0f};
    Vec2f m_delta{0.0f, 0.0f};
    Vec2f m_scroll{0.0f, 0.0f};
    f32 m_timer{0.0f};
    i32 m_click_count{0};
  };
}