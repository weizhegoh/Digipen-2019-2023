#pragma once

#include <Input/Device/Gamepad/State.h>

#define DefineGamepadButton(btn)                                                  \
  Input::Control::Button btn() noexcept                                           \
  {                                                                               \
    return Input::Control::Button{*this,                                          \
                                  m_state.m_prev_button[(u8)GamepadButton::btn],  \
                                  m_state.m_curr_button[(u8)GamepadButton::btn]}; \
  }

namespace Xivi::Input::Device
{

  class Gamepad : public Instance
  {
  public:
    enum class InputAPI : u8
    {
      XInput = 0,
      PS4
    };

    Gamepad(Type type, Name name, u8 index) : Instance(type, name, index)
    {
    }

    DefineGamepadButton(A);
    DefineGamepadButton(B);
    DefineGamepadButton(X);
    DefineGamepadButton(Y);
    DefineGamepadButton(LeftShoulder);
    DefineGamepadButton(RightShoulder);
    DefineGamepadButton(Back);
    DefineGamepadButton(Start);
    DefineGamepadButton(Guide);
    DefineGamepadButton(LeftStick);
    DefineGamepadButton(RightStick);
    DefineGamepadButton(DPadUp);
    DefineGamepadButton(DPadRight);
    DefineGamepadButton(DPadDown);
    DefineGamepadButton(DPadLeft);
    DefineGamepadButton(Cross);
    DefineGamepadButton(Circle);
    DefineGamepadButton(Square);
    DefineGamepadButton(Triangle);
    DefineGamepadButton(South);
    DefineGamepadButton(East);
    DefineGamepadButton(West);
    DefineGamepadButton(North);

    Input::Control::DPad DPad()
    {
      return Input::Control::DPad(*this, DPadUp(), DPadDown(), DPadLeft(), DPadRight());
    }

    Input::Control::Vector2 LeftStickAxis()
    {
      return Input::Control::Vector2(*this, m_state.m_left_stick);
    }

    Input::Control::Vector2 RightStickAxis()
    {
      return Input::Control::Vector2(*this, m_state.m_right_stick);
    }

    Input::Control::Float LeftTrigger()
    {
      return Input::Control::Float(*this, m_state.m_left_trigger);
    }

    Input::Control::Float RightTrigger()
    {
      return Input::Control::Float(*this, m_state.m_right_trigger);
    }

    Input::Control::AnyKey AnyKey() noexcept
    {
      return Input::Control::AnyKey(*this, m_any);
    }

    Input::Control::Button operator[](GamepadButton button) noexcept
    {
      return Input::Control::Button{*this,
                                    m_state.m_prev_button[(u8)button],
                                    m_state.m_curr_button[(u8)button]};
    }

    void API(InputAPI api) noexcept
    {
      m_api = api;
    }

    InputAPI GetAPI() noexcept
    {
      return m_api;
    }

    void Update([[maybe_unused]] f32 dt) noexcept override
    {
      if (!Enabled())
        return;

      std::memcpy(m_state.m_prev_button.data(), m_state.m_curr_button.data(), max_pad_button);
      m_any = false;
    }

    void Clear() noexcept override
    {
      if (!Enabled())
        return;

      std::memset(m_state.m_prev_button.data(), 0, max_pad_button);
      std::memset(m_state.m_curr_button.data(), 0, max_pad_button);
      m_state.m_left_stick = m_state.m_right_stick = 0.0f;
      m_state.m_left_trigger = m_state.m_right_trigger = 0.0f;
    }

    void ButtonUpdate(GamepadButton button, bool pressed) noexcept
    {
      m_state.m_curr_button[(u8)button] = pressed;
      if (pressed)
        m_any = true;
    }

    void AxisUpdate(GamepadAxis axis, f32 value) noexcept
    {
      switch (axis)
      {
      case GamepadAxis::LeftTrigger:
        m_state.m_left_trigger = value;
        break;
      case GamepadAxis::RightTrigger:
        m_state.m_right_trigger = value;
        break;
      case GamepadAxis::LeftX:
        m_state.m_left_stick.x = value;
        break;
      case GamepadAxis::LeftY:
        m_state.m_left_stick.y = value;
        break;
      case GamepadAxis::RightX:
        m_state.m_right_stick.x = value;
        break;
      case GamepadAxis::RightY:
        m_state.m_right_stick.y = value;
        break;
      }
    }

  private:
    GamepadState m_state;
    InputAPI m_api{InputAPI::XInput};
  };

}