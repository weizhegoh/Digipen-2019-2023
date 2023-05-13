/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
//#include "Xivi/Core/Utils/Math/MathFunctions.hpp"
//
//namespace Xivi
//{
//
//	class Application;
//	class DebugDrawer;
//	class LineSegment;
//	class Ray;
//	class Sphere;
//	class Aabb;
//	class Triangle;
//	class Plane;
//	class Frustum;
//
//
//	using Vector4 = Math::SIMDVec4;
//	using Matrix4 = Math::Matrix_4x4;
//
//	//-----------------------------------------------------------------------------DebugShape
//	class DebugShape
//	{
//	public:
//		DebugShape();
//
//		// These functions allow chaining of setting properties (C++ closest thing to named parameters)
//
//		// Set the color of this shape.
//		DebugShape& Color(const Vector4& color);
//		// Does this shape ignore depth test?
//		DebugShape& OnTop(bool state);
//		// How long should this shape draw for?
//		DebugShape& Time(float time);
//		// Set a random bit on the mask which can control what is drawn with the number keys.
//		DebugShape& SetMaskBit(int bitIndex);
//		// A transform to apply to the shape when drawing.
//		// Primarily for spatial partitions in local space to draw their data.
//		DebugShape& SetTransform(const Matrix4& transform);
//
//	private:
//		friend DebugDrawer;
//
//		Math::SIMDVec4 mColor;
//		float mTimer;
//		unsigned int mMask;
//		bool mOnTop;
//		Math::Matrix_4x4 mTransform;
//
//		// All of the lines use to draw this shape
//		std::vector<LineSegment> mSegments;
//	};
//
//	//-----------------------------------------------------------------------------DebugDrawer
//	class DebugDrawer
//	{
//	public:
//		DebugDrawer();
//
//		void Update(float dt);
//		void Draw();
//
//		// Makes a new debug shape and inserts it into the shapes to draw.
//		DebugShape& GetNewShape();
//
//
//		// You must implement all of the below functions
//
//		DebugShape& DrawPoint(const Math::SIMDVec3& point);
//		DebugShape& DrawLine(const LineSegment& line);
//		DebugShape& DrawRay(const Ray& ray, float t);
//		DebugShape& DrawSphere(const Sphere& sphere);
//		DebugShape& DrawAabb(const Aabb& aabb);
//		DebugShape& DrawTriangle(const Triangle& triangle);
//		DebugShape& DrawPlane(const Plane& plane, float sizeX, float sizeY);
//		DebugShape& DrawQuad(const Math::SIMDVec3& p0, const Math::SIMDVec3& p1, const Math::SIMDVec3& p2, const Math::SIMDVec3& p3);
//		DebugShape& DrawFrustum(const Frustum& frustum);
//
//	private:
//		friend Application;
//
//		typedef std::vector<DebugShape> DebugShapeList;
//		DebugShapeList mShapes;
//		// Any active masks used to filter out what's currently drawing.
//		unsigned int mActiveMask;
//
//		Application* mApplication;
//	};
//
//	extern DebugDrawer* gDebugDrawer;
//}