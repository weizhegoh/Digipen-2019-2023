/*****************************************************************
*\file         AnimationStateSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "AnimationStateSystem.h"
#include "Xivi/Core/Declaration.h"
#include <Resource/Manager.h>
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Components/GraphicsComponents.h"
#include "Components/EngineComponents.h"
#include "Systems/Audio/Audio.h"
#include "../Sundown/Source/NComponents/Entity/Player.h"
namespace Xivi::Graphics
{
void AnimationStateSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
}

void AnimationStateSystem::operator()(
  Player &player,
  AnimationState &anim_state,
  AnimatorComponent &animator,
  AnimationMeshComponent &mesh ) noexcept
{
  if ( m_paused )
    return;

  if ( anim_state._animCurrentState == AnimationState::AnimState::anim_jump_start && animator.endFrame )
    anim_state._animCurrentState = AnimationState::AnimState::anim_jump_loop;
  if ( anim_state._animCurrentState == AnimationState::AnimState::anim_jump_end && animator.endFrame )
    anim_state._animCurrentState = AnimationState::AnimState::anim_idle;
  auto &timer = Service::Locator::Get<TimerEngine>();

  if ( anim_state._animCurrentState == AnimationState::AnimState::anim_walking )
  {
    if ( ( anim_state.stepcounter <= 0.f && anim_state.stepcounter < anim_state.walkingStep1 ) && anim_state.walkonce )
    {
      Audio::AudioManager::Instance().playSFX( "SFX_MAINSTEP.ogg", false );
      anim_state.walkonce = false;
    }
    else if ( anim_state.stepcounter >= anim_state.walkingStep2 && !anim_state.walkonce )
    {
      Audio::AudioManager::Instance().playSFX( "SFX_SUBSTEP.ogg", false );
      anim_state.stepcounter = 0.f;
      anim_state.walkonce = true;
    }
    else
      anim_state.stepcounter += timer.GetScaledDeltaTime();
  }

  if ( anim_state._animCurrentState == AnimationState::AnimState::anim_running )
  {
    if ( ( anim_state.stepcounter <= 0.f && anim_state.stepcounter < anim_state.runStep1 ) && anim_state.walkonce )
    {
      Audio::AudioManager::Instance().playSFX( "SFX_MAINSTEP.ogg", false );
      anim_state.walkonce = false;
    }
    else if ( anim_state.stepcounter >= anim_state.runStep2 && !anim_state.walkonce )
    {
      Audio::AudioManager::Instance().playSFX( "SFX_SUBSTEP.ogg", false );
      anim_state.stepcounter = 0.f;
      anim_state.walkonce = true;
    }
    else
      anim_state.stepcounter += timer.GetScaledDeltaTime();
  }

  auto &assetManager = Service::Locator::Get<RM>();
  if ( !m_graphic_manager->win && m_graphic_manager->gearCount >= 10 )
    anim_state._animCurrentState = AnimationState::AnimState::anim_dab;
  if ( anim_state._animCurrentState == AnimationState::AnimState::anim_dab && animator.endFrame )
  {
    m_graphic_manager->win = true;
    anim_state._animCurrentState = AnimationState::AnimState::anim_idle;
  }

  if ( anim_state._animCurrentState != anim_state._animPrevState )
  {
    if ( anim_state._animCurrentState == AnimationState::AnimState::anim_idle )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/idle.BinaryAnimated" );
      animator.speed = 2.f;
      anim_state.run = false;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_walking )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/rat_walk.BinaryAnimated" );
      animator.speed = 1.f;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_jump_start )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/jump_start.BinaryAnimated" );
      animator.speed = 2.f;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_jump_loop )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/jump_loop.BinaryAnimated" );
      animator.speed = 2.0f;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = true;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_jump_end )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/jump_end.BinaryAnimated" );
      animator.speed = 2.f;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_running )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/sprint.BinaryAnimated" );
      animator.speed = 1.f;
      animator.numRun = -1;
      animator.isActive = true;
      //animator.once = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_dab )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/wakemeup.BinaryAnimated" );
      animator.speed = 2.f;
      anim_state.run = false;
      animator.numRun = -1;
      animator.isActive = true;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_push )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/grab_push2.BinaryAnimated" );
      animator.speed = 3.f;
      anim_state.run = false;
      animator.numRun = -1;
      animator.isActive = true;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_clone )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/cast_clone.BinaryAnimated" );
      animator.speed = 4.f;
      animator.numRun = -1;
      animator.isActive = true;
      //anim_state.run = false;
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_open_switch )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/button_slap.BinaryAnimated" );
      animator.speed = 5.f;
      anim_state.run = false;
      animator.numRun = 1;
      animator.isActive = true;
      animator.nextAnimation = "Assets/Models/binary/Animation/idle.BinaryAnimated";
    }
    else if ( anim_state._animCurrentState == AnimationState::AnimState::anim_close_switch )
    {
      mesh.model = assetManager.Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/button_slap_rev.BinaryAnimated" );
      animator.speed = 5.f;
      anim_state.run = false;
      animator.numRun = 1;
      animator.isActive = true;
      animator.nextAnimation = "Assets/Models/binary/Animation/idle.BinaryAnimated";
    }

    animator.animator.PlayAnimation( &mesh.model->animation );
    anim_state._animPrevState = anim_state._animCurrentState;
  }
}
}