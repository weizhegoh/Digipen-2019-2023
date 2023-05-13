#pragma once

#include <Input/Device/Mouse/State.h>

#define DefineMouseControlButton(btn)                                           \
Input::Control::Button btn##Button() noexcept {                                 \
  return Input::Control::Button { *this,                                        \
                                  m_state.m_prev_button[(u8)MouseButton::btn],  \
                                  m_state.m_curr_button[(u8)MouseButton::btn]}; \
}

namespace Xivi::Input::Device
{

class Mouse : public Instance
{
  inline static constexpr f32 click_threshold = 0.4f;

  MouseState m_state;

public:

  Mouse( Type type, Name name, u8 index ) : Instance( type, name, index )
  {}

  DefineMouseControlButton( Left );
  DefineMouseControlButton( Right );
  DefineMouseControlButton( Middle );
  DefineMouseControlButton( Forward );
  DefineMouseControlButton( Backward );

  Input::Control::Integer ClickCount() noexcept
  {
    return Input::Control::Integer { *this,
                                   m_state.m_click_count };
  }

  Input::Control::Vector2 Scroll() noexcept
  {
    return Input::Control::Vector2 { *this,
                                   m_state.m_scroll };
  }

  Input::Control::Vector2 Position() noexcept
  {
    return Input::Control::Vector2 { *this,
                                   m_state.m_position };
  }

  Input::Control::Button operator[]( MouseButton button ) noexcept
  {
    return Input::Control::Button { *this,
      m_state.m_prev_button[(u8) button],
      m_state.m_curr_button[(u8) button] };
  }

  Input::Control::AnyKey AnyKey() noexcept
  {
    return Input::Control::AnyKey( *this, m_any );
  }

  void Update(f32 dt) noexcept override
  {
    if ( !Enabled() )
      return;

    m_state.m_scroll = m_state.m_delta = 0.0f;
    std::memcpy( m_state.m_prev_button.data(), m_state.m_curr_button.data(), max_button );
    if ( m_state.m_timer <= click_threshold )
      m_state.m_timer += dt;
    else
    {
      m_state.m_click_count = 0;
      m_state.m_timer = 0.0f;
    }
    m_any = false;
  }

  void Clear() noexcept override
  {
    m_state.m_scroll = m_state.m_delta = m_state.m_position = 0.0f;
    std::memset( m_state.m_prev_button.data(), 0, max_button );
    std::memset( m_state.m_curr_button.data(), 0, max_button );
    m_state.m_click_count = 0;
    m_state.m_timer = 0.0f;
  }

  void ButtonUpdate( MouseButton button, bool pressed ) noexcept
  {
    m_state.m_curr_button[(u8) button] = pressed;
    if ( pressed )
      m_any = true;

    if ( ( m_state.m_prev_button[(u8) button] & m_state.m_curr_button[(u8) button] ) ^
         m_state.m_curr_button[(u8) button] )
      ++m_state.m_click_count;
  }

  void ScrollUpdate( const f32 &x, const f32 &y ) noexcept
  {
    m_state.m_scroll.x = x;
    m_state.m_scroll.y = y;
  }

  void PositionUpdate( const f32 &x, const f32 &y ) noexcept
  {

    m_state.m_delta.x = x - m_state.m_position.x;
    m_state.m_delta.y = y - m_state.m_position.y;
    m_state.m_position.x = x;
    m_state.m_position.y = y;
  }


};

}