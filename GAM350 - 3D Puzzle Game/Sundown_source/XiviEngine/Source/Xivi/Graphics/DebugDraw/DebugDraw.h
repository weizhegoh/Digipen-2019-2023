/*****************************************************************
*\file         DebugDraw.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <pch.h>
#include "DebugShape.h"
namespace Xivi::Graphics
{
class DebugDrawer
{
  typedef std::vector<DebugShape> DebugShapeList;
  DebugShapeList mShapes;
  // Any active masks used to filter out what's currently drawing.
  unsigned int mActiveMask;
  Math::Vector3 mCameraEye;
  Math::Vector3 mCameraDir;
public:
  DebugDrawer();

  void Update( float dt, Math::Vector3 cameraEye, Math::Vector3 cameraDir );
  GLuint VBO, VAO;
  void InitLineBuffer()
  {
    GLfloat points[12];
    glDeleteBuffers( 1, &VBO );
    glDeleteVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (GLvoid *) ( 3 * sizeof( GLfloat ) ) );
    glBindVertexArray( 0 );
  }

  DebugShape &GetNewShape();
  void CreateDisc( DebugShape &shape,
                   const Math::Vector3 &center,
                   const Math::Vector3 &axis0,
                   const Math::Vector3 &axis1,
                   float radius );
                 // You must implement all of the below functions
  DebugShape &DrawPoint( const Math::Vector3 &point );
  DebugShape &DrawLine( const Shapes::LineSegment &line );
  DebugShape &DrawRay( const Shapes::Ray &ray, float t );
  DebugShape &DrawSphere( const Shapes::Sphere &sphere );
  DebugShape &DrawAabb( const Shapes::Aabb &aabb );
  DebugShape &DrawTriangle( const Shapes::Triangle &triangle );
  DebugShape &DrawPlane( const Shapes::Plane &plane, float sizeX, float sizeY );
  DebugShape &DrawQuad( const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &p2, const Math::Vector3 &p3 );
  DebugShape &DrawFrustum( const Shapes::Frustum &frustum );

  DebugShapeList &ShapeList()
  {
    return mShapes;
  }
  unsigned int isActive()
  {
    return mActiveMask;
  }
};
}