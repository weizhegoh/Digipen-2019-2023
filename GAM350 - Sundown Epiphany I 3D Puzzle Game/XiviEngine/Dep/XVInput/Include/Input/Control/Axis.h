#pragma once

#include "Value.h"


namespace Xivi::Input::Control
{

  enum class ClampingPolicy : u8
  {
    None,
    AfterNormalize, 
    BeforeNormalize
  };

  class Axis : public Value<f32>
  {
    ClampingPolicy m_policy;
    f32 m_clamp_min {.0f};
    f32 m_clamp_max {.0f};
    bool m_invert{false};

  public:
    Axis() = default;
    Axis(Input::Device::Instance &device, f32 &value) : Value(device, value)
    {
    }



  };
}