/*****************************************************************
*\file         Animation.h
*\author(s)    Kenric Tan Wei Liang
         Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "BoneInfo.h"
#include "Bone.h"
#include "AnimationModel.h"
namespace Xivi::Graphics
{
struct AssimpNodeData
{
  #ifdef TEST1
  Math::Matrix4 transformation;
  #else
  glm::mat4 transformation;
  #endif
  std::string name;
  int childrenCount;
  std::vector<AssimpNodeData> children;
};

class Animation
{
public:
  Animation() = default;
  ~Animation()
  {};
  Animation( const std::string &animationPath, AnimatedModel *model )
  {
    XV_CORE_INFO( "Loading Animation from - " + animationPath );
    std::ifstream myFile( animationPath, std::ios::in | std::ios::binary );
    myFile.seekg( model->offset );
    myFile.read( (char *) &m_Duration, sizeof( float ) );
    myFile.read( (char *) &m_TicksPerSecond, sizeof( int ) );

    int boneSize;
    myFile.read( (char *) &boneSize, sizeof( int ) );
    m_Bones.resize( boneSize );
    auto center = model->min + model->max;
    center *= 0.5f;
    for ( int i = 0; i < boneSize; i++ )
    {
      //pos
      int m_positionSize;
      myFile.read( (char *) &m_positionSize, sizeof( int ) );
      m_Bones[i].m_Positions.resize( m_positionSize );
      for ( int j = 0; j < m_positionSize; j++ )
      {
        myFile.read( (char *) &m_Bones[i].m_Positions[j], sizeof( KeyPosition ) );
        //m_Bones[i].m_Positions[j].position -= Math::Helper::GLMHelpers::GetGLMVec(center);
        //auto scale = std::abs((model->min.getY() + model->max.getY()));
        //m_Bones[i].m_Positions[j].position.y -= scale;
      }

      //rot
      int m_rotationSize;
      myFile.read( (char *) &m_rotationSize, sizeof( int ) );
      m_Bones[i].m_Rotations.resize( m_rotationSize );
      for ( int j = 0; j < m_rotationSize; j++ )
        myFile.read( (char *) &m_Bones[i].m_Rotations[j], sizeof( KeyRotation ) );

      //scales
      int m_scaleSize;
      myFile.read( (char *) &m_scaleSize, sizeof( int ) );
      m_Bones[i].m_Scales.resize( m_scaleSize );
      for ( int j = 0; j < m_scaleSize; j++ )
        myFile.read( (char *) &m_Bones[i].m_Scales[j], sizeof( KeyScale ) );

      myFile.read( (char *) &m_Bones[i].m_NumPositions, sizeof( int ) );
      myFile.read( (char *) &m_Bones[i].m_NumRotations, sizeof( int ) );
      myFile.read( (char *) &m_Bones[i].m_NumScalings, sizeof( int ) );
      myFile.read( (char *) &m_Bones[i].m_LocalTransform, sizeof( Math::Matrix4 ) );

      int tmpStringNum = 0;
      myFile.read( (char *) &tmpStringNum, sizeof( int ) );
      char *temp = new char[tmpStringNum + 1];
      myFile.read( temp, tmpStringNum );
      temp[tmpStringNum] = '\0';
      m_Bones[i].m_Name = temp;
      delete[] temp;
      myFile.read( (char *) &m_Bones[i].m_ID, sizeof( int ) );
    }

    readData( m_RootNode, myFile );
    int boneinfomapSize;
    myFile.read( (char *) &boneinfomapSize, sizeof( int ) );

    //read map
    for ( int i = 0; i < boneinfomapSize; i++ )
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
    XV_CORE_INFO( "Animation Loaded - " + animationPath );
  }

  Bone *FindBone( const std::string &name )
  {
    auto iter = std::find_if( m_Bones.begin(), m_Bones.end(),
                              [&]( const Bone &Bone )
    {
      return Bone.GetBoneName() == name;
    }
    );
    if ( iter == m_Bones.end() ) return nullptr;
    else return &( *iter );
  }

  inline float GetTicksPerSecond()
  {
    return (float) m_TicksPerSecond;
  }

  inline float GetDuration()
  {
    return m_Duration;
  }

  inline const AssimpNodeData &GetRootNode()
  {
    return m_RootNode;
  }

  inline const std::map<std::string, BoneInfo> &GetBoneIDMap()
  {
    return m_BoneInfoMap;
  }

private:

  void coutRootData( AssimpNodeData &dest )
  {
    if ( dest.children.size() >= 0 )
    {
      for ( int i = 0; i < dest.childrenCount; i++ )
        coutRootData( dest.children[i] );
    }
  }

  void readData( AssimpNodeData &dest, std::ifstream &myFile )
  {
    int tmpStringNum = 0;
    myFile.read( (char *) &tmpStringNum, sizeof( int ) );

    char *temp = new char[tmpStringNum + 1];
    myFile.read( temp, tmpStringNum );
    temp[tmpStringNum] = '\0';
    dest.name = temp;
    delete[] temp;

    myFile.read( (char *) &dest.transformation, sizeof( Math::Matrix4 ) );
    myFile.read( (char *) &dest.childrenCount, sizeof( int ) );

    for ( int i = 0; i < dest.childrenCount; i++ )
    {
      AssimpNodeData newData;
      readData( newData, myFile );
      dest.children.push_back( newData );
    }
  }

  float m_Duration;
  int m_TicksPerSecond = 0;
  std::vector<Bone> m_Bones;
  AssimpNodeData m_RootNode;
  std::map<std::string, BoneInfo> m_BoneInfoMap;
};
}