///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <BspTree.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_4>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 31 March 2021
//End Header -------------------------------------------------------*/
//
#include "pch.h"
//#include "Shapes.h"
//#include "BspTree.h"
//#include "Geometry.h"
//#include "DebugDraw.h"
//
//namespace Xivi
//{
//	BspTreeQueryData::BspTreeQueryData()
//	{
//		mDepth = 0;
//	}
//
//	void BspTree::SplitTriangle(const Plane& plane, const Triangle& tri, TriangleList& coplanarFront, TriangleList& coplanarBack, TriangleList& front, TriangleList& back, float epsilon)
//	{
//		IntersectionType::Type type = PlaneTriangle(
//			plane.mData,
//			tri.mPoints[0],
//			tri.mPoints[1],
//			tri.mPoints[2],
//			epsilon
//		);
//
//		if (type == IntersectionType::Inside)
//		{
//			front.emplace_back(tri);
//		}
//		else if (type == IntersectionType::Outside)
//		{
//			back.emplace_back(tri);
//		}
//		else if (type == IntersectionType::Coplanar)
//		{
//			Plane candidate{ tri.mPoints[0], tri.mPoints[1], tri.mPoints[2] };
//
//			if (Math::SIMD::dot(candidate.GetNormal(), plane.GetNormal()) > 0.0f)
//				coplanarFront.emplace_back(tri);
//			else
//				coplanarBack.emplace_back(tri);
//		}
//		else
//		{
//			std::vector<Vec3f> tmp_frontPoints;
//			std::vector<Vec3f> tmp_backPoints;
//
//			std::array<IntersectionType::Type, 3> p{
//				PointPlane(tri.mPoints[0], plane.mData, epsilon),
//				PointPlane(tri.mPoints[1], plane.mData, epsilon),
//				PointPlane(tri.mPoints[2], plane.mData, epsilon)
//			};
//
//			for (int A = 0; A < 3; ++A)
//			{
//				int B = (A + 1) % 3;
//
//				//Sutherland-Hodgman clipping algorithm (Full clipping)
//				switch (p[A])
//				{
//				case IntersectionType::Inside:
//				{
//					switch (p[B])
//					{
//					case IntersectionType::Inside:
//					{
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//
//					case IntersectionType::Coplanar:
//					{
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//
//					case IntersectionType::Outside:
//					{
//						float t;
//						RayPlane(tri.mPoints[B], (tri.mPoints[A] - tri.mPoints[B]).normalize(), plane.mData, t, epsilon);
//						Vec3f intersect = tri.mPoints[B] + t * (tri.mPoints[A] - tri.mPoints[B]).normalize();
//
//						tmp_frontPoints.emplace_back(intersect);
//						tmp_backPoints.emplace_back(intersect);
//						tmp_backPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//					}
//				}
//				break;
//				case IntersectionType::Coplanar:
//				{
//					switch (p[B])
//					{
//					case IntersectionType::Inside:
//					{
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//
//					case IntersectionType::Coplanar:
//					{
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//
//					case IntersectionType::Outside:
//					{
//						tmp_backPoints.emplace_back(tri.mPoints[A]);
//						tmp_backPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//					}
//				}
//				break;
//				case IntersectionType::Outside:
//				{
//					switch (p[B])
//					{
//					case IntersectionType::Inside:
//					{
//						float t;
//						RayPlane(tri.mPoints[B], (tri.mPoints[A] - tri.mPoints[B]).normalize(), plane.mData, t, epsilon);
//						Vec3f intersect = tri.mPoints[B] + t * (tri.mPoints[A] - tri.mPoints[B]).normalize();
//						tmp_frontPoints.emplace_back(intersect);
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//						tmp_backPoints.emplace_back(intersect);
//					}
//					break;
//
//					case IntersectionType::Coplanar:
//					{
//						tmp_frontPoints.emplace_back(tri.mPoints[B]);
//						tmp_backPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//
//					case IntersectionType::Outside:
//					{
//						tmp_backPoints.emplace_back(tri.mPoints[B]);
//					}
//					break;
//					}
//				}
//				break;
//
//				}
//			}
//
//			for (unsigned i = 2; i < tmp_frontPoints.size(); ++i)
//			{
//				front.emplace_back(Triangle{ tmp_frontPoints[0], tmp_frontPoints[i - 1], tmp_frontPoints[i] });
//			}
//
//			for (unsigned i = 2; i < tmp_backPoints.size(); ++i)
//			{
//				back.emplace_back(Triangle{ tmp_backPoints[0], tmp_backPoints[i - 1], tmp_backPoints[i] });
//			}
//		}
//	}
//
//	float BspTree::CalculateScore(const TriangleList& triangles, size_t testIndex, float k, float epsilon)
//	{
//		Triangle tri = triangles[testIndex];
//		Vec3f vec = Math::SIMD::cross(tri.mPoints[1] - tri.mPoints[0], tri.mPoints[2] - tri.mPoints[0]);
//
//		if (!vec.length())
//			return Math::PositiveMax();
//
//		Plane plane = Plane(vec.normalize(), tri.mPoints[0]);
//
//		int nFront = 0, nBack = 0, nStraddling = 0;
//		for (unsigned i = 0; i < triangles.size(); ++i)
//		{
//			if (i != testIndex)
//			{
//				IntersectionType::Type type = PlaneTriangle(
//					plane.mData,
//					triangles[i].mPoints[0],
//					triangles[i].mPoints[1],
//					triangles[i].mPoints[2],
//					epsilon
//				);
//
//				if (type == IntersectionType::Inside)
//				{
//					nFront++;
//				}
//				else if (type == IntersectionType::Outside)
//				{
//					nBack++;
//				}
//				else if (type == IntersectionType::Overlaps)
//				{
//					nStraddling++;
//				}
//			}
//		}
//
//		//measuring how good a split plane
//		// score = K*NS + (1 - K) * Abs(NF - NB)
//		return k * nStraddling + (1 - k) * Math::Abs(nFront - nBack);
//	}
//
//	size_t BspTree::PickSplitPlane(const TriangleList& triangles, float k, float epsilon)
//	{
//		float desiredScore = Math::PositiveMax();
//		unsigned index = 0;
//
//		for (unsigned i = 0; i < triangles.size(); ++i)
//		{
//			float currScore = CalculateScore(triangles, i, k, epsilon);
//			if (currScore < desiredScore)
//			{
//				desiredScore = currScore;
//				index = i;
//			}
//		}
//		return index;
//	}
//
//	void BspTree::Construct(const TriangleList& triangles, float k, float epsilon)
//	{
//		m_Root = ConstructRecursive(triangles, k, epsilon);
//	}
//
//	bool BspTree::RayCast(const Ray& ray, float& t, float planeEpsilon, float triExpansionEpsilon, int debuggingIndex)
//	{
//		t = Math::PositiveMax();
//		float tMax = t;
//		float tMin = 0.0f;
//		bool res = RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tMin, tMax, m_Root);
//		t -= triExpansionEpsilon;
//		return res;
//	}
//
//	void BspTree::AllTriangles(TriangleList& triangles) const
//	{
//		AllTrianglesRecursive(triangles, m_Root);
//	}
//
//	void BspTree::Invert()
//	{
//		Invert(m_Root);
//	}
//
//	void BspTree::ClipTo(BspTree* tree, float epsilon)
//	{
//		ClipTo(tree->m_Root, epsilon, m_Root);
//	}
//
//	void BspTree::Union(BspTree* tree, float k, float epsilon)
//	{
//		ClipTo(tree, epsilon);
//		tree->ClipTo(this, epsilon);
//
//		tree->Invert();
//		tree->ClipTo(this, epsilon);
//		tree->Invert();
//
//		TriangleList result;
//		AllTriangles(result);
//		tree->AllTriangles(result);
//
//		Construct(result, k, epsilon);
//	}
//
//	void BspTree::Intersection(BspTree* tree, float k, float epsilon)
//	{
//		// A&B = ~(~A|~B)
//		Invert();
//		tree->Invert();
//		Union(tree, k, epsilon);
//		Invert();
//	}
//
//	void BspTree::Subtract(BspTree* tree, float k, float epsilon)
//	{
//		//A - B = A & ~B
//		tree->Invert();
//		Intersection(tree, k, epsilon);
//	}
//
//	void BspTree::FilloutData(std::vector<BspTreeQueryData>& results) const
//	{
//		FilloutData(results, 0, m_Root);
//	}
//
//	void BspTree::DebugDraw(int level, const Mat4x4& color, int bitMask)
//	{
//		DebugDraw(0, level, color, bitMask, m_Root);
//	}
//
//	BspTree::~BspTree()
//	{
//		Clear(m_Root);
//	}
//
//	void BspTree::Clear(BSPTreeNode* node)
//	{
//		if (node != nullptr)
//		{
//			Clear(node->m_front);
//			Clear(node->m_back);
//
//			delete node;
//			node = nullptr;
//		}
//	}
//
//	BspTree::BSPTreeNode* BspTree::ConstructRecursive(const TriangleList& triangles, float k, float epsilon)
//	{
//		if (triangles.empty())
//			return nullptr;
//		else if (triangles.size() == 1)
//		{
//			BSPTreeNode* newNode = new BSPTreeNode;
//			const Triangle& tri = triangles[0];
//
//			newNode->m_triangleList.push_back(tri);
//			newNode->m_plane = Plane{
//				tri.mPoints[0],
//				tri.mPoints[1],
//				tri.mPoints[2]
//			};
//		}
//
//		BSPTreeNode* newNode = new BSPTreeNode;
//		unsigned index = (unsigned)PickSplitPlane(triangles, k, epsilon);
//		const Triangle& tri = triangles[index];
//
//		newNode->m_plane = Plane{
//			tri.mPoints[0],
//			tri.mPoints[1],
//			tri.mPoints[2]
//		};
//
//		TriangleList front, back;
//
//		for (unsigned i = 0; i < triangles.size(); ++i)
//		{
//			SplitTriangle(newNode->m_plane, triangles[i], newNode->m_triangleList, newNode->m_triangleList, front, back, epsilon);
//		}
//
//		//front
//		if (newNode->m_front)
//			newNode->m_front->m_parent = newNode;
//		newNode->m_front = ConstructRecursive(front, k, epsilon);
//
//		//back
//		if (newNode->m_back)
//			newNode->m_back->m_parent = newNode;
//		newNode->m_back = ConstructRecursive(back, k, epsilon);
//		return newNode;
//	}
//
//	bool BspTree::RayCast(const Ray& ray, float& t, float planeEpsilon, float triExpansionEpsilon, int debuggingIndex, float tmin, float tmax, BSPTreeNode* node)
//	{
//		if (node == nullptr)
//			return false;
//
//		const Plane& plane = node->m_plane;
//		IntersectionType::Type sr_intersect = PointPlane(ray.mStart, plane.mData, planeEpsilon);
//
//		BSPTreeNode* near_side = (sr_intersect == IntersectionType::Inside) ? node->m_front : node->m_back;
//		BSPTreeNode* far_side = (sr_intersect == IntersectionType::Outside) ? node->m_front : node->m_back;
//		float tPlane = 0.0f;
//		bool collided = false;
//
//		//check for coplanar first (case 1)
//		if (sr_intersect == IntersectionType::Coplanar)
//		{
//			//visiting the geometry plane
//			for (unsigned i = 0; i < node->m_triangleList.size(); ++i)
//			{
//				float temp = 0.0f;
//				if (RayTriangle(ray.mStart, ray.mDirection, node->m_triangleList[i].mPoints[0], node->m_triangleList[i].mPoints[1], node->m_triangleList[i].mPoints[2], temp, triExpansionEpsilon))
//				{
//					collided = true;
//
//					if (temp < t)
//					{
//						t = temp;
//					}
//				}
//			}
//			//visit both side
//			if (RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, node->m_front))
//				collided = true;
//
//			if (RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, node->m_back))
//				collided = true;
//
//			return collided;
//		}
//		//ray is parrallel to plane = not insersect
//		else if (!RayPlane(ray.mStart, ray.mDirection, node->m_plane.mData, tPlane))
//		{
//			return RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, near_side);
//		}
//		else
//		{
//			float cos_theta = Math::SIMD::dot(ray.mDirection.normalize(), plane.GetNormal().normalize());
//			float tEpsilon = Math::Abs(planeEpsilon / cos_theta);
//			float tnewmin = tPlane - tEpsilon, tnewmax = tPlane + tEpsilon;
//
//			//case 1
//			if (tmin <= tnewmax && tnewmin <= tmax)
//			{
//				for (unsigned i = 0; i < node->m_triangleList.size(); ++i)
//				{
//					float temp = 0.0f;
//					if (RayTriangle(ray.mStart, ray.mDirection, node->m_triangleList[i].mPoints[0], node->m_triangleList[i].mPoints[1], node->m_triangleList[i].mPoints[2], temp, triExpansionEpsilon))
//					{
//						collided = true;
//
//						if (temp < t)
//						{
//							t = temp;
//						}
//					}
//				}
//
//				if (RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tnewmax, near_side))
//					collided = true;
//
//				if (RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tnewmin, tmax, far_side))
//					collided = true;
//				return collided;
//
//			}
//			//case 2
//			else if (tPlane < 0.0f)
//			{
//				return RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, near_side);
//			}
//			//case 3
//			else if (tmax < tPlane)
//			{
//				return RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, near_side);
//			}
//			//case 4
//			else if (0.0f < tPlane && tPlane < tmin)
//			{
//				return RayCast(ray, t, planeEpsilon, triExpansionEpsilon, debuggingIndex, tmin, tmax, far_side);
//			}
//			return false;
//		}
//
//	}
//
//	void BspTree::AllTrianglesRecursive(TriangleList& triangles, BSPTreeNode* node) const
//	{
//		if (node != nullptr)
//		{
//			for (auto& tri : node->m_triangleList)
//			{
//				triangles.emplace_back(tri);
//			}
//
//			AllTrianglesRecursive(triangles, node->m_front);
//			AllTrianglesRecursive(triangles, node->m_back);
//		}
//	}
//
//	void BspTree::FilloutData(std::vector<BspTreeQueryData>& results, int depth, BSPTreeNode* node) const
//	{
//		if (node)
//		{
//			BspTreeQueryData data;
//			data.mDepth = depth;
//			for (auto& tri : node->m_triangleList)
//			{
//				data.mTriangles.push_back(tri);
//			}
//
//			results.push_back(data);
//
//			FilloutData(results, depth + 1, node->m_front);
//			FilloutData(results, depth + 1, node->m_back);
//		}
//	}
//
//	void BspTree::Invert(BSPTreeNode* node)
//	{
//		if (node != nullptr)
//		{
//			Invert(node->m_front);
//			Invert(node->m_back);
//
//			node->m_plane.mData *= -1;
//
//			for (auto& tri : node->m_triangleList)
//				std::swap(tri.mPoints[0], tri.mPoints[1]);
//
//			std::swap(node->m_front, node->m_back);
//		}
//	}
//
//	void BspTree::ClipTo(BSPTreeNode* tree, float epsilon, BSPTreeNode* node)
//	{
//		if (node != nullptr)
//		{
//			TriangleList tri;
//
//			ClpTriangles(tree, tri, node->m_triangleList, epsilon);
//			node->m_triangleList = tri;
//
//			ClipTo(tree, epsilon, node->m_front);
//			ClipTo(tree, epsilon, node->m_back);
//		}
//	}
//
//	void BspTree::ClpTriangles(BSPTreeNode* tree, TriangleList& triangles, const TriangleList& result, float epsilon)
//	{
//		if (tree != nullptr)
//		{
//			TriangleList front, back;
//			for (const auto& tri : result)
//				SplitTriangle(tree->m_plane, tri, front, back, front, back, epsilon);
//
//			if (tree->m_front)
//				ClpTriangles(tree->m_front, triangles, front, epsilon);
//			else
//			{
//				for (unsigned i = 0; i < front.size(); ++i)
//					triangles.push_back(front[i]);
//			}
//
//			if (tree->m_back)
//				ClpTriangles(tree->m_back, triangles, back, epsilon);
//		}
//	}
//
//	void BspTree::DebugDraw(int depth, int level, const Mat4x4& color, int bitMask, BSPTreeNode* node)
//	{
//		if (node)
//		{
//			if (level == -1 || level == depth)
//			{
//				for (auto& tri : node->m_triangleList)
//				{
//					DebugShape& shape = tri.DebugDraw();
//					shape.Color(color);
//					shape.SetMaskBit(bitMask);
//
//					if (level == depth && level != -1)
//						return;
//				}
//			}
//			DebugDraw(depth + 1, level, color, bitMask, node->m_front);
//			DebugDraw(depth + 1, level, color, bitMask, node->m_back);
//		}
//	}
//}