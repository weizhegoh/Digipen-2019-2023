/*****************************************************************
*\file         BackToWinLoseScreenSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineSystemEventSystem( BackToWinLoseScreenSystem, GUIInputSystem, UpdateGUIBackToWinLoseEvent );

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
Ptr<GSM::GSManager> m_gsm { nullptr };
Ptr<RM> m_resource_manager { nullptr };
ECS::Query::Instance m_canvas_can_go_back_to_winlose_sreen;

void OnCreate() noexcept;
void OnEvent() noexcept;

EndDefine();
}