///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <DebugDraw.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_2>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 23 February 2021
//End Header -------------------------------------------------------*/
//
#include "pch.h"
//#include "DebugDraw.h"
//#include "Shapes.h"
//
//namespace Xivi
//{
//
//#define ShowDebugDrawWarnings true
//
//	DebugDrawer* gDebugDrawer = new DebugDrawer();
//
//	//-----------------------------------------------------------------------------DebugShape
//	DebugShape::DebugShape()
//	{
//		mColor = Vector4(.6f);
//		mMask = (unsigned int)-1;
//		mTimer = 0;
//		mOnTop = false;
//		mTransform = mTransform.identity();
//	}
//
//	DebugShape& DebugShape::Color(const Vector4& color)
//	{
//		mColor = color;
//		return *this;
//	}
//
//	DebugShape& DebugShape::OnTop(bool state)
//	{
//		mOnTop = state;
//		return *this;
//	}
//
//	DebugShape& DebugShape::Time(float time)
//	{
//		mTimer = time;
//		return *this;
//	}
//
//	DebugShape& DebugShape::SetMaskBit(int bitIndex)
//	{
//		mMask = 1 << bitIndex;
//		return *this;
//	}
//
//	DebugShape& DebugShape::SetTransform(const Matrix4& transform)
//	{
//		mTransform = transform;
//		return *this;
//	}
//
//	//-----------------------------------------------------------------------------DebugDrawer
//	DebugDrawer::DebugDrawer()
//	{
//		mActiveMask = (unsigned int)-1;
//		mApplication = NULL;
//	}
//
//	void DebugDrawer::Update(float dt)
//	{
//		std::vector<DebugShape> newShapes;
//		for (size_t i = 0; i < mShapes.size(); ++i)
//		{
//			DebugShape& shape = mShapes[i];
//			shape.mTimer -= dt;
//
//			// If the shape still has time left then add it to the list of shapes to keep drawing,
//			// anything that has a timer that ran out will not be in the new list
//			if (shape.mTimer >= 0)
//				newShapes.push_back(shape);
//		}
//
//		mShapes.swap(newShapes);
//	}
//
//	void DebugDrawer::Draw()
//	{
//		for (size_t i = 0; i < mShapes.size(); ++i)
//		{
//			DebugShape& shape = mShapes[i];
//
//			// If the shape doesn't have one of the active mask bits set then don't draw it
//			if ((shape.mMask & mActiveMask) == 0)
//				continue;
//
//			// If this shape always draws on top then disable depth testing
//			if (shape.mOnTop)
//				glDisable(GL_DEPTH_TEST);
//
//
//			// Decompose the matrix to set the gl transform (too lazy to properly transform the matrix between formats)
//			float radians;
//			Math::SIMDVec3 scale, translation, axis;
//			Math::Matrix_3x3 rotationMat{ 0.0f };
//			shape.mTransform.decompose(&scale, &rotationMat, &translation);
//			Math::ToAxisAngle(Math::ToQuaternion(rotationMat), &axis, &radians);
//			glPushMatrix();
//			// Set the transform
//			glTranslatef(getX(translation), getY(translation), getZ(translation));
//			glRotatef(radians * Math::s_TODEG, getX(axis), getY(axis), getZ(axis));
//			glScalef(getX(scale), getY(scale), getZ(scale));
//
//			glBegin(GL_LINES);
//			glColor3fv(shape.mColor.get());
//
//			// Draw all of the line segments of this shape
//			for (size_t j = 0; j < shape.mSegments.size(); ++j)
//			{
//				LineSegment& segment = shape.mSegments[j];
//
//				glVertex3fv(segment.mStart.get());
//				glVertex3fv(segment.mEnd.get());
//			}
//
//			glEnd();
//			glPopMatrix();
//
//			// Make sure to re-enable depth testing
//			if (shape.mOnTop)
//				glEnable(GL_DEPTH_TEST);
//		}
//	}
//
//	DebugShape& DebugDrawer::GetNewShape()
//	{
//		mShapes.push_back(DebugShape());
//		return mShapes.back();
//	}
//
//	DebugShape& DebugDrawer::DrawPoint(const Math::SIMDVec3& point)
//	{
//		/******Student:Assignment2******/
//		return DrawSphere(Sphere(point, 0.1f));
//	}
//
//	DebugShape& DebugDrawer::DrawLine(const LineSegment& line)
//	{
//		/******Student:Assignment2******/
//		// Draw a simple line
//		DebugShape& shape = GetNewShape();
//		shape.mSegments.push_back(line);
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawRay(const Ray& ray, float t)
//	{
//		/******Student:Assignment2******/
//		// Draw a ray to a given t-length. The ray must have an arrow head for visualization
//
//		DebugShape& shape = GetNewShape();
//
//		//Drawing the line
//		const auto p0 = ray.mStart + t * ray.mDirection;
//		LineSegment line = { ray.mStart, p0 };
//		shape.mSegments.push_back(line);
//
//		auto xAxis = cross(ray.mDirection, Math::SIMDVec3{ 0.0f, 1.0f, 0.0f });
//		//if the direction is parrallel to y normal
//		if (xAxis == Math::SIMDVec3{ 0.0f, 0.0f, 0.0f })
//			xAxis = Math::SIMDVec3{ 1.0f, 1.0f, 0.0f };
//		else
//			xAxis = normalize(xAxis);
//
//		auto yAxis = normalize(cross(xAxis, ray.mDirection));
//
//		const int n = 20;
//		const float radius = 0.2f;
//		Math::SIMDVec3 center = p0 - ray.mDirection;
//		float inc = (Math::s_PI * 2) / n;
//
//		Math::SIMDVec3 lastVector = (cosf((Math::s_PI * 2) - inc) * xAxis + sinf((Math::s_PI * 2) - inc) * yAxis) * radius + center;
//		for (float theta = 0.0f; theta <= (Math::s_PI * 2); theta += inc)
//		{
//			Math::SIMDVec3 currVector = (cosf(theta) * xAxis + sinf(theta) * yAxis) * radius + center;
//			shape.mSegments.push_back({ p0, currVector });
//			shape.mSegments.push_back({ lastVector, currVector });
//
//			lastVector = currVector;
//		}
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawSphere(const Sphere& sphere)
//	{
//		/******Student:Assignment2******/
//		// Draw a sphere with 4 rings: x-axis, y-axis, z-axis, and the horizon disc.
//		// Note: To access the camera's position for the horizon disc calculation use mApplication->mCamera.mTranslation
//		DebugShape& shape = GetNewShape();
//
//		//Drawing the sphere
//		int n = 20;
//		float inc = (Math::s_PI * 2) / n;
//
//		//Setting up the basis
//		Math::SIMDVec3 basisX = Math::SIMDVec3{ 1.0f, 0.0f, 0.0f } *sphere.mRadius;
//		Math::SIMDVec3 basisY = Math::SIMDVec3{ 0.0f, 1.0f, 0.0f } *sphere.mRadius;
//		Math::SIMDVec3 basisZ = Math::SIMDVec3{ 0.0f, 0.0f, 1.0f } *sphere.mRadius;
//
//		for (int i = 0; i <= n; ++i)
//		{
//			float angle1 = (i > 0 ? i - 1 : n) * inc;
//			float angle2 = (i == n ? (Xivi::Math::s_PI * 2) : i * inc);
//
//			// xy condition
//			shape.mSegments.push_back(
//				{ Math::Cos(angle2) * basisX + Math::Sin(angle2) * basisY + sphere.mCenter,
//				Math::Cos(angle1) * basisX + Math::Sin(angle1) * basisY + sphere.mCenter }
//			);
//
//			//xz condition
//			shape.mSegments.push_back(
//				{ Math::Cos(angle2) * basisX + Math::Sin(angle2) * basisZ + sphere.mCenter,
//				Math::Cos(angle1) * basisX + Math::Sin(angle1) * basisZ + sphere.mCenter }
//			);
//
//			//yz condition
//			shape.mSegments.push_back(
//				{ Math::Cos(angle2) * basisY + Math::Sin(angle2) * basisZ + sphere.mCenter,
//				Math::Cos(angle1) * basisY + Math::Sin(angle1) * basisZ + sphere.mCenter }
//			);
//		}
//
//		//float rPrime;
//		//Math::SIMDVec3 cPrime;
//		//Math::SIMDVec3 up;
//		//Math::SIMDVec3 right;
//
//		//horizon disc
//		/*if (mApplication)
//		{
//			auto lookAt = sphere.mCenter - mApplication->mCamera.mTranslation;
//			float d = lookAt.Length();
//			float l = sqrtf((d * d) - (sphere.mRadius * sphere.mRadius));
//			rPrime = (sphere.mRadius * l) / d;
//			float z = sqrtf((sphere.mRadius * sphere.mRadius) - (rPrime * rPrime));
//			cPrime = sphere.mCenter - z * lookAt.Normalized();
//
//			up = Xivi::Math::SIMDVec3(0.0f, 1.0f, 0.0f);
//			right = Math::Cross(lookAt, up).Normalized();
//			up = Math::Cross(right, lookAt).Normalized();
//			right *= rPrime;
//			up *= rPrime;
//
//
//			for (int i = 0; i <= n; ++i)
//			{
//				float angle1 = (i > 0 ? i - 1 : n) * inc;
//				float angle2 = (i == n ? (Xivi::Math::s_PI * 2) : i * inc);
//
//				shape.mSegments.push_back(
//					{ Math::Cos(angle2) * up + Math::Sin(angle2) * right + cPrime,
//					Math::Cos(angle1) * up + Math::Sin(angle1) * right + cPrime }
//				);
//			}
//		}*/
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawAabb(const Aabb& aabb)
//	{
//		/******Student:Assignment2******/
//		// Draw all edges of an aabb. Make sure to not mis-match edges!
//
//		DebugShape& shape = GetNewShape();
//
//		Xivi::Math::SIMDVec3 vec[8]{
//			aabb.mMin, // front_btm left
//			Math::SIMDVec3{getX(aabb.mMin),getY(aabb.mMin),getZ(aabb.mMax)}, //back_btm left
//			Math::SIMDVec3{getX(aabb.mMin),getY(aabb.mMax),getZ(aabb.mMin)}, //front_top left
//			Math::SIMDVec3{getX(aabb.mMin),getY(aabb.mMax),getZ(aabb.mMax)}, //back_top left
//			Math::SIMDVec3{getX(aabb.mMax),getY(aabb.mMin),getZ(aabb.mMin)}, //front_bottom right
//			Math::SIMDVec3{getX(aabb.mMax),getY(aabb.mMin),getZ(aabb.mMax)}, //back_bottom right
//			Math::SIMDVec3{getX(aabb.mMax),getY(aabb.mMax),getZ(aabb.mMin)}, //front_top right
//			aabb.mMax // back_top right
//		};
//
//		shape.mSegments.push_back({ vec[0], vec[1] });
//		shape.mSegments.push_back({ vec[0], vec[2] });
//		shape.mSegments.push_back({ vec[1], vec[3] });
//		shape.mSegments.push_back({ vec[2], vec[3] });
//		shape.mSegments.push_back({ vec[0], vec[4] });
//		shape.mSegments.push_back({ vec[1], vec[5] });
//		shape.mSegments.push_back({ vec[2], vec[6] });
//		shape.mSegments.push_back({ vec[3], vec[7] });
//		shape.mSegments.push_back({ vec[6], vec[7] });
//		shape.mSegments.push_back({ vec[5], vec[7] });
//		shape.mSegments.push_back({ vec[4], vec[6] });
//		shape.mSegments.push_back({ vec[4], vec[5] });
//
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawTriangle(const Triangle& triangle)
//	{
//		/******Student:Assignment2******/
//		// Draw the 3 edges of a triangles
//		DebugShape& shape = GetNewShape();
//		shape.mSegments.push_back({ triangle.mPoints[0], triangle.mPoints[1] });
//		shape.mSegments.push_back({ triangle.mPoints[1], triangle.mPoints[2] });
//		shape.mSegments.push_back({ triangle.mPoints[2], triangle.mPoints[0] });
//		return shape;
//	}
//
//
//	DebugShape& DebugDrawer::DrawPlane(const Plane& plane, float sizeX, float sizeY)
//	{
//		/******Student:Assignment2******/
//		// Draw a quad with a normal at the plane's center.
//		auto p0 = plane.GetNormal() * getW(plane.mData);
//		DebugShape& shape = DrawRay(Ray{ p0 , plane.GetNormal() }, 4.0f);
//
//		auto basis0 = cross(plane.GetNormal(), Math::SIMDVec3{ 0.0f, 1.0f, 0.0f });
//		if (basis0 == Math::SIMDVec3{ 0.0f, 0.0f, 0.0f })
//			basis0 = Math::SIMDVec3{ 1.0f, 0.0f, 0.0f };
//		else
//			basis0 = basis0.normalize();
//
//		auto basis1 = normalize(cross(plane.GetNormal(), basis0));
//
//		auto p1 = basis0 * sizeY + basis1 * sizeX + p0;
//		auto p2 = basis0 * sizeY - basis1 * sizeX + p0;
//		auto p3 = -basis0 * sizeY + basis1 * sizeX + p0;
//		auto p4 = -basis0 * sizeY - basis1 * sizeX + p0;
//		shape.mSegments.emplace_back(LineSegment{ p1,  p2 });
//		shape.mSegments.emplace_back(LineSegment{ p1, p3 });
//		shape.mSegments.emplace_back(LineSegment{ p3, p4 });
//		shape.mSegments.emplace_back(LineSegment{ p4,  p2 });
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawQuad(const Math::SIMDVec3& p0, const Math::SIMDVec3& p1, const Math::SIMDVec3& p2, const Math::SIMDVec3& p3)
//	{
//		/******Student:Assignment2******/
//		// Draw the4 edges of a quad. Make sure to look at this and make sure the quad is not bow-tied.
//		DebugShape& shape = GetNewShape();
//		shape.mSegments.push_back({ p0, p1 });
//		shape.mSegments.push_back({ p1, p2 });
//		shape.mSegments.push_back({ p2, p3 });
//		shape.mSegments.push_back({ p3, p0 });
//		return shape;
//	}
//
//	DebugShape& DebugDrawer::DrawFrustum(const Frustum& frustum)
//	{
//		/******Student:Assignment2******/
//		// Draw the 6 faces of the frustum using the 8 frustum points.
//		// See Frustum.Set for the point order. For example, Points[4] is left-bottom-front.
//
//		DebugShape& shape = GetNewShape();
//		std::vector<LineSegment>copy
//		{
//			//nearPlane
//			LineSegment{frustum.mPoints[0],frustum.mPoints[1]},
//			LineSegment{frustum.mPoints[2],frustum.mPoints[3]},
//			LineSegment{frustum.mPoints[3],frustum.mPoints[0]},
//			LineSegment{frustum.mPoints[1],frustum.mPoints[2]},
//
//			//farPlane
//			LineSegment{frustum.mPoints[4],frustum.mPoints[5]},
//			LineSegment{frustum.mPoints[6],frustum.mPoints[7]},
//			LineSegment{frustum.mPoints[7],frustum.mPoints[4]},
//			LineSegment{frustum.mPoints[5],frustum.mPoints[6]},
//
//			LineSegment{frustum.mPoints[0],frustum.mPoints[4]},
//			LineSegment{frustum.mPoints[1],frustum.mPoints[5]},
//			LineSegment{frustum.mPoints[2],frustum.mPoints[6]},
//			LineSegment{frustum.mPoints[3],frustum.mPoints[7]}
//		};
//
//		shape.mSegments.insert(shape.mSegments.begin(), copy.begin(), copy.end());
//		return shape;
//	}
//}