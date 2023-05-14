/*****************************************************************
*\file         FrameBuffers.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Framebuffer.h"

namespace Xivi::Graphics
{
class Camera3D;
class Camera2D;
//class FrameBuffer;
//class FrameBuffer2D;
class FrameBuffers
{
  std::vector<FrameBuffer> game_list;
  std::vector<FrameBuffer> editor_list;
  std::vector<FrameBuffer> UI_list;
public:
  enum class SplitScreen
  {
    Left = 0,
    Right = 1
  };
  enum class RenderList
  {
    Editor = 0,
    Game = 1,
    UI
  };
  void Create(
    const float width, const float height,
    const RenderList renderlist, const bool splitscreen,
    const float _fov = 45.f,
    const float _near = 0.1f,
    const float _far = 10000.f );

  std::vector<FrameBuffer> &GetList();
  std::vector<Camera3D> &CameraList( const size_t imageIndex );

  const FrameBuffer &operator[] ( const size_t index );
  const unsigned int &Image( const size_t index );
  const unsigned int &ImageGame( const size_t index );
  #ifdef XV_EDITOR
  const unsigned int &ImageEditor( const size_t index );
  Camera3D &CameraEditor( const size_t cameraIndex );
  #endif
  Camera3D &CameraGame( const size_t imageIndex, const size_t cameraIndex );
  Camera3D &Camera( const size_t imageIndex, const size_t cameraIndex );
  Camera3D &Camera( const size_t imageIndex );

  void CameraOnUpdate(
    float width, float height
    , unsigned int button, unsigned int button2,
    int mouseX, int mouseY, bool fps, bool in_viewport = false );

  void NextActiveFB();
  void ToggleList();
  void SetActiveList( RenderList list )
  {
    activeList = list;
  }
  bool isEditor()
  {
    return ( activeList == RenderList::Editor );
  }
  int GetActiveFB()
  {
    return (int) activeFameBuffer;
  }

  void CreateUI(
    const float width, const float height,
    const float _fov = 90.f,
    const float _near = 0.1f,
    const float _far = 1000.f );
  Camera3D &CameraUI();
  FrameBuffer &FrameBufferUI();
  const unsigned int &ImageUI( const size_t index );
  void CameraOnUpdateUI( float width, float height );

  std::vector<FrameBuffer> &GameFB()
  {
    return game_list;
  }
  std::vector<FrameBuffer> &UIFB()
  {
    return UI_list;
  }

  float cameraSentivity = 1.f;
  bool isUI = false;
private:
  RenderList activeList;
  unsigned int activeFameBuffer;
};
}