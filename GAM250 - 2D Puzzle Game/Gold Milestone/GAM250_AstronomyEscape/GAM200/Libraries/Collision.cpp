/*****************************************************************
*\file		 Collision.cpp
*\brief		 Contains definition for AABB and hotspot collision

*\author(s)   Goh Wei Zhe	174 lines x 100% Code contribution
*			Lee Liang Ping  19 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Collision.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

namespace Collision
{
	namespace Aabb
	{
		bool Intersection_RectRect(const AABB& aabb1, const mathEngine::Vector3D& vel1,
			const AABB& aabb2, const mathEngine::Vector3D& vel2)
		{
			//check for static collision
			if (!((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) ||
				(aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y)))
				return true;

			float tFirst = 0; //initialize tFirst to 0.0f
			float tLast = static_cast<float>(g_dt); //initialize tLast to g_dt
			glm::vec2 Vb;
			Vb.x = vel2.x - vel1.x; //caculate vb of x
			Vb.y = vel2.y - vel1.y; //caculate vb of y

			//Check for both object are are not moving
			if (Vb.x == 0.0f && Vb.y == 0.0f)
				return false;

			if (Vb.x < 0.0f)
			{
				if (aabb1.min.x > aabb2.max.x) // check A min < B max
					return false;
				if (aabb1.max.x < aabb2.min.x) // check A max < B min
					tFirst = glm::max((aabb1.max.x - aabb2.min.x) / Vb.x, tFirst);
				if (aabb1.min.x < aabb2.max.x) // check A min < B Max
					tLast = glm::min((aabb1.min.x - aabb2.max.x) / Vb.x, tLast);
			}
			else if (Vb.x > 0.0f)  //caculate the x axis of the AABB when Vb is < 0
			{
				if (aabb1.max.x < aabb2.min.x) //check A max < B min
					return false;
				if (aabb1.min.x > aabb2.max.x) //check A min > B max
					tFirst = glm::max((aabb1.min.x - aabb2.max.x) / Vb.x, tFirst);
				if (aabb1.max.x > aabb2.min.x) //check A min > B min
					tLast = glm::min((aabb1.max.x - aabb2.min.x) / Vb.x, tLast);
			}
			else if ((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x))
				return false; // overlapping along x

			if (Vb.y < 0.0f)
			{
				if (aabb1.min.y > aabb2.max.y) // check A min < B max
					return false;
				if (aabb1.max.y < aabb2.min.y) // check A max < B min
					tFirst = glm::max((aabb1.max.y - aabb2.min.y) / Vb.y, tFirst);
				if (aabb1.min.y < aabb2.max.y) // check A min < B Max
					tLast = glm::min((aabb1.min.y - aabb2.max.y) / Vb.y, tLast);
			}
			else if (Vb.y > 0.0f)
			{
				if (aabb1.max.y < aabb2.min.y) //check A max < B min
					return false;
				if (aabb1.min.y > aabb2.max.y) //check A min > B max
					tFirst = glm::max((aabb1.min.y - aabb2.max.y) / Vb.y, tFirst);
				if (aabb1.max.y > aabb2.min.y) //check A min > B min
					tLast = glm::min((aabb1.max.y - aabb2.min.y) / Vb.y, tLast);
			}
			else if ((aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y))
				return false; // overlapping along y
			if (tFirst > tLast) //if tFirst > tLast no collied
				return false;
			return true; //collied
		}
	}
	namespace OBB
	{
		void BuildLineSegment(LineSegment& lineSegment,
			const mathEngine::Vector3D& pos, float scale, float dir)
		{
			//compute p0 & p1 from the given midpoint.
			lineSegment.m_pt0 = { pos.x + scale / 2 * cosf(dir),
								  pos.y + scale / 2 * sinf(dir) };
			lineSegment.m_pt1 = { pos.x - scale / 2 * cosf(dir),
								  pos.y - scale / 2 * sinf(dir) };
			//Find the normal of the line segment
			lineSegment.m_normal = { lineSegment.m_pt1.y - lineSegment.m_pt0.y,
								   -(lineSegment.m_pt1.x - lineSegment.m_pt0.x) };
			//Normalize the line segment
			lineSegment.m_normal = glm::normalize(lineSegment.m_normal);
		}

		bool StaticPointToStaticCircle(const mathEngine::Vector3D& P, const mathEngine::Vector3D& Center, const float Radius)
		{
			mathEngine::Vector3D dis;
			if (dis.distance(P, Center) <= Radius * Radius)
				return true;
			else
				return false;
		}

		bool StaticPointToStaticRect(
			const mathEngine::Vector3D& point,
			const mathEngine::Vector3D& box,
			const float width,
			const float height)
		{
			float left = box.x - width * 0.5f; // The leftside limit
			float right = box.x + width * 0.5f; // The rightside limit
			float bottom = box.y - height * 0.5f; // The bottomside limit
			float top = box.y + height * 0.5f; // The topside limit
			if (left <= point.x && right >= point.x && bottom <= point.y && point.y <= top)
				return true;
			else
				return false;
		}

		bool StaticPointToOrientedRect(
			const mathEngine::Vector3D& point,
			const mathEngine::Vector3D& box,
			const float width,
			const float height,
			const float angleRad)
		{
			// Compute the rotation matrix
			mathEngine::Matrix3 rotMtx;
			rotMtx.setIdentity();
			rotMtx.RotRad(-angleRad);
			// Compute the translation matrix
			mathEngine::Matrix3  transMtx;
			transMtx.setIdentity();
			transMtx.Translate(-point.x, -point.y);
			// Concatenate the matrices
			mathEngine::Matrix3  MtxResult = rotMtx * transMtx;
			// Get the linear transformation of the point with the resultant matrix
			mathEngine::Vector3D vecResult = MtxResult * point;

			// Call StaticPointToStaticRect to check if the point is colliding with the OBB 
			if (StaticPointToStaticRect(vecResult, box, width, height))
				return true; // Colliding!
			else
				return false; // Not colliding!
		}

		bool CircleToRect(const Circle& circle, const AABB& box)
		{
			glm::vec2 boxCenter = (box.max + box.min);
			boxCenter /= 2.f;
			glm::vec2 boxSize = abs(box.max.x - box.min);
			
			float circleDistanceX = abs(circle.center.x - boxCenter.x);
			float circleDistanceY = abs(circle.center.y - boxCenter.y);

			if (circleDistanceX > (boxSize.x * 0.5f + circle.radius))
				return false;
			if (circleDistanceY > (boxSize.y * 0.5f + circle.radius))
				return false;

			if (circleDistanceX <= (boxSize.x * 0.5f))
				return true;
			if (circleDistanceY <= (boxSize.y * 0.5f)) 
				return true;

			float cornerDistance_sq = (circleDistanceX - boxSize.x * 0.5f) * (circleDistanceX - boxSize.x * 0.5f) +
				(circleDistanceY - boxSize.y * 0.5f) * (circleDistanceY - boxSize.y * 0.5f);

			return (cornerDistance_sq <= (circle.radius  * circle.radius));
		}


	}
	namespace HotSpot
	{
		unsigned int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
		{
			//Points Declaration
			float x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8;
			GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
			//Right										 
			//-hotspot 1
			x1 = PosX + scaleX / 2; //To reach the right side
			y1 = PosY + scaleY / 4; //To go up 1/4 of the height
			globalVar.ConvertScreenToBinary(x1, y1);
			//Right		
			//-hotspot 2
			x2 = PosX + scaleX / 2; //To reach the right side
			y2 = PosY - scaleY / 4; //To go down 1/4 of the height
			globalVar.ConvertScreenToBinary(x2, y2);
			//Left						
			//-hotspot 3
			x3 = PosX - scaleX / 2; //To reach the left side
			y3 = PosY - scaleY / 4; //To go up 1/4 of the height
			globalVar.ConvertScreenToBinary(x3, y3);
			//Left	
			//-hotspot 4
			x4 = PosX - scaleX / 2; //To reach the left side
			y4 = PosY + scaleY / 4; //To go down 1/4 of the height
			globalVar.ConvertScreenToBinary(x4, y4);
			// Top
			//-hotspot 5
			x5 = PosX + scaleX / 4; //To go right 1/4 of the height
			y5 = PosY + scaleY / 2; //To reach the top side
			globalVar.ConvertScreenToBinary(x5, y5);
			// Top
			//-hotspot 6
			x6 = PosX - scaleX / 4; //To go left 1/4 of the height
			y6 = PosY + scaleY / 2; //To reach the top side
			globalVar.ConvertScreenToBinary(x6, y6);
			// Bottom
			//-hotspot 7
			x7 = PosX + scaleX / 4; //To go right 1/4 of the height
			y7 = PosY - scaleY / 2; //To reach the bottom side
			globalVar.ConvertScreenToBinary(x7, y7);
			// Bottom
			//-hotspot 8
			x8 = PosX - scaleX / 4; //To go left 1/4 of the height
			y8 = PosY - scaleY / 2; //To reach the bottom side
			globalVar.ConvertScreenToBinary(x8, y8);

			int Flag = 0; //a "Flag" integer should be initialized to 0

			if ((int)(globalVar.GetCellValue((int)x1, (int)y1)) ||
				(int)(globalVar.GetCellValue((int)x2, (int)y2)))
				Flag = Flag | COLLISION_RIGHT; //Set Flag with Right
			if ((int)(globalVar.GetCellValue((int)x3, (int)y3)) ||
				(int)(globalVar.GetCellValue((int)x4, (int)y4)))
				Flag = Flag | COLLISION_LEFT; // Set Flag with Left
			if ((int)(globalVar.GetCellValue((int)x5, (int)y5))
				|| (int)(globalVar.GetCellValue((int)x6, (int)y6)))
				Flag = Flag | COLLISION_TOP; // Set Flag with Top
			if ((int)(globalVar.GetCellValue((int)x7, (int)y7)) ||
				(int)(globalVar.GetCellValue((int)x8, (int)y8)))
				Flag = Flag | COLLISION_BOTTOM; // Set Flag with Bottom
			return Flag; //return Flag
		}

		void SnapToCell(float& Coordinate)
		{
			Coordinate = (int)(Coordinate)+0.5f; //snapping the coordinate
		}
	}
}