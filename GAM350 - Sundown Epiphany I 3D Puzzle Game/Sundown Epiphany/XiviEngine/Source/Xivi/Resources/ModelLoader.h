/*****************************************************************
*\file         ModelLoader.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
class ModelLoader : public Resource::ILoader<Graphics::StaticModel>
{
public:
  explicit ModelLoader() = default;

  virtual ~ModelLoader() = default;

  void DefaultLoad( const std::string &directory ) noexcept
  {
    Filesystem::resursive_dir_callback( directory, [&]( Filesystem::fs::path file )
    {
      if ( file.extension() == ".BinaryStatic" || file.extension() == ".BinaryObj" )
      {
        auto file_str = file.generic_string();
        auto data = new Graphics::StaticModel( file_str );
        XV_CORE_INFO( "File - {}", file_str );
        Set( file_str, data );
      }
    } );
  }
protected:

  virtual void DoLoad( const std::string &file ) noexcept override
  {
    if ( Filesystem::file_exists( file ) ) [[likely]]
    {
        auto data = new Graphics::StaticModel( file );
        Set( file, data );
    }
    else [[unlikely]]
    {
        SetNotFound( file );
    }
  }
};
}