///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <Shapes.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_2>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 23 February 2021
//End Header -------------------------------------------------------*/
//
#include "pch.h"
//#include "Shapes.h"
//#include "Geometry.h"
////#include "DebugDraw.h"
//
//namespace Xivi
//{
//
//	////-----------------------------------------------------------------------------LineSegment
//	//LineSegment::LineSegment()
//	//{
//	//	mStart = mEnd = { 0.0f };
//	//}
//
//	//LineSegment::LineSegment(Math::Vec3Param start, Math::Vec3Param end)
//	//{
//	//	mStart = start;
//	//	mEnd = end;
//	//}
//
//	////DebugShape& LineSegment::DebugDraw() const
//	////{
//	////	return gDebugDrawer->DrawLine(*this);
//	////}
//
//	////-----------------------------------------------------------------------------Ray
//	//Ray::Ray()
//	//{
//	//	mStart = mDirection = { 0.0f };
//	//}
//
//	//Ray::Ray(Math::Vec3Param start, Math::Vec3Param dir)
//	//{
//	//	mStart = start;
//	//	mDirection = dir;
//	//}
//
//	//Ray Ray::Transform(const Math::Matrix4& transform) const
//	//{
//	//	Ray transformedRay;
//	//	transformedRay.mStart = Math::SIMD::transformPoint(transform, mStart);
//	//	transformedRay.mDirection = Math::SIMD::transformNormal(transform, mDirection);
//	//	return transformedRay;
//	//}
//
//	//Math::Vector3 Ray::GetPoint(float t) const
//	//{
//	//	return mStart + mDirection * t;
//	//}
//
//	////DebugShape& Ray::DebugDraw(float t) const
//	////{
//	////	return gDebugDrawer->DrawRay(*this, t);
//	////}
//
//
//
//
//	////-----------------------------------------------------------------------------Sphere
//	//Sphere::Sphere()
//	//{
//	//	mCenter.setToZero();
//	//	mRadius = 0;
//	//}
//
//	//Sphere::Sphere(const Math::Vector3& center, float radius)
//	//{
//	//	mCenter = center;
//	//	mRadius = radius;
//	//}
//
//	//void Sphere::ComputeCentroid(const std::vector<Math::Vector3>& points)
//	//{
//	//	/******Student:Assignment2******/
//	//	// The centroid method is roughly describe as: find the centroid (not mean) of all
//	//	// points and then find the furthest away point from the centroid.
//
//	//	Math::Vector3 minVec{ FLT_MAX, FLT_MAX, FLT_MAX };
//	//	Math::Vector3 maxVec{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
//
//	//	//2 pass method
//
//	//	//first pass
//	//	for (const auto& inputVec : points)
//	//	{
//	//		for (unsigned int i = 0; i < 3; ++i)
//	//		{
//	//			minVec[i] = Math::Min(minVec[i], inputVec[i]);
//	//			maxVec[i] = Math::Max(maxVec[i], inputVec[i]);
//	//		}
//	//	}
//
//	//	//finding center of AABB
//	//	Math::Vector3 center = (maxVec + minVec) / 2.0f;
//	//	float maxDistance = -FLT_MAX;
//
//	//	//second pass
//	//	for (const auto& inputVec : points)
//	//	{
//	//		Math::Vector3 distance = inputVec - center;
//	//		maxDistance = std::max(maxDistance, length(distance));
//	//	}
//
//	//	mCenter = center;
//	//	//using max distance as radius
//	//	mRadius = maxDistance;
//	//}
//
//	//void Sphere::ComputeRitter(const std::vector<Math::Vector3>& points)
//	//{
//	//	/******Student:Assignment2******/
//	//	// The ritter method:
//	//	// Find the largest spread on each axis.
//	//	// Find which axis' pair of points are the furthest (euclidean distance) apart.
//	//	// Choose the center of this line as the sphere center. Now incrementally expand the sphere.
//
//	//	Math::Vector3 minVec[3];
//	//	Math::Vector3 maxVec[3];
//
//	//	for (int i = 0; i < 3; ++i)
//	//	{
//	//		minVec[i] = Math::Vector3{ FLT_MAX, FLT_MAX, FLT_MAX };
//	//		maxVec[i] = Math::Vector3{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
//	//	}
//
//	//	//finding max and min points
//	//	for (const auto& inputVec : points)
//	//	{
//	//		for (int i = 0; i < 3; ++i)
//	//		{
//	//			if (inputVec[i] > maxVec[i][i])
//	//				maxVec[i] = inputVec;
//	//			if (inputVec[i] < minVec[i][i])
//	//				minVec[i] = inputVec;
//	//		}
//	//	}
//
//	//	//finding largest axis spread (furthest apart)
//	//	int axis = 0;
//	//	float maxDistance = -FLT_MAX;
//
//	//	for (int i = 0; i < 3; ++i)
//	//	{
//	//		float sqdistance = lengthSq(maxVec[i] - minVec[i]);
//	//		if (maxDistance < sqdistance)
//	//		{
//	//			maxDistance = sqdistance;
//	//			axis = i;
//	//		}
//	//	}
//
//	//	//move center to the midpoint and start sphere expansion
//	//	mCenter = (maxVec[axis] + minVec[axis]) / 2;
//	//	mRadius = length(maxVec[axis] - minVec[axis]) / 2;
//
//	//	for (const auto& p : points)
//	//	{
//	//		Math::Vector3 distanceVec = p - mCenter;
//	//		if (mRadius < length(distanceVec))
//	//		{
//	//			Math::Vector3 b = mCenter - mRadius * normalize(distanceVec);
//	//			mCenter = (p + b) / 2;
//	//			mRadius = length(p - mCenter);
//	//		}
//	//	}
//	//}
//	//
//	//bool Sphere::ContainsPoint(const Math::Vector3& point)
//	//{
//	//	return PointSphere(point, mCenter, mRadius);
//	//}
//
//	//Math::Vector3 Sphere::GetCenter() const
//	//{
//	//	return mCenter;
//	//}
//
//	//float Sphere::GetRadius() const
//	//{
//	//	return mRadius;
//	//}
//
//	//bool Sphere::Compare(const Sphere& rhs, float epsilon) const
//	//{
//	//	float posDiff = length(mCenter - rhs.mCenter);
//	//	float radiusDiff = Math::Abs(mRadius - rhs.mRadius);
//
//	//	return posDiff < epsilon&& radiusDiff < epsilon;
//	//}
//
//	////DebugShape& Sphere::DebugDraw() const
//	////{
//	////	return gDebugDrawer->DrawSphere(*this);
//	////}
//
//	////-----------------------------------------------------------------------------Aabb
//	//Aabb::Aabb()
//	//{
//	//	//set the aabb to an initial bad value (where the min is smaller than the max)
//	//	setAll(mMin, Math::PositiveMax());
//	//	setAll(mMax, -Math::PositiveMax());
//	//}
//
//	//Aabb::Aabb(const Math::Vector3& min, const Math::Vector3& max)
//	//{
//	//	mMin = min;
//	//	mMax = max;
//	//}
//
//	//Aabb Aabb::BuildFromCenterAndHalfExtents(const Math::Vector3& center, const Math::Vector3& halfExtents)
//	//{
//	//	return Aabb(center - halfExtents, center + halfExtents);
//	//}
//
//	//float Aabb::GetVolume() const
//	//{
//	//	/******Student:Assignment2******/
//	//	// Return the aabb's volume
//
//	//	float volume = 1.0f;
//	//	for (int i = 0; i < 3; ++i)
//	//	{
//	//		volume *= (mMax[i] - mMin[i]);
//	//	}
//	//	return volume;
//	//}
//
//	//float Aabb::GetSurfaceArea() const
//	//{
//	//	/******Student:Assignment2******/
//	//	// Return the aabb's surface area
//
//	//	float surfaceArea = 0.0f;
//	//	for (int i = 0; i < 3; ++i)
//	//	{
//	//		if ((i + 1) % 3)
//	//			surfaceArea += (mMax[i + 1] - mMin[i + 1]) * (mMax[i] - mMin[i]);
//	//		else
//	//			surfaceArea += (mMax[0] - mMin[0]) * (mMax[i] - mMin[i]);
//	//	}
//	//	return surfaceArea * 2;
//	//}
//
//	//bool Aabb::Contains(const Aabb& aabb) const
//	//{
//	//	/******Student:Assignment2******/
//	//	// Return if aabb is completely contained in this
//	//	return getX(aabb.mMax) <= getX(mMax) && getY(aabb.mMax) <= getY(mMax) && getZ(aabb.mMax) <= getZ(mMax)
//	//		&& getX(aabb.mMin) >= getX(mMin) && getY(aabb.mMin) >= getY(mMin) && getZ(aabb.mMin) >= getZ(mMin);
//	//}
//
//	//void Aabb::Expand(const Math::Vector3& point)
//	//{
//	//	for (unsigned int i = 0; i < 3; ++i)
//	//	{
//	//		mMin[i] = Math::Min(mMin[i], point[i]);
//	//		mMax[i] = Math::Max(mMax[i], point[i]);
//	//	}
//	//}
//
//	//Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
//	//{
//	//	Aabb result;
//	//	for (unsigned int i = 0; i < 3; ++i)
//	//	{
//	//		result.mMin[i] = Math::Min(lhs.mMin[i], rhs.mMin[i]);
//	//		result.mMax[i] = Math::Max(lhs.mMax[i], rhs.mMax[i]);
//	//	}
//	//	return result;
//	//}
//
//	//bool Aabb::Compare(const Aabb& rhs, float epsilon) const
//	//{
//	//	float pos1Diff = length(mMin - rhs.mMin);
//	//	float pos2Diff = length(mMax - rhs.mMax);
//
//	//	return pos1Diff < epsilon&& pos2Diff < epsilon;
//	//}
//
//	//void Aabb::Transform(const Math::Vector3& scale, const Math::Matrix3& rotation, const Math::Vector3& translation)
//	//{
//	//	/******Student:Assignment2******/
//	//	// Compute aabb of the this aabb after it is transformed.
//	//	// You should use the optimize method discussed in class (not transforming all 8 points).
//
//	//	Math::Matrix3 rotate = rotation;
//
//	//	// absolute rotation matrix
//	//	for (int i = 0; i < 3; ++i)
//	//	{
//	//		for (int j = 0; j < 3; ++j)
//	//		{
//	//			rotate[i][j] = Math::Abs(rotate[i][j]);
//	//		}
//	//	}
//
//	//	//finding radius and center
//	//	Math::Vector3 radius = (mMax - mMin) * 0.5f;
//	//	Math::Vector3 center = GetCenter();
//
//	//	//scaling radius and center
//	//	center *= scale;
//	//	radius *= scale;
//
//	//	//transforming center with rotation matrix and translation
//	//	center = rotation * center + translation;
//
//	//	//transforming radius with absolute rotation matrix
//	//	radius = rotate * radius;
//
//	//	mMax = center + radius;
//	//	mMin = center - radius;
//	//}
//
//	//Math::Vector3 Aabb::GetMin() const
//	//{
//	//	return mMin;
//	//}
//
//	//Math::Vector3 Aabb::GetMax() const
//	//{
//	//	return mMax;
//	//}
//
//	//Math::Vector3 Aabb::GetCenter() const
//	//{
//	//	return (mMin + mMax) * 0.5f;
//	//}
//
//	//Math::Vector3 Aabb::GetHalfSize() const
//	//{
//	//	return (mMax - mMin) * 0.5f;
//	//}
//
//	////DebugShape& Aabb::DebugDraw() const
//	////{
//	////	return gDebugDrawer->DrawAabb(*this);
//	////}
//
//	////-----------------------------------------------------------------------------Triangle
//	//Triangle::Triangle()
//	//{
//	//	mPoints[0] = mPoints[1] = mPoints[2] = { 0.0f };
//	//}
//
//	//Triangle::Triangle(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2)
//	//{
//	//	mPoints[0] = p0;
//	//	mPoints[1] = p1;
//	//	mPoints[2] = p2;
//	//}
//
//	////DebugShape& Triangle::DebugDraw() const
//	////{
//	////	return gDebugDrawer->DrawTriangle(*this);
//	////}
//
//	////-----------------------------------------------------------------------------Plane
//	//Plane::Plane()
//	//{
//	//	mData = { 0.0f };
//	//}
//
//	//Plane::Plane(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2)
//	//{
//	//	Set(p0, p1, p2);
//	//}
//
//	//Plane::Plane(const Math::Vector3& normal, const Math::Vector3& point)
//	//{
//	//	Set(normal, point);
//	//}
//
//	//void Plane::Set(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2)
//	//{
//	//	Set((p1 - p0).cross(p2 - p0), p0);
//	//}
//
//	//void Plane::Set(const Math::Vector3& normal, const Math::Vector3& point)
//	//{
//	//	if (length(normal) == 0)
//	//		return;
//	//	Math::Vector3 normalized = normalize(normal);
//	//	setX(mData, getX(normalized));
//	//	setY(mData, getY(normalized));
//	//	setZ(mData, getZ(normalized));
//	//	setW(mData, dot(normalized, point));
//	//}
//
//	//Math::Vector3 Plane::GetNormal() const
//	//{
//	//	return Math::Vector3(getX(mData), getY(mData), getZ(mData));
//	//}
//
//	//float Plane::GetDistance() const
//	//{
//	//	return getW(mData);
//	//}
//
//	////DebugShape& Plane::DebugDraw(float size) const
//	////{
//	////	return DebugDraw(size, size);
//	////}
//
//	////DebugShape& Plane::DebugDraw(float sizeX, float sizeY) const
//	////{
//	////	return gDebugDrawer->DrawPlane(*this, sizeX, sizeY);
//	////}
//
//	////-----------------------------------------------------------------------------Frustum
//	//void Frustum::Set(const Math::Vector3& lbn, const Math::Vector3& rbn, const Math::Vector3& rtn, const Math::Vector3& ltn,
//	//	const Math::Vector3& lbf, const Math::Vector3& rbf, const Math::Vector3& rtf, const Math::Vector3& ltf)
//	//{
//	//	mPoints[0] = lbn;
//	//	mPoints[1] = rbn;
//	//	mPoints[2] = rtn;
//	//	mPoints[3] = ltn;
//	//	mPoints[4] = lbf;
//	//	mPoints[5] = rbf;
//	//	mPoints[6] = rtf;
//	//	mPoints[7] = ltf;
//
//	//	//left
//	//	mPlanes[0].Set(lbf, ltf, lbn);
//	//	//right
//	//	mPlanes[1].Set(rbn, rtf, rbf);
//	//	//top
//	//	mPlanes[2].Set(ltn, ltf, rtn);
//	//	//bot
//	//	mPlanes[3].Set(rbn, lbf, lbn);
//	//	//near
//	//	mPlanes[4].Set(lbn, ltn, rbn);
//	//	//far
//	//	mPlanes[5].Set(rbf, rtf, lbf);
//	//}
//
//	//Math::Vector4* Frustum::GetPlanes() const
//	//{
//	//	return (Math::Vector4*)mPlanes;
//	//}
//
//	//DebugShape& Frustum::DebugDraw() const
//	//{
//	//	return gDebugDrawer->DrawFrustum(*this);
//	//}
//}