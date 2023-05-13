/*****************************************************************
*\file		 Textures.h
*\brief		 Header file declarations for Textures.

*\author(s)   Lee Liang Ping	6 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::Graphics
{
class Texture
{
public:
  unsigned int pTex;
  std::string filePath;

  Texture() = default;
  Texture( const std::string pFileName );
  ~Texture();
};

using TexturePtr = Texture *;
}
