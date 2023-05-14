/*****************************************************************
*\file         GLFW_KeyCodes.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <Input/Device/Gamepad/Enum.h>
#include <Input/Device/Mouse/Enum.h>
#include <Input/Device/Keyboard/Enum.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#define B0 0
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6
#define B7 7
#define B8 8
#define B9 9
#define B10 10
#define B11 11
#define B12 12
#define B13 13
#define B14 14
#define B15 15
#define B16 16
#define B17 17
#define B18 18

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

namespace Xivi::Input::GLFW
{
inline Key XVKey( u32 glfw_key )
{
  static std::unordered_map<u32, Key> key_map =
  {
    // Alphabets
    {GLFW_KEY_A, Key::A},
    {GLFW_KEY_B, Key::B},
    {GLFW_KEY_C, Key::C},
    {GLFW_KEY_D, Key::D},
    {GLFW_KEY_E, Key::E},
    {GLFW_KEY_F, Key::F},
    {GLFW_KEY_G, Key::G},
    {GLFW_KEY_H, Key::H},
    {GLFW_KEY_I, Key::I},
    {GLFW_KEY_J, Key::J},
    {GLFW_KEY_K, Key::K},
    {GLFW_KEY_L, Key::L},
    {GLFW_KEY_M, Key::M},
    {GLFW_KEY_N, Key::N},
    {GLFW_KEY_O, Key::O},
    {GLFW_KEY_P, Key::P},
    {GLFW_KEY_Q, Key::Q},
    {GLFW_KEY_R, Key::R},
    {GLFW_KEY_S, Key::S},
    {GLFW_KEY_T, Key::T},
    {GLFW_KEY_U, Key::U},
    {GLFW_KEY_V, Key::V},
    {GLFW_KEY_W, Key::W},
    {GLFW_KEY_X, Key::X},
    {GLFW_KEY_Y, Key::Y},
    {GLFW_KEY_Z, Key::Z},
    // Digits
    {GLFW_KEY_0, Key::D0},
    {GLFW_KEY_1, Key::D1},
    {GLFW_KEY_2, Key::D2},
    {GLFW_KEY_3, Key::D3},
    {GLFW_KEY_4, Key::D4},
    {GLFW_KEY_5, Key::D5},
    {GLFW_KEY_6, Key::D6},
    {GLFW_KEY_7, Key::D7},
    {GLFW_KEY_8, Key::D8},
    {GLFW_KEY_9, Key::D9},
    // Symbols
    {GLFW_KEY_SPACE, Key::Space},
    {GLFW_KEY_APOSTROPHE, Key::Apostrophe},
    {GLFW_KEY_COMMA, Key::Comma},
    {GLFW_KEY_MINUS, Key::Minus},
    {GLFW_KEY_PERIOD, Key::Period},
    {GLFW_KEY_SLASH, Key::Slash},
    {GLFW_KEY_SEMICOLON, Key::Semicolon},
    {GLFW_KEY_EQUAL, Key::Equal},
    {GLFW_KEY_LEFT_BRACKET, Key::LeftBracket},
    {GLFW_KEY_RIGHT_BRACKET, Key::RightBracket},
    {GLFW_KEY_BACKSLASH, Key::Backslash},
    {GLFW_KEY_GRAVE_ACCENT, Key::GraveAccent},
    // Function Keys
    {GLFW_KEY_TAB, Key::Tab},
    {GLFW_KEY_ESCAPE, Key::Escape},
    {GLFW_KEY_ENTER, Key::Enter},
    {GLFW_KEY_BACKSPACE, Key::Backspace},
    {GLFW_KEY_INSERT, Key::Insert},
    {GLFW_KEY_DELETE, Key::Delete},
    {GLFW_KEY_UP, Key::Up},
    {GLFW_KEY_DOWN, Key::Down},
    {GLFW_KEY_LEFT, Key::Left},
    {GLFW_KEY_RIGHT, Key::Right},
    {GLFW_KEY_PAGE_UP, Key::PageUp},
    {GLFW_KEY_PAGE_DOWN, Key::PageDown},
    {GLFW_KEY_HOME, Key::Home},
    {GLFW_KEY_END, Key::End},
    {GLFW_KEY_CAPS_LOCK, Key::CapsLock},
    {GLFW_KEY_SCROLL_LOCK, Key::ScrollLock},
    {GLFW_KEY_NUM_LOCK, Key::NumLock},
    {GLFW_KEY_PRINT_SCREEN, Key::PrintScreen},
    {GLFW_KEY_PAUSE, Key::Pause},
    {GLFW_KEY_F1, Key::F1},
    {GLFW_KEY_F2, Key::F2},
    {GLFW_KEY_F3, Key::F3},
    {GLFW_KEY_F4, Key::F4},
    {GLFW_KEY_F5, Key::F5},
    {GLFW_KEY_F6, Key::F6},
    {GLFW_KEY_F7, Key::F7},
    {GLFW_KEY_F8, Key::F8},
    {GLFW_KEY_F9, Key::F9},
    {GLFW_KEY_F10, Key::F10},
    {GLFW_KEY_F11, Key::F11},
    {GLFW_KEY_F12, Key::F12},
    {GLFW_KEY_F13, Key::F13},
    {GLFW_KEY_F14, Key::F14},
    {GLFW_KEY_F15, Key::F15},
    {GLFW_KEY_F16, Key::F16},
    {GLFW_KEY_F17, Key::F17},
    {GLFW_KEY_F18, Key::F18},
    {GLFW_KEY_F19, Key::F19},
    {GLFW_KEY_F20, Key::F20},
    {GLFW_KEY_F21, Key::F21},
    {GLFW_KEY_F22, Key::F22},
    {GLFW_KEY_F23, Key::F23},
    {GLFW_KEY_F24, Key::F24},
    {GLFW_KEY_F25, Key::F25},
    // Keypad
    {GLFW_KEY_KP_0, Key::KP0},
    {GLFW_KEY_KP_1, Key::KP1},
    {GLFW_KEY_KP_2, Key::KP2},
    {GLFW_KEY_KP_3, Key::KP3},
    {GLFW_KEY_KP_4, Key::KP4},
    {GLFW_KEY_KP_5, Key::KP5},
    {GLFW_KEY_KP_6, Key::KP6},
    {GLFW_KEY_KP_7, Key::KP7},
    {GLFW_KEY_KP_8, Key::KP8},
    {GLFW_KEY_KP_9, Key::KP9},
    {GLFW_KEY_KP_DECIMAL, Key::KPDecimal},
    {GLFW_KEY_KP_MULTIPLY, Key::KPMultiply},
    {GLFW_KEY_KP_DIVIDE, Key::KPDivide},
    {GLFW_KEY_KP_ADD, Key::KPAdd},
    {GLFW_KEY_KP_SUBTRACT, Key::KPSubtract},
    {GLFW_KEY_KP_ENTER, Key::KPEnter},
    {GLFW_KEY_KP_EQUAL, Key::KPEqual},
    // Special
    {GLFW_KEY_LEFT_SHIFT, Key::LeftShift},
    {GLFW_KEY_LEFT_CONTROL, Key::LeftControl},
    {GLFW_KEY_LEFT_ALT, Key::LeftAlt},
    {GLFW_KEY_LEFT_SUPER, Key::LeftSuper},
    {GLFW_KEY_RIGHT_SHIFT, Key::RightShift},
    {GLFW_KEY_RIGHT_CONTROL, Key::RightControl},
    {GLFW_KEY_RIGHT_ALT, Key::RightAlt},
    {GLFW_KEY_RIGHT_SUPER, Key::RightSuper},
    // Non-US
    {GLFW_KEY_WORLD_1, Key::World1},
    { GLFW_KEY_WORLD_2, Key::World2 }
  };
  return key_map[glfw_key];
}

inline MouseButton XVMouse( u32 glfw_key )
{
  static std::unordered_map<u32, MouseButton> mouse_button_map = {
    {GLFW_MOUSE_BUTTON_LEFT, MouseButton::Left},
    {GLFW_MOUSE_BUTTON_MIDDLE, MouseButton::Middle},
    {GLFW_MOUSE_BUTTON_RIGHT, MouseButton::Right},
    {GLFW_MOUSE_BUTTON_4, MouseButton::Backward},
    {GLFW_MOUSE_BUTTON_5, MouseButton::Forward} };

  return mouse_button_map[glfw_key];
}

inline GamepadButton XVGamepadButton( u32 glfw_key, Input::Device::Gamepad::InputAPI api )
{
  static std::unordered_map<u32, GamepadButton> xinput_button_map =
  {
    {B0, GamepadButton::A},
    {B1, GamepadButton::B},
    {B2, GamepadButton::X},
    {B3, GamepadButton::Y},
    {B4, GamepadButton::LeftShoulder},
    {B5, GamepadButton::RightShoulder},
    {B6, GamepadButton::Back},
    {B7, GamepadButton::Start},
    {B8, GamepadButton::LeftStick},
    {B9, GamepadButton::RightStick},
    {B10, GamepadButton::DPadUp},
    {B11, GamepadButton::DPadRight},
    {B12, GamepadButton::DPadDown},
    {B13, GamepadButton::DPadLeft},
    {B14, GamepadButton::None},
  };

  static std::unordered_map<u32, GamepadButton> ps4_button_map =
  {
    {B0, GamepadButton::X},
    {B1, GamepadButton::A},
    {B2, GamepadButton::B},
    {B3, GamepadButton::Y},
    {B4, GamepadButton::LeftShoulder},
    {B5, GamepadButton::RightShoulder},
    {B6, GamepadButton::None},
    {B7, GamepadButton::None},
    {B8, GamepadButton::Back},
    {B9, GamepadButton::Start},
    {B10, GamepadButton::LeftStick},
    {B11, GamepadButton::RightStick},
    {B12, GamepadButton::Guide},
    {B13, GamepadButton::None},
    {B14, GamepadButton::DPadUp},
    {B15, GamepadButton::DPadRight},
    {B16, GamepadButton::DPadDown},
    {B17, GamepadButton::DPadLeft},
  };

  switch ( api )
  {
    case Input::Device::Gamepad::InputAPI::XInput:
    return xinput_button_map[glfw_key];
    case Input::Device::Gamepad::InputAPI::PS4:
    return ps4_button_map[glfw_key];
  }

  return GamepadButton::None;
}

inline GamepadAxis XVGamepadAxis( u32 glfw_key, Input::Device::Gamepad::InputAPI api )
{
  static std::unordered_map<u32, GamepadAxis> xinput_axis_map =
  {
    {GLFW_GAMEPAD_AXIS_LEFT_X, GamepadAxis::LeftX},
    {GLFW_GAMEPAD_AXIS_LEFT_Y, GamepadAxis::LeftY},
    {GLFW_GAMEPAD_AXIS_RIGHT_X, GamepadAxis::RightX},
    {GLFW_GAMEPAD_AXIS_RIGHT_Y, GamepadAxis::RightY},
    {GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GamepadAxis::LeftTrigger},
    {GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, GamepadAxis::RightTrigger}
  };

  static std::unordered_map<u32, GamepadAxis> ps4_axis_map =
  {
    {A0, GamepadAxis::LeftX},
    {A1, GamepadAxis::LeftY},
    {A2, GamepadAxis::RightX},
    {A3, GamepadAxis::LeftTrigger},
    {A4, GamepadAxis::RightTrigger},
    {A5, GamepadAxis::RightY}
  };

  switch ( api )
  {
    case Input::Device::Gamepad::InputAPI::XInput:
    return xinput_axis_map[glfw_key];
    case Input::Device::Gamepad::InputAPI::PS4:
    return ps4_axis_map[glfw_key];
    default:
    return xinput_axis_map[glfw_key];
  }
}
}