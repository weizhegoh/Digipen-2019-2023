/*****************************************************************
*\file         ShaderLoader.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
class ShaderLoader : ILoader<Graphics::GLSLShader>
{
public:
  explicit ShaderLoader() = default;

  virtual ~ShaderLoader() = default;

protected:
  void LoadFileConfig() noexcept
  {
    for ( auto &file : Filesystem::file_list( "Shaders" ) )
    {
      if ( file.extension() == ".glsl" ) [[likely]]
      {
          auto data = new Graphics::GLSLShader;
          ResourceKey key = { file.generic_string().substr( 0, file.generic_string().find_last_of( "." ) ) };
          Set( key, data );
      }
    }
  }
  virtual void DoLoad( ResourceKey key ) noexcept override
  {
    if ( Filesystem::file_exists( key() + ".glsl" ) ) [[likely]]
    {
        auto data = new Graphics::GLSLShader;
        Set( key, data );
    }
    else [[unlikely]]
    {
        SetNotFound( key );
    }
  }
  virtual void DoLoad( ResourceKey key, std::string_view str ) noexcept override
  {
    if ( Filesystem::file_exists( str ) ) [[likely]]
    {
        auto data = new Graphics::GLSLShader;
        Set( key, data );
    }
    else [[unlikely]]
    {
        SetNotFound( key );
    }
  }
};
}