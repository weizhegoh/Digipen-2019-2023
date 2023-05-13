/*****************************************************************
*\file         AnimationModel.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Vertex.h"
#include "../Mesh.h"
#include "BoneInfo.h"
#include "../../Buffers/OpenGLBuffer.h"
#include "../../Buffers/BufferHeader.h"

namespace Xivi::Graphics
{
class AnimatedModel
{
  std::map<std::string, BoneInfo> m_BoneInfoMap;
  int m_BoneCounter = 0;
public:
  AnimatedModel()
  {}
  ~AnimatedModel()
  {}
  AnimatedModel( std::string path );

  auto &GetBoneInfoMap()
  {
    return m_BoneInfoMap;
  }
  int &GetBoneCount()
  {
    return m_BoneCounter;
  }
  void Draw()
  {
    for ( const auto &x : meshes ) x.Draw();
  }

  std::vector<Mesh> meshes;
  std::string directory;
  int offset;
  Math::Vector3 min = Math::Vector3( 0.f, 0.f, 0.f );
  Math::Vector3 max = Math::Vector3( 0.f, 0.f, 0.f );
};
}