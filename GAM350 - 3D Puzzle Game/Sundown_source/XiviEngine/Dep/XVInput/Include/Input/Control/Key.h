#pragma once

namespace Xivi::Input::Control
{

class Key : public Button
{
  Input::Key m_key{Input::Key::None};

public:
  Key() = default;
  Key( Input::Device::Instance &device,
       Input::Key key,
       bool &prev,
       bool &curr ) :
    Button( device, prev, curr ),
    m_key( key )
  {}

  inline Input::Key KeyCode() const noexcept
  {
    return m_key;
  }
};

}