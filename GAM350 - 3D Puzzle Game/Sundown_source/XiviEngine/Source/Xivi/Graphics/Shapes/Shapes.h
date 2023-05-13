/*****************************************************************
*\file         Shapes.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
namespace Xivi::Graphics::Shapes
{
class LineSegment
{
public:
  LineSegment();
  LineSegment( Math::Vec3Param start, Math::Vec3Param end );

  Math::Vector3 mStart;
  Math::Vector3 mEnd;
};

class Ray
{
public:
  Ray();
  Ray( Math::Vec3Param start, Math::Vec3Param dir );
  Math::Vector3 GetPoint( float t ) const;

  Math::Vector3 mStart;
  Math::Vector3 mDirection;
};

class Sphere
{
public:
  Sphere();
  Sphere( const Math::Vector3 &center, float radius );

  //void ComputeCentroid(const std::vector<Math::Vector3 >& points);
  //void ComputeRitter(const std::vector<Math::Vector3 >& points);

  bool ContainsPoint( const Math::Vector3 &point );

  Math::Vector3  GetCenter() const
  {
    return mCenter;
  }
  float GetRadius() const
  {
    return mRadius;
  }

  bool Compare( const Sphere &rhs, float epsilon ) const;

  Math::Vector3 mCenter;
  float mRadius;
};

class Aabb
{
public:
  Aabb();
  Aabb( const Math::Vector3 &min, const Math::Vector3 &max );

  static Aabb BuildFromCenterAndHalfExtents( const Math::Vector3 &center, const Math::Vector3 &halfExtents );

  // Computes the volume of this aabb.
  float GetVolume() const;
  // Computes the surface area of this aabb.
  float GetSurfaceArea() const;

  // Does this aabb completely contain the given aabb (not an intersection test).
  bool Contains( const Aabb &aabb ) const;
  // Expand the to include the given point.
  void Expand( const Math::Vector3 &point );
  // Combine the two aabbs into a new one
  static Aabb Combine( const Aabb &lhs, const Aabb &rhs );
  // See if this aabb is equal to another (with epsilon). Used for unit testing.
  bool Compare( const Aabb &rhs, float epsilon ) const;
  void Transform( const Math::Vector3 &scale, const Math::Matrix3 &rotation, const Math::Vector3 &translation );

  Math::Vector3 GetMin() const;
  Math::Vector3 GetMax() const;
  Math::Vector3 GetCenter() const;
  Math::Vector3 GetHalfSize() const;

  Math::Vector3 mMin;
  Math::Vector3 mMax;
};

//-----------------------------------------------------------------------------Triangle
class Triangle
{
public:
  Triangle();
  Triangle( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 );

  Math::Vector3 mPoints[3];
};

//-----------------------------------------------------------------------------Plane
class Plane
{
public:
  Plane();
  // Create the plane from a triangle
  Plane( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 );
  // Create the plane from a point and a normal
  Plane( const Math::Vector3 &normal, const Math::Vector3 &point );

  // Create a plane from a triangle. The plane's normal should be normalized.
  void Set( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2 );
  // Create a plane from a point and a normal. The plane's normal should be normalized.
  void Set( const Math::Vector3 &normal, const Math::Vector3 &point );

  // Get the normal of this plane.
  Math::Vector3 GetNormal() const;
  // Get the 'd' value (the distance from the origin) which can be computed as Dot(origin - pointOnPlane, normal)
  float GetDistance() const;
  float getSignedDistanceToPlan( const Vec3f &point ) const
  {
    return Math::dot( GetNormal(), point ) - GetDistance();
  }
  Math::Vector4 mData;
};

//-----------------------------------------------------------------------------Frustum
class Frustum
{
public:

  void Set(
    const Math::Vector3 &lbn, const Math::Vector3 &rbn,
    const Math::Vector3 &rtn, const Math::Vector3 &ltn,
    const Math::Vector3 &lbf, const Math::Vector3 &rbf,
    const Math::Vector3 &rtf, const Math::Vector3 &ltf );

  void Set(
    const Plane left, const Plane right,
    const Plane top, const Plane bot,
    const Plane _near, const Plane _far
  );

  // Returns the start of an array of the 6 planes of the frustum.
  Math::Vector4 *GetPlanes() const;
  Plane LeftPlane() const;
  Plane RightPlane() const;
  Plane TopPlane() const;
  Plane BottomPlane() const;
  Plane NearPlane() const;
  Plane FarPlane() const;

  // The normals all point inwards (towards the centroid of the frustum).
  Plane mPlanes[6];

  // To easily draw the frustum the original points it was constructed from are stored.
  Math::Vector3 mPoints[8];
};
}