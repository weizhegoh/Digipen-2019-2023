/*****************************************************************
*\file         FrameBufferOpenGL.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "FrameBufferOpenGL.h"
#include "../Gfx/Gfx.h"
namespace Xivi::Graphics
{
void OpenGLFrameBuffer::Create(
  const float width, const float height,
  unsigned int &framebuffer,
  unsigned int &texture )
{
  XV_CORE_INFO( "Creating Frambuffer" );
  glGenFramebuffers( 1, &framebuffer );
  glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  unsigned int rbo;
  glGenRenderbuffers( 1, &rbo );
  glBindRenderbuffer( GL_RENDERBUFFER, rbo );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei) width, (GLsizei) height );
  glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0 );
  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo );
  if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    XV_CORE_WARN( "Unable Create Frambuffer" );
}

void OpenGLFrameBuffer::Bind( unsigned int framebuffer )
{
  glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebuffer );
  Gfx::Renderer::ClearBitBuffer( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );
}

void OpenGLFrameBuffer::Unbind()
{
  glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // back to default
  glDisable( GL_DEPTH_TEST );
  Gfx::Renderer::ClearBitBuffer( GL_COLOR_BUFFER_BIT );
}
}