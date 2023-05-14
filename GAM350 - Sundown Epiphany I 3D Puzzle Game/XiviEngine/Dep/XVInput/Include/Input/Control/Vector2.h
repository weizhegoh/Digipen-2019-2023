#pragma once

namespace Xivi::Input::Control
{

class Vector2
{
  Ptr<Input::Device::Instance> m_device { nullptr };
  Ptr<Vec2f> m_vec { nullptr };

public:
  Vector2() = default;
  Vector2( Input::Device::Instance &device,
           Vec2f &vec ) :
    m_device( &device ),
    m_vec( &vec )
  {}

  f32 X() const noexcept
  {
    return m_device ? m_vec->x : 0.0f;
  }

  f32 Y() const noexcept
  {
    return m_device ? m_vec->y : 0.0f;
  }

  Vec2f Vector() const noexcept
  {
    return m_device ? *m_vec : Vec2f{0.0f, 0.0f};
  }
};

}