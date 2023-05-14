/*****************************************************************
*\file         Mesh.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Buffers/BufferHeader.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/IndexBuffer.h"

namespace Xivi::Graphics
{
class LineModel
{
public:
  unsigned int primitive_type;
  unsigned int vaoid;
  unsigned int eboid;
};
class Mesh
{
public:
  Mesh() = default;
  Mesh( Vec3List &vertBuff,
        Vec3List &normal,
        Vec2List &textCoord,
        Indices &_indices,
        Indices &_normal_indices,
        Indices &_uv_indices );
  Mesh( VertexBuffer &vertexBuffer, IndexBuffer &indexBuff );
  ~Mesh();

  VertexBuffer vertexBuffer;
  IndexBuffer indices;

  std::vector<float> vertex_normals;
  std::vector<float> face_normals;
  std::vector<unsigned int> bone_ids;
  std::vector<float> bone_weights;

  unsigned int primitive_type;
  unsigned int vaoid;
  unsigned int eboid;
  unsigned int vboid;
  unsigned int draw_cnt;

  unsigned int primitive_type_normal;
  unsigned int vaoid_normal;
  unsigned int draw_cnt_normal;

  unsigned int primitive_cnt;

  const void Draw() const;
  void ReSetupMesh();
private:
  void SetupMesh();
  void BindMesh( VertexBuffer &openGLBuffer );
  void BuildDrawNormal();
};
}