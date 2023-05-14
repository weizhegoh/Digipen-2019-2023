/*****************************************************************
*\file         Quaternion.hpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "pch.h"
#include "Reals.hpp"
//#include "Vector3.hpp"
//#include "Vector4.hpp"
//#include "Matrix3.hpp"
//#include "Matrix4.hpp"
#include "Vector2D.h"
#include "SIMD.h"
#include "Matrix.h"

namespace Xivi::Math
{

///Forward declaration
// struct ::Xivi::Math::SIMD::SIMDVec3;
typedef const Vec3f &Vec3Param;
typedef Vec3f &Vec3Ref;

///Forward declaration
// struct ::Xivi::Math::SIMD::SIMDVec4;
typedef const Vec4f &Vec4Param;
typedef Vec4f &Vec4Ref;

///Forward declaration
// struct ::Xivi::Math::SIMD::Mat3x3;
typedef const Mat3x3 &Mat3Param;
typedef Mat3x3 &Mat3Ref;

///Forward declaration
// struct ::Xivi::Math::SIMD::Mat4x4;
typedef const Mat4x4 &Mat4Param;
typedef Mat4x4 &Mat4Ref;

using Vector2 = Vec2f;
using Vec2Ptr = Vector2 *;
using Vector3 = Vec3f;
using Vec3Ptr = Vector3 *;
using Vector4 = Vec4f;
using Vec4Ptr = Vector4 *;
using Matrix3 = Mat3x3;
using Matrix4 = Mat4x4;

struct Quaternion;
typedef const Quaternion &QuatParam;
typedef Quaternion &QuatRef;
typedef Quaternion *QuatPtr;
typedef Quaternion Quat;

//------------------------------------------------------------------- Quaternion
struct Quaternion
{
  static const Quaternion cIdentity;

  Quaternion() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
  {};
  explicit Quaternion( float x, float y, float z, float w );

  void operator=( QuatParam rhs );
  void operator+=( QuatParam rhs );
  void operator-=( QuatParam rhs );
  void operator*=( QuatParam rhs );
  void operator*=( float rhs );
  void operator/=( float rhs );

  Quaternion operator-() const;

  Quaternion operator*( QuatParam rhs ) const;
  Quaternion operator+( QuatParam rhs ) const;
  Quaternion operator-( QuatParam rhs ) const;
  Quaternion operator*( float rhs ) const;
  Quaternion operator/( float rhs ) const;

  bool operator==( QuatParam rhs ) const;
  bool operator!=( QuatParam rhs ) const;

  float &operator[]( unsigned index );
  float operator[]( unsigned index ) const;

  void Set( float x, float y, float z, float w );

  void Integrate( Vec3Param vector, float scalar );

  float Normalize();
  Quaternion Normalized() const;
  float Dot( QuatParam rhs ) const;
  float Length() const;
  float LengthSq() const;

  void Conjugate();
  Quaternion Conjugated() const;
  void Invert();
  Quaternion Inverted() const;
  Quaternion Exponent() const;
  Quaternion Logarithm() const;
  //Quaternion Lerp(QuatParam end, float tValue);
  //Quaternion Slerp(QuatParam end, float tValue);
  void RotateVector( Vec3Ptr vector );
  Vector3 RotatedVector( Vec3Param vector ) const;
  void ZeroOut();

  bool Valid() const;

  Vector3 &V3();
  Vector4 &V4();
  const Vector3 &V3() const;
  const Vector4 &V4() const;

  Matrix4 toRotationMatrix();
  Quaternion interpolate( Quaternion a, Quaternion b, float blend );

  float x { 0.0f }, y { 0.0f }, z { 0.0f }, w { 0.0f };
};

Quaternion operator*( float lhs, QuatParam rhs );

void Normalize( QuatPtr quaternion );
Quaternion Normalized( QuatParam quaternion );
float Dot( QuatParam lhs, QuatParam rhs );
float Length( QuatParam quaternion );
float LengthSq( QuatParam quaternion );
Quaternion Lerp( QuatParam start, QuatParam end, float tValue );
Quaternion Slerp( QuatParam start, QuatParam end, float tValue );
Quaternion CreateDiagonalizer( Mat3Param matrix );
}// namespace Math
