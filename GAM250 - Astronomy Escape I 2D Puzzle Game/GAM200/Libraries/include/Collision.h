/*****************************************************************
*\file		 Collision.h
*\brief		 Contains declaration for AABB and hotspot collision

*\author(s)   Goh Wei Zhe	46 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef COLLISION_H_
#define COLLISION_H_

#include <EnginePch.h>
#include <LineSegment.h>
#include <AABB.h>
#include <Circle.h>

namespace Collision
{
	namespace Aabb
	{
		bool Intersection_RectRect(
			const AABB& aabb1,
			const mathEngine::Vector3D& vel1,
			const AABB& aabb2,
			const mathEngine::Vector3D& vel2);
	}
	namespace OBB
	{
		void BuildLineSegment(
			LineSegment& lineSegment,
			const mathEngine::Vector3D& pos,
			float scale, 
			float dir);
		
		bool StaticPointToStaticCircle(
			const mathEngine::Vector3D& P,
			const mathEngine::Vector3D& Center,
			const float Radius);
		
		bool StaticPointToStaticRect(
			const mathEngine::Vector3D& point,
			const mathEngine::Vector3D& box,
			const float width, 
			const float height);
		
		bool StaticPointToOrientedRect(
			const mathEngine::Vector3D& point,
			const mathEngine::Vector3D& box,
			const float width,
			const float height, 
			const float angleRad);
		
		bool CircleToRect(
			const Circle& circle, 
			const AABB& box);
	}

	namespace HotSpot
	{
		unsigned int CheckInstanceBinaryMapCollision(
			float PosX, float PosY, float scaleX, float scaleY);
		void SnapToCell(float& Coordinate);
	}

}



#endif
