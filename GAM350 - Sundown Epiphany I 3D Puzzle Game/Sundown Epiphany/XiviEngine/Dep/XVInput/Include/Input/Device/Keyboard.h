#pragma once

#include <Input/Device/Keyboard/State.h>

#define DefineKeyControlKey(key)                                  \
  Input::Control::Key key() noexcept                              \
  {                                                               \
    return Input::Control::Key{*this,                             \
                               Key::key,                          \
                               m_state.m_prev_key[(u8)Key::key],  \
                               m_state.m_curr_key[(u8)Key::key]}; \
  }

namespace Xivi::Input::Device
{

  class Keyboard : public Instance
  {
    KeyboardState m_state;

  public:
    Keyboard(Type type, Name name, u8 index) : Instance(type, name, index)
    {
    }

    // Alphabets
    DefineKeyControlKey(A);
    DefineKeyControlKey(B);
    DefineKeyControlKey(C);
    DefineKeyControlKey(D);
    DefineKeyControlKey(E);
    DefineKeyControlKey(F);
    DefineKeyControlKey(G);
    DefineKeyControlKey(H);
    DefineKeyControlKey(I);
    DefineKeyControlKey(J);
    DefineKeyControlKey(K);
    DefineKeyControlKey(L);
    DefineKeyControlKey(M);
    DefineKeyControlKey(N);
    DefineKeyControlKey(O);
    DefineKeyControlKey(P);
    DefineKeyControlKey(Q);
    DefineKeyControlKey(R);
    DefineKeyControlKey(S);
    DefineKeyControlKey(T);
    DefineKeyControlKey(U);
    DefineKeyControlKey(V);
    DefineKeyControlKey(W);
    DefineKeyControlKey(X);
    DefineKeyControlKey(Y);
    DefineKeyControlKey(Z);
    // Digits
    DefineKeyControlKey(D0);
    DefineKeyControlKey(D1);
    DefineKeyControlKey(D2);
    DefineKeyControlKey(D3);
    DefineKeyControlKey(D4);
    DefineKeyControlKey(D5);
    DefineKeyControlKey(D6);
    DefineKeyControlKey(D7);
    DefineKeyControlKey(D8);
    DefineKeyControlKey(D9);
    // Symbols
    DefineKeyControlKey(Space);
    DefineKeyControlKey(Apostrophe);
    DefineKeyControlKey(Comma);
    DefineKeyControlKey(Minus);
    DefineKeyControlKey(Period);
    DefineKeyControlKey(Slash);
    DefineKeyControlKey(Semicolon);
    DefineKeyControlKey(Equal);
    DefineKeyControlKey(LeftBracket);
    DefineKeyControlKey(RightBracket);
    DefineKeyControlKey(Backslash);
    DefineKeyControlKey(GraveAccent);
    // Function Keys
    DefineKeyControlKey(Tab);
    DefineKeyControlKey(Escape);
    DefineKeyControlKey(Enter);
    DefineKeyControlKey(Backspace);
    DefineKeyControlKey(Insert);
    DefineKeyControlKey(Delete);
    DefineKeyControlKey(Up);
    DefineKeyControlKey(Down);
    DefineKeyControlKey(Left);
    DefineKeyControlKey(Right);
    DefineKeyControlKey(PageUp);
    DefineKeyControlKey(PageDown);
    DefineKeyControlKey(Home);
    DefineKeyControlKey(End);
    DefineKeyControlKey(CapsLock);
    DefineKeyControlKey(ScrollLock);
    DefineKeyControlKey(NumLock);
    DefineKeyControlKey(PrintScreen);
    DefineKeyControlKey(Pause);
    DefineKeyControlKey(F1);
    DefineKeyControlKey(F2);
    DefineKeyControlKey(F3);
    DefineKeyControlKey(F4);
    DefineKeyControlKey(F5);
    DefineKeyControlKey(F6);
    DefineKeyControlKey(F7);
    DefineKeyControlKey(F8);
    DefineKeyControlKey(F9);
    DefineKeyControlKey(F10);
    DefineKeyControlKey(F11);
    DefineKeyControlKey(F12);
    DefineKeyControlKey(F13);
    DefineKeyControlKey(F14);
    DefineKeyControlKey(F15);
    DefineKeyControlKey(F17);
    DefineKeyControlKey(F18);
    DefineKeyControlKey(F19);
    DefineKeyControlKey(F20);
    DefineKeyControlKey(F21);
    DefineKeyControlKey(F22);
    DefineKeyControlKey(F23);
    DefineKeyControlKey(F24);
    DefineKeyControlKey(F25);
    // Keypad
    DefineKeyControlKey(KP0);
    DefineKeyControlKey(KP1);
    DefineKeyControlKey(KP2);
    DefineKeyControlKey(KP3);
    DefineKeyControlKey(KP4);
    DefineKeyControlKey(KP5);
    DefineKeyControlKey(KP6);
    DefineKeyControlKey(KP7);
    DefineKeyControlKey(KP8);
    DefineKeyControlKey(KP9);
    DefineKeyControlKey(KPDecimal);
    DefineKeyControlKey(KPMultiply);
    DefineKeyControlKey(KPDivide);
    DefineKeyControlKey(KPAdd);
    DefineKeyControlKey(KPSubtract);
    DefineKeyControlKey(KPEnter);
    DefineKeyControlKey(KPEqual);
    // Special
    DefineKeyControlKey(LeftShift);
    DefineKeyControlKey(LeftControl);
    DefineKeyControlKey(LeftAlt);
    DefineKeyControlKey(LeftSuper);
    DefineKeyControlKey(RightShift);
    DefineKeyControlKey(RightControl);
    DefineKeyControlKey(RightAlt);
    DefineKeyControlKey(RightSuper);
    DefineKeyControlKey(Menu);
    // Non-US
    DefineKeyControlKey(World1);
    DefineKeyControlKey(World2);

    Input::Control::Key operator[](Key key) noexcept
    {
      return Input::Control::Key{*this,
                                 key,
                                 m_state.m_prev_key[(u8)key],
                                 m_state.m_curr_key[(u8)key]};
    }

    Input::Control::AnyKey AnyKey() noexcept
    {
      return Input::Control::AnyKey(*this, m_any);
    }

    void Update([[maybe_unused]] f32 dt) noexcept override
    {
      if (!Enabled())
        return;

      std::memcpy(m_state.m_prev_key.data(), m_state.m_curr_key.data(), max_key);
      m_any = false;
    }

    void Clear() noexcept override
    {
      std::memset(m_state.m_prev_key.data(), 0, max_key);
      std::memset(m_state.m_curr_key.data(), 0, max_key);
    }

    void KeyUpdate(Key key, bool pressed) noexcept
    {
      m_state.m_curr_key[(u8)key] = pressed;
      if (pressed)
        m_any = true;
    }
  };

}