/*****************************************************************
*\file         IdleState.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "State.h"
#include "NComponents.h"

#ifndef XV_EDITOR
#include "Xivi/Graphics/Manager/GameStateManager.h"
#endif

namespace Xivi::Game
{
void IdleState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  pc.lock_control = false;

  #ifndef XV_EDITOR

  if ( god_mode )
    return;

  auto &gsm = Service::Locator::Get<GSM::GSManager>();

  if ( !gsm.isMainMenu() )
    world.GetEntity( pc.playerEntity, [&]( Player &player )
  {
    if ( player.moves_total - player.moves_count <= 0 )
      state_component.Switch( ProcessGameOverState::Instance(), UpdateStatus::Begin );
  } );
  #endif
}

void IdleState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void IdleState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &IdleState::Instance()
{
  static IdleState s;
  return s;
}
}