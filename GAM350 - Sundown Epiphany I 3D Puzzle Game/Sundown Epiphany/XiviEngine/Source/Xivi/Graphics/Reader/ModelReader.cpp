/*****************************************************************
*\file         ModelReader.cpp
*\author(s)    Lee Liang Ping
         Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "ModelReader.h"
#include "MeshCreate.h"
#include "../Buffers/OpenGLBuffer.h"
#include "../Model/Mesh.h"
#include "../Model/Model.h"
#include "../Helper/ConversionHelper.h"
//for sorting glm::ivec3
inline bool operator<( const glm::ivec3 &a, const glm::ivec3 &b )
{
  return a.x < b.x ||
    ( a.x == b.x && ( a.y < b.y || ( a.y == b.y && a.z < b.z ) ) );
}
template <> struct std::less<glm::ivec3>
{
  bool operator()( const glm::ivec3 &a, const glm::ivec3 &b )
  {
    return a.x < b.x || ( a.x == b.x && ( a.y < b.y || ( a.y == b.y && a.z < b.z ) ) );
  }
};

namespace Xivi::Graphics
{
void LoadBinaryObj( const std::string &filePath, std::vector<Mesh> &mesh, Math::Vector3 &unitScales )
{
  XV_CORE_ASSERT_MSG( is_fileExist( filePath ), "File does not exist" );
  int indicesSize = 0;
  int verticesSize = 0;
  int uvSize = 0;
  int normalSize = 0;

  std::ifstream myFile( filePath, std::ios::in | std::ios::binary );
  myFile.read( (char *) &verticesSize, sizeof( int ) );
  myFile.read( (char *) &uvSize, sizeof( int ) );
  myFile.read( (char *) &normalSize, sizeof( int ) );
  myFile.read( (char *) &indicesSize, sizeof( int ) );

  Vec3List _vertices( verticesSize );
  Vec2List _uv( uvSize );
  Vec3List _normal( normalSize );
  glm::vec3 min = { 0.f, 0.f, 0.f };
  glm::vec3 max = { 0.f, 0.f, 0.f };

  for ( size_t i = 0; i < verticesSize; ++i )
  {
    float vertX;
    myFile.read( (char *) &vertX, sizeof( float ) );
    _vertices[i].setX( vertX );

    float vertY;
    myFile.read( (char *) &vertY, sizeof( float ) );
    _vertices[i].setY( vertY );

    float vertZ;
    myFile.read( (char *) &vertZ, sizeof( float ) );
    _vertices[i].setZ( vertZ );
  }

  for ( size_t i = 0; i < uvSize; ++i )
  {
    myFile.read( (char *) &_uv[i], sizeof( glm::vec2 ) );
  }
  for ( size_t i = 0; i < normalSize; ++i )
  {
    //myFile.read((char*)&_normal[i], sizeof(12));

    float normX;
    myFile.read( (char *) &normX, sizeof( float ) );
    _normal[i].setX( normX );

    float normY;
    myFile.read( (char *) &normY, sizeof( float ) );
    _normal[i].setY( normY );

    float normZ;
    myFile.read( (char *) &normZ, sizeof( float ) );
    _normal[i].setZ( normZ );
  }

  Indices _vertexIndices( indicesSize );
  Indices _uvIndices( indicesSize );
  Indices _normalIndices( indicesSize );

  for ( size_t i = 0; i < indicesSize; i++ )
  {
    myFile.read( (char *) &_vertexIndices[i], sizeof( int ) );
    myFile.read( (char *) &_uvIndices[i], sizeof( int ) );
    myFile.read( (char *) &_normalIndices[i], sizeof( int ) );
  }

  mesh.push_back( Mesh( _vertices, _normal, _uv, _vertexIndices, _normalIndices, _uvIndices ) );
  float unitScale = 10.f;
  unitScales = Math::Vector3( unitScale );
  XV_CORE_INFO( "BinaryStatic Loaded - " + filePath );
}

void LoadBinaryStatic( const std::string &filepath, std::vector<Mesh> &mesh, Math::Vector3 &unitScales, bool &hasBiTan )
{
  XV_CORE_ASSERT_MSG( is_fileExist( filepath ), "File does not exist" );
  XV_CORE_INFO( "Loading BinaryStatic File - " + filepath );
  std::ifstream myFile( filepath, std::ios::in | std::ios::binary );

  myFile.read( (char *) &hasBiTan, sizeof( bool ) );
  int meshSize;
  myFile.read( (char *) &meshSize, sizeof( int ) );
  for ( int i = 0; i < meshSize; i++ )
  {
    int indicesSize;
    myFile.read( (char *) &indicesSize, sizeof( int ) );

    VertexBuffer 	vertexBuffer;
    IndexBuffer indices;
    indices.data().resize( indicesSize );
    for ( int j = 0; j < indicesSize; j++ )
    {
      myFile.read( (char *) &indices[j], sizeof( int ) );
    }

    int vertexBufferSize;
    myFile.read( (char *) &vertexBufferSize, sizeof( int ) );
    vertexBuffer.data().resize( vertexBufferSize );

    for ( int j = 0; j < vertexBufferSize; j++ )
    {
      //inside vertexBuffer
      myFile.read( (char *) &vertexBuffer[j], 32 );

      auto &V = vertexBuffer[j];

      for ( int k = 0; k < MAX_BONE_INFLUENCE; k++ )
      {
        auto &J = V.jointID[k];
        auto &W = V.weights[k];
        myFile.read( (char *) &J, sizeof( int ) );
        myFile.read( (char *) &W, sizeof( float ) );
        //assert(J==-1);
      }
      myFile.read( (char *) &vertexBuffer.data()[j].texture_coord.x, sizeof( float ) );
      myFile.read( (char *) &vertexBuffer.data()[j].texture_coord.y, sizeof( float ) );

      float tmp = 0.f;
      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].bit.setX( tmp );
      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].bit.setY( tmp );
      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].bit.setZ( tmp );

      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].tan.setX( tmp );
      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].tan.setY( tmp );
      myFile.read( (char *) &tmp, sizeof( float ) );
      vertexBuffer.data()[j].tan.setZ( tmp );
    }
    //ComputeTangentsBitangents(vertexBuffer, indices);
    mesh.push_back( Mesh( vertexBuffer, indices ) );
  }
  float unitScale = 1.f;
  myFile.read( (char *) &unitScale, sizeof( float ) );
  unitScales = Math::Vector3( unitScale );
  XV_CORE_INFO( "BinaryStatic Loaded - " + filepath );
}
}