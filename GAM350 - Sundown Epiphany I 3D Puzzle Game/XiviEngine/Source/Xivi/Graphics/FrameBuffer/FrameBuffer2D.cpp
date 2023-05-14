/*****************************************************************
*\file         FrameBuffer2D.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "FrameBuffer2D.h"
#include "FrameBufferOpenGL.h"
#include "../Camera/Camera2D.h"
namespace Xivi::Graphics
{
FrameBuffer2D::FrameBuffer2D(
  const float width, const float height,
  const Vec3f eye,
  const Vec3f center,
  const Vec3f up )
{
  fb_width = width;
  fb_Height = height;
  OpenGLFrameBuffer::Create( width, height, framebuffer, texture );
  camera2d.SetupCamera2D( eye, center, up );
}

Camera2D &FrameBuffer2D::Camera()
{
  return camera2d;
}

void FrameBuffer2D::Viewport( const float width, const float height )
{
  Camera().Viewport( width, height );
}

void FrameBuffer2D::Bind() const
{
  OpenGLFrameBuffer::Bind( framebuffer );
}

void FrameBuffer2D::Unbind() const
{
  OpenGLFrameBuffer::Unbind();
}
}