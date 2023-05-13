/*****************************************************************
*\file         FrameBuffer.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "Framebuffer.h"
#include "../Gfx/Gfx.h"
#include "FrameBufferOpenGL.h"
namespace Xivi::Graphics
{
FrameBuffer::FrameBuffer(
  const float _width, const float _height,
  const bool splitScreen,
  const float _fov, const float _near, const float _far )
{
  Setup( _width, _height, splitScreen, _fov, _near, _far );
}

void FrameBuffer::Setup(
  const float _width, const float _height,
  const bool splitScreen,
  const float _fov, const float _near, const float _far )
{
  Create( _width, _height );
  fb_width = _width;
  fb_Height = _height;
  float viewPortSizeX = _width;
  float viewPortSizeY = _height;
  m_splitScreen = splitScreen;
  if ( m_splitScreen )
  {
    viewPortSizeX *= 0.5f;
    camera3d.push_back( Camera3D( 0, 0, viewPortSizeX, viewPortSizeY, _fov, _near, _far ) );
    camera3d.push_back( Camera3D( viewPortSizeX, 0, viewPortSizeX, viewPortSizeY, _fov, _near, _far ) );
  }
  else
  {
    camera3d.push_back( Camera3D( 0, 0, viewPortSizeX, viewPortSizeY, _fov, _near, _far ) );
  }
}

void FrameBuffer::Create( const float width, const float height )
{
  OpenGLFrameBuffer::Create( width, height, framebuffer, texture );
  XV_CORE_INFO( "Frambuffer Created" );
}

void FrameBuffer::CreateShadowDepthMap()
{
  glGenFramebuffers( 1, &depthMapFBO );
  glBindFramebuffer( GL_FRAMEBUFFER, depthMapFBO );
  glGenTextures( 1, &depthMap );
  glBindTexture( GL_TEXTURE_2D, depthMap );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );

  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

  // attach depth texture as FBO's depth buffer

  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0 );
  glDrawBuffer( GL_NONE );
  glReadBuffer( GL_NONE );

  if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
  {
    XV_CORE_WARN( "Unable Create Shadow Depth Map" );
    return;
  }

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void FrameBuffer::Bind() const
{
  OpenGLFrameBuffer::Bind( framebuffer );
}

void FrameBuffer::Viewport( const size_t cameraIndex )
{
  Camera( cameraIndex ).Viewport();
}

void FrameBuffer::Unbind() const
{
  OpenGLFrameBuffer::Unbind();
}

Camera3D &FrameBuffer::Camera( const size_t index )
{
  if ( index >= camera3d.size() )
    return camera3d[0];
  return camera3d[index];
}
}