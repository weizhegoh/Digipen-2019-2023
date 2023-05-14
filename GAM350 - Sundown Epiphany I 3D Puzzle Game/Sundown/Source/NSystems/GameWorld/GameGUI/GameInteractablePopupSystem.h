/*****************************************************************
*\file         GameInteractablePopupSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Input/GUIInputSystem.h"

namespace Xivi
{
DefineUpdateSystem( GameInteractablePopupSystem );

ECS::Query::Instance m_canvas_interaction_popup;

void OnCreate() noexcept;
void OnUpdate() noexcept;

EndDefine();
}