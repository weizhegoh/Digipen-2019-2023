#include "pch.h"
///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <Gjk.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_5>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 21 April 2021
//End Header -------------------------------------------------------*/
//
//#include "GraphicPCH.h"
//
////-----------------------------------------------------------------------------SupportShape
//Vector3 SupportShape::GetCenter(const std::vector<Vector3>& localPoints, const Matrix4& transform) const
//{
//	Vector3 center = Vector3::cZero;
//	/******Student:Assignment5******/
//	if (!localPoints.empty()) {
//		// add all vectors
//		for (const Vector3& point : localPoints)
//			center += point;
//		center = TransformPoint(transform, center / static_cast<float>(localPoints.size()));
//	}
//	return center;
//}
//
//Vector3 SupportShape::Support(const Vector3& worldDirection, const std::vector<Vector3>& localPoints, const Matrix4& localToWorldTransform) const
//{
//	Vector3 result = Vector3::cZero;
//	/******Student:Assignment5******/
//	if (!localPoints.empty()) {
//		Vector3 local_dir = Math::TransformNormal(localToWorldTransform.Inverted(), worldDirection);
//		float proj = -Math::PositiveMax();
//		Vector3 pt;
//		for (auto& point : localPoints)
//		{
//			float temp = Math::Dot(local_dir, point);
//			if (temp > proj)
//			{
//				pt = point;
//				proj = temp;
//			}
//		}
//		result = TransformPoint(localToWorldTransform, pt);
//	}
//	return result;
//}
//
//void SupportShape::DebugDraw(const std::vector<Vector3>& localPoints, const Matrix4& localToWorldTransform, const Vector4& color) const
//{
//	/******Student:Assignment5******/
//	for (auto& pt : localPoints)
//	{
//		DebugShape& shape = gDebugDrawer->DrawPoint(TransformPoint(localToWorldTransform, pt));
//		shape.Color(color);
//	}
//}
//
////-----------------------------------------------------------------------------ModelSupportShape
//Vector3 ModelSupportShape::GetCenter() const
//{
//	return SupportShape::GetCenter(mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
//}
//
//Vector3 ModelSupportShape::Support(const Vector3& worldDirection) const
//{
//	return SupportShape::Support(worldDirection, mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
//}
//
//void ModelSupportShape::DebugDraw(const Vector4& color) const
//{
//	SupportShape::DebugDraw(mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
//}
//
////-----------------------------------------------------------------------------PointsSupportShape
//PointsSupportShape::PointsSupportShape()
//{
//	mScale = Vector3(1);
//	mRotation = Matrix3::cIdentity;
//	mTranslation = Vector3::cZero;
//}
//
//Vector3 PointsSupportShape::GetCenter() const
//{
//	Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//	return SupportShape::GetCenter(mLocalSpacePoints, transform);
//}
//
//Vector3 PointsSupportShape::Support(const Vector3& worldDirection) const
//{
//	Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//	return SupportShape::Support(worldDirection, mLocalSpacePoints, transform);
//}
//
//void PointsSupportShape::DebugDraw(const Vector4& color) const
//{
//	Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//	SupportShape::DebugDraw(mLocalSpacePoints, transform, color);
//}
//
////-----------------------------------------------------------------------------SphereSupportShape
//Vector3 SphereSupportShape::GetCenter() const
//{
//	return mSphere.mCenter;
//}
//
//Vector3 SphereSupportShape::Support(const Vector3& worldDirection) const
//{
//	/******Student:Assignment5******/
//	if (worldDirection != Vector3::cZero)
//	{
//		return mSphere.mCenter + worldDirection.Normalized() * mSphere.mRadius;
//	}
//	return mSphere.mCenter;
//}
//
//void SphereSupportShape::DebugDraw(const Vector4& color) const
//{
//	DebugShape& shape = gDebugDrawer->DrawSphere(mSphere);
//	shape.Color(color);
//}
//
////-----------------------------------------------------------------------------ObbSupportShape
//Vector3 ObbSupportShape::GetCenter() const
//{
//	return mTranslation;
//}
//
//Vector3 ObbSupportShape::Support(const Vector3& worldDirection) const
//{
//	/******Student:Assignment5******/
//	Vector3 result = GetCenter();
//	Vector3 localDir = Math::Transform(mRotation.Inverted(), worldDirection);
//	Vector3 halfext = mScale * 0.5f;
//	for (unsigned i = 0; i < 3; ++i)
//		result += Math::GetSign(localDir[i]) * halfext[i] * mRotation.Basis(i);
//	return result;
//}
//
//void ObbSupportShape::DebugDraw(const Vector4& color) const
//{
//	Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//	DebugShape& shape = gDebugDrawer->DrawAabb(Aabb(Vector3(-0.5f), Vector3(0.5f)));
//	shape.Color(color);
//	shape.SetTransform(transform);
//}
//
//
////------------------------------------------------------------ Voronoi Region Tests
//VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
//	size_t& newSize, int newIndices[4],
//	Vector3& closestPoint, Vector3& searchDirection)
//{
//	/******Student:Assignment5******/
//	closestPoint = p0;
//	newSize = 1;
//	searchDirection = q - p0;
//	return VoronoiRegion::Point0;
//}
//
//VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
//	size_t& newSize, int newIndices[4],
//	Vector3& closestPoint, Vector3& searchDirection)
//{
//	/******Student:Assignment5******/
//	float u, v;
//	BarycentricCoordinates(q, p0, p1, u, v);
//	Vector3 tmp;
//	// returning s0
//	if (v <= 0)
//	{
//		getPoint(q, p0, p1, tmp, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point0);
//		return VoronoiRegion::Point0;
//	}
//
//	// returning s1
//	else if (u <= 0)
//	{
//		getPoint(q, p0, p1, tmp, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point1);
//		return VoronoiRegion::Point1;
//	}
//
//	// If u > 0 and v > 0 then p is in the line’s voronoi region
//	getEdge(q, p0, p1, tmp, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge01, u, v);
//	return VoronoiRegion::Edge01;
//}
//
//VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
//	size_t& newSize, int newIndices[4],
//	Vector3& closestPoint, Vector3& searchDirection)
//{
//	/******Student:Assignment5******/
//	float u01, v01, u12, v12, u20, v20;
//	Vector3 tmp;
//
//	BarycentricCoordinates(q, p0, p1, u01, v01);
//	BarycentricCoordinates(q, p1, p2, u12, v12);
//	BarycentricCoordinates(q, p2, p0, u20, v20);
//
//	//s0
//	if (v01 <= 0 && u20 <= 0)
//	{
//		getPoint(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point0);
//		return VoronoiRegion::Point0;
//	}
//
//	//s2
//	if (v20 <= 0 && u12 <= 0)
//	{
//		getPoint(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point2);
//		return VoronoiRegion::Point2;
//	}
//
//	//s1
//	if (v12 <= 0 && u01 <= 0) {
//		getPoint(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point1);
//		return VoronoiRegion::Point1;
//	}
//
//	float u, v, w;
//	BarycentricCoordinates(q, p0, p1, p2, u, v, w);
//
//	//triangle
//	if (u > 0 && v > 0 && w > 0)
//	{
//		getTriangle(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Triangle012, u, v, w);
//		return VoronoiRegion::Triangle012;
//	}
//
//	//edge 01
//	if (u01 > 0 && v01 > 0 && w < 0)
//	{
//		getEdge(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge01, u01, v01);
//		return VoronoiRegion::Edge01;
//	}
//
//	//edge 02
//	else if (u20 > 0 && v20 > 0 && v < 0)
//	{
//		getEdge(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge02, u20, v20);
//		return VoronoiRegion::Edge02;
//	}
//
//	//edge 12
//	else if (u12 > 0 && v12 > 0 && u < 0)
//	{
//		getEdge(q, p0, p1, p2, tmp, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge12, u12, v12);
//		return VoronoiRegion::Edge12;
//	}
//	return VoronoiRegion::Unknown;
//}
//
//VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//	size_t& newSize, int newIndices[4],
//	Vector3& closestPoint, Vector3& searchDirection)
//{
//	/******Student:Assignment5******/
//	float u01, v01, u12, v12, u02, v02, u03, v03, u13, v13, u23, v23;
//
//	BarycentricCoordinates(q, p0, p1, u01, v01);
//	BarycentricCoordinates(q, p0, p2, u02, v02);
//	BarycentricCoordinates(q, p0, p3, u03, v03);
//	BarycentricCoordinates(q, p1, p2, u12, v12);
//	BarycentricCoordinates(q, p1, p3, u13, v13);
//	BarycentricCoordinates(q, p2, p3, u23, v23);
//
//	//s0
//	if (v01 <= 0 && v02 <= 0 && v03 <= 0)
//	{
//		getPoint(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point0);
//		return VoronoiRegion::Point0;
//	}
//
//	//s1
//	if (u01 <= 0 && v12 <= 0 && v13 <= 0)
//	{
//		getPoint(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point1);
//		return VoronoiRegion::Point1;
//	}
//
//	//s2
//	if (u02 <= 0 && u12 <= 0 && v23 <= 0)
//	{
//		getPoint(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point2);
//		return VoronoiRegion::Point2;
//	}
//
//	//s3
//	if (u03 <= 0 && u13 <= 0 && u23 <= 0)
//	{
//		getPoint(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Point3);
//		return VoronoiRegion::Point3;
//	}
//
//	float u013, v013, w013, u021, v021, w021, u023, v023, w023, u123, v123, w123;
//
//	BarycentricCoordinates(q, p0, p1, p3, u013, v013, w013);
//	BarycentricCoordinates(q, p0, p2, p1, u021, v021, w021);
//	BarycentricCoordinates(q, p1, p2, p3, u123, v123, w123);
//	BarycentricCoordinates(q, p0, p2, p3, u023, v023, w023);
//
//	//edge 01
//	if (u01 > 0 && v01 > 0 && w013 < 0 && v021 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge01, u01, v01);
//		return VoronoiRegion::Edge01;
//	}
//
//	//edge 12
//	if (u12 > 0 && v12 > 0 && u021 < 0 && w123 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge12, u12, v12);
//		return VoronoiRegion::Edge12;
//	}
//
//	//edge 02
//	if (u02 > 0 && v02 > 0 && w021 < 0 && w023 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge02, v02, u02);
//		return VoronoiRegion::Edge02;
//	}
//
//	//edge 03
//	if (u03 > 0 && v03 > 0 && v013 < 0 && v023 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge03, u03, v03);
//		return VoronoiRegion::Edge03;
//	}
//
//	//edge 13
//	if (u13 > 0 && v13 > 0 && u013 < 0 && v123 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge13, u13, v13);
//		return VoronoiRegion::Edge13;
//	}
//
//	//edge 23
//	if (u23 > 0 && v23 > 0 && u023 < 0 && u123 < 0)
//	{
//		getEdge(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection, VoronoiRegion::Edge23, u23, v23);
//		return VoronoiRegion::Edge23;
//	}
//
//	//triangle 013
//	if (u013 > 0 && v013 > 0 && w013 > 0)
//	{
//		Vector3 normal = Math::Cross(p1 - p0, p3 - p0);
//		if (Math::Dot(normal, p2 - p0) > 0)
//			normal *= -1;
//
//		if (Math::Dot(normal, q - p0) >= 0)
//		{
//			getTriangle(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection,
//				VoronoiRegion::Triangle013, u013, v013, w013);
//			return VoronoiRegion::Triangle013;
//		}
//	}
//
//	//triangle 012
//	if (u021 > 0 && v021 > 0 && w021 > 0)
//	{
//		Vector3 normal = Math::Cross(p1 - p0, p2 - p0);
//		if (Math::Dot(normal, p3 - p0) > 0)
//			normal *= -1;
//
//		if (Math::Dot(normal, q - p0) >= 0)
//		{
//			getTriangle(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection,
//				VoronoiRegion::Triangle012, u021, w021, v021);
//			return VoronoiRegion::Triangle012;
//		}
//	}
//
//	//triangle 023
//	if (u023 > 0 && v023 > 0 && w023 > 0)
//	{
//		Vector3 normal = Math::Cross(p2 - p0, p3 - p0);
//		if (Math::Dot(normal, p1 - p0) > 0)
//			normal *= -1;
//
//		if (Math::Dot(normal, q - p0) >= 0)
//		{
//			getTriangle(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection,
//				VoronoiRegion::Triangle023, u023, v023, w023);
//			return VoronoiRegion::Triangle023;
//		}
//	}
//
//	//triangle 123
//	if (u123 > 0 && v123 > 0 && w123 > 0)
//	{
//		Vector3 normal = Math::Cross(p2 - p1, p3 - p1);
//		if (Math::Dot(normal, p0 - p1) > 0)
//			normal *= -1;
//
//		if (Math::Dot(normal, q - p1) >= 0)
//		{
//			getTriangle(q, p0, p1, p2, p3, newSize, newIndices, closestPoint, searchDirection,
//				VoronoiRegion::Triangle123, u123, v123, w123);
//			return VoronoiRegion::Triangle123;
//		}
//	}
//
//	//Else point is inside tetrahedron
//	getTetrahedron(q, newSize, newIndices, closestPoint, searchDirection);
//	return VoronoiRegion::Tetrahedra0123;
//}
//
//Gjk::Gjk()
//{
//}
//
//bool Gjk::Intersect(const SupportShape* shapeA, const SupportShape* shapeB, unsigned int maxIterations, CsoPoint& closestPoint, float epsilon, int debuggingIndex, bool debugDraw)
//{
//	//Getting the first direction
//	Vector3 direction = shapeA->GetCenter() - shapeB->GetCenter();
//	float sqDir = direction.LengthSq();
//
//	if (-epsilon < sqDir && sqDir < epsilon)
//		direction = Vector3(-1.f, 0.f, 0.f);
//
//	CsoPoint points[4];
//
//	points[0] = ComputeSupport(shapeA, shapeB, direction);
//
//	size_t size = 1;
//	int newIndices[4]{ 0 };
//	Vector3 tmpClosest, searchDirection;
//
//	for (unsigned i = 0; i < maxIterations; ++i)
//	{
//		if (size == 1)
//		{
//			//point
//			IdentifyVoronoiRegion(Vector3::cZero, points[0].mCsoPoint, size, newIndices, tmpClosest, searchDirection);
//			shrinkPoints(points, size, newIndices);
//		}
//		else if (size == 2)
//		{
//			//edge
//			IdentifyVoronoiRegion(Vector3::cZero, points[0].mCsoPoint, points[1].mCsoPoint, size, newIndices, tmpClosest, searchDirection);
//			shrinkPoints(points, size, newIndices);
//		}
//		else if (size == 3)
//		{
//			//triangle
//			IdentifyVoronoiRegion(Vector3::cZero, points[0].mCsoPoint, points[1].mCsoPoint, points[2].mCsoPoint, size, newIndices, tmpClosest, searchDirection);
//			shrinkPoints(points, size, newIndices);
//		}
//		else if (size == 4)
//		{
//			//thetra
//			IdentifyVoronoiRegion(Vector3::cZero, points[0].mCsoPoint, points[1].mCsoPoint, points[2].mCsoPoint, points[3].mCsoPoint, size, newIndices, tmpClosest, searchDirection);
//			shrinkPoints(points, size, newIndices);
//		}
//
//		if (tmpClosest == Vector3::cZero)
//			return true;
//		else
//		{
//			searchDirection.Normalize();
//			CsoPoint next = ComputeSupport(shapeA, shapeB, searchDirection);
//			float distance = Math::Dot(next.mCsoPoint - tmpClosest, searchDirection);
//
//			//check if anymore progress can be made
//			if (distance <= epsilon)
//			{
//				filLClosestPoints(closestPoint, epsilon, size, points, tmpClosest);
//				return false;
//			}
//			else
//				//iterate through
//				points[size++] = next;
//		}
//	}
//
//	// after maxIterations
//	closestPoint = points[size - 1];
//	return false;
//}
//
//Gjk::CsoPoint Gjk::ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB, const Vector3& direction)
//{
//	/******Student:Assignment5******/
//	CsoPoint result;
//	result.mPointA = shapeA->Support(direction);
//	result.mPointB = shapeB->Support(-direction);
//	result.mCsoPoint = result.mPointA - result.mPointB;
//	return result;
//}
//
//
//void Gjk::getPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, size_t& newSize, int newIndices[4],
//	Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type)
//{
//	if (type == VoronoiRegion::Point0)
//	{
//		closestPoint = p0;
//		newSize = 1;
//		newIndices[0] = 0;
//	}
//	else if (type == VoronoiRegion::Point1)
//	{
//		closestPoint = p1;
//		newSize = 1;
//		newIndices[0] = 1;
//	}
//	else if (type == VoronoiRegion::Point2)
//	{
//		closestPoint = p2;
//		newSize = 1;
//		newIndices[0] = 2;
//	}
//	else if (type == VoronoiRegion::Point3)
//	{
//		closestPoint = p3;
//		newSize = 1;
//		newIndices[0] = 3;
//	}
//	searchDirection = q - closestPoint;
//}
//
//void Gjk::getEdge(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//	size_t& newSize, int newIndices[4], Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type
//	, float u, float v)
//{
//	if (type == VoronoiRegion::Edge01)
//	{
//		closestPoint = u * p0 + v * p1;
//		newSize = 2;
//		newIndices[0] = 0;
//		newIndices[1] = 1;
//	}
//	else if (type == VoronoiRegion::Edge02)
//	{
//		newSize = 2;
//		closestPoint = u * p2 + v * p0;
//		newIndices[0] = 0;
//		newIndices[1] = 2;
//	}
//	else if (type == VoronoiRegion::Edge03)
//	{
//		newSize = 2;
//		closestPoint = u * p0 + v * p3;
//		newIndices[0] = 0;
//		newIndices[1] = 3;
//	}
//	else if (type == VoronoiRegion::Edge12)
//	{
//		newSize = 2;
//		closestPoint = u * p1 + v * p2;
//		newIndices[0] = 1;
//		newIndices[1] = 2;
//	}
//	else if (type == VoronoiRegion::Edge13)
//	{
//		newSize = 2;
//		closestPoint = u * p1 + v * p3;
//		newIndices[0] = 1;
//		newIndices[1] = 3;
//	}
//	else if (type == VoronoiRegion::Edge23)
//	{
//		newSize = 2;
//		closestPoint = u * p2 + v * p3;
//		newIndices[0] = 2;
//		newIndices[1] = 3;
//	}
//	searchDirection = q - closestPoint;
//
//}
//
//void Gjk::getTriangle(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
//	size_t& newSize, int newIndices[4], Vector3& closestPoint, Vector3& searchDirection, VoronoiRegion::Type type,
//	float u, float v, float w)
//{
//	if (type == VoronoiRegion::Triangle012)
//	{
//		newSize = 3;
//		closestPoint = u * p0 + v * p1 + w * p2;
//		newIndices[0] = 0;
//		newIndices[1] = 1;
//		newIndices[2] = 2;
//	}
//	else if (type == VoronoiRegion::Triangle013)
//	{
//		newSize = 3;
//		closestPoint = u * p0 + v * p1 + w * p3;
//		newIndices[0] = 0;
//		newIndices[1] = 1;
//		newIndices[2] = 3;
//	}
//	else if (type == VoronoiRegion::Triangle023)
//	{
//		newSize = 3;
//		closestPoint = u * p0 + v * p2 + w * p3;
//		newIndices[0] = 0;
//		newIndices[1] = 2;
//		newIndices[2] = 3;
//	}
//	else if (type == VoronoiRegion::Triangle123)
//	{
//		newSize = 3;
//		closestPoint = u * p1 + v * p2 + w * p3;
//		newIndices[0] = 1;
//		newIndices[1] = 2;
//		newIndices[2] = 3;
//	}
//	searchDirection = q - closestPoint;
//}
//
//void Gjk::getTetrahedron(const Vector3& q, size_t& newSize, int newIndices[4], Vector3& closestPoint, Vector3& searchDirection)
//{
//	newSize = 4;
//	newIndices[0] = 0;
//	newIndices[1] = 1;
//	newIndices[2] = 2;
//	newIndices[3] = 3;
//	closestPoint = q;
//	searchDirection = Vector3::cZero;
//}
//
//
//void Gjk::shrinkPoints(CsoPoint points[4], size_t size, int newIndices[4])
//{
//	//point
//	if (size == 1)
//	{
//		points[0] = points[newIndices[0]];
//	}
//	//edge
//	else if (size == 2)
//	{
//		CsoPoint temp = points[newIndices[1]];
//		points[0] = points[newIndices[0]];
//		points[1] = temp;
//	}
//	//triangle
//	else if (size == 3)
//	{
//		CsoPoint temp = points[newIndices[1]];
//		CsoPoint temp2 = points[newIndices[2]];
//		points[0] = points[newIndices[0]];
//		points[1] = temp;
//		points[2] = temp2;
//	}
//}
//
//void Gjk::filLClosestPoints(CsoPoint& closestPoint, float epsilon, size_t size, CsoPoint points[4], Vector3 tmpClosest)
//{
//	if (size == 1)
//	{
//		closestPoint = points[0];
//	}
//	else if (size == 2)
//	{
//		closestPoint.mCsoPoint = tmpClosest;
//		float u, v;
//		BarycentricCoordinates(tmpClosest, points[0].mCsoPoint, points[1].mCsoPoint,
//			u, v, epsilon);
//
//		closestPoint.mPointA = points[0].mPointA * u + points[1].mPointA * v;
//		closestPoint.mPointB = points[0].mPointB * u + points[1].mPointB * v;
//	}
//	else if (size == 3)
//	{
//		closestPoint.mCsoPoint = tmpClosest;
//		float u, v, w;
//		BarycentricCoordinates(tmpClosest, points[0].mCsoPoint, points[1].mCsoPoint, points[2].mCsoPoint, u, v, w, epsilon);
//		closestPoint.mPointA = points[0].mPointA * u + points[1].mPointA * v + points[2].mPointA * w;
//		closestPoint.mPointB = points[0].mPointB * u + points[1].mPointB * v + points[2].mPointB * w;
//	}
//}