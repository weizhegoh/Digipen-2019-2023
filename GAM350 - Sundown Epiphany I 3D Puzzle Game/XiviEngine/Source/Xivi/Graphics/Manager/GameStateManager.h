/*****************************************************************
*\file         GameStateManager.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "GameState.h"
#include "Xivi/Graphics/CutScene/CutSceneList.h"
#include <unordered_map>
#include "Xivi/Core/Declaration.h"

namespace Xivi::GSM
{
class GSManager
{
  std::unordered_map<GS_STATES, std::string> stateLoadFile;
  std::map<GS_STATES, std::string> stateList;
  unsigned currentIndex;
  Ptr<ECS::World::Instance> world;
  Ptr<RM> resource_manager;

  const static u32 s_number_of_level = 8;

  void UpdateListByIndex( u32 ) noexcept;

public:
  Graphics::CutSceneFunc cutSceneFunc;
  unsigned mGS_current;
  unsigned mGS_previous;
  unsigned mGS_next;
  void InitManager( Ptr<ECS::World::Instance> w, Ptr<RM> r, GS_STATES state );
  bool isQuit();
  bool isSameState();
  bool isRestart();
  void ChangeState( GS_STATES state );
  void NextStage();
  bool isMainMenu();
  // for loop use don't use at other side
  void UpdateState();
  void SetCurrentNext_toPrev();
  GS_STATES GetPreviousState();
  GS_STATES GetNextStage( GS_STATES state );

  void InitState();
  void InitScene();
  std::string GetSelectedStateFileName( GS_STATES state );
  std::string GetCurrentStateFileName();
  std::string GetCurrentStateAudioFileName();
  void UpdateList();
  unsigned GetCurrentIndex()
  {
    return currentIndex;
  }
  bool runOnce = true;
  i32 collectables_collected = 0;
  i32 move_left = 0;
  bool once = true;
  bool retry = false;
  std::array<i32, s_number_of_level> list_collectable_collected;
  std::array<i32, s_number_of_level> list_move_left;
  std::array<i32, s_number_of_level> list_collectable_highscore;
  std::array<i32, s_number_of_level> list_move_highscore;
};
}