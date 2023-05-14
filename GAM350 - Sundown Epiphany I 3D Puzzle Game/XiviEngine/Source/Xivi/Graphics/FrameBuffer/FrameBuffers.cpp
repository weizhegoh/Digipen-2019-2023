/*****************************************************************
*\file         FrameBuffers.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "FrameBuffers.h"

#include "FrameBuffer.h"
//#include "FrameBuffer2D.h"

namespace Xivi::Graphics
{
void FrameBuffers::Create(
  const float width, const float height,
  const RenderList renderlist, const bool splitscreen,
  const float _fov, const float _near, const float _far )
{
  bool iseditor = ( renderlist == RenderList::Editor );
  iseditor ? XV_CORE_INFO( "Framebuffer Editor" ) : XV_CORE_INFO( "Framebuffer Game" );
  auto &fb_list = iseditor ? editor_list : game_list;
  fb_list.push_back( FrameBuffer( width, height, splitscreen, _fov, _near, _far ) );
}

std::vector<FrameBuffer> &FrameBuffers::GetList()
{
  #ifdef XV_EDITOR
  return ( activeList == RenderList::Editor ) ? editor_list : game_list;
  #else
  return game_list;
  #endif
}

std::vector<Camera3D> &FrameBuffers::CameraList( const size_t imageIndex )
{
  return GetList()[imageIndex].CameraList();
}

const FrameBuffer &FrameBuffers::operator[] ( const size_t index )
{
  return GetList()[index];
}
const unsigned int &FrameBuffers::Image( const size_t index )
{
  return GetList()[index].Image();
}
const unsigned int &FrameBuffers::ImageGame( const size_t index )
{
  XV_CORE_ASSERT_MSG( game_list.size() > 0, "No Framebuffer Exist" );
  if ( index >= game_list.size() )
    return game_list[0].Image();
  return game_list[index].Image();
}

#ifdef XV_EDITOR
const unsigned int &FrameBuffers::ImageEditor( const size_t index )
{
  XV_CORE_ASSERT_MSG( editor_list.size() > 0, "No Framebuffer Exist" );
  if ( index >= editor_list.size() )
    return editor_list[0].Image();
  return editor_list[index].Image();
}
Camera3D &FrameBuffers::CameraEditor( const size_t cameraIndex )
{
  return editor_list[0].Camera( cameraIndex );
}
#endif

Camera3D &FrameBuffers::CameraGame( const size_t imageIndex, const size_t cameraIndex )
{
  return game_list[imageIndex].Camera( cameraIndex );
}

Camera3D &FrameBuffers::Camera( const size_t imageIndex, const size_t cameraIndex )
{
  return GetList()[imageIndex].Camera( cameraIndex );
}
Camera3D &FrameBuffers::Camera( const size_t imageIndex )
{
  return GetList()[imageIndex].Camera( 0 );
}

void FrameBuffers::CameraOnUpdate(
  float width, float height,
  unsigned int button,
  unsigned int button2,
  int mouseX, int mouseY, bool FPS, bool in_viewport
)
{
  bool isEditor = ( activeList == RenderList::Editor );
  auto &fbList = ( isEditor ) ? editor_list : game_list;
  for ( unsigned i = 0; i < fbList.size(); ++i )
  {
    auto &frameBuffer = fbList[i];
    for ( unsigned j = 0; j < CameraList( i ).size(); ++j )
    {
      auto &camera = frameBuffer.Camera( j );
      #ifdef XV_EDITOR
      if ( isEditor )
      {
        //change to &=
        bool active = ( i == activeFameBuffer ) && in_viewport;
        //if (in_viewport)
        {
          if ( fbList[i].isScreenSplit() )
          {
            if ( j == (int) SplitScreen::Left )
            {
              camera.ProcessMouseInput( button, mouseX, mouseY, active );
              camera.FPSInputLeft( active, cameraSentivity );
            }
            else if ( j == (int) SplitScreen::Right )
            {
              camera.ProcessMouseInput( button2, mouseX, mouseY, active );
              camera.FPSInputRight( active, cameraSentivity );
            }
          }
          else
          {
            camera.ProcessMouseInput( button2, mouseX, mouseY, active );
            camera.FPSInputRight( active, cameraSentivity );
          }
        }
      }
      else
        #endif
      {
        //Camera3D::Mode camMode = (FPS) ? Camera3D::Mode::Fps : Camera3D::Mode::Orbit;
        //camera.SetMode(camMode);
        //bool active = (i == activeFameBuffer);

        if ( FPS )
        {
          if ( j == (int) SplitScreen::Left )
          {
            camera.ProcessMouseInput( button, mouseX, mouseY, true );
            camera.FPSInputLeft( true, cameraSentivity );
          }
          else if ( j == (int) SplitScreen::Right )
          {
            camera.ProcessMouseInput( button2, mouseX, mouseY, true );
            camera.FPSInputRight( true, cameraSentivity );
          }
        }
        else
        {
          if ( j == (int) SplitScreen::Left )
          {
            camera.ProcessMouseInput( button, mouseX, mouseY, true );
            camera.FPSInputLeft( true, cameraSentivity );
          }
          else if ( j == (int) SplitScreen::Right )
          {
            camera.ProcessMouseInput( button2, mouseX, mouseY, true );
            camera.FPSInputRight( true, cameraSentivity );
          }
        }
      }
      camera.FPSViewTransfrom();
      #ifdef XV_EDITOR
      if ( !frameBuffer.isScreenSplit() )
        camera.PerspectiveTransform3D( width, height );
      else
        #endif
        camera.PerspectiveTransform3D( width * 0.5f, height );

      //UpdateViewport(postionX, positionY, float width, float height)
      //camera.createFrustumFromCamera();
      //camera.GetFrustum().createFrustumFromCamera(camera, camera.GetAspectRatio(), camera.GetFOV(), camera.GetNearPlane(), camera.GetFarPlane());
    }
  }
}

void FrameBuffers::CreateUI( float width, float height,
                             const float _fov,
                             const float _near,
                             const float _far )
{
  UI_list.push_back( FrameBuffer( width, height, false, _fov, _near, _far ) );
}

void FrameBuffers::CameraOnUpdateUI( float width, float height )
{
  //auto& mouse = *Service::Locator::Get<Input::Manager>().Mouse();
  //auto mousePosition = mouse.Position();
  //auto rightScreenMouseControl = (unsigned int)Input::MouseButton::Right;

  //CameraUI().ProcessMouseInput(rightScreenMouseControl, (float)mousePosition.X(), (float)mousePosition.Y(), true);
  //CameraUI().FPSInputRight(true, 0.1f);
  CameraUI().FPSViewTransfrom();
  CameraUI().PerspectiveTransform3D( width, height );
}

FrameBuffer &FrameBuffers::FrameBufferUI()
{
  return UI_list[0];
}

Camera3D &FrameBuffers::CameraUI()
{
  return UI_list[0].Camera( 0 );
}
const unsigned int &FrameBuffers::ImageUI( const size_t index )
{
  return UI_list[0].Image();
}

void FrameBuffers::NextActiveFB()
{
  if ( activeFameBuffer < editor_list.size() - 1 )
    activeFameBuffer++;
  else
    activeFameBuffer = 0;
}

void FrameBuffers::ToggleList()
{
  activeList = ( activeList == RenderList::Editor ) ?
    RenderList::Game : RenderList::Editor;
  activeFameBuffer = 0;
}
}