/*****************************************************************
*\file		 ShaderManager.cpp
*\brief		 ShaderManager functions definitions.

*\author(s)   Lee Liang Ping	23 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "ShaderManager.h"
namespace Xivi::Graphics
{
void ShaderManager::AddShader( ShaderOption option, std::string vert, std::string frag )
{
  auto temp = shdrpgms->find( option );
  XV_CORE_ASSERT_MSG( temp == shdrpgms->end(), "Shader type is Registered" );
  std::vector<std::pair<GLenum, std::string>> shdr_files;
  shdr_files.push_back( std::make_pair( GL_VERTEX_SHADER, vert ) );
  shdr_files.push_back( std::make_pair( GL_FRAGMENT_SHADER, frag ) );
  GLSLShader shdr_pgm;
  shdr_pgm.CompileLinkValidate( shdr_files );
  ShaderManager::shdrpgms->insert( { option, shdr_pgm } );
}

ShaderManager::~ShaderManager()
{
  for ( auto &x : *shdrpgms )
    x.second.DeleteShaderProgram();
  delete shdrpgms;
}

GLSLShader &ShaderManager::GetShader( const ShaderOption index )
{
  auto temp = shdrpgms->find( index );
  XV_CORE_ASSERT_MSG( temp != shdrpgms->end(), "pass in index > shader size" );
  return temp->second;
}
}