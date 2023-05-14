#pragma once

namespace Xivi::Input::Control
{
class DPad
{
  Ptr<Input::Device::Instance> m_device { nullptr };
  Button m_up;
  Button m_down;
  Button m_left;
  Button m_right;

public:
  DPad() = default;
  DPad( Input::Device::Instance &device,
        Button up,
        Button down,
        Button left,
        Button right ) :
    m_device( &device ),
    m_up( up ),
    m_down( down ),
    m_left( left ),
    m_right( right )
  {}

  inline Ptr<Input::Device::Instance> Device()
  {
    return m_device ? m_device : nullptr;
  }

  inline Button Up() const noexcept
  {
    return m_up;
  }

  inline Button Down() const noexcept
  {
    return m_down;
  }

  inline Button Left() const noexcept
  {
    return m_left;
  }

  inline Button Right() const noexcept
  {
    return m_right;
  }

  inline Vec2f Vector() const noexcept
  {
    if ( !m_device )
      return { 0.0f, 0.0f };

    Vec2f result { 0.0f, 0.0f };
    if ( m_up.Pressed() )
      result.y += 1.0f;
    if ( m_down.Pressed() )
      result.y -= 1.0f;
    if ( m_left.Pressed() )
      result.x -= 1.0f;
    if ( m_right.Pressed() )
      result.x += 1.0f;
    return result;
  }
};
}