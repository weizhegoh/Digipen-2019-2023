/*****************************************************************
*\file         UIBarFillSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "UIButtonMovementSystem.h"

namespace Xivi::Graphics
{
class Manager;
}

namespace Xivi
{
DefineSystemEventSystem( UIBarFillSystem, UIButtonMovementSystem, UIBarFillEvent );

//i32 m_cur_gamma = 10; // from 0 - 10
Ptr<Graphics::Manager> m_graphic_manager { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
void OnCreate() noexcept;
void OnEvent( bool &left, bool &right, Entity &hovered_entity ) noexcept;

EndDefine();
}