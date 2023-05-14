/*****************************************************************
*\file         HowToPlaySystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineSystemEventSystem( HowToPlaySystem, GUIInputSystem, UpdateGUIHowToPlayPageEvent );

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
Ptr<RM> m_resource_manager { nullptr };
ECS::Query::Instance m_how_to_play;
i32 m_cur_pg = 0;

const static i32 s_number_of_pg = 5;

const std::array<std::string, s_number_of_pg> m_list_of_how_to__play
{
  "Assets/Textures/How_to_Play_Pg1.dds",
  "Assets/Textures/How_to_Play_Pg2.dds",
  "Assets/Textures/How_to_Play_Pg3.dds",
  "Assets/Textures/How_to_Play_Pg4.dds",
  "Assets/Textures/How_to_Play_Pg5.dds"
};

void OnCreate() noexcept;
void OnEvent( const bool &left_triggered, const bool &right_triggered ) noexcept;

EndDefine();
}