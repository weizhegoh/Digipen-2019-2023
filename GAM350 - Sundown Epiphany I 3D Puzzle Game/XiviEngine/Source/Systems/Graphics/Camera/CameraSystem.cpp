/*****************************************************************
*\file         CameraSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "CameraSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include "Components/GraphicsComponents.h"
#include "Components/EngineComponents.h"
#include "../Sundown/Source/NComponents/Entity/Player.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "../Sundown/Source/NComponents/Tags.h"

namespace Xivi::Graphics
{
void CameraSystem::OnCreate() noexcept
{
  //m_query.AllOf<StaticBodyTag>();
  m_query.NoneOf<UI, Camera_c>();
  goalQuery.AllOf<GoalObj>();
  setTargetQuery.AllOf<CameraSetTarget>();
  cameraInitQuery.AllOf<CameraInitalized>();
  autoOrbitQuery.AllOf<CameraAutoOrbit>();
  playerQuery.AllOf<Player>();
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  GetOrCreateSingletonComponent<GlobalAmbient>();
}

void CameraSystem::OnResume() noexcept
{
  #ifdef XV_EDITOR
  if ( m_graphic_manager->enableFreeCam )
  {
    auto &cameraLeft = m_framebuffer->CameraGame( 0, 0 );
    auto &editorCam = m_framebuffer->CameraEditor( 0 );
    cameraLeft.CameraEye() = editorCam.CameraEye();
    cameraLeft.AssignDir( editorCam.GetPhi(), editorCam.GetTheta() );
    return;
  }
  #endif
}

void CameraSystem::OnUpdate() noexcept
{
  auto &timer = Service::Locator::Get<TimerEngine>();
  auto &mouse = *Service::Locator::Get<Input::Manager>().Mouse();
  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
  auto mousePosition = mouse.Position();
  auto leftScreenMouseControl = (unsigned int) Input::MouseButton::Left;
  auto rightScreenMouseControl = (unsigned int) Input::MouseButton::Right;
  auto windowWidth = (float) Service::Locator::Get<GLFW_Window>().Width();
  auto windowHeight = (float) Service::Locator::Get<GLFW_Window>().Height();
  #ifdef XV_EDITOR
      //if (keyboard.RightControl().Triggered())
      //{
      //	m_graphic_manager->picked = false;
      //}

  #endif
  bool isFreeCam = !m_framebuffer->isUI && ( m_framebuffer->isEditor() || m_graphic_manager->enableFreeCam );
  m_framebuffer->CameraOnUpdateUI( windowWidth, windowHeight );
  if ( isFreeCam )
  {
    m_framebuffer->CameraGame( 0, 0 ).SetMode( Graphics::Camera3D::Fps );
    if ( !m_framebuffer->isEditor() )
      m_framebuffer->CameraOnUpdate(
      windowWidth,
      windowHeight,
      leftScreenMouseControl,
      rightScreenMouseControl,
      (int) mousePosition.X(), (int) mousePosition.Y(),
      true, true );
    return;
  }

  ForEach( cameraInitQuery, [&](
    Entity &entity,
    Position &position,
    Camera_c &camera_c )
  {
    auto &camera = m_framebuffer->CameraGame( 0, 0 );
    auto &cameraTarget = camera.CameraTarget();
    auto &cameraEye = camera.CameraEye();
    camera.ResetEye();
    camera.SetFov( camera_c.mFov );
    camera.SetFar( camera_c.mFar );
    camera_c.SetPhi = camera_c.mPhi_initial;
    camera_c.SetTheta = camera_c.mTheta_initial;
    camera_c.distance = camera_c.newDistance = camera_c.maxDistance;
    camera_c.dontFollow = false;
    camera.AssignDir( Math::DegToRad( camera_c.mPhi_initial ), Math::DegToRad( camera_c.mTheta_initial ) );
    camera_c.mTargetOffet = camera_c.mTarget;
    cameraTarget = camera_c.mTarget;
    cameraEye = position.m_position;
    camera_c.cameraX = camera_c.cameraY = 0.f;
    #ifdef XV_EDITOR
    AddOrRemoveComponents<CList<CameraSetTarget>, CList<CameraInitalized>>( entity );
    #else
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    if ( gsm.isMainMenu() )
      AddOrRemoveComponents<CList<>, CList<CameraInitalized>>( entity );
    else
      AddOrRemoveComponents<CList<CameraSetTarget>, CList<CameraInitalized>>( entity );
    #endif
  } );

  ForEach( setTargetQuery, [&](
    Entity &entity,
    Camera_c &camera_c )
  {
    Vec3f goalPos;
    Vec3f playerPos;
    ForEach( goalQuery, [&]( Position &position )
    {
      goalPos = position.m_position;
    } );
    ForEach( playerQuery, [&]( Position &position )
    {
      playerPos = position.m_position;
    } );
    auto center = ( goalPos + playerPos ) * 0.5f;
    center.setY( center.getY() + camera_c.bezieHeightOffset );
    camera_c.MovetoTarget( goalPos, center );
    //camera_c.ZoomIn();
    AddOrRemoveComponents<CList<>, CList<CameraSetTarget>>( entity );
  } );
  ForEach( autoOrbitQuery, [&](
    Entity &entity,
    Camera_c &camera_c )
  {
    camera_c.mTheta_initial += camera_c.toGoalSentivity;
    if ( abs( camera_c.mTheta_initial ) >= 360.f )
    {
      camera_c.mTheta_initial = 0.f;;
      camera_c.SetTheta = 0.f;
    }
    camera_c.SetTheta = Math::Lerp( camera_c.SetTheta, camera_c.mTheta_initial, camera_c.toGoalSentivity );
  } );

  ForEach( [&](
    Entity &entity,
    Position &position,
    Camera_c &camera_c )
  {
    auto &camera = m_framebuffer->Camera( 0, 0 );
    camera.SetMode( Graphics::Camera3D::Orbit );

    //camera.ProcessGameMouseInput(
    //	(int)camera_c.cameraX, (int)camera_c.cameraY, camera_c.SetPhi, camera_c.SetTheta, true, camera_c.sentivity);
    camera.SetPhi( Math::DegToRad( camera_c.SetPhi ) );
    camera.SetTheta( Math::DegToRad( camera_c.SetTheta ) );
    auto dir = -camera.ComputeDirection().normalize();

    if ( camera_c.toGoal )
    {
      #ifndef XV_EDITOR
      auto &gsm = Service::Locator::Get<GSM::GSManager>();
      std::vector<unsigned int> image;
      auto &cs = gsm.cutSceneFunc;
      if ( !cs.isActive )
      {
        #endif
        camera_c.currentDuration -= timer.GetScaledDeltaTime();
        if ( camera_c.currentDuration <= 0.f )
        {
          camera_c.currentDuration = 0.f;
          camera_c.toGoal = false;
          camera_c.ratio = 0.f;
          camera_c.ZoomOut();
        }
        camera_c.ratio = Math::Clamp( camera_c.ratio + timer.GetDeltaTime() * camera_c.toGoalSentivity );
        camera_c.mTarget = Math::Helper::MathFunction::Bezier( camera_c.tempTarget, camera_c.bezierOffset, camera_c.mTargetOffet, camera_c.ratio );
        if ( ( camera_c.mTarget - camera_c.mTargetOffet ).lengthSq() < 1.f )
          camera_c.ZoomIn();

        #ifndef XV_EDITOR
      }
      #endif
    }
    else
    {
      ForEach( [&]( Player &player, Position &position )
      {
        camera_c.mTargetOffet = ( camera_c.dontFollow ) ? camera_c.mTarget : position.m_position;
      } );
      camera_c.mTarget = Math::SIMD::lerp( camera_c.mTarget, camera_c.mTargetOffet, camera_c.toGoalSentivity );
    }
    //camera_c.mTarget = Math::SIMD::lerp(camera_c.mTarget, camera_c.mTargetOffet, camera_c.toGoalSentivity);
    camera_c.distance = Math::Helper::MathFunction::LinearInterpolate( camera_c.distance, camera_c.newDistance, camera_c.sentivity );
    camera_c.offset = camera_c.mTarget + Math::Vector3( dir.getX(), dir.getY(), dir.getZ() ) * ( camera_c.distance );
    position.m_position = Math::SIMD::lerp( position.m_position, camera_c.offset, camera_c.sentivity );

    camera.SetPhi( camera_c.SetPhi );
    camera.SetTheta( camera_c.SetTheta );
    camera.SetFov( camera_c.mFov );
    camera.SetFar( camera_c.mFar );
    camera.SetNear( camera_c.mNear );

    auto &cameraTarget = camera.CameraTarget();
    auto &cameraEye = camera.CameraEye();
    cameraTarget = camera_c.mTarget; //player position
    cameraEye = position.m_position;
    camera.OrbitViewTransform();
    camera.PerspectiveTransform3D( windowWidth, windowHeight );
    return false;
  } );
}
}