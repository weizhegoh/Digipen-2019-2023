/*****************************************************************
*\file         ConversionHelper.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
namespace Xivi::Math::Helper
{
class GLMHelpers
{
public:

  static inline glm::vec3 GetGLMVec( const Vec3f &vec )
  {
    return glm::vec3( vec.getX(), vec.getY(), vec.getZ() );
  }

  static inline glm::vec4 GetGLMVec( const Vec4f &vec )
  {
    return glm::vec4( vec.getX(), vec.getY(), vec.getZ(), vec.getW() );
  }

  static inline glm::quat GetGLMQuat( const Xivi::Math::Quaternion &pOrientation )
  {
    return glm::quat( pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z );
  }

  static inline glm::mat4 ConvertMatrixToGLMFormat( const Xivi::Math::Matrix4 &from )
  {
    glm::mat4  to;
    to[0][0] = from[0][0]; to[1][0] = from[1][0]; to[2][0] = from[2][0]; to[3][0] = from[3][0];
    to[0][1] = from[0][1]; to[1][1] = from[1][1]; to[2][1] = from[2][1]; to[3][1] = from[3][1];
    to[0][2] = from[0][2]; to[1][2] = from[1][2]; to[2][2] = from[2][2]; to[3][2] = from[3][2];
    to[0][3] = from[0][3]; to[1][3] = from[1][3]; to[2][3] = from[2][3]; to[3][3] = from[3][3];
    return to;
  }

  static inline Vec3f GetGLMVec( const glm::vec3 &vec )
  {
    return Vec3f( vec.x, vec.y, vec.z );
  }

  static inline Vec4f GetGLMVec( const glm::vec4 &vec )
  {
    return Vec4f( vec.x, vec.y, vec.z, vec.w );
  }

  static inline Xivi::Math::Quaternion GetGLMQuat( const  glm::quat &pOrientation )
  {
    return Xivi::Math::Quaternion( pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z );
  }

  static inline Xivi::Math::Matrix4 ConvertMatrixToSIMDFormat( const glm::mat4 &from )
  {
    Xivi::Math::Matrix4  to;
    to[0][0] = from[0][0]; to[1][0] = from[1][0]; to[2][0] = from[2][0]; to[3][0] = from[3][0];
    to[0][1] = from[0][1]; to[1][1] = from[1][1]; to[2][1] = from[2][1]; to[3][1] = from[3][1];
    to[0][2] = from[0][2]; to[1][2] = from[1][2]; to[2][2] = from[2][2]; to[3][2] = from[3][2];
    to[0][3] = from[0][3]; to[1][3] = from[1][3]; to[2][3] = from[2][3]; to[3][3] = from[3][3];
    return to;
  }
};
}