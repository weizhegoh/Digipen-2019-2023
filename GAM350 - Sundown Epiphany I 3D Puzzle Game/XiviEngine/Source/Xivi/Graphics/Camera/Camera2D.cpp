/*****************************************************************
*\file         Camera2D.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Camera2D.h"
#include "../Helper/GraphicHelper.h"
namespace Xivi::Graphics
{
void Camera2D::SetupCamera2D(
  const Vec3f position,
  const Vec3f centre,
  const Vec3f up )
{
  mTarget2D = position;
  mCentre = centre;
  mUpVec = up;
  mView2D = Math::Matrix4( 1.f );
  mProj2D = Math::Matrix4( 1.f );
}
Math::Matrix4 Camera2D::ViewTransform()
{
  return Math::Helper::MathFunction::LookAt( mTarget2D, mCentre, mUpVec );
}
Math::Matrix4 Camera2D::ProjectionTransfrom2D( const float width, const float height )
{
  return Math::Helper::MathFunction::Ortho( ( -width ), float( width ), float( -height ), float( height ) );
}
void Camera2D::SetupTransfrom2D( float width, float height )
{
  mView2D = ViewTransform();
  mProj2D = ProjectionTransfrom2D( width, height );
}
void Camera2D::Viewport( const float width, const float height )
{
  glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
}
}