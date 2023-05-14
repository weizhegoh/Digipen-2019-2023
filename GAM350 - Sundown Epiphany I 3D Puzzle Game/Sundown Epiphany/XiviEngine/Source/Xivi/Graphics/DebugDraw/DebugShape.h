/*****************************************************************
*\file         DebugShape.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Shapes/Shapes.h"
namespace Xivi::Graphics
{
class DebugShape
{
public:
  DebugShape();
  DebugShape &Color( const Math::Vector3 &color );
  // Does this shape ignore depth test?
  DebugShape &OnTop( bool state );
  DebugShape &Time( float time );
  DebugShape &SetMaskBit( int bitIndex );
  DebugShape &SetTransform( const Math::Matrix4 &transform );

  std::vector<Shapes::LineSegment> mSegments;
  float mTimer;
  unsigned int mMask;
  Math::Vec3 mColor;
  bool mOnTop;
  Math::Matrix4 mTransform;

private:
};
}