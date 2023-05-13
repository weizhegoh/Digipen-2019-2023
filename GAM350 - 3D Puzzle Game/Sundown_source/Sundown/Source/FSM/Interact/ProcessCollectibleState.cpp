/*****************************************************************
*\file         ProcessCollectibleState.cpp
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
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
void ProcessCollectibleState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto &player = world.GetComponent<Player>( pc.playerEntity );
  world.SendGlobalEvent<UpdateGUICollectableEvent>( player.collectibles_total - ++player.collectibles_count );
  world.SendGlobalEvent<CloseCheesePopupDialogue>();

  // Process Animation;
  Audio::AudioManager::Instance().playSFX( "SFX_COLLECTIBLE.ogg", false );

  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
}

void ProcessCollectibleState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessCollectibleState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessCollectibleState::Instance()
{
  static ProcessCollectibleState s;
  return s;
}
}