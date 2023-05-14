/*****************************************************************
*\file         Window.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <Types/Base.h>
#include <Utils/NonCopyable.h>

#include "Xivi/Core/Utils/Math.h"
#include "Xivi/Graphics/Graphics.h"
#include "Config.h"
#include <string>

namespace Xivi
{
namespace Graphics
{
enum class RendererAPI : u32;
}

class Window : public NonCopyable
{
public:

  Window( const WindowConfig &c ) : m_config( c )
  {}
  virtual ~Window() = default;

  virtual void Title( const std::string &title ) = 0;
  virtual std::string Title() const = 0;

  virtual void Width( u32 width ) = 0;
  virtual u32 Width() const = 0;

  virtual void Height( u32 height ) = 0;
  virtual u32 Height() const = 0;

  virtual void DPIScale( f32 dpi ) = 0;
  virtual f32 DPIScale() const
  {
    return 1.0f;
  }

  virtual void Fullscreen( bool b ) = 0;
  virtual bool Fullscreen() const = 0;

  virtual void Borderless( bool b ) = 0;
  virtual bool Borderless() const = 0;

  virtual void Console( bool b ) = 0;
  virtual bool Console() const = 0;

  virtual void ToggleVSync() = 0;
  virtual void VSync( bool b ) = 0;
  virtual bool VSync() const = 0;

  virtual void Resizable( bool b ) = 0;
  virtual bool Resizable() const = 0;

  virtual void Cursor( bool b ) = 0;
  virtual bool Cursor() const = 0;
  virtual void CursorPosition( const Vec2f &pos ) = 0;

  virtual void Focus( bool b ) = 0;
  virtual bool Focus() const = 0;

  virtual Ptr<void> Handle()
  {
    return nullptr;
  }

  virtual f32 ScreenRatio() const = 0;

  virtual void Update() = 0;
  virtual void Process() = 0;

  virtual void SetIcon( std::string_view path, std::string_view small_path = "" )
  {
    (void) path;
    (void) small_path;
  };

protected:
  WindowConfig m_config;
};
}