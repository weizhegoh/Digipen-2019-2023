#pragma once

namespace Xivi::Input::Control
{
class Float
{
  Ptr<Input::Device::Instance> m_device {nullptr};
  Ptr<f32> m_float {nullptr};

public:
  Float() = default;
  Float( Input::Device::Instance &device,
           f32 &floating_point ) :
    m_device( &device ),
    m_float( &floating_point )
  {}

  inline Ptr<Input::Device::Instance> Device()
  {
    return m_device ? m_device : nullptr;
  }

  inline f32 Value() const noexcept
  {
    return m_device ? *m_float : 0.0f;
  }
};
}