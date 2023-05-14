/*****************************************************************
*\file         GraphicHelper.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#define TEST1
namespace Xivi::Math::Helper
{
class MathFunction
{
public:
  template <typename T>
  static inline T clamp( const T &n, const T lower, const T upper )
  {
    return std::max( lower, std::min( n, upper ) );
  }

  static inline void clamp( Vec3f &n, float lower, float upper )
  {
    n = Vec3f(
      std::max( lower, std::min( n.getX(), upper ) ),
      std::max( lower, std::min( n.getY(), upper ) ),
      std::max( lower, std::min( n.getZ(), upper ) ) );
  }

  void clamp( Vec4f &n, float lower, float upper )
  {
    n = Vec4f(
      std::max( lower, std::min( n.getX(), upper ) ),
      std::max( lower, std::min( n.getY(), upper ) ),
      std::max( lower, std::min( n.getZ(), upper ) ),
      std::max( lower, std::min( n.getW(), upper ) )
    );
  }

  template<typename T>
  static inline Math::Matrix4 Ortho( T left, T right, T bottom, T top )
  {
    Math::Matrix4  Result( static_cast<T>( 1 ) );
    Result.identity();
    Result[0][0] = static_cast<T>( 2 ) / ( right - left );
    Result[1][1] = static_cast<T>( 2 ) / ( top - bottom );
    Result[2][2] = -static_cast<T>( 1 );
    Result[3][0] = -( right + left ) / ( right - left );
    Result[3][1] = -( top + bottom ) / ( top - bottom );
    return Result;
  }

  static inline Math::Matrix4 Perspective( float angle, float ratio, float nearPlane, float farPlane )
  {
    auto const scale = tanf( angle * 0.5f );
    Math::Matrix4 mm;
    mm[0][0] = 1 / scale / ratio;
    mm[1][1] = 1 / scale;
    mm[2][2] = -( farPlane + nearPlane ) / ( farPlane - nearPlane );
    mm[2][3] = -1.f;
    mm[3][2] = -( farPlane * nearPlane * 2.f ) / ( farPlane - nearPlane );
    mm[3][3] = 0;
    return mm;
  }

  static inline Math::Matrix4 LookAt( const Vector3 &eye, const Vector3 &at, const Vector3 &up )
  {
    Vector3 w = ( eye - at ).normalize();
    Vector3 u = up.cross( w ).normalize();
    Vector3 v = w.cross( u );

    Math::Matrix4  viewMat;
    viewMat.setRow1( Vec4f( u.getX(), v.getX(), w.getX(), 0 ) );
    viewMat.setRow2( Vec4f( u.getY(), v.getY(), w.getY(), 0 ) );
    viewMat.setRow3( Vec4f( u.getZ(), v.getZ(), w.getZ(), 0 ) );
    viewMat.setRow4( Vec4f( multiply( -u, eye ), multiply( -v, eye ), multiply( -w, eye ), 1 ) );
    return viewMat;
  }
  template<typename T>
  static inline constexpr Vec4f GetNormalisedDeviceCoordinates(
    const T &window_width,
    const T &window_height,
    const T &mouse_x,
    const T &mouse_y )
  {
    T x = static_cast<T>( 2.0 ) * mouse_x / window_width - static_cast<T>( 1.0 );
    T y = -( static_cast<T>( 2.0 ) * mouse_y / window_height - static_cast<T>( 1.0 ) );
    T z = static_cast<T>( -1.0 );
    T w = static_cast<T>( 1.0 );
    return  Vec4f( (float) x, (float) y, (float) z, (float) w );
  }

  static inline Vec4f ToViewCoords( const Math::Matrix4 &projection_matrix, const Vec4f &viewCoords )
  {
    Math::Matrix4 invProj;
    Math::SIMD::inverse( projection_matrix, invProj );
    Vec4f result = invProj.transpose() * viewCoords;
    return result;
  }

  static inline Vec3f ToWorldCoords( const Math::Matrix4 &view_matrix, const Vec4f &viewCoords )
  {
    Math::Matrix4 invView;
    Math::SIMD::inverse( view_matrix, invView );
    Vec4f worldSpace = invView.transpose() * viewCoords;
    return Vec3f( worldSpace.getX(), worldSpace.getY(), worldSpace.getZ() );
  }

  static inline Math::Matrix4 rotate( Math::Matrix4  const &m, float angle, Vec3f const &v )
  {
    float const a = angle;
    float const c = cos( a );
    float const s = sin( a );

    Vec3f axis( normalize( v ) );
    Vec3f temp( ( float( 1 ) - c ) * axis );

    Math::Matrix4 Rotate;
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    Math::Matrix4 Result;
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    Result[3] = m[3];
    return Result;
  }

  static inline float Angle( Vector3 from, Vector3 to )
  {
    from = from.normalize();
    to = to.normalize();
    float dir = from.dot( to );
    clamp( dir, -1.f, 1.f );
    return std::acosf( dir ) * 57.29578f;
  }

  static inline Math::Vector4 QuaternionEuler( const Vector3 &rot )
  {
    float cr = std::cosf( rot.getX() * 0.5f );
    float sr = std::sinf( rot.getX() * 0.5f );

    float cy = std::cosf( rot.getZ() * 0.5f );
    float sy = std::sinf( rot.getZ() * 0.5f );

    float cp = std::cosf( rot.getY() * 0.5f );
    float sp = std::sinf( rot.getY() * 0.5f );

    return Vector4 { cy * sr * cp - sy * cr * sp,
            cy * cr * sp + sy * sr * cp,
            sy * cr * cp - cy * sr * sp,
            cy * cr * cp + sy * sr * sp }.normalize();
  }

  static inline Matrix4 ToRotationMatrix( const Vec3f quat )
  {
    return ToRotationMatrix( QuaternionEuler( quat ) );
  }

  static inline Matrix4 ToRotationMatrix( const Math::Vector4 quat )
  {
    float x = quat.getX();
    float y = quat.getY();
    float z = quat.getZ();
    float w = quat.getW();
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

  template <typename T>
  static inline T LinearInterpolate( T point1, T point2, T coefficent )
  {
    return point1 * ( 1 - coefficent ) + point2 * coefficent;
  }

  static inline Vec3f Bezier( Vec3f &bezier_point0, Vec3f &bezier_point1, Vec3f &bezier_point2, f32 &ratio )
  {
    auto c1 = Math::SIMD::lerp( bezier_point0, bezier_point1, ratio );
    auto c2 = Math::SIMD::lerp( bezier_point1, bezier_point2, ratio );
    return Vec3Interpolate( c1, c2, ratio );
  }

  static inline Math::Vector3 Vec3Interpolate( Math::Vector3 point1, Math::Vector3 point2, float coefficent )
  {
    Math::Vector3 ret;
    ret.setX( LinearInterpolate( point1.getX(), point2.getX(), coefficent ) );
    ret.setY( LinearInterpolate( point1.getY(), point2.getY(), coefficent ) );
    ret.setZ( LinearInterpolate( point1.getZ(), point2.getZ(), coefficent ) );
    return ret;
  }

  static inline bool DynamicAabbAabb( const Math::Vector3 &aabbMin0, const Math::Vector3 &aabbMax0,
                                      const Math::Vector3 &aabbMin1, const Math::Vector3 &aabbMax1,
                                      const Math::Vector3 &vel0, const Math::Vector3 &vel1 )
  {
    if ( ( getX( aabbMax0 ) < getX( aabbMin1 ) ) || ( ( getX( aabbMin0 ) > getX( aabbMax1 ) ) )
         || ( getY( aabbMax0 ) < getY( aabbMin1 ) ) || ( getY( aabbMin0 ) > getY( aabbMax1 ) )
         || ( getZ( aabbMax0 ) < getZ( aabbMin1 ) ) || ( getZ( aabbMin0 ) > getZ( aabbMax1 ) ) )
    {
      return false;
    }
    else
    {
      //Calculate relative veloctiy to aabb0
      Math::Vector3 relVel = vel1 - vel0;
      float tFirst = 0.0f;
      float tLast = Service::Locator::Get<TimerEngine>().GetScaledDeltaTime();
      //X-Axis
      if ( relVel.getX() < 0 )
      {
        //CaSE 1
        if ( aabbMin0.getX() > aabbMax1.getX() )
        {
          return false;
        }
        //Case 4
        if ( aabbMax0.getX() < aabbMin1.getX() )
        {
          float diff = ( aabbMax0.getX() - aabbMin1.getX() ) / relVel.getX();
          tFirst = diff > tFirst ? diff : tFirst;
        }
        if ( aabbMin0.getX() < aabbMax1.getX() )
        {
          float diff = ( aabbMin0.getX() - aabbMax1.getX() ) / relVel.getX();
          tLast = diff < tLast ? diff : tLast;
        }
      }
      if ( relVel.getX() > 0 )
      {
        //cAse 2

        if ( aabbMin0.getX() > aabbMax1.getX() )
        {
          float diff = ( aabbMin0.getX() - aabbMax1.getX() ) / relVel.getX();
          tFirst = diff < tFirst ? diff : tFirst;
        }
        if ( aabbMax0.getX() > aabbMin1.getX() )
        {
          float diff = ( aabbMax0.getX() - aabbMin1.getX() ) / relVel.getX();
          tLast = diff > tLast ? diff : tLast;
        }
        //Case 3
        if ( aabbMax0.getX() < aabbMin1.getX() )
        {
          return false;
        }
      }
      //Y-Axis
      if ( relVel.getY() < 0 )
      {
        //CaSE 1
        if ( aabbMin0.getY() > aabbMax1.getY() )
        {
          return false;
        }
        //Case 4
        if ( aabbMax0.getY() < aabbMin1.getY() )
        {
          float diff = ( aabbMax0.getY() - aabbMin1.getY() ) / relVel.getY();
          tFirst = diff > tFirst ? diff : tFirst;
        }
        if ( aabbMin0.getY() < aabbMax1.getY() )
        {
          float diff = ( aabbMin0.getY() - aabbMax1.getY() ) / relVel.getY();
          tLast = diff < tLast ? diff : tLast;
        }
      }
      if ( relVel.getY() > 0 )
      {
        //cAse 2
        if ( aabbMin0.getY() > aabbMax1.getY() )
        {
          float diff = ( aabbMin0.getY() - aabbMax1.getY() ) / relVel.getY();
          tFirst = diff < tFirst ? diff : tFirst;
        }
        if ( aabbMax0.getY() > aabbMin1.getY() )
        {
          float diff = ( aabbMax0.getY() - aabbMin1.getY() ) / relVel.getY();
          tLast = diff > tLast ? diff : tLast;
        }
        //CAse 3
        if ( aabbMax0.getY() < aabbMin1.getY() )
        {
          return false;
        }
      }
      //Z-Axis
      if ( relVel.getZ() < 0 )
      {
        //CaSE 1
        if ( aabbMin0.getZ() > aabbMax1.getZ() )
        {
          return false;
        }
        //Case 4
        if ( aabbMax0.getZ() < aabbMin1.getZ() )
        {
          float diff = ( aabbMax0.getZ() - aabbMin1.getZ() ) / relVel.getZ();
          tFirst = diff > tFirst ? diff : tFirst;
        }
        if ( aabbMin0.getZ() < aabbMax1.getZ() )
        {
          float diff = ( aabbMin0.getZ() - aabbMax1.getZ() ) / relVel.getZ();
          tLast = diff < tLast ? diff : tLast;
        }
      }
      if ( relVel.getZ() > 0 )
      {
        //cAse 2
        if ( aabbMin0.getZ() > aabbMax1.getZ() )
        {
          float diff = ( aabbMin0.getZ() - aabbMax1.getZ() ) / relVel.getZ();
          tFirst = diff < tFirst ? diff : tFirst;
        }
        if ( aabbMax0.getZ() > aabbMin1.getZ() )
        {
          float diff = ( aabbMax0.getZ() - aabbMin1.getZ() ) / relVel.getZ();
          tLast = diff > tLast ? diff : tLast;
        }
        //Case 3
        if ( aabbMax0.getZ() < aabbMin1.getZ() )
        {
          return false;
        }
      }
      //Check if 0
      if ( relVel.getX() == 0 || relVel.getY() == 0 || relVel.getZ() == 0 )
      {
        return false;
      }
      //caSe 5
      if ( tFirst > tLast )
      {
        return false;
      }

      return true;
    }
  }

  static inline bool AabbAabb( const Math::Vector3 &aabbMin0, const Math::Vector3 &aabbMax0,
                               const Math::Vector3 &aabbMin1, const Math::Vector3 &aabbMax1 )
  {
    //++Application::mStatistics.mAabbAabbTests;
    if ( ( getX( aabbMax0 ) < getX( aabbMin1 ) ) || ( ( getX( aabbMin0 ) > getX( aabbMax1 ) ) )
         || ( getY( aabbMax0 ) < getY( aabbMin1 ) ) || ( getY( aabbMin0 ) > getY( aabbMax1 ) )
         || ( getZ( aabbMax0 ) < getZ( aabbMin1 ) ) || ( getZ( aabbMin0 ) > getZ( aabbMax1 ) ) )
    {
      return false;
    }
    return true;
  }

  static inline bool PointAabb( const Math::Vector3 &point, const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax )
  {
    //(void)aabbMax;
    //(void)aabbMin;
    //not intersecting
    return !( ( getX( point ) < getX( aabbMin ) || getX( point ) > getX( aabbMax ) )
              || ( getY( point ) < getY( aabbMin ) || getY( point ) > getY( aabbMax ) )
              || ( getZ( point ) < getZ( aabbMin ) || getZ( point ) > getZ( aabbMax ) ) );
  }

  static inline bool RayAabb( const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
                              const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax, float &t )
  {
    //++Application::mStatistics.mRayAabbTests;
    if ( PointAabb( rayStart, aabbMin, aabbMax ) )
    {
      t = 0.0f;
      return true;
    }

    Math::Vector3 min, max;
    for ( int i = 0; i < 3; ++i )
    {
      //equation gotten by subbing the ray equation into the "Plane" AABB equation
      min[i] = ( aabbMin[i] - rayStart[i] ) / rayDir[i];
      max[i] = ( aabbMax[i] - rayStart[i] ) / rayDir[i];
      if ( rayDir[i] < std::numeric_limits<float>::epsilon() )
        std::swap( min[i], max[i] );

      //prevents division by 0 and checks for parallel
      else if ( rayDir[i] == 0.0f )
        if ( aabbMin[i] > rayStart[i] || rayStart[i] > aabbMax[i] )
          return false;
    }

    float tMax = std::min( std::min( max[0], max[1] ), max[2] );
    float tMin = std::max( std::max( min[0], min[1] ), min[2] );

    if ( tMin <= tMax )
    {
      t = tMin;
      if ( tMin >= 0.0f )
        return true;
    }
    return false;
  }

  static inline Math::Vector3 Transform( Mat3Param matrix, Vec3Param vector )
  {
    float x = matrix.getRow1().dot( vector );
    float y = matrix.getRow2().dot( vector );
    float z = matrix.getRow3().dot( vector );
    return Vector3( x, y, z );
  }

  static inline bool SphereAABB( const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax
                                 , const Math::Vector3 &sphereCenter, float sphereRadius )
  {
    float x = Math::Max( aabbMin[0], Math::Min( sphereCenter[0], aabbMax[0] ) );
    float y = Math::Max( aabbMin[1], Math::Min( sphereCenter[1], aabbMax[1] ) );
    float z = Math::Max( aabbMin[2], Math::Min( sphereCenter[2], aabbMax[2] ) );

    float distance = Math::Sqrt( ( x - sphereCenter[0] ) * ( x - sphereCenter[0] ) +
                                 ( y - sphereCenter[1] ) * ( y - sphereCenter[1] ) +
                                 ( z - sphereCenter[2] ) * ( z - sphereCenter[2] ) );

    return distance < sphereRadius;
  }

  static inline bool SphereSphere( const Math::Vector3 &sphereCenter0, float sphereRadius0,
                                   const Math::Vector3 &sphereCenter1, float sphereRadius1 )
  {
    if ( ( sphereCenter1 - sphereCenter0 ).lengthSq() <= ( sphereRadius0 + sphereRadius1 ) * ( sphereRadius0 + sphereRadius1 ) )
      return true;
    return false;
  }

private:
  static inline float multiply( const Vector3 &l, const Vector3 &r )
  {
    return l.getX() * r.getX() + l.getY() * r.getY() + l.getZ() * r.getZ();
  }
};
//Settings setups
using InBETWN_Vec3 = std::pair<Math::Vector3, Math::Vector3>;
using InBETWN_Vec2 = std::pair<Math::Vector2, Math::Vector2>;
using InBETWN_Float = std::pair<float, float>;
using InBETWN_Int = std::pair<int, int>;

class RandFunction
{
public:
  template<typename T>
  static inline T RandNumberBetween( T lower, T higher )
  {
    if ( lower > higher )
      std::swap( lower, higher );
    else if ( lower == higher )
      return lower;
    auto rd = std::random_device();
    std::default_random_engine r( rd() );
    std::uniform_real_distribution<float> urd( static_cast<float>( lower ), static_cast<float>( higher ) );
    return static_cast<T>( urd( r ) );
  }

  template<typename T, typename T2>
  static inline T2 RandNumberBetween( T lower, T higher )
  {
    return static_cast<T2>( RandNumberBetween( lower, higher ) );
  }

  static inline Math::Vector3 RandNumberBetween( Math::Vector3 lower, Math::Vector3 higher )
  {
    return Math::Vector3
    (
      RandNumberBetween( lower.getX(), higher.getX() ),
      RandNumberBetween( lower.getY(), higher.getY() ),
      RandNumberBetween( lower.getZ(), higher.getZ() )
    );
  }

  static inline float RandBetweenFloat( InBETWN_Float value )
  {
    return RandNumberBetween( value.first, value.second );
  }

  static inline  Math::Vector3 RandBetweenVec3( InBETWN_Vec3 value )
  {
    return RandNumberBetween( value.first, value.second );
  }
};
}