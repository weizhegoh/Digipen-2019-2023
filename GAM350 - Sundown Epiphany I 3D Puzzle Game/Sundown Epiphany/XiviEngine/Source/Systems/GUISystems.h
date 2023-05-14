/*****************************************************************
*\file         GUISystems.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "GUI/GUISystem.h"
#include "GUI/UICheckBoxSystem.h"
#include "GUI/UIDragSystem.h"
#include "GUI/UIHoverSystem.h"
#include "GUI/UITriggerSystem.h"

namespace Xivi
{
using Notifier_GUI_SList = SList<>;

using Global_GUI_SList = SList<>;

using Update_GUI_SList = SList<
  GUI::GUISystem,
  GUI::UICheckBoxSystem,
  GUI::UIDragSystem,
  GUI::UIHoverSystem,
  GUI::UITriggerSystem
>;
}
