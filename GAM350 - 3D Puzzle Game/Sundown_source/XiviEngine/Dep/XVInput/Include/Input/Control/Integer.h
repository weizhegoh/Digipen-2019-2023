#pragma once

namespace Xivi::Input::Control
{
class Integer
{
  Ptr<Input::Device::Instance> m_device { nullptr };
  Ptr<i32> m_integer { nullptr };

public:
  Integer() = default;
  Integer( Input::Device::Instance &device,
           i32 &integer ) :
    m_device( &device ),
    m_integer( &integer )
  {}

  inline Ptr<Input::Device::Instance> Device()
  {
    return m_device ? m_device : nullptr;
  }

  inline i32 Value() const noexcept
  {
    return m_device ? *m_integer : 0;
  }
};
}