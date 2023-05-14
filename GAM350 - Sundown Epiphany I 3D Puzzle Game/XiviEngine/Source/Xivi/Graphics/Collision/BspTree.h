///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <BspTree.hpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_4>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 31 March 2021
//End Header -------------------------------------------------------*/
//#pragma once
//
//namespace Xivi
//{
//
//	class Plane;
//	class Triangle;
//	class Ray;
//
//	typedef std::vector<Triangle> TriangleList;
//
//	// Result data for the bsp tree. Should be filled out with all of the triangles at the given tree depth.
//	class BspTreeQueryData
//	{
//	public:
//		BspTreeQueryData();
//
//		std::vector<Triangle> mTriangles;
//		int mDepth;
//	};
//
//	/******Student:Assignment4******/
//	// Implement a bsp-tree and the CSG operation of subtraction
//	class BspTree
//	{
//	public:
//
//		// Splits the triangle by the given plane into 4 different lists. The front and back side list should be self explanatory (using the proper clipping table described in class).
//		// The coplanar front list should be added to when a triangle is coplanar and the normal points the same direction as the plane normal. When the normal points in the opposite
//		// direction then the coplanar back list should be added to. Note: When more than 3 points need to be added to a side you should split your polygon into a triangle using a
//		// triangle fan. That is (i0, i1, i2), (i0, i2, i3) in order to match my output.
//		static void SplitTriangle(const Plane& plane, const Triangle& tri, TriangleList& coplanarFront, TriangleList& coplanarBack, TriangleList& front, TriangleList& back, float epsilon);
//
//		// Calculate the score of a triangle (using the heuristic described in class) defined by the given test index.
//		// Note: Do not count coplanar triangles in the score!
//		static float CalculateScore(const TriangleList& triangles, size_t testIndex, float k, float epsilon);
//
//		// Return the index of the triangle to use as the split plane. You should use the passed in
//		// k-value in your heuristic to balance the cost of balancing the tree with splitting triangles.
//		size_t PickSplitPlane(const TriangleList& triangles, float k, float epsilon);
//
//		// Given a list of triangles, construct a bsp-tree. You should use the heuristic shown in class
//		// with the given weight (k) to choose between less splits and a more balanced tree.
//		void Construct(const TriangleList& triangles, float k, float epsilon);
//
//		// Given a ray determine if and and what time the ray hits this tree. This should return the t-first value.
//		// This method should be an optimized ray-cast that computes a tMin and tMax as discussed in class in order to not check every triangle!
//		bool RayCast(const Ray& ray, float& t, float planeEpsilon, float triExpansionEpsilon, int debuggingIndex);
//
//		// Just a simple helper to return all triangles in this tree.
//		void AllTriangles(TriangleList& triangles) const;
//		// Invert the tree. This requires swapping all positive and negative sides of the tree
//		// (split planes, inside/outside pointers, triangle winding order).
//		void Invert();
//		// Clips all triangles in this tree against all of the split planes of the passed in tree.
//		// Use the provided epsilon for triangle splitting.
//		void ClipTo(BspTree* tree, float epsilon);
//		// Update this tree so that it is a union of the current tree with the passed in tree.
//		void Union(BspTree* tree, float k, float epsilon);
//		// Update this tree to be the intersection of the current tree with the passed in tree.
//		void Intersection(BspTree* tree, float k, float epsilon);
//		// Update this tree to be the subtraction of the other tree from this tree.
//		// That is, when A = this and B = other compute A - B.
//		void Subtract(BspTree* tree, float k, float epsilon);
//
//		// Fillout the array of triangles with the contents of the tree. The tree should do a pre-order depth-first insertion into the array
//		// (where pre-order means triangle in the node, then recurse down the front of the plane, then the back).
//		// Note: When building the tree make sure to add the split triangle as the first on inside the node and then all other coplanar triangles in order afterwards.
//		void FilloutData(std::vector<BspTreeQueryData>& results) const;
//		// Draw the tree at the given level. Level of -1 means draw the entire tree. You should set the color and
//		// bit-mask on the debug shape returned from a debug drawing operation using ".Color(float4)" and ".MaskBit(size_t)".
//		// The mask bit is just a helper to allow run-time toggling of debug shapes.
//		void DebugDraw(int level, const Mat4x4& color, int bitMask = 0);
//
//		~BspTree();
//
//		// Add your implementation here
//	private:
//		struct BSPTreeNode
//		{
//			BSPTreeNode* m_front = nullptr;
//			BSPTreeNode* m_back = nullptr;
//			BSPTreeNode* m_parent = nullptr;
//			Plane m_plane{};
//			TriangleList m_triangleList = {};
//		};
//
//		//recursive functions
//		void Clear(BSPTreeNode* node);
//		BSPTreeNode* ConstructRecursive(const TriangleList& triangles, float k, float epsilon);
//		bool RayCast(const Ray& ray, float& t, float planeEpsilon, float triExpansionEpsilon, int debuggingIndex, float tmin, float tmax, BSPTreeNode* node);
//		void AllTrianglesRecursive(TriangleList& triangles, BSPTreeNode* node) const;
//		void Invert(BSPTreeNode* n);
//		void ClipTo(BSPTreeNode* tree, float epsilon, BSPTreeNode* node);
//		void FilloutData(std::vector<BspTreeQueryData>& results, int depth, BSPTreeNode* node) const;
//		void DebugDraw(int depth, int level, const Mat4x4& color, int bitMask, BSPTreeNode* node);
//		void ClpTriangles(BSPTreeNode* tree, TriangleList& triangles, const TriangleList& result, float epsilon);
//		BSPTreeNode* m_Root = nullptr;
//	};
//}