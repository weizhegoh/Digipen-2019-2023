#pragma once

namespace Xivi::Input::Control
{

class Base
{
  Ptr<Input::Device::Instance> m_device {nullptr};

public:
  Base() = default;

  Base(Input::Device::Instance& device) : m_device(&device) {}

};

}