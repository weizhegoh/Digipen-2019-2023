/*****************************************************************
*\file         GUIInputSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Xivi/ECS/XVECS.h"
#include "Xivi/Core/Declaration.h"
#include "NEvents/StepEvents.h"

namespace Xivi::Input::Device
{
class Keyboard;
}

namespace Xivi
{
DefineUpdateSystem( GUIInputSystem );

using Events = tuplet::tuple<
  UpdateGUIBackToMenuEvent,
  UpdateGUIBackToWinLoseEvent,
  UIButtonMovementEvent,
  UpdateGUIInstructionPageEvent,
  UpdateGUIHowToPlayPageEvent
>;

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<RM> m_resource_manager { nullptr };
Ptr<Input::Device::Keyboard> m_keyboard { nullptr };

bool m_is_controller { false };
void ShowKeyboardorController() noexcept;
void OnCreate() noexcept;
void OnUpdate() noexcept;

EndDefine();
}