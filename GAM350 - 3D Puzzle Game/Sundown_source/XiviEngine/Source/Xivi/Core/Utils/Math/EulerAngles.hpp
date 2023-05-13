///////////////////////////////////////////////////////////////////////////////
///
///  \file EulerAngles.hpp
///  Declaration of the EulerAngle structure, interface referenced from
///  Insomniac Games, implementation referenced from Graphics Gems IV.
///  
///  Authors: Benjamin Strukus
///  Copyright 2010-2012, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "pch.h"
#include "EulerOrder.hpp"

namespace Xivi::Math
{

typedef const Vec2f& Vec2Param;
typedef Vec2f& Vec2Ref;
///Forward declaration
// struct Vec3f;
typedef const Vec3f& Vec3Param;
typedef Vec3f& Vec3Ref;

///Forward declaration
// struct Vec4f;
typedef const Vec4f& Vec4Param;
typedef Vec4f& Vec4Ref;

///Forward declaration
// struct Mat3x3;
typedef const Mat3x3& Mat3Param;
typedef Mat3x3& Mat3Ref;

///Forward declaration
// struct Mat4x4;
typedef const Mat4x4& Mat4Param;
typedef Mat4x4& Mat4Ref;

using Vector2 = Vec2f;
using Vec2Ptr = Vector2*;
using Vector3 = Vec3f;
using Vec3Ptr = Vector3*;
using Vector4 = Vec4f;
using Vec4Ptr = Vector4*;

///Forward declaration
struct Quaternion;
typedef const Quaternion& QuatParam;
typedef Quaternion& QuatRef;

struct EulerAngles;
typedef const EulerAngles&  EulerAnglesParam;
typedef EulerAngles&        EulerAnglesRef;
typedef EulerAngles*        EulerAnglesPtr;

//----------------------------------------------------------------- Euler Angles
///Structure to provide a consistent way to deal with rotations stored as Euler
///angles.
struct EulerAngles
{
  EulerAngles(EulerOrderParam order);
  EulerAngles(Vec3Param xyzRotation, EulerOrderParam order);
  EulerAngles(float xRotation, float yRotation, float zRotation, 
              EulerOrderParam order);
  EulerAngles(Mat3Param matrix, EulerOrderParam order);
  EulerAngles(Mat4Param matrix, EulerOrderParam order);
  EulerAngles(QuatParam quaternion, EulerOrderParam order);

  ///Index operator to access the Angles data directly.
  float operator [] (unsigned index) const;

  ///Index operator to access the Angles data directly.
  float& operator [] (unsigned index);


  float I() const;
  float J() const;
  float K() const;
  float H() const;

  void I(float i);
  void J(float j);
  void K(float k);
  void H(float h);

  void Reorder(EulerOrderParam newOrder);

  Vector3      Angles;
  EulerOrder   Order;
};

}// namespace Math
