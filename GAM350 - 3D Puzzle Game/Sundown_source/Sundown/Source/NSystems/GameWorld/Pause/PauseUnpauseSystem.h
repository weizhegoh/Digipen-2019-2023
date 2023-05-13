/*****************************************************************
*\file         PauseUnpauseSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Xivi/ECS/XVECS.h"
#include "Xivi/Core/Declaration.h"
#include "NEvents/StepEvents.h"

namespace Xivi
{
DefineGlobalEventSystem( PauseUnpauseSystem, PauseUnpauseEvent );

bool m_paused { false };

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
ECS::Query::Instance m_gui_window;

/*float m_bgm_vol{ 0.0f };
float m_prev_bgm_vol{ 0.0f };*/

void OnCreate() noexcept;
void OnEvent() noexcept;

EndDefine();
}