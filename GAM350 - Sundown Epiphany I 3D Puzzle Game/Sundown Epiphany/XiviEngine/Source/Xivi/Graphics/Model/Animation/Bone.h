/*****************************************************************
*\file         Bone.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../../Helper/ConversionHelper.h"
#include "../../Helper/GraphicHelper.h"
//

#ifdef TEST1

namespace Xivi::Graphics
{
struct KeyPosition
{
  glm::vec3 position;
  float timeStamp;
};

struct KeyRotation
{
  //Math::Quaternion orientation;
  glm::quat orientation;
  float timeStamp;
};

struct KeyScale
{
  glm::vec3 scale;
  float timeStamp;
};

class Bone
{
public:
  Bone()
  {}

  void Update( float animationTime )
  {
    Math::Matrix4 translation = InterpolatePosition( animationTime );
    Math::Matrix4 rotation = InterpolateRotation( animationTime );
    Math::Matrix4 scale = InterpolateScaling( animationTime );

    //std::cout << "translation " << translation << std::endl;
    //std::cout << "rotation " << rotation << std::endl;
    //std::cout << "scale " << scale << std::endl;
    //std::cout << "glm translation " << glm::to_string(Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(translation)) << std::endl;
    //std::cout << "glm rotation " << glm::to_string(Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(rotation)) << std::endl;
    //std::cout << "glm scale " << glm::to_string(Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(scale)) << std::endl;

    m_LocalTransform = translation ^ rotation ^ scale;
    //m_LocalTransform = m_LocalTransform.transpose();
    //m_LocalTransform_glm =
    //	Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(translation)
    //	* Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(rotation)
    //	* Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(scale);

    //std::cout <<""<< glm::to_string(m_LocalTransform_glm) << std::endl;
    //std::cout <<"m_LocalTransform\n "<< m_LocalTransform << std::endl;
    //std::cout <<"transpose\n "<< m_LocalTransform.transpose() << std::endl;
    //std::cout << "hu" << std::endl;
  }
  Math::Matrix4 GetLocalTransform()
  {
    return m_LocalTransform;
  }
  std::string GetBoneName() const
  {
    return m_Name;
  }
  int GetBoneID()
  {
    return m_ID;
  }

  int GetPositionIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumPositions - 1; ++index )
    {
      if ( animationTime < m_Positions[index + 1].timeStamp )
        return index;
    }
    return 0;
  }

  int GetRotationIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumRotations - 1; ++index )
    {
      if ( animationTime < m_Rotations[index + 1].timeStamp )
        return index;
    }
    return 0;
  }

  int GetScaleIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumScalings - 1; ++index )
    {
      if ( animationTime < m_Scales[index + 1].timeStamp )
        return index;
    }
    return 0;
  }
  std::vector<KeyPosition> m_Positions;
  std::vector<KeyRotation> m_Rotations;
  std::vector<KeyScale> m_Scales;
  int m_NumPositions;
  int m_NumRotations;
  int m_NumScalings;

  Math::Matrix4 m_LocalTransform;
  //glm::mat4 m_LocalTransform_glm;
  std::string m_Name;
  int m_ID;

private:

  float GetScaleFactor( float lastTimeStamp, float nextTimeStamp, float animationTime )
  {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
  }

  Math::Matrix4 InterpolatePosition( float animationTime )
  {
    if ( 1 == m_NumPositions )
      return Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::translate( glm::mat4( 1.0f ), m_Positions[0].position ) );

    int p0Index = GetPositionIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime );
    glm::vec3 finalPosition = glm::mix( m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor );
    return Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::translate( glm::mat4( 1.0f ), finalPosition ) );
  }

  Math::Matrix4 InterpolateRotation( float animationTime )
  {
    if ( 1 == m_NumRotations )
    {
      auto rotation = glm::normalize( m_Rotations[0].orientation );
      return Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::toMat4( rotation ) );
    }

    int p0Index = GetRotationIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime );
    glm::quat finalRotation = glm::slerp( m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor );
    finalRotation = glm::normalize( finalRotation );
    return  Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::toMat4( finalRotation ) );

    //if (1 == m_NumRotations)
    //{
    //	auto rotation = m_Rotations[0].orientation;
    //	//rotation.Normalize();
    //	return rotation.toRotationMatrix();
    //}

    //int p0Index = GetRotationIndex(animationTime);
    //int p1Index = p0Index + 1;
    //float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
    //	m_Rotations[p1Index].timeStamp, animationTime);
    //Math::Quat finalRotation;
    //finalRotation = Math::Slerp( m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);
    ////std::cout << finalRotation.x << " " << finalRotation.y << " " << finalRotation.z << std::endl;
    //
    ////finalRotation.Normalize();
    //return finalRotation.toRotationMatrix();;
  }

  Math::Matrix4 InterpolateScaling( float animationTime )
  {
    if ( 1 == m_NumScalings )
      return Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::scale( glm::mat4( 1.0f ), m_Scales[0].scale ) );

    int p0Index = GetScaleIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Scales[p0Index].timeStamp,
                                        m_Scales[p1Index].timeStamp, animationTime );
    glm::vec3 finalScale = glm::mix( m_Scales[p0Index].scale, m_Scales[p1Index].scale
                                     , scaleFactor );
    return Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::scale( glm::mat4( 1.0f ), finalScale ) );
  }
};
}

#else

namespace Xivi::Graphics
{
struct KeyPosition
{
  glm::vec3 position;
  float timeStamp;
};

struct KeyRotation
{
  glm::quat orientation;
  float timeStamp;
};

struct KeyScale
{
  glm::vec3 scale;
  float timeStamp;
};

class Bone
{
public:
  Bone()
  {}

  void Update( float animationTime )
  {
    glm::mat4 translation = InterpolatePosition( animationTime );
    glm::mat4 rotation = InterpolateRotation( animationTime );
    glm::mat4 scale = InterpolateScaling( animationTime );
    m_LocalTransform = translation * rotation * scale;

    //std::cout << "translation :  " << glm::to_string(translation) << std::endl;
    //std::cout << "rotation :  " << glm::to_string(rotation) << std::endl;
    //std::cout << "scale :  " << glm::to_string(scale) << std::endl;
    //std::cout << "m_LocalTransform :  " << glm::to_string(m_LocalTransform) << std::endl;
  }
  glm::mat4 GetLocalTransform()
  {
    return m_LocalTransform;
  }
  std::string GetBoneName() const
  {
    return m_Name;
  }
  int GetBoneID()
  {
    return m_ID;
  }

  int GetPositionIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumPositions - 1; ++index )
    {
      if ( animationTime < m_Positions[index + 1].timeStamp )
        return index;
    }
    return 0;
  }

  int GetRotationIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumRotations - 1; ++index )
    {
      if ( animationTime < m_Rotations[index + 1].timeStamp )
        return index;
    }
    return 0;
  }

  int GetScaleIndex( float animationTime )
  {
    for ( int index = 0; index < m_NumScalings - 1; ++index )
    {
      if ( animationTime < m_Scales[index + 1].timeStamp )
        return index;
    }
    return 0;
  }
  std::vector<KeyPosition> m_Positions;
  std::vector<KeyRotation> m_Rotations;
  std::vector<KeyScale> m_Scales;
  int m_NumPositions;
  int m_NumRotations;
  int m_NumScalings;

  glm::mat4 m_LocalTransform;
  std::string m_Name;
  int m_ID;

private:

  float GetScaleFactor( float lastTimeStamp, float nextTimeStamp, float animationTime )
  {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
  }

  glm::mat4 InterpolatePosition( float animationTime )
  {
    if ( 1 == m_NumPositions )
      return glm::translate( glm::mat4( 1.0f ), m_Positions[0].position );

    int p0Index = GetPositionIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Positions[p0Index].timeStamp,
                                        m_Positions[p1Index].timeStamp, animationTime );
    glm::vec3 finalPosition = glm::mix( m_Positions[p0Index].position, m_Positions[p1Index].position
                                        , scaleFactor );

    return glm::translate( glm::mat4( 1.0f ), finalPosition );
  }

  glm::mat4 InterpolateRotation( float animationTime )
  {
    if ( 1 == m_NumRotations )
    {
      auto rotation = glm::normalize( m_Rotations[0].orientation );
      return glm::toMat4( rotation );
    }

    int p0Index = GetRotationIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Rotations[p0Index].timeStamp,
                                        m_Rotations[p1Index].timeStamp, animationTime );
    glm::quat finalRotation = glm::slerp( m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation
                                          , scaleFactor );
    finalRotation = glm::normalize( finalRotation );
    //std::cout << "InterpolateRotation :  " << glm::to_string(glm::toMat4(finalRotation)) << std::endl;
    return glm::toMat4( finalRotation );
  }

  glm::mat4 InterpolateScaling( float animationTime )
  {
    if ( 1 == m_NumScalings )
      return glm::scale( glm::mat4( 1.0f ), m_Scales[0].scale );

    int p0Index = GetScaleIndex( animationTime );
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor( m_Scales[p0Index].timeStamp,
                                        m_Scales[p1Index].timeStamp, animationTime );
    glm::vec3 finalScale = glm::mix( m_Scales[p0Index].scale, m_Scales[p1Index].scale
                                     , scaleFactor );
                                   //std::cout << "InterpolateScaling :  " << glm::to_string(finalScale) << std::endl;
    return glm::scale( glm::mat4( 1.0f ), finalScale );
  }
};
}

#endif // DEBUG