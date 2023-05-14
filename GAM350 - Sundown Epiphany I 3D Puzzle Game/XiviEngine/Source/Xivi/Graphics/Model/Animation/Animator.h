/*****************************************************************
*\file         Animator.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Animation.h"

#ifdef TEST1

namespace Xivi::Graphics
{
class Animator
{
public:
  Animator()
  {}

  Animator( Animation *animation )
  {
    Setup( animation );
  }

  void Setup( Animation *animation )
  {
    m_CurrentTime = 0.0;
    m_CurrentAnimation = animation;
    for ( int i = 0; i < 50; i++ )
      m_FinalBoneMatrices[i] = ( Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::mat4( 1.0f ) ) );
  }

  void Change( Animation *animation )
  {
    m_CurrentTime = 0.0;
    m_CurrentAnimation = animation;
    for ( auto &x : m_FinalBoneMatrices )
    {
      x = Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::mat4( 1.0f ) );
    }
  }

  void UpdateAnimation( float dt, bool &callback, float &startFrameTimer, float &endFrameTimer )
  {
    m_DeltaTime = dt;

    if ( m_CurrentAnimation )
    {
      m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
      //if(dt > 0 )
      callback = ( m_CurrentTime >= m_CurrentAnimation->GetDuration() );

      m_CurrentTime = fmod( m_CurrentTime, m_CurrentAnimation->GetDuration() );
      CalculateBoneTransform( &m_CurrentAnimation->GetRootNode(), Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::mat4( 1.0f ) ) );

      startFrameTimer = m_CurrentTime;
      endFrameTimer = m_CurrentAnimation->GetDuration();
    }
  }

  void PlayAnimation( Animation *pAnimation )
  {
    if ( m_FinalBoneMatrices.size() == 0 )
    {
      Setup( pAnimation );
    }
    else
    {
      m_CurrentAnimation = pAnimation;
      m_CurrentTime = 0.0f;
    }
  }

  void CalculateBoneTransform( const AssimpNodeData *node, Math::Matrix4 parentTransform )
  {
    std::string nodeName = node->name;
    Math::Matrix4 nodeTransform = node->transformation;

    Bone *Bone = m_CurrentAnimation->FindBone( nodeName );

    if ( Bone )
    {
      Bone->Update( m_CurrentTime );
      nodeTransform = Bone->GetLocalTransform();
    }

    Math::Matrix4 globalTransformation = parentTransform ^ nodeTransform;
    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if ( boneInfoMap.find( nodeName ) != boneInfoMap.end() )
    {
      unsigned int index = boneInfoMap[nodeName].id;
      if ( index >= 50 )
        index = 50;
      Math::Matrix4 offset = boneInfoMap[nodeName].offset;
      m_FinalBoneMatrices[index] = globalTransformation ^ offset;
    }

    for ( int i = 0; i < node->childrenCount; i++ )
      CalculateBoneTransform( &node->children[i], globalTransformation );
  }

  std::array< Math::Matrix4, 60> &GetFinalBoneMatrices()
  {
    return m_FinalBoneMatrices;
  }

private:
  std::array< Math::Matrix4, 60> m_FinalBoneMatrices;
  Animation *m_CurrentAnimation;
  float m_CurrentTime;
  float m_DeltaTime;
};
}

#else

namespace Xivi::Graphics
{
class Animator
{
public:
  Animator()
  {}
  ~Animator()
  {}

  Animator( Animation *animation )
  {
    Setup( animation );
  }

  void Setup( Animation *animation )
  {
    m_CurrentTime = 0.0;
    m_CurrentAnimation = animation;

    m_FinalBoneMatrices.reserve( 50 );

    for ( int i = 0; i < 50; i++ )
      m_FinalBoneMatrices.push_back( glm::mat4( 1.0f ) );
  }

  void UpdateAnimation( float dt )
  {
    m_DeltaTime = dt;
    if ( m_CurrentAnimation )
    {
      m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
      m_CurrentTime = fmod( m_CurrentTime, m_CurrentAnimation->GetDuration() );
      CalculateBoneTransform( &m_CurrentAnimation->GetRootNode(), glm::mat4( 1.0f ) );
    }
  }

  void PlayAnimation( Animation *pAnimation )
  {
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
  }

  void CalculateBoneTransform( const AssimpNodeData *node, glm::mat4 parentTransform )
  {
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone *Bone = m_CurrentAnimation->FindBone( nodeName );

    if ( Bone )
    {
      Bone->Update( m_CurrentTime );
      nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    //std::cout << "nodeTransform: " << glm::to_string(nodeTransform) << std::endl;
    //std::cout << "parentTransform: " << glm::to_string(parentTransform) << std::endl;
    //std::cout << "globalTransformation: " << glm::to_string(globalTransformation) << std::endl;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if ( boneInfoMap.find( nodeName ) != boneInfoMap.end() )
    {
      int index = boneInfoMap[nodeName].id;
      glm::mat4 offset = boneInfoMap[nodeName].offset;
      m_FinalBoneMatrices[index] = globalTransformation * offset;
      //std::cout << "offset: " << glm::to_string(offset) << std::endl;
    }

    for ( int i = 0; i < node->childrenCount; i++ )
      CalculateBoneTransform( &node->children[i], globalTransformation );
  }

  std::vector<glm::mat4> &GetFinalBoneMatrices()
  {
    return m_FinalBoneMatrices;
  }

private:
  std::vector<glm::mat4> m_FinalBoneMatrices;
  Animation *m_CurrentAnimation;
  float m_CurrentTime;
  float m_DeltaTime;
};
}

#endif
