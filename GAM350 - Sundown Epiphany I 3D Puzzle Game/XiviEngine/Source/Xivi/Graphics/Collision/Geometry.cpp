/* Start Header ------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: <Geometry.cpp>
Purpose: <To specify the objectives of the assignment>
Language: <C++17, Visual Studio>
Platform: <Visual Studio 2019, Windows 10>
Project: <weiliangkenric.tan_CS350_1>
Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
Creation date: 02 February 2021
End Header -------------------------------------------------------*/

#include "pch.h"
#include "../Shapes/Shapes.h"
#include "Geometry.h"

namespace Xivi
{
Math::Vector3 NormalCalculation( const Math::Vector3 &a, const Math::Vector3 b )
{
  return ( a - b ) / ( a - b ).length();
}

Math::Vector3 ProjectPointOnPlane( const Math::Vector3 &point, const Math::Vector3 &normal, float planeDistance )
{
  float distance = point.dot( normal ) - planeDistance;
  return point - ( distance * normal );
}

bool BarycentricCoordinates( const Math::Vector3 &point, const Math::Vector3 &a, const Math::Vector3 &b,
                             float &u, float &v, float epsilon )
{
  Math::Vector3 normal { a - b };
  float length = normal.lengthSq();

  if ( length <= epsilon * epsilon )
  {
    v = u = 0.0f;
    return false;
  }
  else
  {
    u = ( point - b ).dot( normal ) / length;
    v = 1 - u;

    return !( u < -epsilon || u >( 1 + epsilon ) ||
              v < -epsilon || v >( 1 + epsilon ) );
  }
}

bool BarycentricCoordinates( const Math::Vector3 &point, const Math::Vector3 &a, const Math::Vector3 &b, const Math::Vector3 &c,
                             float &u, float &v, float &w, float epsilon )
{
  if ( a == b || b == c || c == a )
  {
    u = v = w = 0;
    return false;
  }

  if ( ( a != b ) && ( a != c ) && ( b != c ) )
  {
    //three edge vector of the triangle
    Math::Vector3 v0 = point - c;
    Math::Vector3 v1 = a - c;
    Math::Vector3 v2 = b - c;

    //defining values for cramer rule
    float A = v1.dot( v1 );
    float B = v1.dot( v2 );
    float C = B;
    float D = v2.dot( v2 );
    float E = v0.dot( v1 );
    float F = v0.dot( v2 );

    float det = A * D - B * C;
    u = ( E * D - B * F ) / det;
    v = ( A * F - E * C ) / det;
    //since barycentric coordinates add up to one
    w = 1.0f - u - v;

    if ( u == std::clamp( u, -epsilon, 1.0f + epsilon )
         && ( v == std::clamp( v, -epsilon, 1.0f + epsilon ) )
         && ( w == std::clamp( w, -epsilon, 1.0f + epsilon ) ) )
      return true;
  }
  return false;
}

IntersectionType::Type PointPlane( const Math::Vector3 &point, const Math::Vector4 &plane, float epsilon )
{
  //find distance by the dot product of the plane and the point
  float distance = plane.dot( Math::Vector4( getX( point ), getY( point ), getZ( point ), -1 ) );

  if ( distance > epsilon )
    return IntersectionType::Inside;
  else if ( distance < -epsilon )
    return IntersectionType::Outside;
  return IntersectionType::Coplanar;
}

bool PointSphere( const Math::Vector3 &point, const Math::Vector3 &sphereCenter, float sphereRadius )
{
  return ( sphereCenter - point ).lengthSq() <= ( sphereRadius * sphereRadius );
}

bool PointAabb( const Math::Vector3 &point, const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax )
{
  //(void)aabbMax;
  //(void)aabbMin;
  //not intersecting
  return !( ( getX( point ) < getX( aabbMin ) || getX( point ) > getX( aabbMax ) )
            || ( getY( point ) < getY( aabbMin ) || getY( point ) > getY( aabbMax ) )
            || ( getZ( point ) < getZ( aabbMin ) || getZ( point ) > getZ( aabbMax ) ) );
}

bool RayPlane( const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
               const Math::Vector4 &plane, float &t, float epsilon )
{
  //++Application::mStatistics.mRayPlaneTests;
  //direction dot normal
  Math::Vector3 plane3 { getX( plane ), getY( plane ), getZ( plane ) };
  float denom = dot( rayDir, plane3 );
  if ( denom > -epsilon && denom < epsilon )
    return false;

  //calculating t  , t = -( (sr-po) dot n) / ( dr dot n )
  //if t is more than 0 it will intersect
  return ( ( t = ( ( getW( plane ) - dot( rayStart, plane3 ) ) / denom ) ) > 0.f );
}

bool RayTriangle( const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
                  const Math::Vector3 &triP0, const Math::Vector3 &triP1, const Math::Vector3 &triP2,
                  float &t, float triExpansionEpsilon )
{
  //++Application::mStatistics.mRayTriangleTests;

  if ( !RayPlane( rayStart, rayDir, Graphics::Shapes::Plane( triP0, triP1, triP2 ).mData, t ) )
    return false;
  Math::Vector3 pointOnPlane = ( rayDir * t ) + rayStart;
  Math::Vector3 barycentric;
  return BarycentricCoordinates( pointOnPlane, triP0, triP1, triP2,
                                 barycentric[0], barycentric[1], barycentric[2], triExpansionEpsilon );
}

bool RaySphere( const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
                const Math::Vector3 &sphereCenter, float sphereRadius,
                float &t )
{
  //++Application::mStatistics.mRaySphereTests;
  if ( PointSphere( rayStart, sphereCenter, sphereRadius ) )
    return true;

  Math::Vector3 m = sphereCenter - rayStart;
  //deifning variables for quadratic equation to solve for t
  float a, b, c;

  t = 0;
  a = dot( rayDir, rayDir );
  b = -2 * dot( m, rayDir );
  c = dot( m, m ) - sphereRadius * sphereRadius;

  //find discriminant to check for interection
  float discriminant = b * b - 4 * a * c;

  if ( discriminant < 0 )
    return false;
  else if ( discriminant == 0 )
  {
    t = -b / ( 2 * a );
    return true;
  }

  // if discriminant is more than 0
  //check for t using quadratic equation (-b+-sqrt(b*b - 4ac))/2a
  if ( ( -b + sqrtf( discriminant ) ) / ( 2.0f * a ) < 0 )
    return false;

  t = ( -b - sqrtf( discriminant ) ) / ( 2.0f * a );

  //if t is less than 0, it implies that t is behind
  if ( t < 0 )
    t = 0;

  return true;
}

bool RayAabb( const Math::Vector3 &rayStart, const Math::Vector3 &rayDir,
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

IntersectionType::Type PlaneTriangle( const Math::Vector4 &plane,
                                      const Xivi::Math::Vector3 &triP0, const Math::Vector3 &triP1, const Math::Vector3 &triP2,
                                      float epsilon )
{
  //++Application::mStatistics.mPlaneTriangleTests;
  std::array<IntersectionType::Type, 3> type;
  type[0] = ( PointPlane( triP0, plane, epsilon ) );
  type[1] = ( PointPlane( triP1, plane, epsilon ) );
  type[2] = ( PointPlane( triP2, plane, epsilon ) );

  if ( type[0] == type[1] && type[1] == type[2] && type[0] == type[2] )
    return type[0];
  else if ( type[0] != type[1] && type[1] != type[2] && type[0] != type[2] )
    return IntersectionType::Overlaps;
  else
  {
    auto Overlap = []( IntersectionType::Type first, IntersectionType::Type second, IntersectionType::Type third ) noexcept -> IntersectionType::Type
    {
      if ( first == IntersectionType::Coplanar )
      {
        if ( second != IntersectionType::Coplanar )
          return second;
        else
          return third;
      }
      else
        return IntersectionType::NotImplemented;
    };
    IntersectionType::Type ret = IntersectionType::NotImplemented;

    ret = Overlap( type[0], type[1], type[2] );
    if ( ret != IntersectionType::NotImplemented )
      return ret;
    ret = Overlap( type[1], type[0], type[2] );
    if ( ret != IntersectionType::NotImplemented )
      return ret;
    ret = Overlap( type[2], type[0], type[1] );
    if ( ret != IntersectionType::NotImplemented )
      return ret;

    return IntersectionType::Overlaps;
  }
}

IntersectionType::Type PlaneSphere( const Math::Vector4 &plane,
                                    const Math::Vector3 &sphereCenter, float sphereRadius )
{
  //++Application::mStatistics.mPlaneSphereTests;

  //find the projected point on the plane
  Math::Vector3 projPlane = ProjectPointOnPlane( sphereCenter, Math::Vector3( getX( plane ), getY( plane ), getZ( plane ) ), getW( plane ) );
  Math::Vector3 distance = sphereCenter - projPlane;

  // if the distance^2 <= sphereRadius^2 it will overlap
  if ( lengthSq( distance ) <= sphereRadius * sphereRadius )
    return IntersectionType::Overlaps;
  else
    return PointPlane( sphereCenter, plane, sphereRadius );
}

IntersectionType::Type PlaneAabb( const Math::Vector4 &plane,
                                  const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax )
{
  //++Application::mStatistics.mPlaneAabbTests;

  //use a sphere to replace a plane
  Math::Vector3 n( getX( plane ), getY( plane ), getZ( plane ) );
  //center of the plane
  Math::Vector3 center = ( aabbMin + aabbMax ) / 2;
  //radius of the plane
  Math::Vector3 halfExt = aabbMax - center;

  //real radius , need to * the sign of the normal
  float realRadius = ( getX( halfExt ) * std::abs( getX( n ) ) ) + ( getY( halfExt ) * std::abs( getY( n ) ) ) + ( getZ( halfExt ) * std::abs( getZ( n ) ) );

  //Compare using plane sphere
  if ( PlaneSphere( plane, center, realRadius ) == IntersectionType::Coplanar )
    return IntersectionType::Overlaps;
  else
    return PlaneSphere( plane, center, realRadius );
}

IntersectionType::Type FrustumTriangle( const Math::Vector4 planes[6],
                                        const Math::Vector3 &triP0, const Math::Vector3 &triP1, const Math::Vector3 &triP2,
                                        float epsilon )
{
  //++Application::mStatistics.mFrustumTriangleTests;

  std::vector<IntersectionType::Type> intersectType;
  int count = 0;
  //since a frustrum have 6 sides
  for ( int i = 0; i < 6; ++i )
  {
    intersectType.push_back( PlaneTriangle( planes[i], triP0, triP1, triP2, epsilon ) );
    if ( intersectType[i] == IntersectionType::Outside )
      return IntersectionType::Outside;
    else if ( intersectType[i] == IntersectionType::Inside )
    {
      count++;
      if ( count == 6 )
        return IntersectionType::Inside;
    }
  }

  return IntersectionType::Overlaps;
}

IntersectionType::Type FrustumSphere( const Math::Vector4 planes[6],
                                      const Math::Vector3 &sphereCenter, float sphereRadius, size_t &lastAxis )
{
  (void) lastAxis;
  //++Application::mStatistics.mFrustumSphereTests;
  std::vector<IntersectionType::Type> intersectType;
  int count = 0;
  //since a frustrum have 6 sides
  for ( int i = 0; i < 6; ++i )
  {
    intersectType.push_back( PlaneSphere( planes[i], sphereCenter, sphereRadius ) );
    if ( intersectType[i] == IntersectionType::Outside )
      return IntersectionType::Outside;
    else if ( intersectType[i] == IntersectionType::Inside )
    {
      count++;
      if ( count == 6 )
        return IntersectionType::Inside;
    }
  }
  return IntersectionType::Overlaps;
}

IntersectionType::Type FrustumAabb( const Math::Vector4 planes[6],
                                    const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax, size_t &lastAxis )
{
  //++Application::mStatistics.mFrustumAabbTests;
  size_t numPlanesInside = 0;

  //IntersectionType::Type result = IntersectionType::Inside;
  IntersectionType::Type intersectResult = PlaneAabb( planes[lastAxis], aabbMin, aabbMax );
  if ( intersectResult == IntersectionType::Outside )
    return IntersectionType::Outside;
  else if ( intersectResult == IntersectionType::Inside )
    ++numPlanesInside;

  for ( size_t i = 0; i < 6; ++i )
  {
    if ( i == lastAxis )
      continue;
    IntersectionType::Type _intersectResult = PlaneAabb( planes[i], aabbMin, aabbMax );
    if ( _intersectResult == IntersectionType::Outside )
    {
      lastAxis = i;
      return IntersectionType::Outside;
    }
    else if ( _intersectResult == IntersectionType::Inside )
    {
      ++numPlanesInside;
    }
  }

  if ( numPlanesInside == 6 )
    return IntersectionType::Inside;
  return IntersectionType::Overlaps;
}

bool SphereSphere( const Math::Vector3 &sphereCenter0, float sphereRadius0,
                   const Math::Vector3 &sphereCenter1, float sphereRadius1 )
{
  //++Application::mStatistics.mSphereSphereTests;

  // |center|^2 <= radius*radius
  if ( ( sphereCenter1 - sphereCenter0 ).lengthSq() <= ( sphereRadius0 + sphereRadius1 ) * ( sphereRadius0 + sphereRadius1 ) )
    return true;
  return false;
}

bool AabbAabb( const Math::Vector3 &aabbMin0, const Math::Vector3 &aabbMax0,
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

  /*if ((getX(aabbMax0) < getX(aabbMin1)) || (getX(aabbMax1) < getX(aabbMin0))
    || (getY(aabbMax0) < getY(aabbMin1)) || (getY(aabbMax1) < getY(aabbMin0))
    || (getZ(aabbMax0) < getZ(aabbMin1)) || (getZ(aabbMax1) < getZ(aabbMin0)))
    return false;
  return true;*/
}

//dyanmic check for aabb
bool DynamicAabbAabb( const Math::Vector3 &aabbMin0, const Math::Vector3 &aabbMax0,
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
    /*
    //Calculate relative veloctiy to aabb0
    Math::Vector3 relVel = vel1 - vel0;
    float tFirst = 0.0f;
    float tLast = TimerEngine::Instance().GetDeltaTime();
    //X-Axis
    if (relVel.getX() < 0)
    {
      //CaSE 1
      if (aabbMin0.getX() > aabbMax1.getX())
      {
        return false;
      }
      //Case 4
      if (aabbMax0.getX() < aabbMin1.getX())
      {
        float diff = (aabbMax0.getX() - aabbMin1.getX()) / relVel.getX();
        tFirst = diff > tFirst ? diff : tFirst;
      }
      if (aabbMin0.getX() < aabbMax1.getX())
      {
        float diff = (aabbMin0.getX() - aabbMax1.getX()) / relVel.getX();
        tLast = diff < tLast ? diff : tLast;
      }
    }
    if (relVel.getX() > 0)
    {
      //cAse 2

      if (aabbMin0.getX() > aabbMax1.getX())
      {
        float diff = (aabbMin0.getX() - aabbMax1.getX()) / relVel.getX();
        tFirst = diff < tFirst ? diff : tFirst;
      }
      if (aabbMax0.getX() > aabbMin1.getX())
      {
        float diff = (aabbMax0.getX() - aabbMin1.getX()) / relVel.getX();
        tLast = diff > tLast ? diff : tLast;
      }
      //Case 3
      if (aabbMax0.getX() < aabbMin1.getX())
      {
        return false;
      }
    }
    //Y-Axis
    if (relVel.getY() < 0)
    {
      //CaSE 1
      if (aabbMin0.getY() > aabbMax1.getY())
      {
        return false;
      }
      //Case 4
      if (aabbMax0.getY() < aabbMin1.getY())
      {
        float diff = (aabbMax0.getY() - aabbMin1.getY()) / relVel.getY();
        tFirst = diff > tFirst ? diff : tFirst;
      }
      if (aabbMin0.getY() < aabbMax1.getY())
      {
        float diff = (aabbMin0.getY() - aabbMax1.getY()) / relVel.getY();
        tLast = diff < tLast ? diff : tLast;
      }
    }
    if (relVel.getY() > 0)
    {
      //cAse 2
      if (aabbMin0.getY() > aabbMax1.getY())
      {
        float diff = (aabbMin0.getY() - aabbMax1.getY()) / relVel.getY();
        tFirst = diff < tFirst ? diff : tFirst;
      }
      if (aabbMax0.getY() > aabbMin1.getY())
      {
        float diff = (aabbMax0.getY() - aabbMin1.getY()) / relVel.getY();
        tLast = diff > tLast ? diff : tLast;
      }
      //CAse 3
      if (aabbMax0.getY() < aabbMin1.getY())
      {
        return false;
      }
    }
    //Z-Axis
    if (relVel.getZ() < 0)
    {
      //CaSE 1
      if (aabbMin0.getZ() > aabbMax1.getZ())
      {
        return false;
      }
      //Case 4
      if (aabbMax0.getZ() < aabbMin1.getZ())
      {
        float diff = (aabbMax0.getZ() - aabbMin1.getZ()) / relVel.getZ();
        tFirst = diff > tFirst ? diff : tFirst;
      }
      if (aabbMin0.getZ() < aabbMax1.getZ())
      {
        float diff = (aabbMin0.getZ() - aabbMax1.getZ()) / relVel.getZ();
        tLast = diff < tLast ? diff : tLast;
      }
    }
    if (relVel.getZ() > 0)
    {
      //cAse 2
      if (aabbMin0.getZ() > aabbMax1.getZ())
      {
        float diff = (aabbMin0.getZ() - aabbMax1.getZ()) / relVel.getZ();
        tFirst = diff < tFirst ? diff : tFirst;
      }
      if (aabbMax0.getZ() > aabbMin1.getZ())
      {
        float diff = (aabbMax0.getZ() - aabbMin1.getZ()) / relVel.getZ();
        tLast = diff > tLast ? diff : tLast;
      }
      //Case 3
      if (aabbMax0.getZ() < aabbMin1.getZ())
      {
        return false;
      }
    }
    //Check if 0
    if (relVel.getX() == 0 || relVel.getY() == 0 || relVel.getZ() == 0)
    {
      return false;
    }
    //caSe 5
    if (tFirst > tLast)
    {
      return false;
    }
    */
    return true;
  }
}

bool SphereAABB( const Math::Vector3 &aabbMin, const Math::Vector3 &aabbMax
                 , const Math::Vector3 &sphereCenter, float sphereRadius )
{
  // Compute squared distance between sphere center and AABB
  // the sqrt(dist) is fine to use as well, but this is faster.

  // Sphere and AABB intersect if the (squared) distance between them is
  // less than the (squared) sphere radius.
  //

  //float sqDist = 0.0f;
  //for (int i = 0; i < 3; i++) {
  //	// for each axis count any excess distance outside box extents
  //	float v = sphereCenter[i];
  //	if (v < aabbMin[i]) sqDist += (aabbMin[i] - v) * (aabbMin[i] - v);
  //	if (v > aabbMax[i]) sqDist += (v - aabbMax[i]) * (v - aabbMax[i]);
  //}

  //return sqDist <= sphereRadius * sphereRadius;

  //test 2
  /*float dmin = 0;

  if (sphereCenter[0] < aabbMin[0]) {
    dmin += ((sphereCenter[0] - aabbMin[0]) * (sphereCenter[0] - aabbMin[0]));
  }
  else if (sphereCenter[0] > aabbMax[0]) {
    dmin += ((sphereCenter[0] - aabbMax[0]) * (sphereCenter[0] - aabbMax[0]));
  }

  if (sphereCenter[1] < aabbMin[1]) {
    dmin += ((sphereCenter[1] - aabbMin[1]) * (sphereCenter[1] - aabbMin[1]));
  }
  else if (sphereCenter[1] > aabbMax[1]) {
    dmin += ((sphereCenter[1] - aabbMax[1]) * (sphereCenter[1] - aabbMax[1]));
  }

  if (sphereCenter[2] < aabbMin[2]) {
    dmin += ((sphereCenter[2] - aabbMin[2]) * (sphereCenter[2] - aabbMin[2]));
  }
  else if (sphereCenter[2] > aabbMax[2]) {
    dmin += ((sphereCenter[2] - aabbMax[2]) * (sphereCenter[2] - aabbMax[2]));
  }

  return dmin <= sphereRadius* sphereRadius;*/

  //test 3

  float x = Math::Max( aabbMin[0], Math::Min( sphereCenter[0], aabbMax[0] ) );
  float y = Math::Max( aabbMin[1], Math::Min( sphereCenter[1], aabbMax[1] ) );
  float z = Math::Max( aabbMin[2], Math::Min( sphereCenter[2], aabbMax[2] ) );

  // this is the same as isPointInsideSphere
  float distance = Math::Sqrt( ( x - sphereCenter[0] ) * ( x - sphereCenter[0] ) +
                               ( y - sphereCenter[1] ) * ( y - sphereCenter[1] ) +
                               ( z - sphereCenter[2] ) * ( z - sphereCenter[2] ) );

  return distance < sphereRadius;

  // Calculate closest point on box to sphere.
  //Math::Vector3 point;
  //float coordinate = 0.0f;

  //coordinate = getX(sphereCenter);
  //coordinate = Math::Max(coordinate, getX(aabbMin)); // min
  //coordinate = Math::Max(coordinate, getX(aabbMax)); // max
  //point[0] = coordinate;

  //coordinate = getY(sphereCenter);
  //coordinate = Math::Max(coordinate, getY(aabbMin)); // min
  //coordinate = Math::Max(coordinate, getY(aabbMax)); // max
  //point[1] = coordinate;

  //coordinate = getZ(sphereCenter);
  //coordinate = Math::Max(coordinate, getZ(aabbMin)); // min
  //coordinate = Math::Max(coordinate, getZ(aabbMax)); // max
  //point[2] = coordinate;

  //// Calculate vector from point_ to sphere centre.
  //Math::Vector3 vec = point - sphereCenter;

  //float vecLength = vec.length();

  //if (vecLength < sphereRadius)
  //{
  //	return true;
  //}

  //return false;
}
}