#pragma once

namespace Xivi::Input::Control
{

class AnyKey
{
  Ptr<Input::Device::Instance> m_device{nullptr};
  Ptr<bool> m_any {nullptr};

public:
  AnyKey() = default;
  AnyKey( Input::Device::Instance &device,
          bool &any) :
    m_device( &device ),
    m_any(&any)
  {}

  inline Ptr<Input::Device::Instance> Device()
  {
    return m_device ? m_device : nullptr;
  }

  inline bool Value()
  {
    return m_device ? m_any : false;
  }
};


}