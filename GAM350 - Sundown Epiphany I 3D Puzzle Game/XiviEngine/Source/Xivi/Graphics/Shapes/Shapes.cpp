/*****************************************************************
*\file         Shapes.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "Shapes.h"
#include "../Helper/GraphicHelper.h"
namespace Xivi::Graphics::Shapes
{
//-------------------------------------------------------------------------------------------------------------------LineSegment
LineSegment::LineSegment()
{
  mStart.setToZero();
  mEnd.setToZero();
}
LineSegment::LineSegment( Math::Vec3Param start, Math::Vec3Param end )
{
  mStart = start; mEnd = end;
}

//-------------------------------------------------------------------------------------------------------------------Ray
Ray::Ray()
{
  mStart.setToZero();
  mDirection.setToZero();
}
Ray::Ray( Math::Vec3Param start, Math::Vec3Param dir )
{
  mStart = start; mDirection = dir;
}
Math::Vector3 Ray::GetPoint( float t ) const
{
  return mStart + mDirection * t;
}
//-------------------------------------------------------------------------------------------------------------------Sphere

Sphere::Sphere()
{
  mCenter.setToZero();
  mRadius = 0.f;
}
Sphere::Sphere( const Math::Vector3 &center, float radius )
{
  mCenter = center;
  mRadius = radius;
}

static inline bool PointSphere(
  const Math::Vector3 &point,
  const Math::Vector3 &sphereCenter,
  float sphereRadius )
{
  if ( ( point - sphereCenter ).lengthSq() > sphereRadius * sphereRadius )
    return false;
  return true;
}

bool Sphere::ContainsPoint( const Math::Vector3 &point )
{
  return PointSphere( point, mCenter, mRadius );
}

bool Sphere::Compare( const Sphere &rhs, float epsilon ) const
{
  float posDiff = ( mCenter - rhs.mCenter ).length();
  float radiusDiff = Math::Abs( mRadius - rhs.mRadius );
  return posDiff < epsilon &&radiusDiff < epsilon;
}

//-------------------------------------------------------------------------------------------------------------------AABB
Aabb::Aabb()
{
  mMin = Math::Vector3( Math::PositiveMax() );
  mMax = Math::Vector3( -Math::PositiveMax() );
}

Aabb::Aabb( const Math::Vector3 &min, const Math::Vector3 &max )
{
  mMin = min;
  mMax = max;
}

Aabb Aabb::BuildFromCenterAndHalfExtents( const Math::Vector3 &center, const Math::Vector3 &halfExtents )
{
  return Aabb( center - halfExtents, center + halfExtents );
}

float Aabb::GetVolume() const
{
  Math::Vector3 l = ( mMax - mMin );
  return ( l.getX() * l.getY() * l.getZ() );
}

float Aabb::GetSurfaceArea() const
{
  Math::Vector3 l = ( mMax - mMin );
  return ( l.getX() * l.getY() + l.getX() * l.getZ() + l.getY() * l.getZ() ) * 2;
}

bool Aabb::Contains( const Aabb &aabb ) const
{
  if ( ( mMax.getX() >= aabb.GetMax().getX() && mMax.getY() >= aabb.GetMax().getY() && mMax.getZ() >= aabb.GetMax().getZ() )
       && ( mMin.getX() <= aabb.GetMin().getX() && mMin.getY() <= aabb.GetMin().getY() && mMin.getZ() <= aabb.GetMin().getZ() ) )
    return true;
  else
    return false;
}

void Aabb::Expand( const Math::Vector3 &point )
{
  for ( size_t i = 0; i < 3; ++i )
  {
    mMin[i] = Math::Min( mMin[i], point[i] );
    mMax[i] = Math::Max( mMax[i], point[i] );
  }
}

Aabb Aabb::Combine( const Aabb &lhs, const Aabb &rhs )
{
  Aabb result;
  for ( size_t i = 0; i < 3; ++i )
  {
    result.mMin[i] = Math::Min( lhs.mMin[i], rhs.mMin[i] );
    result.mMax[i] = Math::Max( lhs.mMax[i], rhs.mMax[i] );
  }
  return result;
}

bool Aabb::Compare( const Aabb &rhs, float epsilon ) const
{
  float pos1Diff = ( mMin - rhs.mMin ).length();
  float pos2Diff = ( mMax - rhs.mMax ).length();

  return pos1Diff < epsilon &&pos2Diff < epsilon;
}

void Aabb::Transform( const Math::Vector3 &scale, const Math::Matrix3 &rotation, const Math::Vector3 &translation )
{
  Math::Matrix3 rot = rotation;
  for ( int i = 0; i < 3; ++i )
    for ( int j = 0; j < 3; ++j )
      rot[i][j] = Math::Abs( rot[i][j] );

  Math::Vector3 e = ( mMax - mMin ) * 0.5f * scale;
  e = Math::Helper::MathFunction::Transform( rot, e );
  Math::Vector3 center = GetCenter() * scale;
  center = Math::Helper::MathFunction::Transform( rotation, center );
  mMax = center + e + translation;
  mMin = center - e + translation;
}

Math::Vector3 Aabb::GetMin() const
{
  return mMin;
}

Math::Vector3 Aabb::GetMax() const
{
  return mMax;
}

Math::Vector3 Aabb::GetCenter() const
{
  return ( mMin + mMax ) * 0.5f;
}

Math::Vector3 Aabb::GetHalfSize() const
{
  return ( mMax - mMin ) * 0.5f;
}

//-----------------------------------------------------------------------------Triangle
Triangle::Triangle()
{
  mPoints[0] = mPoints[1] = mPoints[2] = { 0.0f };
}

Triangle::Triangle( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 )
{
  mPoints[0] = p0;
  mPoints[1] = p1;
  mPoints[2] = p2;
}

//DebugShape& Triangle::DebugDraw() const
//{
//	return gDebugDrawer->DrawTriangle(*this);
//}

//-----------------------------------------------------------------------------Plane
Plane::Plane()
{
  mData = { 0.0f };
}

Plane::Plane( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 )
{
  Set( p0, p1, p2 );
}

Plane::Plane( const Math::Vector3 &normal, const Math::Vector3 &point )
{
  Set( normal, point );
}

void Plane::Set( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 )
{
  Set( ( p1 - p0 ).cross( p2 - p0 ), p0 );
}

void Plane::Set( const Math::Vector3 &normal, const Math::Vector3 &point )
{
  if ( length( normal ) == 0 )
    return;
  Math::Vector3 normalized = normalize( normal );
  setX( mData, getX( normalized ) );
  setY( mData, getY( normalized ) );
  setZ( mData, getZ( normalized ) );
  setW( mData, dot( normalized, point ) );
}

Math::Vector3 Plane::GetNormal() const
{
  return Math::Vector3( getX( mData ), getY( mData ), getZ( mData ) );
}

float Plane::GetDistance() const
{
  return getW( mData );
}

//-----------------------------------------------------------------------------Frustum
void Frustum::Set( const Math::Vector3 &lbn, const Math::Vector3 &rbn, const Math::Vector3 &rtn, const Math::Vector3 &ltn,
                   const Math::Vector3 &lbf, const Math::Vector3 &rbf, const Math::Vector3 &rtf, const Math::Vector3 &ltf )
{
  mPoints[0] = lbn;
  mPoints[1] = rbn;
  mPoints[2] = rtn;
  mPoints[3] = ltn;
  mPoints[4] = lbf;
  mPoints[5] = rbf;
  mPoints[6] = rtf;
  mPoints[7] = ltf;

  //left
  mPlanes[0].Set( lbf, ltf, lbn );
  //right
  mPlanes[1].Set( rbn, rtf, rbf );
  //top
  mPlanes[2].Set( ltn, ltf, rtn );
  //bot
  mPlanes[3].Set( rbn, lbf, lbn );
  //near
  mPlanes[4].Set( lbn, ltn, rbn );
  //far
  mPlanes[5].Set( rbf, rtf, lbf );
}

void Frustum::Set(
  const Plane left, const Plane right,
  const Plane top, const Plane bot,
  const Plane _near, const Plane _far
)
{
  //left
  mPlanes[0] = left;
  //right
  mPlanes[1] = right;
  //top
  mPlanes[2] = top;
  //bot
  mPlanes[3] = bot;
  //near
  mPlanes[4] = _near;
  //far
  mPlanes[5] = _far;
}

Plane Frustum::LeftPlane() const
{
  return mPlanes[0];
}
Plane Frustum::RightPlane() const
{
  return mPlanes[1];
}
Plane Frustum::TopPlane() const
{
  return mPlanes[2];
}
Plane Frustum::BottomPlane() const
{
  return mPlanes[3];
}
Plane Frustum::NearPlane() const
{
  return mPlanes[4];
}
Plane Frustum::FarPlane() const
{
  return mPlanes[5];
}

Math::Vector4 *Frustum::GetPlanes() const
{
  return (Math::Vector4 *) mPlanes;
}
}