/*****************************************************************
*\file         ProcessHoleState.cpp
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

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
#include "Xivi/Core/Declaration.h"
#include <Resource/Manager.h>
namespace Xivi::Game
{
void ProcessHoleState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
}

void ProcessHoleState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessHoleState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessHoleState::Instance()
{
  static ProcessHoleState s;
  return s;
}
}