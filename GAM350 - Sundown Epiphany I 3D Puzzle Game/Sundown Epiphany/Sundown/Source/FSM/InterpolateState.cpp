/*****************************************************************
*\file         InterpolateState.cpp
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "State.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "NComponents.h"

namespace Xivi::Game
{
void InterpolateState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
}

void InterpolateState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  bool end { true };

  #pragma region Scale
  /**
   * @brief
   * Scale
   */
  ECS::Query::Instance scale;
  scale.AllOf<Scale>();
  world.ForEach( scale, [&]( Entity &entity, Position &pos, Direction &dir, Transform &transform, NewTransform &newTransform )
  {
    // Interpolate logic
    newTransform.ratio = Math::Clamp( newTransform.ratio + Service::Locator::Get<TimerEngine>().GetDeltaTime() * newTransform.speed );
    transform.scale = Math::SIMD::lerp( transform.scale, newTransform.scale, newTransform.ratio );
    auto len = ( transform.scale - newTransform.scale ).lengthSq();
    if ( len <= 1.f )
    {
      transform.scale = newTransform.scale;
      world.AddOrRemoveComponents<CList<>, CList<Scale>>( entity );
    }
    else
      end = false;
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
  #pragma endregion

  #pragma region Rotate
  /**
   * @brief
   * Rotate
   */
  ECS::Query::Instance rotate;
  rotate.AllOf<Rotate>();
  world.ForEach( rotate, [&]( Entity &entity, Direction &dir, Transform &transform )
  {
    transform.ratio = Math::Clamp( transform.ratio + Service::Locator::Get<TimerEngine>().GetDeltaTime() * dir.speed );
    //transform.axis = Math::Slerp(transform.axis, transform.axisOffset, transform.ratio);
    transform.axis = Math::SIMD::lerp( transform.axis, transform.axisOffset, transform.ratio );
    auto len = ( transform.axis - transform.axisOffset ).lengthSq();
    if ( len <= 1.f )
    {
      transform.axis = transform.axisOffset;
      setX( dir.m_dir, -std::sin( Math::DegToRad( transform.axis.getY() ) ) );
      setY( dir.m_dir, 0.f );
      setZ( dir.m_dir, std::cos( Math::DegToRad( transform.axis.getY() ) ) );
      world.AddOrRemoveComponents<CList<>, CList<Rotate>>( entity );
    }
    else
      end = false;
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
  #pragma endregion

  #pragma region Move
  /**
   * @brief
   * Move
   */
  ECS::Query::Instance move;
  move.AllOf<Move>();
  world.ForEach( move, [&]( Entity &entity, Position &position, NewPosition &new_pos )
  {
    new_pos.ratio = Math::Clamp( new_pos.ratio + Service::Locator::Get<TimerEngine>().GetDeltaTime() * new_pos.speed );
    if ( world.HaveComponent<BezierTag>( entity ) )
    {
      auto c1 = Math::SIMD::lerp( new_pos.bezier_point0, new_pos.bezier_point1, new_pos.ratio );
      auto c2 = Math::SIMD::lerp( new_pos.bezier_point1, new_pos.value, new_pos.ratio );
      position.m_position = Math::SIMD::lerp( c1, c2, new_pos.ratio );
    }
    else
      position.m_position = Math::SIMD::lerp( position.m_position, new_pos.value, new_pos.ratio );

    auto len = ( new_pos.value - position.m_position ).lengthSq();

    if ( len <= 1.f )
    {
      position.m_position = new_pos.value;
      world.AddOrRemoveComponents<CList<>, CList<Move, BezierTag>>( entity );
    }
    else
      end = false;
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
  #pragma endregion

  #pragma region Animation
  /**
   * @brief
   * Move
   */
  ECS::Query::Instance animating;
  animating.AllOf<Animating>();
  world.ForEach( animating, [&]( Entity &entity, AnimatorComponent &animator )
  {
    //if (!animator.endFrame )
    //  XV_CORE_WARN("Not Ending");
    if ( !animator.endFrameAni )
      end = false;
    else
      world.AddOrRemoveComponents<CList<>, CList<Animating>>( entity );
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
  #pragma endregion

  if ( end )
  {
    world.ForEach( [&]( Entity &entity, AnimatorComponent &animator )
    {
      if ( animator.endFrameAni )
        animator.endFrameAni = false;
    } );

    state_component.Set( *state_component.next, state_component.next_status, IdleState::Instance(), UpdateStatus::Begin );
  }
}

void InterpolateState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
}

State &InterpolateState::Instance()
{
  static InterpolateState s;
  return s;
}
}