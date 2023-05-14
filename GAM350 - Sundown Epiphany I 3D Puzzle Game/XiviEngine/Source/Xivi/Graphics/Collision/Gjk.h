///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <Gjk.hpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_5>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 21 April 2021
//End Header -------------------------------------------------------*/
//#pragma once
//
//#include "../math/MathFunctions.hpp"
//#include "Shapes.h"
//#include "DebugDraw.h"
//
//class Model;
//
////-----------------------------------------------------------------------------SupportShape
//class SupportShape
//{
//public:
//
//  virtual Vector3 GetCenter() const = 0;
//  virtual Vector3 Support(const Vector3& worldDirection) const = 0;
//  virtual void DebugDraw(const Vector4& color = Vector4::cZero) const = 0;
//
//  Vector3 GetCenter(const std::vector<Vector3>& localPoints, const Matrix4& localToWorldTransform) const;
//  Vector3 Support(const Vector3& worldDirection, const std::vector<Vector3>& localPoints, const Matrix4& localToWorldTransform) const;
//  void DebugDraw(const std::vector<Vector3>& localPoints, const Matrix4& transform, const Vector4& color = Vector4::cZero) const;
//};
//
////-----------------------------------------------------------------------------ModelSupportShape
//class ModelSupportShape : public SupportShape
//{
//public:
//
//  Vector3 GetCenter() const override;
//  Vector3 Support(const Vector3& worldDirection) const override;
//  void DebugDraw(const Vector4& color = Vector4::cZero) const override;
//
//  Model* mModel;
//};
//
////-----------------------------------------------------------------------------PointsSupportShape
//class PointsSupportShape : public SupportShape
//{
//public:
//  PointsSupportShape();
//  Vector3 GetCenter() const override;
//  Vector3 Support(const Vector3& worldDirection) const override;
//  void DebugDraw(const Vector4& color = Vector4::cZero) const override;
//
//  std::vector<Vector3> mLocalSpacePoints;
//  Vector3 mScale;
//  Matrix3 mRotation;
//  Vector3 mTranslation;
//};
//
////-----------------------------------------------------------------------------SphereSupportShape
//class SphereSupportShape : public SupportShape
//{
//public:
//  Vector3 GetCenter() const override;
//  Vector3 Support(const Vector3& worldDirection) const override;
//  void DebugDraw(const Vector4& color = Vector4::cZero) const override;
//
//  Sphere mSphere;
//};
//
////-----------------------------------------------------------------------------ObbSupportShape
//class ObbSupportShape : public SupportShape
//{
//public:
//
//  Vector3 GetCenter() const override;
//  Vector3 Support(const Vector3& worldDirection) const override;
//  void DebugDraw(const Vector4& color = Vector4::cZero) const override;
//
//  Vector3 mScale;
//  Matrix3 mRotation;
//  Vector3 mTranslation;
//};
//
//namespace VoronoiRegion
//{
//  enum Type { Point0, Point1, Point2, Point3,
//              Edge01, Edge02, Edge03, Edge12, Edge13, Edge23,
//              Triangle012, Triangle013, Triangle023, Triangle123,
//              Tetrahedra0123,
//              Unknown};
//  static const char* Names[] = {"Point0", "Point1", "Point2", "Point3",
//                                "Edge01", "Edge02", "Edge03", "Edge12", "Edge13", "Edge23",
//                                "Triangle012", "Triangle013", "Triangle023", "Triangle123",
//                                "Tetrahedra0123",
//                                "Unknown"};
//}
//
///******Student:Assignment5******/
//// Implement gjk
////-----------------------------------------------------------------------------Gjk
//class Gjk
//{
//public:
//
//  // Point Test
//  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
//                                                   size_t& newSize, int newIndices[4],
//                                                   Vector3& closestPoint, Vector3& searchDirection);
//
//  // Edge Test
//  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
//                                                   size_t& newSize, int newIndices[4],
//                                                   Vector3& closestPoint, Vector3& searchDirection);
//
//  // Triangle Test
//  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
//                                                   size_t& newSize, int newIndices[4],
//                                                   Vector3& closestPoint, Vector3& searchDirection);
//
//  // Tetrahedron Tests
//  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//                                                   size_t& newSize, int newIndices[4],
//                                                   Vector3& closestPoint, Vector3& searchDirection);
//
//  // Simple structure that contains all information for a point in Gjk.
//  struct CsoPoint
//  {
//    Vector3 mPointA;
//    Vector3 mPointB;
//    Vector3 mCsoPoint;
//  };
//
//  Gjk();
//
//  // Returns true if the shapes intersect. If the shapes don't intersect then closestPoint is filled out with the closest points
//  // on each object as well as the cso point. Epsilon should be used for checking if sufficient progress has been made at any step.
//  // The debugging values are for your own use (make sure they don't interfere with the unit tests).
//  bool Intersect(const SupportShape* shapeA, const SupportShape* shapeB, unsigned int maxIterations, CsoPoint& closestPoint, float epsilon, int debuggingIndex, bool debugDraw);
//  // Finds the point furthest in the given direction on the CSO (and the relevant points from each object)
//  CsoPoint ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB, const Vector3& direction);
//
//  // Add your implementation here
//
//  static void getPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, size_t& newSize, int newIndices[4],
//      Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type);
//
//  static void getEdge(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//      size_t& newSize, int newIndices[4],
//      Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type, float u, float v);
//
//  static void getTriangle(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//      size_t& newSize, int newIndices[4],
//      Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type, float u, float v, float w);
//
//  static void getTetrahedron(const Vector3& q, size_t& newSize, int newIndices[4], Vector3& closestPoint, Vector3& searchDirection);
//
//  static void shrinkPoints(CsoPoint points[4], size_t size, int newIndices[4]);
//
//  static void filLClosestPoints(CsoPoint& closestPoint, float epsilon, size_t size, CsoPoint points[4], Vector3 tmpClosest);
//};
