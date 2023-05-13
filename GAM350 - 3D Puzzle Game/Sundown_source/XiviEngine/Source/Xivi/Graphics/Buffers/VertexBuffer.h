/*****************************************************************
*\file         VertexBuffer.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Model/Vertex.h"
#include "BufferHeader.h"
#include "IndexBuffer.h"
namespace Xivi::Graphics
{
class VertexBuffer
{
  std::vector<Vertex> buffer;
public:

  void AddSpace( size_t size )
  {
    for ( size_t i = 0; i < size; ++i )
      buffer.push_back( Vertex() );
  }
  Vertex &operator[]( const size_t index )
  {
    return buffer[index];
  }
  const Vertex &operator[]( const size_t index ) const
  {
    return buffer[index];
  }
  size_t size() const
  {
    return buffer.size();
  }
  void push_back( Vertex value )
  {
    buffer.push_back( value );
  }
  std::vector<Vertex> &data()
  {
    return buffer;
  };
  void add_list( Vec3List &vertBuff, Vec3List &normal, Vec2List &textCoord )
  {
    for ( size_t i = 0; i < vertBuff.size(); ++i )
    {
      Vertex vertex;
      vertex.position = vertBuff[i];
      vertex.normal = normal[i];
      if ( textCoord.size() )
        vertex.texture_coord = textCoord[i];
      else
        vertex.texture_coord = Math::Vector2( 0.f, 0.f );

      buffer.push_back( vertex );
    }
  }
};
}