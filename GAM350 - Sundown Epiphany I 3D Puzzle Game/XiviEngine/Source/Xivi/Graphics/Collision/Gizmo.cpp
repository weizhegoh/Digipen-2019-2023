/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#include "Precompiled.hpp"
//#include "Gizmo.hpp"
//#include "DebugDraw.hpp"
//#include "Components.hpp"
//#include "Geometry.hpp"
//#include "Application.hpp"
//#include "pch.h"
//
//void Gizmo::GetBasis(Vector3& pos, Vector3& basisX, Vector3& basisY, Vector3& basisZ)
//{
//  if(mSelectedObject == NULL)
//    return;
//
//  Transform* transform = mSelectedObject->has(Transform);
//  if(transform != NULL)
//  {
//    pos = transform->mTranslation;
//    basisX = Vector3::cXAxis;
//    basisY = Vector3::cYAxis;
//    basisZ = Vector3::cZAxis;
//  }
//}
//
//TranslationGizmo::TranslationGizmo()
//{
//  mCaptured = false;
//}
//
//void TranslationGizmo::OnUpdate(Application* application, float dt)
//{
//  if(mSelectedObject == NULL)
//    return;
//
//  Vector3 pos, basisX, basisY, basisZ;
//  GetBasis(pos, basisX, basisY, basisZ);
//
//  float xVal = mHightlightedAxis == 0 ? 1 : 0.5f;
//  float yVal = mHightlightedAxis == 1 ? 1 : 0.5f;
//  float zVal = mHightlightedAxis == 2 ? 1 : 0.5f;
//
//  gDebugDrawer->DrawRay(Ray(pos, basisX), 5).Color(Vector4(xVal, 0, 0, 1));
//  gDebugDrawer->DrawRay(Ray(pos, basisY), 5).Color(Vector4(0, yVal, 0, 1));
//  gDebugDrawer->DrawRay(Ray(pos, basisZ), 5).Color(Vector4(0, 0, zVal, 1));
//}
//
//bool TranslationGizmo::OnMouseInput(Application* application, unsigned int button, bool isDown, const Ray& ray)
//{
//  if(mSelectedObject == NULL)
//    return false;
//
//  if(button != SDL_BUTTON_LEFT)
//    return false;
//
//  if(isDown == false)
//  {
//    if(mCaptured)
//    {
//      mCaptured = false;
//      return true;
//    }
//
//    return false;
//  }
//
//  float cylinderLength = 5;
//  float cylinderRadius = 0.5;
//
//  Vector3 pos, basisX, basisY, basisZ;
//  GetBasis(pos, basisX, basisY, basisZ);
//  Vector3 bases[3] = {basisX, basisY, basisZ};
//
//  float tX, tY, tZ;
//  bool resultX = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisX, cylinderRadius, tX);
//  bool resultY = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisY, cylinderRadius, tY);
//  bool resultZ = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisZ, cylinderRadius, tZ);
//
//  if(resultX == false && resultY == false && resultZ == false)
//    return false;
//
//  float minT = Math::PositiveMax();
//  if(resultX)
//  {
//    minT = Math::Min(tX, minT);
//    mHightlightedAxis = 0;
//  }
//  if(resultY)
//  {
//    minT = Math::Min(tY, minT);
//    mHightlightedAxis = 1;
//  }
//  if(resultZ)
//  {
//    minT = Math::Min(tZ, minT);
//    mHightlightedAxis = 2;
//  }
//
//
//  mPlaneNormal = bases[(mHightlightedAxis + 1) % 3];
//  Vector3 otherAxis = bases[(mHightlightedAxis + 2) % 3];
//  Vector3 eyeDirection = application->mCamera.GetDirection();
//
//  float axisA = Math::Dot(eyeDirection, mPlaneNormal);
//  float axisB = Math::Dot(eyeDirection, otherAxis);
//  if(Math::Abs(axisA) < Math::Abs(axisB))
//    Math::Swap(mPlaneNormal, otherAxis);
//
//  Plane plane(mPlaneNormal, pos);
//  RayPlane(ray.mStart, ray.mDirection, plane.mData, minT);
//
//  mLocalPoint = ray.GetPoint(minT);
//  mCaptured = true;
//  mLocalOffset = Math::Dot(mLocalPoint - pos, bases[mHightlightedAxis]);
//
//  return true;
//}
//
//bool TranslationGizmo::OnMouseMove(Application* application, const Ray& ray)
//{
//  if(mSelectedObject == NULL)
//    return false;
//
//  float cylinderLength = 5;
//  float cylinderRadius = 0.5;
//
//  Vector3 pos, basisX, basisY, basisZ;
//  GetBasis(pos, basisX, basisY, basisZ);
//  Vector3 bases[3] = {basisX, basisY, basisZ};
//  if(mCaptured)
//  {
//    float t = 0;
//    //Vector3 normal = bases[(mHightlightedAxis + 1) % 3];
//    //normal = application->mCamera.GetDirection().Normalized();
//    Vector3 dir = bases[mHightlightedAxis];
//
//    Plane plane(mPlaneNormal, pos);
//    RayPlane(ray.mStart, ray.mDirection, plane.mData, t);
//    Vector3 pt = ray.GetPoint(t);
//
//    float offset = Math::Dot(pt - pos, dir);
//
//    Transform* transform = mSelectedObject->has(Transform);
//    if(transform != nullptr)
//    {
//      transform->mTranslation += dir * (offset - mLocalOffset);
//      mSelectedObject->TransformUpdate(TransformUpdateFlags::Translation);
//    }
//    return true;
//  }
//
//  mHightlightedAxis = -1;
//  float tX, tY, tZ;
//  bool resultX = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisX, cylinderRadius, tX);
//  bool resultY = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisY, cylinderRadius, tY);
//  bool resultZ = Helpers::RayCylinder(ray.mStart, ray.mDirection, pos, pos + cylinderLength * basisZ, cylinderRadius, tZ);
//
//  if(resultX == false && resultY == false && resultZ == false)
//    return false;
//
//  float minT = Math::PositiveMax();
//  if(resultX)
//  {
//    minT = Math::Min(tX, minT);
//    if(!mCaptured)
//      mHightlightedAxis = 0;
//  }
//  if(resultY)
//  {
//    minT = Math::Min(tY, minT);
//    if(!mCaptured)
//      mHightlightedAxis = 1;
//  }
//  if(resultZ)
//  {
//    minT = Math::Min(tZ, minT);
//    if(!mCaptured)
//      mHightlightedAxis = 2;
//  }
//
//  return mCaptured;
//}
//
//bool TranslationGizmo::OnMouseScroll(Application* application, int x, int y)
//{
//  if(mSelectedObject == NULL)
//    return false;
//
//  return false;
//}