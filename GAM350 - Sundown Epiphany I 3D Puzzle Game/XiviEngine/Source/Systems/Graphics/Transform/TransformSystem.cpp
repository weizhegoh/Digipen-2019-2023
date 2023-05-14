/*****************************************************************
*\file         TransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "Xivi/Core/Declaration.h"
#include <Resource/Manager.h>
#include "TransformSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
namespace Xivi::Graphics
{
void TransformSystem::OnCreate() noexcept
{
  m_query.NoneOf<UI>();
}
void TransformSystem::OnResume() noexcept
{
  ForEach( m_query, [&]( AnimatorComponent &animator )
  {
    animator.UpdateAnimation( 0 );
  } );
}
void TransformSystem::OnUpdate() noexcept
{
  auto &m_graphicManager = Service::Locator::Get<Graphics::Manager>();

  #ifndef XV_EDITOR
  auto &gs = Service::Locator::Get<GSM::GSManager>();
  if ( gs.cutSceneFunc.isActive )
  {
    ECS::Query::Instance uiCutsceneOnly;
    uiCutsceneOnly.AllOf<UI, CutSceneUI>();
    ForEach( uiCutsceneOnly, [&](
      ECS::Entity &entity, Position &pos, Transform &transform )
    {
      Math::Matrix4 model( 1.f );
      Math::Matrix4 scale( 1.f );
      Math::Matrix4 rotate( 1.f );
      Math::Vector3 offset = ( pos.shaky ) ? Math::Helper::RandFunction::RandNumberBetween( Math::Vector3( 1.f, 1.f, 1.f ), Math::Vector3( -1.f, -1.f, -1.f ) ) : Math::Vector3( 0.f, 0.f, 0.f );
      Math::Vector3 posTemp = pos.m_position + offset;
      Math::Vector3 scaleTemp = transform.scale;
      model = model.translate( posTemp.getX(), posTemp.getY(), posTemp.getZ() );
      Vec3f axisRad = Vec3f(
        Math::DegToRad( transform.axis.getX() ),
        Math::DegToRad( transform.axis.getY() ),
        Math::DegToRad( transform.axis.getZ() ) );
      rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
      scale = scale.scale( scaleTemp.getX(), scaleTemp.getY(), scaleTemp.getZ() );
      transform.LocalToWorld = ( model * rotate * scale );
    } );
    return;
  }
  #endif;

  auto &m_frameBuffer = Service::Locator::Get<Graphics::M_FrameBuffer>();
  auto &m_debugDrawer = m_graphicManager.GetDebugDraw();
  #ifdef XV_EDITOR
  auto &camera = m_frameBuffer.CameraEditor( 0 );
  #endif
  {
    //ECS::Query::Instance changeAnimation;
    //changeAnimation.AllOf<ChangeAnimation>();
    //ForEach(changeAnimation, [&](Entity& entity, AnimatorComponent& animator, AnimationMeshComponent& mesh)
    //	{
    //		auto& assetManager = Service::Locator::Get<RM>();
    //		mesh.model = assetManager.Get<Graphics::AnimationModel>(animator.nextAnimation);
    //		animator.animator.PlayAnimation(&mesh.model->animation);
    //		AddOrRemoveComponents<CList<ChangeAnimation>>(entity);
    //	});

    XV_PROFILE_ZONESCOPED_N( "Transform - Animator" );
    auto &timer = Service::Locator::Get<TimerEngine>();
    ECS::Query::Instance animatorInit;
    animatorInit.AllOf<AnimatorInitalized>();
    ForEach( animatorInit, [&](
      Entity &entity,
      AnimatorComponent &animator )
    {
      animator.UpdateAnimation( 0.f );
      AddOrRemoveComponents<CList<>, CList<AnimatorInitalized>>( entity );
    } );

    ForEach( [&]( Entity &entity, AnimatorComponent &animator, AnimationMeshComponent &mesh )
    {
      if ( animator.isActive )
      {
        animator.UpdateAnimation( timer.GetScaledDeltaTime() );
        if ( animator.numRun == -1 )
          return;
        if ( animator.currentRun == animator.numRun )
        {
          animator.isActive = false;
          animator.currentRun = 0;
          if ( !animator.nextAnimation.empty() )
          {
            auto &assetManager = Service::Locator::Get<RM>();
            mesh.model = assetManager.Get<Graphics::AnimationModel>( animator.nextAnimation );
            animator.animator.PlayAnimation( &mesh.model->animation );
            animator.nextAnimation.clear();
            animator.UpdateAnimation( 0 );
          }
        }

        if ( animator.endFrame )
          animator.currentRun++;
      }
    } );
  }
//#ifdef XV_EDITOR
  {
    //if (m_frameBuffer.isEditor())
    {
      XV_PROFILE_ZONESCOPED_N( "Transform - AABBUpdate" );
      ForEach( m_query, [&]( ECS::Entity &entity, Position &pos, Transform &transform, AABB &aabb )
      {
        auto &position = pos.m_position;
        if ( aabb.type == CollisionType::AABB || aabb.type == CollisionType::SPHERE )
        {
          auto &minScale = aabb.minScale;
          auto &maxScale = aabb.maxScale;
          Math::Vector3 scale = transform.scale;
          aabb.AABB_mMin = Vec3f {
            position.getX() - scale.getX() * minScale.getX(),
            position.getY() - scale.getY() * minScale.getY(),
            position.getZ() - scale.getZ() * minScale.getZ() };
          aabb.AABB_mMax = Vec3f {
            position.getX() + scale.getX() * maxScale.getX(),
            position.getY() + scale.getY() * maxScale.getY(),
            position.getZ() + scale.getZ() * maxScale.getZ() };

          aabb.AABB_center = pos.m_position;
        }
        if ( aabb.type == CollisionType::SPHERE )
        {
          aabb.Sphere_mCenter = pos.m_position;
          aabb.Sphere_mRadius = ( aabb.AABB_mMax - aabb.AABB_mMin ).length() / 2.f;
        }
        m_debugDrawer.DrawAabb( Graphics::Shapes::Aabb( aabb.AABB_mMin, aabb.AABB_mMax ) );
        return false;
      } );
    }
  }
//#endif
  SendEvent<ChildEvent>( this );
}

void TransformSystem::OnPostUpdate() noexcept
{
  //ECS::Query::Instance once;
  //once.AllOf<AnimationUpdateOnce>();
  //ForEach(once,[&](Entity& entity, AnimatorComponent& animator)
  //	{
  //		if (!animator.isActive)
  //		{
  //			animator.UpdateAnimation(0);
  //			AddOrRemoveComponents< CList<>, CList<AnimationUpdateOnce>>(entity);
  //		}
  //	});
}
}