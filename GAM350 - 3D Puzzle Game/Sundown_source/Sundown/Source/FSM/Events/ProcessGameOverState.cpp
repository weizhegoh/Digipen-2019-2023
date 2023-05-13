/*****************************************************************
*\file         ProcessGameOverState.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "../State.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "NComponents.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"

namespace Xivi::Game
{
void ProcessGameOverState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  XV_CORE_WARN( "GameOver" );
  #ifndef XV_EDITOR

  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  if ( static_cast<GSM::GS_STATES>( gsm.mGS_current ) == GSM::LEVEL2_STAGE1 || static_cast<GSM::GS_STATES>( gsm.mGS_current ) == GSM::LEVEL2_STAGE2 )
    gsm.ChangeState( GSM::GAMEOVER2 );
  else
    gsm.ChangeState( GSM::GAMEOVER );
  #endif
}

void ProcessGameOverState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessGameOverState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessGameOverState::Instance()
{
  static ProcessGameOverState s;
  return s;
}
}