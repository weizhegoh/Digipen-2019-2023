/*****************************************************************
*\file         Config.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <string>
#include <Types/Base.h>
#include "Xivi/Graphics/Graphics.h"

namespace Xivi
{
class Window;

struct WindowConfig
{
  WindowConfig( const std::string &title = "Xivi",
                u32 width = 1920,
                u32 height = 1080,
                f32 dpi = 1.0f,
                #ifdef XV_RENDER_OPENGL
                Graphics::RendererAPI renderer_api = Graphics::RendererAPI::OpenGL,
                #endif
                #ifdef XV_RENDER_VULKAN
                Graphics::RendererAPI renderer_api = Graphics::RendererAPI::Vulkan,
                #endif
                #ifdef XV_EDITOR
                bool fullscreen = false,
                bool borderless = false,
                #else
                bool fullscreen = true,
                bool borderless = true,
                #endif
                bool console = false,
                bool vsync = false,
                bool resizable = false,
                #ifdef XV_EDITOR
                bool cursor = true,
                #else
                bool cursor = false,
                #endif
                bool focus = false,
                bool close = false )
    :
  m_window( nullptr ),
    m_title( title ),
    m_width( width ),
    m_height( height ),
    m_dpi( dpi ),
    m_renderer_api( renderer_api ),
    m_fullscreen( fullscreen ),
    m_borderless( borderless ),
    m_console( console ),
    m_vsync( vsync ),
    m_resizable( resizable ),
    m_cursor( cursor ),
    m_focus( focus ),
    m_close( close )
  {}
  Ptr<Window> m_window;
  std::string m_title;
  i32 m_width;
  i32 m_height;
  f32 m_dpi;
  Graphics::RendererAPI m_renderer_api;
  bool m_fullscreen;
  bool m_borderless;
  bool m_console;
  bool m_vsync;
  bool m_resizable;
  bool m_cursor;
  bool m_focus;
  bool m_close;
};
}