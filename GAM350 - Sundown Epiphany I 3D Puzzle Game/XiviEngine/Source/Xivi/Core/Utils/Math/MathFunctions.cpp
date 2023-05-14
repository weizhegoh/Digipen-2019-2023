///////////////////////////////////////////////////////////////////////////////
///
/// \file MathFunctions.cpp
/// Central location for all the math used by the Zero engine.
///
/// Authors: Benjamin Strukus
/// Copyright 2010-2012, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "MathFunctions.hpp"
#include "EulerOrder.hpp"
#include "Vector2D.h"
#include "SIMD.h"

namespace Xivi::Math
{
typedef Vec2f            Vec2;
typedef Vec3f            Vec3;
typedef Vec4f            Vec4;
typedef Mat3x3          Matrix3;
typedef Mat4x4          Matrix4;
typedef Quaternion          Quat;

namespace
{
const unsigned I = 0;
const unsigned J = 1;
const unsigned H = 2;

void ClampAngle( float *angle )
{
  while ( *angle < -s_PI )
  {
    *angle += s_PI * 2;
  }
  while ( *angle > s_PI )
  {
    *angle -= s_PI * 2;
  }
}
} // namespace

///Creates a skew symmetric matrix from the given 3D vector. Multiplying a
///vector by this matrix is equivalent to the cross product using the input
///vector.
Matrix3 SkewSymmetric( Vec3Param vec3 )
{
  Matrix3 mtx { 0.0f };
  mtx[2][2] = mtx[1][1] = mtx[0][0] = 0.0f;
  mtx[0][1] = -vec3[2];
  mtx[0][2] = vec3[1];
  mtx[1][0] = vec3[2];
  mtx[1][2] = -vec3[0];
  mtx[2][0] = -vec3[1];
  mtx[2][1] = vec3[0];
  return mtx;
}

Vector3 ProjectOut( Vec3Param vector, Vec3Param normal )
{
  float projCoef = dot( vector, normal );
  return vector - normal * projCoef;
}

///Converts a quaternion to an axis-angle pair (in radians). Axis is stored in
///the Vector4's xyz and the angle is stored in the w.
Vector4 ToAxisAngle( QuatParam quaternion )
{
  Vector4 axisAngle;
  ToAxisAngle( quaternion, &axisAngle );
  return axisAngle;
}

void ToAxisAngle( QuatParam quaternion, Vec4Ptr axisAngle )
{
  XV_CORE_ASSERT_MSG( axisAngle != NULL, "[MathFunctions] Null pointer passed for axis-angle pair." );
  Quat tempQuat( Normalized( quaternion ) );

  setW( *axisAngle, 2.0f * ArcCos( tempQuat.w ) );
  float invSinAngle = Sqrt( 1.0f - tempQuat.w * tempQuat.w );

  if ( Abs( invSinAngle ) < 0.0005f )
  {
    invSinAngle = 1.0f;
  }
  else
  {
    invSinAngle = 1.0f / invSinAngle;
  }

  setX( axisAngle, tempQuat.x * invSinAngle );
  setY( axisAngle, tempQuat.y * invSinAngle );
  setZ( axisAngle, tempQuat.z * invSinAngle );
}

///Converts a quaternion to an axis-angle pair (in radians).
void ToAxisAngle( QuatParam quaternion, Vec3Ptr axis, float *radians )
{
  XV_CORE_ASSERT_MSG( axis != NULL, "[MathFunctions] Null pointer passed for axis." );
  XV_CORE_ASSERT_MSG( radians != NULL, "[MathFunctions] Null pointer passed for radians." );
  Quat tempQuat( Normalized( quaternion ) );

  *radians = 2.0f * ArcCos( tempQuat.w );
  float invSinAngle = Sqrt( 1.0f - tempQuat.w * tempQuat.w );

  if ( Abs( invSinAngle ) < 0.0005f )
  {
    invSinAngle = 1.0f;
  }
  else
  {
    invSinAngle = 1.0f / invSinAngle;
  }
  setX( axis, tempQuat.x * invSinAngle );
  setY( axis, tempQuat.y * invSinAngle );
  setZ( axis, tempQuat.z * invSinAngle );
}

///Convert a 3x3 matrix to a set of Euler angles (in radians). The desired order
///of the rotations is expected to be in the given Euler angle structure.
EulerAngles ToEulerAngles( Mat3Param matrix, EulerOrders::Enum order )
{
  EulerAngles eulerAngles( order );
  ToEulerAngles( matrix, &eulerAngles );
  return eulerAngles;
}

void ToEulerAngles( Mat3Param matrix, EulerAnglesPtr eulerAngles )
{
  XV_CORE_ASSERT_MSG( eulerAngles != NULL, "[MathFunctions] Null pointer passed for Euler angles." );
  unsigned i, j, k, h, parity, repeated, frame;
  EulerOrder::GetOrder( eulerAngles->Order, i, j, k, h, parity, repeated, frame );
  if ( EulerOrders::Yes == repeated )
  {
    float sy = Sqrt( matrix( i, j ) * matrix( i, j ) +
                     matrix( i, k ) * matrix( i, k ) );
    if ( sy > 16.0f * FLT_EPSILON )
    {
      ( *eulerAngles )[cX] = ArcTan2( matrix( i, j ), matrix( i, k ) );
      ( *eulerAngles )[cY] = ArcTan2( sy, matrix( i, i ) );
      ( *eulerAngles )[cZ] = ArcTan2( matrix( j, i ), -matrix( k, i ) );
    }
    else
    {
      ( *eulerAngles )[cX] = ArcTan2( -matrix( j, k ), matrix( j, j ) );
      ( *eulerAngles )[cY] = ArcTan2( sy, matrix( i, i ) );
      ( *eulerAngles )[cZ] = 0.0f;
    }
  }
  else
  {
    float cy = Sqrt( matrix( i, i ) * matrix( i, i ) +
                     matrix( j, i ) * matrix( j, i ) );
    if ( cy > 16.0f * FLT_EPSILON )
    {
      ( *eulerAngles )[cX] = ArcTan2( matrix( k, j ), matrix( k, k ) );
      ( *eulerAngles )[cY] = ArcTan2( -matrix( k, i ), cy );
      ( *eulerAngles )[cZ] = ArcTan2( matrix( j, i ), matrix( i, i ) );
    }
    else
    {
      ( *eulerAngles )[cX] = ArcTan2( -matrix( j, k ), matrix( j, j ) );
      ( *eulerAngles )[cY] = ArcTan2( -matrix( k, i ), cy );
      ( *eulerAngles )[cZ] = 0.0f;
    }
  }
  if ( EulerOrders::Odd == parity )
  {
    ( *eulerAngles )[cX] *= -1.0f;
    ( *eulerAngles )[cY] *= -1.0f;
    ( *eulerAngles )[cZ] *= -1.0f;
  }

  ClampAngle( &( ( *eulerAngles )[cX] ) );
  ClampAngle( &( ( *eulerAngles )[cY] ) );
  ClampAngle( &( ( *eulerAngles )[cZ] ) );

  if ( EulerOrders::Rotated == frame )
  {
    Swap( ( *eulerAngles )[cX], ( *eulerAngles )[cZ] );
  }
}

///Convert a 4x4 matrix to a set of Euler angles in radians. The desired order
///of the rotations is expected to be in the given Euler angle structure.
EulerAngles ToEulerAngles( Mat4Param matrix, EulerOrders::Enum order )
{
  EulerAngles eulerAngles( order );
  ToEulerAngles( matrix, &eulerAngles );
  return eulerAngles;
}

void ToEulerAngles( Mat4Param matrix, EulerAnglesPtr eulerAngles )
{
  XV_CORE_ASSERT_MSG( eulerAngles != NULL, "[MathFunctions] Null pointer passed for Euler angles." );

  unsigned i, j, k, h, parity, repeated, frame;
  EulerOrder::GetOrder( eulerAngles->Order, i, j, k, h, parity, repeated, frame );
  if ( EulerOrders::Yes == repeated )
  {
    float sy = Sqrt( matrix( i, j ) * matrix( i, j ) +
                     matrix( i, k ) * matrix( i, k ) );
    if ( sy > 16.0f * FLT_EPSILON )
    {
      ( *eulerAngles )[cX] = ArcTan2( matrix( i, j ), matrix( i, k ) );
      ( *eulerAngles )[cY] = ArcTan2( sy, matrix( i, i ) );
      ( *eulerAngles )[cZ] = ArcTan2( matrix( j, i ), -matrix( k, i ) );
    }
    else
    {
      ( *eulerAngles )[cX] = ArcTan2( -matrix( j, k ), matrix( j, j ) );
      ( *eulerAngles )[cY] = ArcTan2( sy, matrix( i, i ) );
      ( *eulerAngles )[cZ] = 0.0f;
    }
  }
  else
  {
    float cy = Sqrt( matrix( i, i ) * matrix( i, i ) +
                     matrix( j, i ) * matrix( j, i ) );
    if ( cy > 16.0f * FLT_EPSILON )
    {
      ( *eulerAngles )[cX] = ArcTan2( matrix( k, j ), matrix( k, k ) );
      ( *eulerAngles )[cY] = ArcTan2( -matrix( k, i ), cy );
      ( *eulerAngles )[cZ] = ArcTan2( matrix( j, i ), matrix( i, i ) );
    }
    else
    {
      ( *eulerAngles )[cX] = ArcTan2( -matrix( j, k ), matrix( j, j ) );
      ( *eulerAngles )[cY] = ArcTan2( -matrix( k, i ), cy );
      ( *eulerAngles )[cZ] = 0.0f;
    }
  }
  if ( EulerOrders::Odd == parity )
  {
    ( *eulerAngles )[cX] *= -1.0f;
    ( *eulerAngles )[cY] *= -1.0f;
    ( *eulerAngles )[cZ] *= -1.0f;
  }

  ClampAngle( &( ( *eulerAngles )[cX] ) );
  ClampAngle( &( ( *eulerAngles )[cY] ) );
  ClampAngle( &( ( *eulerAngles )[cZ] ) );

  if ( EulerOrders::Rotated == frame )
  {
    Swap( ( *eulerAngles )[cX], ( *eulerAngles )[cZ] );
  }
}

///Convert a quaternion to a set of Euler angles (in radians). The desired order
///of the rotations is expected to be in the given Euler angle structure.
EulerAngles ToEulerAngles( QuatParam quaternion, EulerOrders::Enum order )
{
  EulerAngles eulerAngles( order );
  ToEulerAngles( quaternion, &eulerAngles );
  return eulerAngles;
}

void ToEulerAngles( QuatParam quaternion, EulerAnglesPtr eulerAngles )
{
  XV_CORE_ASSERT_MSG( eulerAngles != NULL, "[MathFunctions] Null pointer passed for Euler angles." );

  Matrix3 matrix { 0.0f };
  ToMatrix3( quaternion, &matrix );
  ToEulerAngles( matrix, eulerAngles );
}

/// Converts from Vector3 to Vector2, removing the z component of the Vector3.
Vector2 ToVector2( Vec3Param v3 )
{
  return Vector2( getX( v3 ), getY( v3 ) );
}

/// Converts from Vector2 to Vector3, adding the given z component.
Vector3 ToVector3( Vec2Param v2, float z )
{
  return Vector3( v2.x, v2.y, z );
}

///Converts an axis-angle pair to a 3x3 (in radians). Axis is stored in the
///Vector4's xyz and the angle is stored in the w. Axis is assumed to be
///normalized.
Matrix3 ToMatrix3( Vec4Param axisAngle )
{
  Matrix3 matrix { 0.0f };
  ToMatrix3( axisAngle, &matrix );
  return matrix;
}

void ToMatrix3( Vec4Param axisAngle, Mat3Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  float c0 = Cos( getW( axisAngle ) );
  float n1C0 = 1.0f - c0;
  float s0 = Sin( getW( axisAngle ) );

  //| x^2(1-c0)+c0  xy(1-c0)-zs0  xz(1-c0)+ys0 |
  //| xy(1-c0)+zs0  y^2(1-c0)+c0  yz(1-c0)-xs0 |
  //| xz(1-c0)-ys0  yz(1-c0)+xs0  z^2(1-c0)+c0 |
  matrix[0][0] = getX( axisAngle ) * getX( axisAngle ) * n1C0 + c0;
  matrix[0][1] = getX( axisAngle ) * getY( axisAngle ) * n1C0 - getZ( axisAngle ) * s0;
  matrix[0][2] = getX( axisAngle ) * getZ( axisAngle ) * n1C0 + getY( axisAngle ) * s0;

  matrix[1][0] = getX( axisAngle ) * getY( axisAngle ) * n1C0 + getZ( axisAngle ) * s0;
  matrix[1][1] = getY( axisAngle ) * getY( axisAngle ) * n1C0 + c0;
  matrix[1][2] = getY( axisAngle ) * getZ( axisAngle ) * n1C0 - getX( axisAngle ) * s0;

  matrix[2][0] = getX( axisAngle ) * getZ( axisAngle ) * n1C0 - getY( axisAngle ) * s0;
  matrix[2][1] = getY( axisAngle ) * getZ( axisAngle ) * n1C0 + getX( axisAngle ) * s0;
  matrix[2][2] = getZ( axisAngle ) * getZ( axisAngle ) * n1C0 + c0;
}

///Converts an axis-angle pair to a 3x3 matrix (in radians). Axis is assumed to
///be normalized.
Matrix3 ToMatrix3( Vec3Param axis, float radians )
{
  Matrix3 matrix { 0.0f };
  ToMatrix3( axis, radians, &matrix );
  return matrix;
}

void ToMatrix3( Vec3Param axis, float radians, Mat3Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  float c0 = Cos( radians );
  float n1C0 = 1.0f - c0;
  float s0 = Sin( radians );

  //| x^2(1-c0)+c0  xy(1-c0)-zs0  xz(1-c0)+ys0 |
  //| xy(1-c0)+zs0  y^2(1-c0)+c0  yz(1-c0)-xs0 |
  //| xz(1-c0)-ys0  yz(1-c0)+xs0  z^2(1-c0)+c0 |
  matrix[0][0] = getX( axis ) * getX( axis ) * n1C0 + c0;
  matrix[0][1] = getX( axis ) * getY( axis ) * n1C0 - getZ( axis ) * s0;
  matrix[0][2] = getX( axis ) * getZ( axis ) * n1C0 + getY( axis ) * s0;

  matrix[1][0] = getX( axis ) * getY( axis ) * n1C0 + getZ( axis ) * s0;
  matrix[1][1] = getY( axis ) * getY( axis ) * n1C0 + c0;
  matrix[1][2] = getY( axis ) * getZ( axis ) * n1C0 - getX( axis ) * s0;

  matrix[2][0] = getX( axis ) * getZ( axis ) * n1C0 - getY( axis ) * s0;
  matrix[2][1] = getY( axis ) * getZ( axis ) * n1C0 + getX( axis ) * s0;
  matrix[2][2] = getZ( axis ) * getZ( axis ) * n1C0 + c0;
}

///Convert a set of Euler angles to a 3x3 matrix (in radians).
Matrix3 ToMatrix3( EulerAnglesParam eulerAngles )
{
  Matrix3 matrix { 0.0f };
  ToMatrix3( eulerAngles, &matrix );
  return matrix;
}

void ToMatrix3( EulerAnglesParam eulerAngles, Mat3Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  float angles[3] = { eulerAngles[0], eulerAngles[1], eulerAngles[2] };
  unsigned i, j, k, h, parity, repeated, frame;
  EulerOrder::GetOrder( eulerAngles.Order, i, j, k, h, parity, repeated, frame );
  if ( EulerOrders::Rotated == frame )
  {
    Swap( angles[cX], angles[cZ] );
  }
  if ( EulerOrders::Odd == parity )
  {
    angles[cX] *= -1.0f;
    angles[cY] *= -1.0f;
    angles[cZ] *= -1.0f;
  }

  float t[3], c[3], s[3];
  t[I] = angles[cX];      t[J] = angles[cY];      t[H] = angles[cZ];
  c[I] = Cos( t[I] ); c[J] = Cos( t[J] ); c[H] = Cos( t[H] );
  s[I] = Sin( t[I] ); s[J] = Sin( t[J] ); s[H] = Sin( t[H] );

  const float cc = c[I] * c[H];
  const float cs = c[I] * s[H];
  const float sc = s[I] * c[H];
  const float ss = s[I] * s[H];
  if ( EulerOrders::Yes == repeated )
  {
    ( *matrix )( i, i ) = c[J];
    ( *matrix )( i, j ) = c[J] * s[I];
    ( *matrix )( i, k ) = c[J] * c[I];

    ( *matrix )( j, i ) = c[J] * s[H];
    ( *matrix )( j, j ) = -c[J] * ss + cc;
    ( *matrix )( j, k ) = -c[J] * cs - sc;

    ( *matrix )( k, i ) = -c[J] * c[H];
    ( *matrix )( k, j ) = c[J] * sc + cs;
    ( *matrix )( k, k ) = c[J] * cc - ss;
  }
  else
  {
    ( *matrix )( i, i ) = c[J] * c[H];
    ( *matrix )( j, i ) = c[J] * s[H];
    ( *matrix )( k, i ) = -s[J];

    ( *matrix )( i, j ) = s[J] * sc - cs;
    ( *matrix )( j, j ) = s[J] * ss + cc;
    ( *matrix )( k, j ) = c[J] * s[I];

    ( *matrix )( i, k ) = s[J] * cc + ss;
    ( *matrix )( j, k ) = s[J] * cs - sc;
    ( *matrix )( k, k ) = c[J] * c[I];
  }
}

///Convert a 4x4 matrix to a 3x3 matrix. Simply copies the 4x4 matrix's upper
///3x3 matrix (rotation & scale) to the 3x3 matrix.
Matrix3 ToMatrix3( Mat4Param matrix )
{
  Matrix3 mat3 { 0.0f };
  ToMatrix3( matrix, &mat3 );
  return mat3;
}

void ToMatrix3( Mat4Param mat4, Mat3Ptr mat3 )
{
  XV_CORE_ASSERT_MSG( mat3 != NULL, "[MathFunctions] Null pointer passed for matrix." );

  //First "cross" components
  mat3[0][0] = mat4[0][0];
  mat3[0][1] = mat4[0][1];
  mat3[0][2] = mat4[0][2];

  ////Second "cross" components
  mat3[1][0] = mat4[1][0];
  mat3[1][1] = mat4[1][1];
  mat3[1][2] = mat4[1][2];

  ////Third "cross" components
  mat3[2][0] = mat4[2][0];
  mat3[2][1] = mat4[2][1];
  mat3[2][2] = mat4[2][2];
}

///Converts a quaternion to a 3x3 rotation matrix (in radians).
Matrix3 ToMatrix3( QuatParam quaternion )
{
  Matrix3 matrix { 0.0f };
  ToMatrix3( quaternion, &matrix );
  return matrix;
}

void ToMatrix3( QuatParam quaternion, Mat3Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  //     |       2     2                                |
  //     | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW     |
  //     |                                              |
  //     |                       2     2                |
  // M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW     |
  //     |                                              |
  //     |                                      2     2 |
  //     | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y  |
  //     |                                              |

  float xx = quaternion.x * quaternion.x;
  float xy = quaternion.x * quaternion.y;
  float xz = quaternion.x * quaternion.z;
  float yy = quaternion.y * quaternion.y;
  float yz = quaternion.y * quaternion.z;
  float zz = quaternion.z * quaternion.z;
  float zw = quaternion.z * quaternion.w;
  float yw = quaternion.y * quaternion.w;
  float xw = quaternion.x * quaternion.w;

  matrix[0][0] = 1.0f - 2.0f * ( yy + zz );
  matrix[0][1] = 2.0f * ( xy - zw );
  matrix[0][2] = 2.0f * ( xz + yw );

  matrix[1][0] = 2.0f * ( xy + zw );
  matrix[1][1] = 1.0f - 2.0f * ( xx + zz );
  matrix[1][2] = 2.0f * ( yz - xw );

  matrix[2][0] = 2.0f * ( xz - yw );
  matrix[2][1] = 2.0f * ( yz + xw );
  matrix[2][2] = 1.0f - 2.0f * ( xx + yy );
}

///Convert a set of Euler angles to a 4x4 matrix (in radians).
Matrix4 ToMatrix4( EulerAnglesParam eulerAngles )
{
  Matrix4 matrix { 0.0f };
  ToMatrix4( eulerAngles, &matrix );
  return matrix;
}

void ToMatrix4( EulerAnglesParam eulerAngles, Mat4Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  float angles[3] = { eulerAngles[0], eulerAngles[1], eulerAngles[2] };
  unsigned i, j, k, h, parity, repeated, frame;
  EulerOrder::GetOrder( eulerAngles.Order, i, j, k, h, parity, repeated, frame );
  if ( EulerOrders::Rotated == frame )
  {
    Swap( angles[cX], angles[cZ] );
  }
  if ( EulerOrders::Odd == parity )
  {
    angles[cX] *= -1.0f;
    angles[cY] *= -1.0f;
    angles[cZ] *= -1.0f;
  }
  float t[3], c[3], s[3];
  t[I] = angles[cX];       t[J] = angles[cY];       t[H] = angles[cZ];
  c[I] = Cos( t[I] ); c[J] = Cos( t[J] ); c[H] = Cos( t[H] );
  s[I] = Sin( t[I] ); s[J] = Sin( t[J] ); s[H] = Sin( t[H] );
  float cc = c[I] * c[H];
  float cs = c[I] * s[H];
  float sc = s[I] * c[H];
  float ss = s[I] * s[H];
  if ( EulerOrders::Yes == repeated )
  {
    ( *matrix )( i, i ) = c[J];
    ( *matrix )( i, j ) = c[J] * s[I];
    ( *matrix )( i, k ) = c[J] * c[I];

    ( *matrix )( j, i ) = c[J] * s[H];
    ( *matrix )( j, j ) = -c[J] * ss + cc;
    ( *matrix )( j, k ) = -c[J] * cs - sc;

    ( *matrix )( k, i ) = -c[J] * c[H];
    ( *matrix )( k, j ) = c[J] * sc + cs;
    ( *matrix )( k, k ) = c[J] * cc - ss;
  }
  else
  {
    ( *matrix )( i, i ) = c[J] * c[H];
    ( *matrix )( j, i ) = c[J] * s[H];
    ( *matrix )( k, i ) = -s[J];

    ( *matrix )( i, j ) = s[J] * sc - cs;
    ( *matrix )( j, j ) = s[J] * ss + cc;
    ( *matrix )( k, j ) = c[J] * s[I];

    ( *matrix )( i, k ) = s[J] * cc + ss;
    ( *matrix )( j, k ) = s[J] * cs - sc;
    ( *matrix )( k, k ) = c[J] * c[I];
  }
  matrix[0][3] = 0.0f;  matrix[1][3] = 0.0f;  matrix[2][3] = 0.0f;
  matrix[3][0] = 0.0f;  matrix[3][1] = 0.0f;  matrix[3][2] = 0.0f;
  matrix[3][3] = 1.0f;
}

///Convert a 3x3 matrix to a 4x4 matrix. Simply copies the 3x3 matrix's values
///into the rotational part of the 4x4 matrix.
Matrix4 ToMatrix4( Mat3Param matrix )
{
  Matrix4 matrix4 { 0.0f };
  ToMatrix4( matrix, &matrix4 );
  return matrix4;
}

void ToMatrix4( Mat3Param mat3, Mat4Ptr mat4 )
{
  XV_CORE_ASSERT_MSG( mat4 != NULL, "[MathFunctions] Null pointer passed for matrix." );

  //First "cross" components
  mat4[0][0] = mat3[0][0];
  mat4[0][1] = mat3[0][1];
  mat4[0][2] = mat3[0][2];
  mat4[0][3] = 0.0f;

  //Second "cross" components
  mat4[1][0] = mat3[1][0];
  mat4[1][1] = mat3[1][1];
  mat4[1][2] = mat3[1][2];
  mat4[1][3] = 0.0f;

  //Third "cross" components
  mat4[2][0] = mat3[2][0];
  mat4[2][1] = mat3[2][1];
  mat4[2][2] = mat3[2][2];
  mat4[2][3] = 0.0f;

  //Fourth "cross" components
  mat4[2][0] = 0.0f;
  mat4[2][1] = 0.0f;
  mat4[2][2] = 0.0f;
  mat4[2][3] = 1.0f;
}

///Converts a quaternion to a 4x4 rotation matrix (in radians).
Matrix4 ToMatrix4( QuatParam quaternion )
{
  Matrix4 matrix { 0.0f };
  ToMatrix4( quaternion, &matrix );
  return matrix;
}

void ToMatrix4( QuatParam quaternion, Mat4Ptr matrix )
{
  XV_CORE_ASSERT_MSG( matrix != NULL, "[MathFunctions] Null pointer passed for matrix." );

  //     |       2     2                                |
  //     | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW     |
  //     |                                              |
  //     |                       2     2                |
  // M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW     |
  //     |                                              |
  //     |                                      2     2 |
  //     | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y  |
  //     |                                              |

  float xx = quaternion.x * quaternion.x;
  float xy = quaternion.x * quaternion.y;
  float xz = quaternion.x * quaternion.z;
  float yy = quaternion.y * quaternion.y;
  float yz = quaternion.y * quaternion.z;
  float zz = quaternion.z * quaternion.z;
  float zw = quaternion.z * quaternion.w;
  float yw = quaternion.y * quaternion.w;
  float xw = quaternion.x * quaternion.w;

  matrix[0][0] = 1.0f - 2.0f * ( yy + zz );
  matrix[0][1] = 2.0f * ( xy - zw );
  matrix[0][2] = 2.0f * ( xz + yw );
  matrix[0][3] = 0.0f;

  matrix[1][0] = 2.0f * ( xy + zw );
  matrix[1][1] = 1.0f - 2.0f * ( xx + zz );
  matrix[1][2] = 2.0f * ( yz - xw );
  matrix[1][3] = 0.0f;

  matrix[2][0] = 2.0f * ( xz - yw );
  matrix[2][1] = 2.0f * ( yz + xw );
  matrix[2][2] = 1.0f - 2.0f * ( xx + yy );
  matrix[2][3] = 0.0f;

  matrix[3][0] = 0.0f;
  matrix[3][1] = 0.0f;
  matrix[3][2] = 0.0f;
  matrix[3][3] = 1.0f;
}

///Converts a 3D vector to a quaternion.
Quat ToQuaternion( Vec3Param vector )
{
  Quat quaternion;
  ToQuaternion( vector, &quaternion );
  return quaternion;
}

void ToQuaternion( Vec3Param vector, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  quaternion->Set( getX( vector ), getY( vector ), getZ( vector ), 0.0f );
}

///Converts an axis-angle pair to a quaternion (in radians). Axis is stored in
///the Vector4's xyz and the angle is stored in the w. Axis is assumed to be
///normalized.
Quat ToQuaternion( Vec4Param axisAngle )
{
  Quat quaternion;
  ToQuaternion( axisAngle, &quaternion );
  return quaternion;
}

void ToQuaternion( Vec4Param axisAngle, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  float alpha = getW( axisAngle ) * 0.5f;
  float sinAlpha = Sin( alpha );

  quaternion->x = getX( axisAngle ) * sinAlpha;
  quaternion->y = getY( axisAngle ) * sinAlpha;
  quaternion->z = getZ( axisAngle ) * sinAlpha;
  quaternion->w = Cos( alpha );
}

///Converts an axis-angle pair to a quaternion (in radians). Axis is assumed to
///be normalized.
Quaternion ToQuaternion( Vec3Param axis, float radians )
{
  Quat quaternion;
  ToQuaternion( axis, radians, &quaternion );
  return quaternion;
}

void ToQuaternion( Vec3Param axis, float radians, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  float alpha = radians * 0.5f;
  float sinAlpha = Sin( alpha );

  quaternion->x = getX( axis ) * sinAlpha;
  quaternion->y = getY( axis ) * sinAlpha;
  quaternion->z = getZ( axis ) * sinAlpha;
  quaternion->w = Cos( alpha );
}

///Convert a set of Euler angles to a quaternion (in radians).
Quat ToQuaternion( EulerAnglesParam eulerAngles )
{
  Quat quaternion;
  ToQuaternion( eulerAngles, &quaternion );
  return quaternion;
}

void ToQuaternion( EulerAnglesParam eulerAngles, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  float angles[3] = { eulerAngles[cX], eulerAngles[cY], eulerAngles[cZ] };
  unsigned i, j, k, h, parity, repeated, frame;
  EulerOrder::GetOrder( eulerAngles.Order, i, j, k, h, parity, repeated, frame );
  if ( EulerOrders::Rotated == frame )
  {
    Swap( angles[cX], angles[cZ] );
  }

  if ( EulerOrders::Odd == parity )
  {
    angles[cY] *= -1.0f;
  }

  float t[3], c[3], s[3];
  t[I] = angles[cX] * 0.5f; c[I] = Cos( t[I] ); s[I] = Sin( t[I] );
  t[J] = angles[cY] * 0.5f; c[J] = Cos( t[J] ); s[J] = Sin( t[J] );
  t[H] = angles[cZ] * 0.5f; c[H] = Cos( t[H] ); s[H] = Sin( t[H] );

  const float cc = c[I] * c[H];
  const float cs = c[I] * s[H];
  const float sc = s[I] * c[H];
  const float ss = s[I] * s[H];
  if ( EulerOrders::Yes == repeated )
  {
    angles[i] = c[J] * ( cs + sc );
    angles[j] = s[J] * ( cc + ss );
    angles[k] = s[J] * ( cs - sc );
    quaternion->w = c[J] * ( cc - ss );
  }
  else
  {
    angles[i] = c[J] * sc - s[J] * cs;
    angles[j] = c[J] * ss + s[J] * cc;
    angles[k] = c[J] * cs - s[J] * sc;
    quaternion->w = c[J] * cc + s[J] * ss;
  }
  if ( EulerOrders::Odd == parity )
  {
    angles[j] *= -1.0f;
  }
  quaternion->x = angles[cX];
  quaternion->y = angles[cY];
  quaternion->z = angles[cZ];
}

///Converts a 3x3 matrix to a quaternion (in radians).
Quat ToQuaternion( Mat3Param matrix )
{
  Quat quaternion;
  ToQuaternion( matrix, &quaternion );
  return quaternion;
}

void ToQuaternion( Mat3Param matrix, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  if ( matrix[0][0] + matrix[1][1] + matrix[2][2] > 0.0f )
  {
    float t = matrix[0][0] + matrix[1][1] + matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[3] = s * t;
    ( *quaternion )[2] = ( matrix[1][0] - matrix[0][1] ) * s;
    ( *quaternion )[1] = ( matrix[0][2] - matrix[2][0] ) * s;
    ( *quaternion )[0] = ( matrix[2][1] - matrix[1][2] ) * s;
  }
  else if ( matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2] )
  {
    float t = matrix[0][0] - matrix[1][1] - matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[0] = s * t;
    ( *quaternion )[1] = ( matrix[1][0] + matrix[0][1] ) * s;
    ( *quaternion )[2] = ( matrix[0][2] + matrix[2][0] ) * s;
    ( *quaternion )[3] = ( matrix[2][1] - matrix[1][2] ) * s;
  }
  else if ( matrix[1][1] > matrix[2][2] )
  {
    float t = -matrix[0][0] + matrix[1][1] - matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[1] = s * t;
    ( *quaternion )[0] = ( matrix[1][0] + matrix[0][1] ) * s;
    ( *quaternion )[3] = ( matrix[0][2] - matrix[2][0] ) * s;
    ( *quaternion )[2] = ( matrix[2][1] + matrix[1][2] ) * s;
  }
  else
  {
    float t = -matrix[0][0] - matrix[1][1] + matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[2] = s * t;
    ( *quaternion )[3] = ( matrix[1][0] - matrix[0][1] ) * s;
    ( *quaternion )[0] = ( matrix[0][2] + matrix[2][0] ) * s;
    ( *quaternion )[1] = ( matrix[2][1] + matrix[1][2] ) * s;
  }
}

///Converts a 4x4 matrix to a quaternion (in radians).
Quat ToQuaternion( Mat4Param matrix )
{
  Quat quaternion;
  ToQuaternion( matrix, &quaternion );
  return quaternion;
}

void ToQuaternion( Mat4Param matrix, QuatPtr quaternion )
{
  XV_CORE_ASSERT_MSG( quaternion != NULL, "[MathFunctions] Null pointer passed for quaternion." );

  if ( matrix[0][0] + matrix[1][1] + matrix[2][2] > 0.0f )
  {
    float t = matrix[0][0] + matrix[1][1] + matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[3] = s * t;
    ( *quaternion )[2] = ( matrix[1][0] - matrix[0][1] ) * s;
    ( *quaternion )[1] = ( matrix[0][2] - matrix[2][0] ) * s;
    ( *quaternion )[0] = ( matrix[2][1] - matrix[1][2] ) * s;
  }
  else if ( matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2] )
  {
    float t = matrix[0][0] - matrix[1][1] - matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[0] = s * t;
    ( *quaternion )[1] = ( matrix[1][0] + matrix[0][1] ) * s;
    ( *quaternion )[2] = ( matrix[0][2] + matrix[2][0] ) * s;
    ( *quaternion )[3] = ( matrix[2][1] - matrix[1][2] ) * s;
  }
  else if ( matrix[1][1] > matrix[2][2] )
  {
    float t = -matrix[0][0] + matrix[1][1] - matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[1] = s * t;
    ( *quaternion )[0] = ( matrix[1][0] + matrix[0][1] ) * s;
    ( *quaternion )[3] = ( matrix[0][2] - matrix[2][0] ) * s;
    ( *quaternion )[2] = ( matrix[2][1] + matrix[1][2] ) * s;
  }
  else
  {
    float t = -matrix[0][0] - matrix[1][1] + matrix[2][2] + 1.0f;
    float s = Rsqrt( t ) * 0.5f;

    ( *quaternion )[2] = s * t;
    ( *quaternion )[3] = ( matrix[1][0] - matrix[0][1] ) * s;
    ( *quaternion )[0] = ( matrix[0][2] + matrix[2][0] ) * s;
    ( *quaternion )[1] = ( matrix[2][1] + matrix[1][2] ) * s;
  }
}

///Generates a set of orthonormal vectors from the given vectors, modifying u
///and v.
void GenerateOrthonormalBasis( Vec3Param w, Vec3Ptr u, Vec3Ptr v )
{
  XV_CORE_ASSERT_MSG( u != NULL, "[MathFunctions] Null pointer passed for vector U." );
  XV_CORE_ASSERT_MSG( v != NULL, "[MathFunctions] Null pointer passed for vector V." );

  if ( ( Abs( getX( w ) ) >= Abs( getY( w ) ) ) && ( Abs( getX( w ) ) >= Abs( getZ( w ) ) ) )
  {
    setX( u, -getY( w ) );
    setY( u, getX( w ) );
    setZ( u, 0.0f );
  }
  else
  {
    setX( u, 0.0f );
    setY( u, getZ( w ) );
    setZ( u, -getY( w ) );
  }
  normalize( u );
  *v = cross( w, *u );
  normalize( v );
}

///Generates a set of orthonormal vectors from the given vectors while using
///debug checks, modifies u and v
void DebugGenerateOrthonormalBasis( Vec3Param w, Vec3Ptr u, Vec3Ptr v )
{
  XV_CORE_ASSERT_MSG( u != NULL, "[MathFunctions] Null pointer passed for vector U." );
  XV_CORE_ASSERT_MSG( v != NULL, "[MathFunctions] Null pointer passed for vector V." );

  if ( ( Abs( getX( w ) ) >= Abs( getY( w ) ) ) && ( Abs( getX( w ) ) >= Abs( getZ( w ) ) ) )
  {
    setX( u, -getY( w ) );
    setY( u, getX( w ) );
    setZ( u, 0.0f );
  }
  else
  {
    setX( u, 0.0f );
    setY( u, getZ( w ) );
    setZ( u, -getY( w ) );
  }
  attemptNormalize( u );
  *v = cross( w, *u );
  attemptNormalize( v );
}

///Perform the dot product with the specified basis vector of the 3x3 matrix and
///the given vector.
float BasisNDot( Mat3Param matrix, unsigned basisIndex, Vec3Param vector )
{
  return dot( vector, matrix.basis( basisIndex ) );
}

///Perform the dot product with the specified basis vector of the 3x3 matrix and
///the given vector.
float BasisNDot( Mat4Param matrix, unsigned basisIndex, Vec4Param vector )
{
  return dot( vector, matrix.basis( basisIndex ) );
}

///Perform the dot product with the basis vector of the 3x3 matrix that
///represents the x-axis (if the matrix is pure rotation) and the given vector.
float BasisXDot( Mat3Param matrix, Vec3Param vector )
{
  return dot( vector, matrix.basis( 1 ) );
}

///Perform the dot product with the basis vector of the 3x3 matrix that
///represents the y-axis (if the matrix is pure rotation) and the given vector.
float BasisYDot( Mat3Param matrix, Vec3Param vector )
{
  return dot( vector, matrix.basis( 2 ) );
}

///Perform the dot product with the basis vector of the 3x3 matrix that
///represents the z-axis (if the matrix is pure rotation) and the given vector.
float BasisZDot( Mat3Param matrix, Vec3Param vector )
{
  return dot( vector, matrix.basis( 3 ) );
}

///Perform the dot product with the basis vector of the 4x4 matrix that
///represents the x-axis (if the matrix is pure rotation) and the given vector.
float BasisXDot( Mat4Param matrix, Vec4Param vector )
{
  return dot( vector, matrix.basis( 1 ) );
}

///Perform the dot product with the basis vector of the 4x4 matrix that
///represents the y-axis (if the matrix is pure rotation) and the given vector.
float BasisYDot( Mat4Param matrix, Vec4Param vector )
{
  return dot( vector, matrix.basis( 2 ) );
}

///Perform the dot product with the basis vector of the 4x4 matrix that
///represents the z-axis (if the matrix is pure rotation) and the given vector.
float BasisZDot( Mat4Param matrix, Vec4Param vector )
{
  return dot( vector, matrix.basis( 3 ) );
}

//----------------------------------------------------------- Rotation Functions
float Angle( Vec2Param a, Vec2Param b )
{
  return ArcCos( dot( a, b ) );
}

float Angle( Vec3Param a, Vec3Param b )
{
  return ArcCos( dot( a, b ) );
}

float Angle( QuatParam a, QuatParam b )
{
  float dot = Dot( a, b );
  if ( dot == 0.0f )
    return 0.0f;
  float angle = 2.0f * ArcCos( dot );
  return angle;
}

Vector2 Slerp( Vec2Param start, Vec2Param end, float t )
{
  float d = dot( start, end );
  float theta = ArcCos( d ) * t;
  Vector2 relativeVec = end - start * d;
  relativeVec = normalize( relativeVec );
  return ( start * Cos( theta ) ) + ( relativeVec * Sin( theta ) );
}

Vector3 Slerp( Vec3Param start, Vec3Param end, float t )
{
  float d = dot( start, end );
  float theta = ArcCos( d ) * t;
  Vector3 relativeVec = end - start * d;
  relativeVec = normalize( relativeVec );
  return ( start * Cos( theta ) ) + ( relativeVec * Sin( theta ) );
}

template<typename type>
type GenericTowards( const type &a, const type &b, float maxAngle )
{
  const float cAngleEpsilon = 0.0000001f;

  float angle = Angle( a, b );

  if ( angle > s_PI )
  {
    angle -= s_PI * 2;
  }

  angle = Abs( angle );
  if ( angle > cAngleEpsilon )
  {
    float t = maxAngle / angle;
    if ( t > 1.0f )
    {
      t = 1.0f;
    }
    return Slerp( a, b, t );
  }
  else
  {
    return b;
  }
}

Vector2 RotateTowards( Vec2Param a, Vec2Param b, float maxAngle )
{
  Vector2 an = normalize( a );
  Vector2 bn = normalize( b );
  return GenericTowards( an, bn, maxAngle );
}

Vector3 RotateTowards( Vec3Param a, Vec3Param b, float maxAngle )
{
  Vec3 an = normalize( a );
  Vec3 bn = normalize( b );
  return GenericTowards( an, bn, maxAngle );
}

Quat RotateTowards( QuatParam a, QuatParam b, float maxAngle )
{
  return GenericTowards( a, b, maxAngle );
}

// Get the rotation angle between two vectors (in radians)
float SignedAngle( Vec3Param a, Vec3Param b, Vec3Param up )
{
    // Get the right vector
  Vec3 right = cross( a, up );
  attemptNormalize( right );

  // Get the forward and right dot products
  float forwardDot = Clamp( dot( a, b ), -1.0f, 1.0f );
  float rightDot = Clamp( dot( right, b ), -1.0f, 1.0f );

  // Get the actual angle from the forward dot product
  float finalAngle = ArcCos( forwardDot );

  // If we're actually on the left side...
  if ( rightDot > 0.0f )
  {
      // Compute the float final angle given the quadrant it's in (kinda like atan2)
    finalAngle = -finalAngle;
  }

  // Return the finally computed angle
  return finalAngle;
}
}// namespace Xivi::Math