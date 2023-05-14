/*****************************************************************
*\file         AnimationModel.cpp
*\author(s)    Kenric Tan Wei Liang
               Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "AnimationModel.h"

namespace Xivi::Graphics
{
AnimatedModel::AnimatedModel( std::string path )
{
  XV_CORE_INFO( "Loading BinaryAnimated File - " + path );
  directory = path;
  std::ifstream myFile( path, std::ios::in | std::ios::binary );

  int meshSize;
  myFile.read( (char *) &meshSize, sizeof( int ) );
  meshes.resize( meshSize );

  min = ( std::numeric_limits<float>::max() );
  max = ( std::numeric_limits<float>::min() );

  //read mesh
  for ( int i = 0; i < meshSize; i++ )
  {
    int indicesSize;
    myFile.read( (char *) &indicesSize, sizeof( int ) );
    IndexBuffer indexbuffer;
    indexbuffer.data().resize( indicesSize );
    for ( int j = 0; j < indicesSize; j++ )
    {
      myFile.read( (char *) &indexbuffer.data()[j], sizeof( int ) );
    };

    //vertexBuffer
    int vertexBufferSize;
    myFile.read( (char *) &vertexBufferSize, sizeof( int ) );

    VertexBuffer vertexBuffer;
    vertexBuffer.data().resize( vertexBufferSize );
    for ( int j = 0; j < vertexBufferSize; j++ )
    {
      myFile.read( (char *) &vertexBuffer.data()[j], 32 );

      for ( int k = 0; k < MAX_BONE_INFLUENCE; k++ )
      {
        myFile.read( (char *) &vertexBuffer.data()[j].jointID[k], sizeof( int ) );
        myFile.read( (char *) &vertexBuffer.data()[j].weights[k], sizeof( float ) );
      }
      myFile.read( (char *) &vertexBuffer.data()[j].texture_coord.x, sizeof( float ) );
      myFile.read( (char *) &vertexBuffer.data()[j].texture_coord.y, sizeof( float ) );
    }

    for ( auto &x : vertexBuffer.data() )
    {
      auto &pos = x.position;
      min.setX( std::min( pos.getX(), min.getX() ) );
      min.setY( std::min( pos.getY(), min.getY() ) );
      min.setZ( std::min( pos.getZ(), min.getZ() ) );
      max.setX( std::max( pos.getX(), max.getX() ) );
      max.setY( std::max( pos.getY(), max.getY() ) );
      max.setZ( std::max( pos.getZ(), max.getZ() ) );
    }
    meshes.push_back( Mesh( vertexBuffer, indexbuffer ) );
  }

  //auto scale = std::abs((min.getY() + max.getY()));
  //auto center = min + max;
  //center *= 0.5f;
  //for (auto& mesh : meshes)
  //{
  //    for (auto& x : mesh.vertexBuffer.data())
  //    {
  //        x.position -= center;
  //    }
  //}

  myFile.read( (char *) &m_BoneCounter, sizeof( int ) );
  //read map
  for ( int i = 0; i < m_BoneCounter; i++ )
  {
    int tmpStringNum = 0;
    std::string tmpString;
    myFile.read( (char *) &tmpStringNum, sizeof( int ) );

    char *temp = new char[tmpStringNum + 1];
    myFile.read( temp, tmpStringNum );
    temp[tmpStringNum] = '\0';
    tmpString = temp;
    delete[] temp;

    BoneInfo tmpBoneinfo;
    myFile.read( (char *) &tmpBoneinfo.id, sizeof( int ) );
    myFile.read( (char *) &tmpBoneinfo.offset, sizeof( Math::Matrix4 ) );

    m_BoneInfoMap[tmpString] = tmpBoneinfo;
  }
  offset = (int) myFile.tellg();
  XV_CORE_INFO( "BinaryAnimated Loaded - " + path );
}
}