#pragma once

namespace Xivi::Input
{
struct KeyboardState
{
  std::array<bool, max_key> m_prev_key { false };
  std::array<bool, max_key> m_curr_key { false };
};

}