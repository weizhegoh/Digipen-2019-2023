/*****************************************************************
*\file         GLFW_Window.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>
#include "Xivi/Core/Window.h"
#include "Systems/Audio/Audio.h"

namespace Xivi
{
namespace Graphics
{
enum class RendererAPI : u32;
}

class GLFW_Window : public Window
{
public:
  GLFW_Window( const WindowConfig &c );
  ~GLFW_Window();

  XV_INLINE void Title( const std::string &title ) override
  {
    glfwSetWindowTitle( m_handle, title.c_str() );
    m_config.m_title = title;
  }

  XV_INLINE std::string Title() const override
  {
    return m_config.m_title;
  }

  XV_INLINE void Width( u32 width ) override
  {
    m_config.m_width = width;
  }

  XV_INLINE u32 Width() const override
  {
    return m_config.m_width;
  }

  XV_INLINE void Height( u32 height ) override
  {
    m_config.m_height = height;
  }

  XV_INLINE u32 Height() const override
  {
    return m_config.m_height;
  }

  XV_INLINE void DPIScale( f32 dpi ) override
  {
    m_config.m_dpi = dpi;
  }

  XV_INLINE f32 DPIScale() const override
  {
    return m_config.m_dpi;
  }

  XV_INLINE void Fullscreen( bool b ) override
  {
    m_config.m_fullscreen = b;
  }

  XV_INLINE bool Fullscreen() const override
  {
    return m_config.m_fullscreen;
  }

  XV_INLINE void Borderless( bool b ) override
  {
    if ( b )
    {
      m_config.m_borderless = true;
      glfwWindowHint( GLFW_DECORATED, false );
    }
    else
    {
      m_config.m_borderless = false;
      glfwWindowHint( GLFW_DECORATED, true );
    }
  }

  XV_INLINE bool Borderless() const override
  {
    return m_config.m_borderless;
  }

  XV_INLINE void Console( bool b ) override
  {
    m_config.m_console = b;
  }

  XV_INLINE bool Console() const override
  {
    return m_config.m_console;
  }

  XV_INLINE void ToggleVSync() override
  {
    if ( m_config.m_vsync )
      VSync( false );
    else
      VSync( true );
  }

  XV_INLINE void VSync( bool b ) override
  {
    if ( b )
    {
      m_config.m_vsync = true;
      glfwSwapInterval( 1 );
    }
    else
    {
      m_config.m_vsync = false;
      glfwSwapInterval( 0 );
    }
  }

  XV_INLINE bool VSync() const override
  {
    return m_config.m_vsync;
  }

  XV_INLINE void Resizable( bool b ) override
  {
    m_config.m_resizable = b;
  }

  XV_INLINE bool Resizable() const override
  {
    return m_config.m_resizable;
  }

  XV_INLINE void Cursor( bool b ) override
  {
    if ( b )
    {
      m_config.m_cursor = true;
      glfwSetInputMode( m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }
    else
    {
      m_config.m_cursor = false;
      glfwSetInputMode( m_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
    }
  }

  XV_INLINE bool Cursor() const override
  {
    return m_config.m_cursor;
  }

  XV_INLINE void CursorPosition( const Vec2f &pos ) override;

  XV_INLINE void Focus( bool b ) override
  {
    m_config.m_focus = b;
    if ( b )
      Xivi::Audio::AudioManager::Instance().UnpauseAllAudio();
    else
      Xivi::Audio::AudioManager::Instance().PauseAllAudio();
  }

  XV_INLINE bool Focus() const override
  {
    return m_config.m_focus;
  }

  XV_INLINE Ptr<void> Handle() override
  {
    return m_handle;
  }

  XV_INLINE Ptr<GLFWwindow> GLFWHandle()
  {
    return m_handle;
  }

  XV_INLINE f32 ScreenRatio() const override
  {
    return static_cast<f32>( m_config.m_width ) / static_cast<f32>( m_config.m_height );
  }

  XV_INLINE void Update() override
  {
    #ifdef XV_RENDER_OPENGL
    glfwSwapBuffers( m_handle );
    #else

    #endif
  }

  void Process() override;

  void Initialise();

private:

  Ptr<GLFWwindow> m_handle;
};
}