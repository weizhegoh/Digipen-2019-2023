/*****************************************************************
*\file         GUIComponents.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "GUI/CanvasTag.h"
#include "GUI/UICheckBox.h"
#include "GUI/UIDrag.h"
#include "GUI/UITrigger.h"
#include "GUI/UIHover.h"
#include "GUI/UIInteractableTag.h"
#include "GUI/GUIWindowTag.h"
#include "GUI/GUIActiveTag.h"
#include "GUI/UI.h"
#include "GUI/Tags.h"
#include "GUI/UIBar.h"
#include "GUI/UICheese.h"
#include "GUI/PopupDialogueComponents.h"

namespace Xivi
{
using GUI_CList = CList <
  GUI::CanvasTag,
  GUI::UICheckBox,
  GUI::UIDrag,
  GUI::UITrigger,
  GUI::UIHover,
  GUI::UIInteractableTag,
  GUI::GUIActiveTag,
  GUI::WindowTag,
  GUI::PauseCanvasTag,
  GUI::MainMenuCanvasTag,
  GUI::InstructionCanvasTag,
  GUI::SettingsCanvasTag,
  GUI::HowToPlayCanvasTag,
  GUI::UIBar,
  GUI::UICheese,
  GUI::SwitchPopupDialogue,
  GUI::PressurePlatePopupDialogue,
  GUI::VolumeBarTag,
  GUI::GammaBarTag,
  GUI::CreditsCanvasTag,
  GUI::SummaryCanvasTag,
  GUI::ExitGamePopupCanvasTag,
  GUI::WinCanvasTag,
  GUI::LoseCanvasTag,
  GUI::GoToMainMenuPopupCanvasTag,
  GUI::PressEscapeToMainMenuTag,
  GUI::PressEscapeToWinLoseScreenTag,
  GUI::GameInteratablePopup,
  GUI::LevelSelectorCanvasTag,
  GUI::L1_1Tag,
  GUI::L1_2Tag,
  GUI::L1_3Tag,
  GUI::L1_4Tag,
  GUI::L2_1Tag,
  GUI::L2_2Tag,
  GUI::LevelSelectorImage,
  GUI::LevelSelectorRat,
  GUI::MoveLeftTextTag,
  GUI::GUINumberOnes,
  GUI::GUINumberTens,
  GUI::GUINumberHundreds,
  GUI::GUICheeseGameMenu,
  GUI::TutorialButtonUITag,
  GUI::KeyboardButtonUITag,
  GUI::ControllerButtonUITag,
  GUI::CheesePopupDialogue,
  GUI::CartPopupDialogue,
  GUI::HolePopupDialogue,
  GUI::ObstaclePopupDialogue,
  GUI::LevelNumberGameUITag,
  UI
> ;
}