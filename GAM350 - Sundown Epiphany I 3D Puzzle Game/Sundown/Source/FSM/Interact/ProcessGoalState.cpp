/*****************************************************************
*\file         ProcessGoalState.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "../State.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Components/ParticleComponents.h"
#include "NComponents.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
namespace Xivi::Game
{
void ProcessGoalState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [dir, rotDir, transform, grid_pos, newtransform] = world.GetComponents<Direction, EntityDirection, Transform, GridPosition, NewTransform>( pc.playerEntity );
  pc.currenturn;
  auto type = grid.GetCellType( grid_pos.value + Vec3i( 0, -1, 0 ) );
  if ( type == GameType::Goal )
  {
    auto ppEntity = grid.GetCellEntity( grid_pos.value + Vec3i( 0, -1, 0 ) );
    auto &child = world.GetComponent<Child>( ppEntity );
    for ( auto &entity : child.m_entities )
    {
      if ( world.HaveComponent<ParticleEmitter>( entity ) )
      {
        auto &emitter = world.GetComponent<ParticleEmitter>( entity );
        emitter.isActive = true;
      }
    }
  }
  Audio::AudioManager::Instance().playSFX( "DIALOGUE_DIMENSIONSUCC.ogg", false );
  transform.axisOffset.setY( 360 );
  transform.ratio = 0.f;
  newtransform.scale = 0.1f;
  newtransform.speed = 1.f;
  newtransform.ratio = 0.f;
  dir.speed = 1.f;
  //dir.ratio = 0.f;
  world.AddOrRemoveComponents<CList<Rotate>>( pc.playerEntity );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessGoalState::Instance(), UpdateStatus::Process );
  //TURN PLAYER
  //add next level
}

void ProcessGoalState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [dir, rotDir, transform, grid_pos, newTransform] = world.GetComponents<Direction, EntityDirection, Transform, GridPosition, NewTransform>( pc.playerEntity );

  if ( pc.currenturn <= 5 )
  {
    if ( transform.axis.getY() >= 360.0f )
    {
      transform.axis.setY( 0.f );
      transform.axisOffset.setY( 360.0f );
      newTransform.ratio = 0.f;
      pc.currenturn++;
    }
  }
  #ifndef XV_EDITOR
  else
  {
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    world.ForEach( [&]( Player &player )
    {
      gsm.collectables_collected = player.collectibles_count;
      gsm.move_left = player.moves_total - player.moves_count;
    } );
    if ( static_cast<GSM::GS_STATES>( gsm.mGS_current ) == GSM::LEVEL2_STAGE1 || static_cast<GSM::GS_STATES>( gsm.mGS_current ) == GSM::LEVEL2_STAGE2 )
      gsm.ChangeState( GSM::WIN2 );
    else
      gsm.ChangeState( GSM::WIN );
    // gsm.NextStage();
  }
  #endif
  transform.ratio = 0.f;
  world.AddOrRemoveComponents<CList<Rotate>>( pc.playerEntity );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessGoalState::Instance(), UpdateStatus::Process );
}

void ProcessGoalState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessGoalState::Instance()
{
  static ProcessGoalState s;
  return s;
}
}