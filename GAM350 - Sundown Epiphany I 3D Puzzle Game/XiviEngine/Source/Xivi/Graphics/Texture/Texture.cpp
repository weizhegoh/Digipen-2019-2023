/*****************************************************************
*\file		 Textures.cpp
*\brief		 Textures functions definitions.

*\author(s)   Lee Liang Ping	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "../Texture/Texture.h"
#include "../Reader/TextureReader.h"
#include "../Gfx/Gfx.h"
namespace Xivi::Graphics
{
Texture::Texture( const std::string pFileName )
{
  filePath = pFileName;
  pTex = TextureReader::LoadGLTexture( pFileName.c_str() );
  assert( pTex );
}

Texture::~Texture()
{
  Graphics::Gfx::Renderer::DeleteVAO( pTex );
}
}