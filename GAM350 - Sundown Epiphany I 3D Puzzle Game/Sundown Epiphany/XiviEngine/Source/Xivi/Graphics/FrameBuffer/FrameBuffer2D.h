/*****************************************************************
*\file         FrameBuffer2D.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Camera/Camera2D.h"
namespace Xivi::Graphics
{
class Camera2D;
class FrameBuffer2D
{
  unsigned int framebuffer;
  unsigned int texture;
  Camera2D camera2d;
public:
  float fb_width;
  float fb_Height;
  FrameBuffer2D( const float width, const float height,
                 const Vec3f eye,
                 const Vec3f center,
                 const Vec3f up );
  Camera2D &Camera();
  const unsigned int &Image()
  {
    return texture;
  }
  unsigned int GetFrameBufferID()
  {
    return framebuffer;
  }
  void Viewport( const float width, const float height );
  void Bind() const;
  void Unbind() const;
};
}