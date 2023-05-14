/*****************************************************************
*\file         DebugDraw.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "DebugDraw.h"
namespace Xivi::Graphics
{
DebugDrawer::DebugDrawer()
{
  mActiveMask = (unsigned int) -1;
  mCameraEye = Math::Vector3( 0.f );
  mCameraDir = Math::Vector3( 0.f );;
}

void DebugDrawer::Update( float dt, Math::Vector3 cameraEye, Math::Vector3 cameraDir )
{
  mCameraEye = cameraEye; mCameraDir = cameraDir;
  std::vector<DebugShape> newShapes;
  for ( size_t i = 0; i < mShapes.size(); ++i )
  {
    DebugShape &shape = mShapes[i];
    shape.mTimer -= dt;
    if ( shape.mTimer >= 0 )
      newShapes.push_back( shape );
  }
  mShapes.swap( newShapes );
}

DebugShape &DebugDrawer::GetNewShape()
{
  mShapes.push_back( DebugShape() );
  return mShapes.back();
}
void DebugDrawer::CreateDisc(
  DebugShape &shape,
  const Math::Vector3 &center,
  const Math::Vector3 &axis0,
  const Math::Vector3 &axis1,
  float radius )
{
  if ( Math::cross( axis0, axis1 ) == Math::Vector3( 0.f, 0.f, 0.f ) )
    return;
  float alpha = 0;
  for ( int i = 0; i < 20; ++i )
  {
    Math::Vector3 p0 = center + radius * ( axis0 * Math::Cos( alpha ) + axis1 * Math::Sin( alpha ) );
    alpha += ( 2.f * Math::s_PI ) / 20.f;
    Math::Vector3 p1 = center + radius * ( axis0 * Math::Cos( alpha ) + axis1 * Math::Sin( alpha ) );
    shape.mSegments.push_back( Shapes::LineSegment( p0, p1 ) );
  }
}

DebugShape &DebugDrawer::DrawPoint( const Math::Vector3 &point )
{
  return DrawSphere( Shapes::Sphere( point, 0.1f ) );
}

DebugShape &DebugDrawer::DrawLine( const Shapes::LineSegment &line )
{
  // Draw a simple line
  DebugShape &shape = GetNewShape();
  shape.mSegments.push_back( line );
  return shape;
}

DebugShape &DebugDrawer::DrawRay( const Shapes::Ray &ray, float t )
{
  // Draw a ray to a given t-length. The ray must have an arrow head for visualization
  DebugShape &shape = GetNewShape();

  Math::Vector3 end = ray.mStart + ray.mDirection * t;
  shape.mSegments.push_back( Shapes::LineSegment( ray.mStart, end ) );
  Math::Vector3 nDir = ray.mDirection;
  nDir = nDir.normalize();
  Math::Vector3 axisX, axisY;
  Math::GenerateOrthonormalBasis( nDir, &axisX, &axisY );
  axisX *= 0.2f;
  axisY *= 0.2f;
  Math::Vector3 center = end - nDir * 2 * 0.2f;
  CreateDisc( shape, center, axisX, axisY, 1 );
  //link end to disc
  shape.mSegments.push_back( Shapes::LineSegment( end, center + axisX ) );
  shape.mSegments.push_back( Shapes::LineSegment( end, center - axisX ) );
  shape.mSegments.push_back( Shapes::LineSegment( end, center + axisY ) );
  shape.mSegments.push_back( Shapes::LineSegment( end, center - axisY ) );
  return shape;
}

const Math::Vector3 cXAxis( 1.0f, 0.0f, 0.0f );
const Math::Vector3 cYAxis( 0.0f, 1.0f, 0.0f );
const Math::Vector3 cZAxis( 0.0f, 0.0f, 1.0f );
static inline float DebugEpsilon()
{
  return 0.00000001f;
}

static inline float AttemptNormalize( Math::Vector3 &var1 )
{
  float lengthSq = var1.lengthSq();
  if ( lengthSq >= DebugEpsilon() * DebugEpsilon() )
  {
    lengthSq = Math::Sqrt( lengthSq );
    var1 /= lengthSq;
  }
  return lengthSq;
}

DebugShape &DebugDrawer::DrawSphere( const Shapes::Sphere &sphere )
{
  DebugShape &shape = GetNewShape();
  CreateDisc( shape, sphere.mCenter, cXAxis, cYAxis, sphere.mRadius );
  CreateDisc( shape, sphere.mCenter, cXAxis, cZAxis, sphere.mRadius );
  CreateDisc( shape, sphere.mCenter, cYAxis, cZAxis, sphere.mRadius );

  //if (mApplication != nullptr)
  {
    Math::Vector3 eye = mCameraEye;
    Math::Vector3 eyeDir = mCameraDir;
    Math::Vector3 eyeToCenter = sphere.mCenter - eye;

    float distanceToSphere = AttemptNormalize( eyeToCenter );
    if ( distanceToSphere > sphere.mRadius )
    {
      float distSq = distanceToSphere * distanceToSphere;
      float radiusSq = sphere.mRadius * sphere.mRadius;
      float projectedRadius = Math::Sqrt( distSq - radiusSq ) * sphere.mRadius / distanceToSphere;
      float distanceFromCenter = Math::Sqrt( radiusSq - projectedRadius * projectedRadius );

      Math::Vector3 newCenter = sphere.mCenter - eyeToCenter * distanceFromCenter;

      Math::Vector3 basis0, basis1;
      Math::GenerateOrthonormalBasis( eyeToCenter, &basis0, &basis1 );
      CreateDisc( shape, newCenter, basis0, basis1, projectedRadius );
    }
  }
  return shape;
}

DebugShape &DebugDrawer::DrawAabb( const Shapes::Aabb &aabb )
{
  // Draw all edges of an aabb. Make sure to not mis-match edges!
  DebugShape &shape = GetNewShape();

  Math::Vector3 min = aabb.GetMin();
  Math::Vector3 max = aabb.GetMax();

  Math::Vector3 f_bottom_left = min;
  Math::Vector3 f_bottom_right = Math::Vector3( max.getX(), min.getY(), min.getZ() );
  Math::Vector3 f_top_right = Math::Vector3( max.getX(), max.getY(), min.getZ() );
  Math::Vector3 f_top_left = Math::Vector3( min.getX(), max.getY(), min.getZ() );

  Math::Vector3 b_bottom_left = Math::Vector3( min.getX(), min.getY(), max.getZ() );
  Math::Vector3 b_bottom_right = Math::Vector3( max.getX(), min.getY(), max.getZ() );
  Math::Vector3 b_top_right = Math::Vector3( max.getX(), max.getY(), max.getZ() );
  Math::Vector3 b_top_left = Math::Vector3( min.getX(), max.getY(), max.getZ() );

  //front box
  shape.mSegments.push_back( Shapes::LineSegment( f_bottom_left, f_bottom_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_bottom_right, f_top_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_top_right, f_top_left ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_top_left, f_bottom_left ) );

  //connector lines
  shape.mSegments.push_back( Shapes::LineSegment( f_bottom_left, b_bottom_left ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_bottom_right, b_bottom_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_top_right, b_top_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( f_top_left, b_top_left ) );

  //back box
  shape.mSegments.push_back( Shapes::LineSegment( b_bottom_left, b_bottom_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( b_bottom_right, b_top_right ) );
  shape.mSegments.push_back( Shapes::LineSegment( b_top_right, b_top_left ) );
  shape.mSegments.push_back( Shapes::LineSegment( b_top_left, b_bottom_left ) );
  return shape;
}

DebugShape &DebugDrawer::DrawTriangle( const Shapes::Triangle &triangle )
{
  // Draw the 3 edges of a triangles
  DebugShape &shape = GetNewShape();
  shape.mSegments.push_back( Shapes::LineSegment( triangle.mPoints[0], triangle.mPoints[1] ) );
  shape.mSegments.push_back( Shapes::LineSegment( triangle.mPoints[0], triangle.mPoints[2] ) );
  shape.mSegments.push_back( Shapes::LineSegment( triangle.mPoints[1], triangle.mPoints[2] ) );
  return shape;
}

DebugShape &DebugDrawer::DrawPlane( const Shapes::Plane &plane, float sizeX, float sizeY )
{
  // Draw a quad with a normal at the plane's center.
  //DebugShape& shape = GetNewShape();

  Math::Vector3 normal = plane.GetNormal();
  Math::Vector3 center = normal * plane.GetDistance();
  Math::Vector3 basis0, basis1;
  Math::GenerateOrthonormalBasis( normal, &basis0, &basis1 );
  Math::Vector3 p0 = center + basis0 * sizeX + basis1 * sizeY;
  Math::Vector3 p1 = center - basis0 * sizeX + basis1 * sizeY;
  Math::Vector3 p2 = center - basis0 * sizeX - basis1 * sizeY;
  Math::Vector3 p3 = center + basis0 * sizeX - basis1 * sizeY;

  DebugShape &shape = DrawRay( Shapes::Ray( center, normal ), 100 );
  shape.mSegments.push_back( Shapes::LineSegment( p0, p1 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p1, p2 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p2, p3 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p3, p0 ) );
  return shape;
}

DebugShape &DebugDrawer::DrawQuad( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2, const Math::Vector3 &p3 )
{
  // Draw the4 edges of a quad. Make sure to look at this and make sure the quad is not bow-tied.
  DebugShape &shape = GetNewShape();
  shape.mSegments.push_back( Shapes::LineSegment( p0, p1 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p1, p2 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p2, p3 ) );
  shape.mSegments.push_back( Shapes::LineSegment( p3, p0 ) );
  return shape;
}

DebugShape &DebugDrawer::DrawFrustum( const Shapes::Frustum &frustum )
{
  // Draw the 6 faces of the frustum using the 8 frustum points.
  // See Frustum.Set for the point order. For example, Points[4] is left-bottom-front.
  DebugShape &shape = GetNewShape();

  //front
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[0], frustum.mPoints[1] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[1], frustum.mPoints[2] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[2], frustum.mPoints[3] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[0], frustum.mPoints[3] ) );
  //back
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[4], frustum.mPoints[5] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[5], frustum.mPoints[6] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[6], frustum.mPoints[7] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[4], frustum.mPoints[7] ) );
  //connect front to back
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[0], frustum.mPoints[4] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[1], frustum.mPoints[5] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[2], frustum.mPoints[6] ) );
  shape.mSegments.push_back( Shapes::LineSegment( frustum.mPoints[3], frustum.mPoints[7] ) );

  return shape;
}
}