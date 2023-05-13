/*****************************************************************
*\file         Framebuffer.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Camera/Camera3D.h"

namespace Xivi::Graphics
{
class Camera3D;
class FrameBuffer
{
  unsigned int framebuffer;
  unsigned int texture;
  unsigned int depthMapFBO = NULL;
  unsigned int depthMap = NULL;
  std::vector<Camera3D> camera3d;
  bool m_splitScreen;

public:

  float fb_width;
  float fb_Height;
  bool first = true;
  FrameBuffer(
    const float width, const float height,
    const bool splitscreen,
    float _fov, float _near, float _far );
  void Setup(
    const float _width, const float _height,
    const bool splitScreen,
    const float _fov, const float _near, const float _far );

  void Bind() const;
  void Unbind() const;
  const unsigned int &Image()
  {
    return texture;
  }
  Camera3D &Camera( const size_t index );
  std::vector<Camera3D> &CameraList()
  {
    return camera3d;
  }
  void Viewport( const size_t cameraIndex );
  bool isScreenSplit()
  {
    return m_splitScreen;
  }
  unsigned int GetFrameBufferID()
  {
    return framebuffer;
  }
  unsigned int GetDepthMapFBO()
  {
    return depthMapFBO;
  }
  unsigned int GetDepthMap()
  {
    return depthMap;
  }
  void CreateShadowDepthMap();
  //void VieportValues(float width, float height);

private:
  void Create( const float width, const float height );
};
}