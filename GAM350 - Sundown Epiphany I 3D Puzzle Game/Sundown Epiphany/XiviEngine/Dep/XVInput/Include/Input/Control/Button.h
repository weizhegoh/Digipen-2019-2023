#pragma once

namespace Xivi::Input::Control
{

class Button
{
protected:
  Ptr<Input::Device::Instance> m_device { nullptr };
  Ptr<bool> m_prev { nullptr };
  Ptr<bool> m_curr { nullptr };

public:
  Button() = default;
  Button( Input::Device::Instance &device,
          bool &prev,
          bool &curr ) :
    m_device( &device ),
    m_prev( &prev ),
    m_curr( &curr )
  {}

  inline Ptr<Input::Device::Instance> Device()
  {
    return m_device ? m_device : nullptr;
  }

  inline bool Triggered() const noexcept
  {
    return m_device ? ( ( *m_prev & *m_curr ) ^ *m_curr ) : false;
  }

  inline bool Pressed() const noexcept
  {
    return m_device ? ( *m_prev && *m_curr ) : false;
  }

  inline bool Released() const noexcept
  {
    return m_device ? ( *m_prev && !( *m_curr ) ) : false;
  }
};

}