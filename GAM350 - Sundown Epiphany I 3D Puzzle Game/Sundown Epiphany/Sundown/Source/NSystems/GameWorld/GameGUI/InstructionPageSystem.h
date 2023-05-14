/*****************************************************************
*\file         InstructionPageSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineSystemEventSystem( InstructionPageSystem, GUIInputSystem, UpdateGUIInstructionPageEvent );

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
Ptr<RM> m_resource_manager { nullptr };
ECS::Query::Instance m_instruction;
i32 m_cur_pg = 0;

void OnCreate() noexcept;
void OnEvent( const bool &left_triggered, const bool &right_triggered ) noexcept;

EndDefine();
}