/*****************************************************************
*\file         Mesh.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "../Buffers/OpenGLBuffer.h"
#include "Vertex.h"
#include "Mesh.h"
#include "../Gfx/Gfx.h"
namespace Xivi::Graphics
{
Mesh::Mesh( Vec3List &vertBuff,
            Vec3List &normal,
            Vec2List &textCoord,
            Indices &_indices,
            Indices &_normal_indices,
            Indices &_uv_indices
)
{
  vertexBuffer.add_list( vertBuff, normal, textCoord );
  indices.copy( _indices );
  SetupMesh();
}

Mesh::Mesh( VertexBuffer &vertBuffer, IndexBuffer &idBuff )
{
  vertexBuffer = vertBuffer;
  indices = idBuff;
  SetupMesh();
}

Mesh::~Mesh()
{
  //Gfx::Renderer::DeleteVAO(vaoid);
  //Gfx::Renderer::DeleteVAO(vaoid_normal);
}

const void Mesh::Draw() const
{
  Gfx::Renderer::DrawElements( vaoid, eboid, vboid, draw_cnt );
}

void Mesh::SetupMesh()
{
  //VertexBuffer openGLBuffer;
  //OpenGLBuffer::SetupArrayBuffer_WithIndices(openGLBuffer, vertexBuffer, indices);
  //BindMesh(openGLBuffer);
  OpenGLBuffer::SetupElementBuffer( vaoid, vboid, eboid, vertexBuffer, indices );
  draw_cnt = static_cast<GLuint>( indices.size() );
  primitive_type = GL_TRIANGLES;
  BuildDrawNormal();
}

void Mesh::ReSetupMesh()
{
  Gfx::Renderer::DeleteVAO( vaoid );
  Gfx::Renderer::DeleteVAO( vaoid_normal );
  SetupMesh();
}

void  Mesh::BindMesh( VertexBuffer &openGLBuffer )
{
  vaoid = OpenGLBuffer::SetupModelArrayBuffer( openGLBuffer );
  draw_cnt = static_cast<GLuint>( indices.size() );
  primitive_type = GL_TRIANGLES;
  primitive_cnt = static_cast<GLuint>( vertexBuffer.size() );
}

void Mesh::BuildDrawNormal()
{
  Vec3List drwNormal;
  //store normal vertices to draw line
  for ( size_t i = 0; i < vertexBuffer.size(); ++i )
  {
    drwNormal.emplace_back( vertexBuffer[i].position );
    drwNormal.emplace_back( vertexBuffer[i].position + ( vertexBuffer[i].normal * 0.05f ) );
  }
  if ( !drwNormal.empty() )
  {
    VertexBuffer openGLBuffer;
    OpenGLBuffer::SetupArrayBufferNormalVert( openGLBuffer, drwNormal );
    vaoid_normal = OpenGLBuffer::SetupModelArrayBuffer( openGLBuffer );
    draw_cnt_normal = static_cast<GLuint>( drwNormal.size() );
  }
}
}