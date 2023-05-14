/*****************************************************************
*\file         DebugShape.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "DebugShape.h"
namespace Xivi::Graphics
{
DebugShape::DebugShape()
{
  mColor = Math::Vec3( .6f );
  mMask = (unsigned int) -1;
  mTimer = 0;
  mOnTop = false;
  mTransform.identity();
}

DebugShape &DebugShape::Color( const Math::Vector3 &color )
{
  mColor = color;
  return *this;
}

DebugShape &DebugShape::OnTop( bool state )
{
  mOnTop = state;
  return *this;
}

DebugShape &DebugShape::Time( float time )
{
  mTimer = time;
  return *this;
}

DebugShape &DebugShape::SetMaskBit( int bitIndex )
{
  mMask = 1 << bitIndex;
  return *this;
}

DebugShape &DebugShape::SetTransform( const Math::Matrix4 &transform )
{
  mTransform = transform;
  return *this;
}
}