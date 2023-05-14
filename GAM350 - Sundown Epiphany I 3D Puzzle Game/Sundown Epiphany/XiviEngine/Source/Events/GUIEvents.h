/*****************************************************************
*\file         GUIEvents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct GameGUIEvent : ECS::Event::Instance<>
{};
using GUI_EList = EList<>;
}