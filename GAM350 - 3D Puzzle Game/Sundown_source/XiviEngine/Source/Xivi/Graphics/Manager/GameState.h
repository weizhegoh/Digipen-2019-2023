/*****************************************************************
*\file         GameState.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi::GSM
{
enum GS_STATES
{
  GS_QUIT = 0,
  GS_RESTART,
  MAINMENU,
  MAINMENU_NOLOGO,
  TUTORIAL_1,
  LEVEL1_STAGE1,
  TUTORIAL_2,
  LEVEL1_STAGE2,
  LEVEL1_STAGE3,
  LEVEL1_STAGE4,
  LEVEL2_STAGE1,
  LEVEL2_STAGE2,
  //END_CS,
  END,
  SETTING,
  GAMEOVER,
  GAMEOVER2,
  WIN,
  WIN2
};
}