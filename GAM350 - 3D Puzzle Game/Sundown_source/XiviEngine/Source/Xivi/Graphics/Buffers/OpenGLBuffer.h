/*****************************************************************
*\file         OpenGLBuffer.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "BufferHeader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Xivi::Graphics
{
class OpenGLBuffer
{
public:
  static GLuint SetupModelArrayBuffer( VertexBuffer &buffer )
  {
    GLuint vbo_hdl;
    CreateArrayBuffer( vbo_hdl, buffer.data() );
    GLuint vao_hdl;
    glCreateVertexArrays( 1, &vao_hdl );
    glVertexArrayVertexBuffer( vao_hdl, 0, vbo_hdl, 0, sizeof( Vertex ) );
    VertexArrayBinding( vao_hdl, 0, 3, offsetof( Vertex, Vertex::position ) );
    VertexArrayBinding( vao_hdl, 1, 3, offsetof( Vertex, Vertex::normal ) );
    VertexArrayBinding( vao_hdl, 2, 2, offsetof( Vertex, Vertex::texture_coord ) );
    VertexArrayBinding( vao_hdl, 3, 3, offsetof( Vertex, Vertex::tan ) );
    VertexArrayBinding( vao_hdl, 4, 3, offsetof( Vertex, Vertex::bit ) );
    VertexArrayBinding( vao_hdl, 5, 4, offsetof( Vertex, Vertex::jointID ) );
    VertexArrayBinding( vao_hdl, 6, 4, offsetof( Vertex, Vertex::weights ) );
    glBindVertexArray( 0 );
    return vao_hdl;
  }

  static void SetupArrayBuffer_WithIndices(
    VertexBuffer &openglBuffer,
    const Vec3List &allVertices,
    const Vec3List &allVertexNormals,
    const Vec2List &allVertexUV,
    const Indices &allIndices,
    const Indices &normalIndices,
    const Indices &uvIndices
  )
  {
    for ( size_t i = 0; i < allIndices.size(); ++i )
    {
      Vertex vertex;
      vertex.position = allVertices[allIndices[i] - 1];
      vertex.normal = allVertexNormals[normalIndices[i] - 1];
      vertex.texture_coord = allVertexUV[uvIndices[i] - 1];
      openglBuffer.push_back( vertex );
    }
  }

  static void SetupArrayBuffer_WithIndices(
    VertexBuffer &openglBuffer,
    const VertexBuffer &vertices,
    const IndexBuffer &indices )
  {
    for ( size_t i = 0; i < indices.size(); ++i )
    {
      Vertex vertex;
      vertex.position = vertices[indices[i]].position;
      vertex.normal = vertices[indices[i]].normal;
      vertex.texture_coord = vertices[indices[i]].texture_coord;
      openglBuffer.push_back( vertex );
    }
  }

  static void SetupArrayBufferNormalVert(
    VertexBuffer &openglBuffer,
    const Vec3List &drwNormal )
  {
    for ( size_t i = 0; i < drwNormal.size(); ++i )
    {
      Vertex vertex;
      vertex.position = drwNormal[i];
      openglBuffer.push_back( vertex );
    }
  }

  static void SetupElementBuffer(
    unsigned int &vaoid, unsigned int &vboid, unsigned int &eboid,
    VertexBuffer &vertices, IndexBuffer &indices )
  {
    CreateElementBuffer( vaoid, vboid, eboid, vertices.data(), indices.data() );
    VertexVertexAttribBinding( 0, 3, 0 );
    VertexVertexAttribBinding( 1, 3, offsetof( Vertex, Vertex::normal ) );
    VertexVertexAttribBinding( 2, 2, offsetof( Vertex, Vertex::texture_coord ) );
    VertexVertexAttribBinding( 3, 3, offsetof( Vertex, Vertex::tan ) );
    VertexVertexAttribBinding( 4, 3, offsetof( Vertex, Vertex::bit ) );
    VertexVertexAttribBinding( 5, 4, offsetof( Vertex, Vertex::jointID ) );
    VertexVertexAttribBinding( 6, 4, offsetof( Vertex, Vertex::weights ) );
    BindVertexArray( 0 );
  }

private:
  static void *f( unsigned __int64 i )
  {
    return (void *) i;
  }

  static void BindVertexArray( unsigned int value )
  {
    glBindVertexArray( value );
  }

  static void CreateElementBuffer(
    unsigned int &vaoid, unsigned int &vboid, unsigned int &eboid,
    std::vector<Vertex> &vertices, Indices &indices )
  {
    glGenVertexArrays( 1, &vaoid );
    glGenBuffers( 1, &vboid );
    glGenBuffers( 1, &eboid );

    glBindVertexArray( vaoid );
    glBindBuffer( GL_ARRAY_BUFFER, vboid );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vertex ), &vertices[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eboid );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ),
                  &indices.data()[0], GL_STATIC_DRAW );
  }
  static void VertexVertexAttribBinding( GLuint locaion, GLuint size, GLuint offset )
  {
    glEnableVertexAttribArray( locaion );
    glVertexAttribPointer( locaion, size, GL_FLOAT, GL_FALSE, sizeof( Vertex ), f( offset ) );
  }

  static void VertexArrayBinding( GLuint &vao_hdl, GLuint locaion, GLuint size, GLuint offset )
  {
    glEnableVertexArrayAttrib( vao_hdl, locaion );
    glVertexArrayAttribFormat( vao_hdl, locaion, size, GL_FLOAT, GL_FALSE, offset );
    glVertexArrayAttribBinding( vao_hdl, locaion, 0 );
  }

  static void CreateArrayBuffer( GLuint &vbo_hdl, std::vector<Vertex> &buffer )
  {
    size_t buffer_size = sizeof( Vertex ) * buffer.size();
    glCreateBuffers( 1, &vbo_hdl );
    glNamedBufferStorage( vbo_hdl, buffer_size, nullptr, GL_DYNAMIC_STORAGE_BIT );
    glNamedBufferSubData( vbo_hdl, 0, buffer_size, buffer.data() );
  }
};
}