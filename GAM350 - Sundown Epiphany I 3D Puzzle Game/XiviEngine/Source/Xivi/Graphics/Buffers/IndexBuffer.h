/*****************************************************************
*\file         IndexBuffer.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../Buffers/BufferHeader.h"
namespace Xivi::Graphics
{
class IndexBuffer
{
  std::vector<int> buffer;
public:

  void BuildIndices( int width, int height, std::vector<Vertex> vertices )
  {
    int row0, row1, i0, i1, i2;
    int stride = height + 1;

    for ( int i = 0; i < width; ++i )
    {
      for ( int j = 0; j < height; ++j )
      {
        row0 = i * stride;
        row1 = row0 + stride;

        /*  First triangle */
        i0 = row0 + j;
        i1 = row1 + j;
        i2 = row0 + j + 1;

        /*  Ignore degenerate triangle */
        if ( !DegenerateTri( vertices.data()[i0].position,
             vertices[i1].position,
             vertices[i2].position ) )
        {
          buffer.push_back( i0 );
          buffer.push_back( i1 );
          buffer.push_back( i2 );
        }

        /*  Second triangle */
        i0 = row0 + j + 1;
        i1 = row1 + j;
        i2 = row1 + j + 1;

        /*  Ignore degenerate triangle */
        if ( !DegenerateTri( vertices.data()[i0].position,
             vertices[i1].position,
             vertices[i2].position ) )
        {
          buffer.push_back( i0 );
          buffer.push_back( i1 );
          buffer.push_back( i2 );
        }
      }
    }
  }

  bool DegenerateTri( const  Vec3f &v0, const  Vec3f &v1, const  Vec3f &v2 )
  {
    return ( ( v1 - v0 ).length() < 0.00001f ||
             ( v2 - v1 ).length() < 0.00001f ||
             ( v0 - v2 ).length() < 0.00001f );
  }

  size_t size() const
  {
    return buffer.size();
  }

  int &operator[]( const size_t index )
  {
    return buffer[index];
  }
  const int &operator[]( const size_t index ) const
  {
    return buffer[index];
  }

  void push_back( int value )
  {
    buffer.push_back( value );
  }
  Indices &data()
  {
    return buffer;
  };

  void copy( Indices &indices )
  {
    for ( size_t i = 0; i < indices.size(); ++i )
      buffer.push_back( indices[i] - 1 );;
  }
};
}