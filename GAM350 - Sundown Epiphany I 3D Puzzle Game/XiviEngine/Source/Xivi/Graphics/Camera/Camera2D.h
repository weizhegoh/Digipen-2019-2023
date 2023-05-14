/*****************************************************************
*\file         Camera2D.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"

namespace Xivi::Graphics
{
class Camera2D
{
  Vec3f mTarget2D;
  Vec3f mUpVec;
  Vec3f mCentre;
public:
  Math::Matrix4 mView2D;
  Math::Matrix4 mProj2D;
  void SetupCamera2D(
    const Vec3f position,
    const Vec3f centre,
    const Vec3f up );

  Math::Matrix4  ViewTransform();
  Math::Matrix4  ProjectionTransfrom2D( const float width, const float height );
  void SetupTransfrom2D( float width, float height );
  void Viewport( const float width, const float height );
};
}