/*****************************************************************
*\file         Camera_c.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Graphics/Camera/Camera3D.h"

namespace Xivi
{
struct Camera_c
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Camera"
  };
  bool mFreeCam = false;
  bool dontFollow = false;

  Vec3f offset;
  Vec3f mTarget;
  Vec3f mTargetOffet;
  float mPhi_initial = 120.f;
  float mTheta_initial = 0.f;
  float SetPhi = 0.f;
  float SetTheta = 0.f;

  float distance = 300.f;
  float newDistance = 300.f;
  float maxDistance = 300.f;
  float minDistance = 100.f;

  float sentivity = 0.05f;
  float toGoalSentivity = 0.40f;
  float tGoalbackSentivity = 0.01f;

  float mFar = 10000.f;
  float mNear = 0.1f;
  float mFov = 45.f;

  float cameraX = 0.f;
  float cameraY = 0.f;

  bool toGoal = false;
  float currentDuration = 0.f;
  float viewDuration = 0.f;
  Vec3f bezierOffset = 0.f;
  float bezieHeightOffset = 50.f;
  Vec3f tempTarget = 0.f;
  float ratio = 0.f;
  void MovetoTarget( Vec3f position, Vec3f BezierOffset )
  {
    toGoal = true;
    mTargetOffet = position;
    bezierOffset = BezierOffset;
    currentDuration = viewDuration;
    tempTarget = mTarget;
  }
  void MoveUp( f32 step = 1.f )
  {
    cameraY -= step;
  }
  void MoveDown( f32 step = 1.f )
  {
    cameraY += step;
  }
  void MoveLeft( f32 step = 1.f )
  {
    cameraX += step;
  }
  void MoveRight( f32 step = 1.f )
  {
    cameraX -= step;
  }
  void UpdateDirPos( float X, float Y )
  {
    cameraX += X; cameraY += Y;
  }
  void ZoomIn()
  {
    newDistance = minDistance;
  }
  void ZoomOut()
  {
    newDistance = maxDistance;
  }
  void ZoomTo( const float dis )
  {
    newDistance = dis;
  }
};
}

namespace Xivi::Components::Camera::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Camera_c>( std::string( Xivi::Camera_c::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Camera_c>.m_id ) )
  .property( "Enable Movement", &Xivi::Camera_c::dontFollow )( rttr::policy::prop::as_reference_wrapper )
  .property( "Camera Target", &Xivi::Camera_c::mTarget )( rttr::policy::prop::as_reference_wrapper )
  .property( "Camera Target offset", &Xivi::Camera_c::mTargetOffet )( rttr::policy::prop::as_reference_wrapper )
  .property( "Initial Phi", &Xivi::Camera_c::mPhi_initial )( rttr::policy::prop::as_reference_wrapper )
  .property( "Initial Theta", &Xivi::Camera_c::mTheta_initial )( rttr::policy::prop::as_reference_wrapper )
  .property( "Current Phi", &Xivi::Camera_c::SetPhi )( rttr::policy::prop::as_reference_wrapper )
  .property( "Current Theta", &Xivi::Camera_c::SetTheta )( rttr::policy::prop::as_reference_wrapper )
  .property( "distance", &Xivi::Camera_c::distance )( rttr::policy::prop::as_reference_wrapper )
  .property( "Maximum Zoom In Distance", &Xivi::Camera_c::maxDistance )( rttr::policy::prop::as_reference_wrapper )
  .property( "Minimum Zoom In Distance", &Xivi::Camera_c::minDistance )( rttr::policy::prop::as_reference_wrapper )
  .property( "Fov", &Xivi::Camera_c::mFov )( rttr::policy::prop::as_reference_wrapper )
  .property( "Far", &Xivi::Camera_c::mFar )( rttr::policy::prop::as_reference_wrapper )
  .property( "Near", &Xivi::Camera_c::mNear )( rttr::policy::prop::as_reference_wrapper )
  .property( "Movement Sentivity", &Xivi::Camera_c::sentivity )( rttr::policy::prop::as_reference_wrapper )
  .property( "To Goal Sentivity", &Xivi::Camera_c::toGoalSentivity )( rttr::policy::prop::as_reference_wrapper )
  .property( "To Goal back to char Sentivity", &Xivi::Camera_c::tGoalbackSentivity )( rttr::policy::prop::as_reference_wrapper )
  .property( "View Duration", &Xivi::Camera_c::viewDuration )( rttr::policy::prop::as_reference_wrapper )
    .property( "Bezie Height Offset", &Xivi::Camera_c::bezieHeightOffset )( rttr::policy::prop::as_reference_wrapper )
    ;
}
}