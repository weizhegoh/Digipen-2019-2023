/*****************************************************************
*\file        ModelReader.cpp
*\author(s)    Lee Liang Ping
         Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"

namespace Xivi::Graphics
{
inline bool is_fileExist( const std::string &name )
{
  std::ifstream f( name.c_str() );
  return f.good();
}

class Mesh;
class StaticModel;
void LoadBinaryObj( const std::string &filePath, std::vector<Mesh> &mesh, Math::Vector3 &unitScale );
void LoadBinaryStatic( const std::string &filepath, std::vector<Mesh> &mesh, Math::Vector3 &unitScale, bool &hasBiTan );
}