/*****************************************************************
*\file         FrameBufferOpenGL.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <pch.h>
namespace Xivi::Graphics
{
class OpenGLFrameBuffer
{
public:
  static void Create(
    const float width, const float height,
    unsigned int &framebuffer,
    unsigned int &texture );
  static void Bind( unsigned int framebuffer );
  static void Unbind();
};
}