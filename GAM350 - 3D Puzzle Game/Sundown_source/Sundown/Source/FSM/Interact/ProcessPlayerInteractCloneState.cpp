/*****************************************************************
*\file         ProcessPlayerInteractCloneState.cpp
*\author(s)    Kenric Tan Wei Liang

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
#include "Components/ParticleComponents.h"
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
using GridType = CList<GridPosition, EntityDirection>;

using PlayerTypeClone = CList<
  Position,
  Transform,
  AnimationMeshComponent,
  AnimationState,
  AnimatorComponent,
  Material,
  Colour,
  RenderSettings,
  Direction,
  AABB,
  DirectionInitialisationTag,
  LightData, ParticleSettings, ParticleEmitter
>
::Extend<GridType>;

static bool create_clone { false };

static Position original_pos;
static GridPosition original_grid_pos;
static Direction original_direction;
static Transform original_transform;

static unordered_map<Vec3i, Vec3f, RotateHasher> player_rotate_map =
{
  { Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}},
  { Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}},
  { Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}},
  { Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}
};

void ProcessPlayerInteractCloneState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  bool process { true };
  bool valid = world.GetEntity( pc.cloneEntity,
                                [&]( GridPosition &grid_pos_clone )
  {
    auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
    auto ppEntity = grid.GetCellEntity( grid_pos_clone.value + Vec3i( 0, -1, 0 ) );

    world.GetEntity( pc.playerEntity,
                     [&]( GridPosition &grid_pos_player )
    {
      if ( grid_pos_clone.value == grid_pos_player.value )
      {
        process = false;
        state_component.SetCurrent( IdleState::Instance(), UpdateStatus::Begin );
      }
    } );
  } );

  create_clone = valid ? false : true;

  if ( process )
  {
    world.AddOrRemoveComponents<CList<Move, Rotate, Animating, BezierTag> >(
      pc.playerEntity,
      [&]( Position &pos, NewPosition &new_pos, GridPosition &grid_pos, Direction &dir, Transform &transform, AnimationState &anim_state )
    {
      original_pos = pos;
      original_grid_pos = grid_pos;
      original_direction = dir;
      original_transform = transform;

      // Handle Jump backward
      auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
      new_pos.bezier_point0 = pos.m_position;
      new_pos.bezier_point1 = ( grid.WorldSpace( pc.previous_tile ) + pos.m_position ) / 2.f;
      new_pos.bezier_point1.setY( new_pos.bezier_point1.getY() + 25.f );
      new_pos.value = grid.WorldSpace( pc.previous_tile ) + grid_pos.offset;
      new_pos.ratio = 0.f;

      // Handle Rotation
      auto vec3_dir = grid_pos.value - pc.previous_tile;
      transform.axisOffset = player_rotate_map[vec3_dir];
      transform.ratio = 0.f;

      // Handle Animation
      anim_state.UpdateState( AnimationState::AnimState::anim_clone );
      Audio::AudioManager::Instance().playSFX( "SFX_CLONE.ogg", false );
      world.SendGlobalEvent<ClosePressurePlatePopupDialogue>();

      // Update Grid
      grid.UpdateCell( grid_pos.value, pc.previous_tile );
      grid_pos.value = pc.previous_tile;
      XV_CORE_INFO( "Original Pos x - {}, y - {}, z - {}", original_pos.m_position.getX(), original_pos.m_position.getY(), original_pos.m_position.getZ() );
      XV_CORE_INFO( "New Pos x - {}, y - {}, z - {}", new_pos.value.getX(), new_pos.value.getY(), new_pos.value.getZ() );
    } );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::Process );
  }
}

void ProcessPlayerInteractCloneState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  if ( !create_clone )
    world.GetEntity( pc.cloneEntity,
                     [&]( Position &pos_clone,
                     GridPosition &grid_pos_clone,
                     Direction &dir_clone,
                     ParticleEmitter &emiiter,
                     ParticleSettings &settings )
  {
    auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
    auto ppEntity = grid.GetCellEntity( grid_pos_clone.value + Vec3i( 0, -1, 0 ) );

    world.GetEntity( pc.playerEntity,
                     [&]( Position &pos_player,
                     GridPosition &grid_pos_player,
                     Direction &dir_player
    )
    {
      for ( size_t i = 0; i < settings.count; ++i )
      {
        auto &particle = emiiter.particles[i];
        //particle.position.m_position = pos_player.m_position;
      }

      pos_clone = original_pos;
      grid_pos_clone = original_grid_pos;
      dir_clone = original_direction;
      pc.changeClone = true;
      auto [ppchild, ppswitch_control] = world.GetComponents<Child, Switch>( ppEntity );
      if ( world.HaveComponent<Child>( ppEntity ) )
      {
        world.AddOrRemoveComponents<CList<Activated, Animating>>( ppswitch_control.linked_entity );
        bool no_pressure_plate_obj { true };
        for ( auto &entity : ppchild.m_entities ) // Get pressure plate entity
          if ( world.HaveComponent<PressurePlateObj>( entity ) )
          {
            no_pressure_plate_obj = false;
            world.GetComponent<AnimatorComponent>( entity ).ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/PressurePlate_PressedReverse.BinaryAnimated" );
            Audio::AudioManager::Instance().playSFX( "SFX_PLATERELEASE.ogg", false );
            state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::End );
          }

        if ( no_pressure_plate_obj )
        {
          XV_CORE_WARN( "[ProcessPlayerInteractCloneState] No Pressure Plate Object" );
          state_component.Switch( IdleState::Instance(), UpdateStatus::Begin );
        }
      }
    } );
  } );
  else // Create  Pressure Plate Clone
  {
    auto &archetype = world.GetOrCreateArchetype<PlayerTypeClone>( "PlayerClone" );
    pc.cloneEntity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, AnimationMeshComponent &mesh,
                                             Material &material, Colour &colour, RenderSettings &renderSettings, AABB &aabb,
                                             Direction &dir, GridPosition &grid_pos, AnimatorComponent &animator, ParticleSettings &particleSettings, ParticleEmitter &emiiter ) noexcept
    {
      particleSettings.count = 64;
      particleSettings.rateOfSpawn = 64;
      particleSettings.rateOverDistance = 10;
      particleSettings.spawnInterval = 1.f;
      particleSettings.direction = InBETWN_Vec3( Vec3f( -1, 0, -1 ), Vec3f( 1, 1, 1 ) );
      particleSettings.speed = InBETWN_Float( 100.f, 100.f );
      particleSettings.lifetime = InBETWN_Float( 1.f, 0.1f );
      particleSettings.starttime = InBETWN_Float( 0.f, 0.f );
      particleSettings.transparency = InBETWN_Float( 0.5f, 0.5f );
      particleSettings.scale = InBETWN_Float( 15.f, 3.5f );
      particleSettings.colour = Vec3f( 1.f, 1.f, 1.f );

      particleSettings.positionOffSet = Vec3f( 0.f, original_pos.m_position.getY() - original_transform.scale.getY(), 0.f );
      particleSettings.positionOffSet2 = Vec3f( 0.f, original_pos.m_position.getY() - original_transform.scale.getY(), 0.f );
      emiiter.isActive = true;

      dir = original_direction;
      grid_pos = original_grid_pos;
      pos = original_pos;
      xform.scale = original_transform.scale;
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/ratUV2.dds" );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/ratUV2.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      renderSettings.transparent = 0.90f;
      //entity_type.value = GameType::Player;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/idle.BinaryAnimated" );
      animator.animator.PlayAnimation( &mesh.model->animation );
      animator.numRun = -1;
      animator.UpdateAnimation( 0 );
      animator.isActive = true;
    } );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::End );
  }
}

void ProcessPlayerInteractCloneState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto [transformPlayer, posPlayer, gridPos, playerAnimator, playerDir, playerAnimationState] = world.GetComponents<
    Transform, Position, GridPosition, AnimatorComponent, Direction, AnimationState>( pc.playerEntity );

  if ( playerAnimationState._animCurrentState == AnimationState::AnimState::anim_clone && playerAnimator.endFrame && !pc.changeClone )
  {
    playerAnimationState.UpdateState( AnimationState::AnimState::anim_idle );
    state_component.SetCurrent( IdleState::Instance(), UpdateStatus::Begin );
  }
  else if ( playerAnimationState._animCurrentState == AnimationState::AnimState::anim_clone && playerAnimator.endFrame && pc.changeClone )
  {
    playerAnimationState.UpdateState( AnimationState::AnimState::anim_idle );
    state_component.Set( ProcessUnlockState::Instance(), UpdateStatus::Begin, IdleState::Instance(), UpdateStatus::Begin );
    pc.changeClone = false;
  }
}

State &ProcessPlayerInteractCloneState::Instance()
{
  static ProcessPlayerInteractCloneState s;
  return s;
}
}