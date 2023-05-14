/*****************************************************************
*\file         BackToMainMenuSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineSystemEventSystem( BackToMainMenuSystem, GUIInputSystem, UpdateGUIBackToMenuEvent );

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
ECS::Query::Instance m_canvas_to_exit;

void OnCreate() noexcept;
void OnEvent() noexcept;

EndDefine();
}