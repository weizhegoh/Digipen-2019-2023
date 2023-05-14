/*****************************************************************
*\file         GameWorld.h
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "NSystems/GameWorld/GameWorldSystem.h"

// Player
#include "NSystems/GameWorld/Player/Input/ProcessPlayerInputSystem.h"
#include "NSystems/GameWorld/Player/Input/ProcessPlayerSystem.h"

// Notifer Systems
#include "Editor/Systems/Base/EntityCreateSystem.h"
#include "Editor/Systems/Base/EntityDestroySystem.h"

// Global Event Systems
#include "NSystems/GameWorld/GameGUI/UpdateGUIMoveSystem.h"
#include "NSystems/GameWorld/GameGUI/UpdateGUICollectableSystem.h"
#include "NSystems/GameWorld/Pause/PauseUnpauseSystem.h"
#include "NSystems/GameWorld/GameGUI/UpdateSummarySystem.h"
#include "NSystems/GameWorld/GameGUI/InstructionPageSystem.h"
#include "NSystems/GameWorld/GameGUI/InstructionPageSystem.h"
#include "NSystems/GameWorld/GameGUI/ActivatePopupDialogueSystem.h"
#include "NSystems/GameWorld/GameGUI/ClosePopupDialogueSystem.h"

// System Event Systems
#include "NSystems/GameWorld/GameGUI/BackToMainMenuSystem.h"
#include "NSystems/GameWorld/GameGUI/BackToWinLoseScreenSystem.h"
#include "NSystems/GameWorld/GameGUI/UIButtonMovementSystem.h"
#include "NSystems/GameWorld/GameGUI/UIBarFillSystem.h"
#include "NSystems/GameWorld/GameGUI/LevelSelectorUpdateSystem.h"
#include "NSystems/GameWorld/GameGUI/HowToPlaySystem.h"

// Update
#include "NSystems/GameWorld/GameGUI/Input/GUIInputSystem.h"
#include "NSystems/GameWorld/GameGUI/GameInteractablePopupSystem.h"

namespace Xivi
{
using GameWorld_SList_Notifier = SList<
  Editor::EntityCreateSystem,
  Editor::EntityDestroySystem
>;

using GameWorld_SList_Global = SList<
  UpdateGUIMoveSystem,
  UpdateGUICollectableSystem,
  PauseUnpauseSystem,
  UpdateSummarySystem,
  ActivateSwitchPopupDialogueSystem,
  CloseSwitchPopupDialogueSystem,
  ActivatePressurePlatePopupDialogueSystem,
  ClosePressurePlatePopupDialogueSystem,
  ActivateCheesePopupDialogueSystem,
  CloseCheesePopupDialogueSystem,
  ActivateObstaclePopupDialogueSystem,
  CloseObstaclePopupDialogueSystem,
  ActivateHolePopupDialogueSystem,
  CloseHolePopupDialogueSystem,
  ActivateCartPopupDialogueSystem,
  CloseCartPopupDialogueSystem
>;

using GameWorld_SList_Update = SList<
  GameWorldSystem,
  ProcessPlayerInputSystem,
  ProcessPlayerSystem,
  GUIInputSystem,
  BackToMainMenuSystem,
  BackToWinLoseScreenSystem,
  InstructionPageSystem,
  UIButtonMovementSystem,
  UIBarFillSystem,
  LevelSelectorUpdateSystem,
  HowToPlaySystem,
  GameInteractablePopupSystem
>;
}