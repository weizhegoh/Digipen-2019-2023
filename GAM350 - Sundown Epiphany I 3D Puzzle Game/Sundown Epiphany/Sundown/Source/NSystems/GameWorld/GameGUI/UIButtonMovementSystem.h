/*****************************************************************
*\file         UIButtonMovementSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineSystemEventSystem( UIButtonMovementSystem, GUIInputSystem, UIButtonMovementEvent );

struct ButtonSort
{
  bool operator()( const Vec2f &lhs, const Vec2f &rhs ) const noexcept;
};

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
Ptr<TimerEngine> m_timer { nullptr };
Ptr<Input::Device::Keyboard> m_keyboard { nullptr };
std::map<Vec2f, Entity, ButtonSort> m_buttons_arrangement;
Entity m_hovered_button;
bool m_holding_axis { false };

using Events = tuplet::tuple<
  UIBarFillEvent,
  LevelSelectorEvent
>;

void OnCreate() noexcept;
void OnEvent() noexcept;

EndDefine();
}