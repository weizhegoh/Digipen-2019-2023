/*****************************************************************
*\file         ActivatePopupDialogueSystem.h
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
DefineGlobalEventSystem( ActivateSwitchPopupDialogueSystem, ActivateSwitchPopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ActivatePressurePlatePopupDialogueSystem, ActivatePressurePlatePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ActivateCheesePopupDialogueSystem, ActivateCheesePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ActivateObstaclePopupDialogueSystem, ActivateObstaclePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ActivateHolePopupDialogueSystem, ActivateHolePopupDialogue );
void OnEvent() noexcept;
EndDefine();

DefineGlobalEventSystem( ActivateCartPopupDialogueSystem, ActivateCartPopupDialogue );
void OnEvent() noexcept;
EndDefine();
}