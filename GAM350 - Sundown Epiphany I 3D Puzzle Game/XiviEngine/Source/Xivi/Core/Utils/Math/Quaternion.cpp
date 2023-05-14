/*****************************************************************
*\file         Quaternion.cpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#include "Quaternion.hpp"
#include "MathFunctions.hpp"

namespace Xivi::Math
{
typedef Quaternion Quat;

const Quaternion Quaternion::cIdentity = Quaternion( 0.0f, 0.0f,
                                                     0.0f, 1.0f );

Quaternion::Quaternion( float xx, float yy, float zz, float ww )
{
  x = xx;
  y = yy;
  z = zz;
  w = ww;
}

void Quaternion::operator=( QuatParam rhs )
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
}

void Quaternion::operator+=( QuatParam rhs )
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
}

void Quaternion::operator-=( QuatParam rhs )
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
}

void Quaternion::operator*=( QuatParam rhs )
{
  Quat lhs( *this );
  x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
  y = lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
  z = lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
  w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
}

void Quaternion::operator*=( float rhs )
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  w *= rhs;
}

void Quaternion::operator/=( float rhs )
{
  XV_CORE_ASSERT_MSG( !Math::DebugIsZero( rhs ), "[Quarternion] Division by zero." );
  float reciprocal = 1.0f / rhs;
  x *= reciprocal;
  y *= reciprocal;
  z *= reciprocal;
  w *= reciprocal;
}

Quaternion Quaternion::operator-() const
{
  return Quaternion( -x, -y, -z, -w );
}

Quaternion Quaternion::operator*( QuatParam quat ) const
{
  return Quat( w * quat.x + x * quat.w + y * quat.z - z * quat.y,
               w * quat.y + y * quat.w + z * quat.x - x * quat.z,
               w * quat.z + z * quat.w + x * quat.y - y * quat.x,
               w * quat.w - x * quat.x - y * quat.y - z * quat.z );
}

Quaternion Quaternion::operator+( QuatParam rhs ) const
{
  return Quat( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
}

Quaternion Quaternion::operator-( QuatParam rhs ) const
{
  return Quat( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
}

Quaternion Quaternion::operator*( float rhs ) const
{
  return Quat( x * rhs, y * rhs, z * rhs, w * rhs );
}

Quaternion Quaternion::operator/( float rhs ) const
{
  XV_CORE_ASSERT_MSG( !Math::DebugIsZero( rhs ), "[Quarternion] Division by zero." );
  float reciprocal = 1.0f / rhs;
  return Quat( x * reciprocal, y * reciprocal, z * reciprocal, w * reciprocal );
}

bool Quaternion::operator==( QuatParam rhs ) const
{
  return x == rhs.x &&
    y == rhs.y &&
    z == rhs.z &&
    w == rhs.w;
}

bool Quaternion::operator!=( QuatParam rhs ) const
{
  return !( *this == rhs );
}

float &Quaternion::operator[]( unsigned index )
{
  XV_CORE_ASSERT_MSG( index <= 3, "[Quarternion] Subscript out of range." );
  return V4()[index];
}

float Quaternion::operator[]( unsigned index ) const
{
  XV_CORE_ASSERT_MSG( index <= 3, "[Quarternion] Subscript out of range." );
  return V4()[index];
}

void Quaternion::Set( float xx, float yy, float zz, float ww )
{
  x = xx;
  y = yy;
  z = zz;
  w = ww;
}

void Quaternion::Integrate( Vec3Param vector, float scalar )
{
  Quat q( getX( vector ) * scalar, getY( vector ) * scalar, getZ( vector ) * scalar, 0.0f );

  q *= *this;

  x += 0.5f * q.x;
  y += 0.5f * q.y;
  z += 0.5f * q.z;
  w += 0.5f * q.w;
}

float Quaternion::Normalize()
{
  float length = LengthSq();

  if ( length <= 0.00001f )
  {
    x = 0.0f;
    y = x;
    z = x;
    w = 1.0f;
    return 0.0f;
  }
  else
  {
    length = 1.0f / Length();
    *this *= length;
    return length;
  }
}

Quaternion Quaternion::Normalized() const
{
  float length = LengthSq();

  if ( length == 0.0f )
  {
    return Quat( 0.0f, 0.0f, 0.0f, 1.0f );
  }
  else
  {
    length = 1.0f / Length();
    return ( *this ) * length;
  }
}

float Quaternion::Dot( QuatParam rhs ) const
{
  return V4().dot( rhs.V4() );
}

float Quaternion::Length() const
{
  return V4().length();
}

float Quaternion::LengthSq() const
{
  return V4().lengthSq();
}

void Quaternion::Conjugate()
{
  x *= -1.0f;
  y *= -1.0f;
  z *= -1.0f;
}

Quaternion Quaternion::Conjugated() const
{
  return Quat( -x, -y, -z, w );
}

void Quaternion::Invert()
{
  Conjugate();
  float lengthSq = LengthSq();

  XV_CORE_ASSERT_MSG( !Math::DebugIsZero( lengthSq ), "[Quarternion] Division by zero." );
  *this /= lengthSq;
}

Quaternion Quaternion::Inverted() const
{
  float lengthSq = LengthSq();
  XV_CORE_ASSERT_MSG( !Math::DebugIsZero( lengthSq ), "[Quarternion] Division by zero." );
  lengthSq = 1.0f / lengthSq;
  return Quat( -x * lengthSq, -y * lengthSq, -z * lengthSq, w * lengthSq );
}

Quaternion Quaternion::Exponent() const
{
  float angle = V3().length();
  Quat quaternion( x, y, z, 0.0f );

  if ( Math::Abs( angle ) > Math::DebugEpsilon() )
  {
    quaternion.w = Math::Cos( angle );
    angle = Math::Sin( angle ) / angle;
    quaternion.x *= angle;
    quaternion.y *= angle;
    quaternion.z *= angle;
  }
  return quaternion;
}

Quaternion Quaternion::Logarithm() const
{
  Quat quaternion( x, y, z, 0.0f );
  float theta = Math::ArcCos( w );
  float sinTheta = Math::Sin( theta );

  if ( Math::Abs( sinTheta ) > Math::DebugEpsilon() )
  {
    theta = theta / sinTheta;
    quaternion.x *= theta;
    quaternion.y *= theta;
    quaternion.z *= theta;
  }
  return quaternion;
}

void Quaternion::RotateVector( Vec3Ptr vector )
{
  Quat tempVec = Math::ToQuaternion( *vector );
  Quat conjugate( -x, -y, -z, w );
  Quat result( *this );
  result *= tempVec;
  result *= conjugate;
  vector->set( result.x, result.y, result.z );
}

Vector3 Quaternion::RotatedVector( Vec3Param vector ) const
{
  Quat tempVec = Math::ToQuaternion( vector );
  Quat conjugate( -x, -y, -z, w );
  Quat result( *this );
  result *= tempVec;
  result *= conjugate;
  return Vector3( result.x, result.y, result.z );
}

void Quaternion::ZeroOut()
{
  V3().setToZero();
  w = 1.0f;
}

bool Quaternion::Valid() const
{
  return Math::IsValid( x ) && Math::IsValid( y ) &&
    Math::IsValid( z ) && Math::IsValid( w );
}

Vector3 &Quaternion::V3()
{
  return *(Vector3 *) this;
}

Vector4 &Quaternion::V4()
{
  return *(Vector4 *) this;
}

const Vector3 &Quaternion::V3() const
{
  return *(Vector3 *) this;
}

const Vector4 &Quaternion::V4() const
{
  return *(Vector4 *) this;
}

Quaternion operator*( float lhs, QuatParam rhs )
{
  return rhs * lhs;
}

void Normalize( QuatPtr quaternion )
{
  quaternion->Normalize();
}

Quaternion Normalized( QuatParam quaternion )
{
  return quaternion.Normalized();
}

float Dot( QuatParam lhs, QuatParam rhs )
{
  return lhs.Dot( rhs );
}

float Length( QuatParam quaternion )
{
  return quaternion.Length();
}

float LengthSq( QuatParam quaternion )
{
  return quaternion.LengthSq();
}

Quaternion Lerp( QuatParam pStart, QuatParam pEnd, float tValue )
{
  if ( !Math::InRange( tValue, 0.0f, 1.0f ) )
    XV_CORE_WARN( "[Quarternion] Interpolation value is not in the range of [0, 1]" );
  float alpha = tValue;
  float oneMinusAlpha = 1.0f - alpha;
  Quat quaternion( pStart.x * oneMinusAlpha + pStart.x * alpha,
                   pStart.y * oneMinusAlpha + pStart.y * alpha,
                   pStart.z * oneMinusAlpha + pStart.z * alpha,
                   pStart.w * oneMinusAlpha + pStart.w * alpha );
  Normalize( &quaternion );
  return quaternion;
}

Quaternion Slerp( QuatParam pStart, QuatParam pEnd, float tValue )
{
  //if(!Math::InRange(tValue, 0.0f, 1.0f))
  //    XV_CORE_WARN("Quaternion - Interpolation value is not in the range of [0, 1]");

  //
  // Quaternion Interpolation With Extra Spins, pp. 96f, 461f
  // Jack Morrison, Graphics Gems III, AP Professional
  //
    // calc cosine theta
  float cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

  // adjust signs (if necessary)
  Quaternion end = pEnd;
  if ( cosom < static_cast<float>( 0.0f ) )
  {
    cosom = -cosom;
    end.x = -end.x;   // Reverse all signs
    end.y = -end.y;
    end.z = -end.z;
    end.w = -end.w;
  }

  // Calculate coefficients
  float sclp, sclq;
  if ( ( static_cast<float>( 1.0f ) - cosom ) > static_cast<float>( 0.0001f ) ) // 0.0001 -> some epsillon
  {
      // Standard case (slerp)
    float omega, sinom;
    omega = std::acosf( cosom ); // extract theta from dot product's cos theta
    sinom = std::sinf( omega );
    sclp = std::sinf( ( 1.f - tValue ) * omega ) / sinom;
    sclq = std::sinf( tValue * omega ) / sinom;
  }
  else
  {
      // Very close, do linear interp (because it's faster)
    sclp = 1.f - tValue;
    sclq = tValue;
  }
  Quaternion pOut;
  pOut.x = sclp * pStart.x + sclq * end.x;
  pOut.y = sclp * pStart.y + sclq * end.y;
  pOut.z = sclp * pStart.z + sclq * end.z;
  pOut.w = sclp * pStart.w + sclq * end.w;
  return pOut;
}

Quaternion CreateDiagonalizer( Mat3Param matrix )
{
  const unsigned cMaxSteps = 50;

  Quaternion quat( 0.0f, 0.0f, 0.0f, 1.0f );
  Matrix3 quatMatrix { 0.0f };
  Matrix3 diagMatrix { 0.0f };
  for ( unsigned i = 0; i < cMaxSteps; ++i )
  {
    ToMatrix3( quat, &quatMatrix );
    diagMatrix = quatMatrix * matrix * quatMatrix.transpose();

    //Elements not on the diagonal
    Vector3 offDiag( diagMatrix( 1, 2 ), diagMatrix( 0, 2 ), diagMatrix( 0, 1 ) );

    //Magnitude of the off-diagonal elements
    Vector3 magDiag = abs( offDiag );

    //Index of the largest element
    unsigned k = ( ( getX( magDiag ) > getY( magDiag ) ) && ( getX( magDiag ) > getY( magDiag ) ) ) ? 0 :
      ( ( getY( magDiag ) > getZ( magDiag ) ) ? 1 : 2 );
    unsigned k1 = ( k + 1 ) % 3;
    unsigned k2 = ( k + 2 ) % 3;

    //Diagonal already
    if ( offDiag[k] == 0.0f )
    {
      break;
    }

    float theta = ( diagMatrix( k2, k2 ) - diagMatrix( k1, k1 ) ) /
      ( 2.0f * offDiag[k] );
    float sign = Math::GetSign( theta );

    //Make theta positive
    theta *= sign;

    //Large term in T
    float thetaTerm = theta < 1e6f ? Sqrt( Sq( theta ) + 1.0f )
      : theta;

//Sign(T) / (|T| + sqrt(T^2 + 1))
    float t = sign / ( theta + thetaTerm );

    //c = 1 / (t^2 + 1)      t = s / c
    float c = 1.0f / Sqrt( Sq( t ) + 1.0f );

    //No room for improvement - reached machine precision.
    if ( c == 1.0f )
    {
      break;
    }

    //Jacobi rotation for this iteration
    Quaternion jacobi( 0.0f, 0.0f, 0.0f, 0.0f );

    //Using 1/2 angle identity sin(a/2) = sqrt((1-cos(a))/2)
    jacobi[k] = sign * Sqrt( ( 1.0f - c ) / 2.0f );

    //Since our quat-to-matrix convention was for v*M instead of M*v
    jacobi.w = Sqrt( 1.0f - Sq( jacobi[k] ) );

    //Reached limits of floating point precision
    if ( jacobi.w == 1.0f )
    {
      break;
    }

    quat *= jacobi;
    Normalize( &quat );
  }

  return quat;
}

Matrix4 Quaternion::toRotationMatrix()
{
  Matrix4 matrix;
  float xy = x * y;
  float xz = x * z;
  float xw = x * w;
  float yz = y * z;
  float yw = y * w;
  float zw = z * w;
  float xSquared = x * x;
  float ySquared = y * y;
  float zSquared = z * z;
  matrix.setBasis( 0, 1 - 2 * ( ySquared + zSquared ), 2 * ( xy - zw ), 2 * ( xz + yw ), 0 );
  matrix.setBasis( 1, 2 * ( xy + zw ), 1 - 2 * ( xSquared + zSquared ), 2 * ( yz - xw ), 0 );
  matrix.setBasis( 2, 2 * ( xz - yw ), 2 * ( yz + xw ), 1 - 2 * ( xSquared + ySquared ), 0 );
  matrix.setBasis( 3, 0, 0, 0, 1 );
  return matrix;
}

Quaternion Quaternion::interpolate( Quaternion a, Quaternion b, float blend )
{
  Quaternion result = Quaternion( 0, 0, 0, 1 );
  float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
  float blendI = 1.f - blend;
  if ( dot < 0 )
  {
    result.w = blendI * a.w + blend * -b.w;
    result.x = blendI * a.x + blend * -b.x;
    result.y = blendI * a.y + blend * -b.y;
    result.z = blendI * a.z + blend * -b.z;
  }
  else
  {
    result.w = blendI * a.w + blend * b.w;
    result.x = blendI * a.x + blend * b.x;
    result.y = blendI * a.y + blend * b.y;
    result.z = blendI * a.z + blend * b.z;
  }
  result.Normalized();
  return result;
}
}// namespace Xivi::Math