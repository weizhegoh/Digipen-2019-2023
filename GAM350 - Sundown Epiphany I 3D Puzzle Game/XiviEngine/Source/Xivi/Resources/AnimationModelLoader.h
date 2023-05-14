/*****************************************************************
*\file         AnimationModelLoader.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
class AnimationModelLoader : public Resource::ILoader<Graphics::AnimationModel>
{
public:
  explicit AnimationModelLoader() = default;

  virtual ~AnimationModelLoader() = default;

  void DefaultLoad( const std::string &directory ) noexcept
  {
    Filesystem::resursive_dir_callback( directory, [&]( Filesystem::fs::path file )
    {
      if ( file.extension() == ".BinaryAnimated" )
      {
        auto file_str = file.generic_string();
        auto data = new Graphics::AnimationModel( file_str );
        Set( file_str, data );
      }
    } );
  }
protected:

  virtual void DoLoad( const std::string &file ) noexcept override
  {
    if ( Filesystem::file_exists( file ) ) [[likely]]
    {
        auto data = new Graphics::AnimationModel( file );
        Set( file, data );
    }
    else [[unlikely]]
    {
        SetNotFound( file );
    }
  }
};
}