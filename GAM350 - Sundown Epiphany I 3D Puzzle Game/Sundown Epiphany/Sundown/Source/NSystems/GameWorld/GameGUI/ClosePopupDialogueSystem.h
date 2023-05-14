/*****************************************************************
*\file         ClosePopupDialogueSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "XiviEngine.h"
#include "NEvents/StepEvents.h"

namespace Xivi
{
DefineGlobalEventSystem( CloseSwitchPopupDialogueSystem, CloseSwitchPopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ClosePressurePlatePopupDialogueSystem, ClosePressurePlatePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( CloseCheesePopupDialogueSystem, CloseCheesePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( CloseObstaclePopupDialogueSystem, CloseObstaclePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( CloseHolePopupDialogueSystem, CloseHolePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( CloseCartPopupDialogueSystem, CloseCartPopupDialogue );
void OnEvent() noexcept;
EndDefine();
}