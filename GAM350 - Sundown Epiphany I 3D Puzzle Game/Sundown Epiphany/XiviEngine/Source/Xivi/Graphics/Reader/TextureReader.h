/*****************************************************************
*\file         TextureReader.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Model/Mesh.h"
#include "tinyddsloader.h"
namespace Xivi::Graphics
{
struct TextureReader
{
  static unsigned int LoadGLTexture( const std::string pFileName );

  struct GLSwizzle
  {
    GLenum m_r, m_g, m_b, m_a;
  };

  struct GLFormat
  {
    tinyddsloader::DDSFile::DXGIFormat m_dxgiFormat;
    GLenum m_type;
    GLenum m_format;
    GLSwizzle m_swizzle;
  };

  static bool TranslateFormat( tinyddsloader::DDSFile::DXGIFormat fmt, GLFormat *outFormat );
  static bool IsCompressed_test( GLenum fmt );
};
}