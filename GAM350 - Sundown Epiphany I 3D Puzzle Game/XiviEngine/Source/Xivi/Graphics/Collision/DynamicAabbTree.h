///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <DynamicAabbTree.hpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_3>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 14 March 2021
//End Header -------------------------------------------------------*/
//
//#pragma once
//
//#include "pch.h"
//#include "SpatialPartition.h"
//#include "Shapes.h"
//
//namespace Xivi
//{
//
//	/******Student:Assignment3******/
//	/// You must implement a dynamic aabb tree as we discussed in class.
//	class DynamicAabbTree : public SpatialPartition
//	{
//	public:
//		DynamicAabbTree();
//		~DynamicAabbTree();
//
//		// Spatial Partition Interface
//		void InsertData(SpatialPartitionKey& key, SpatialPartitionData& data) override;
//		void UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data) override;
//		void RemoveData(SpatialPartitionKey& key) override;
//
//		void DebugDraw(int level, const Xivi::Math::Matrix4& transform, const Math::Vector4& color = Math::Vector4(1), int bitMask = 0) override;
//
//		void CastRay(const Ray& ray, CastResults& results) override;
//		void CastFrustum(const Frustum& frustum, CastResults& results) override;
//
//		void SelfQuery(QueryResults& results) override;
//
//		void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;
//
//		static const float mFatteningFactor;
//
//		// Add your implementation here
//		struct Node
//		{
//			Aabb m_Aabb;
//			void* m_ClientData = nullptr;
//			Node* m_Left = nullptr;
//			Node* m_Right = nullptr;
//			Node* m_Parent = nullptr;
//			int m_Height = 0;
//			size_t m_LastAxis = 0;
//		};
//
//		Node* GetNewNode();
//		Node* surfaceArea_Selection(Node* insertingNode, Node* left, Node* right); //done
//		void Balance(Node* node);
//		void CalculateHeight(Node* node);
//		void UpdateUpwards(Node* node);
//		void freeNode(Node* node);
//		void RemoveDataKey(SpatialPartitionKey& key);
//
//		//recursive functions
//		void insert_Rec(Node* insertingNode, Node* node, SpatialPartitionKey& key, SpatialPartitionData& data);
//		void DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color, int bitMask, Node* node, int currDepth);
//		void CastRay(const Ray& ray, CastResults& results, Node* node);
//		void CastFrustum(const Frustum& frustum, CastResults& results, Node* node, bool addInside);
//		void SelfQuery(Node* node, QueryResults& QList);
//		void SelfQuery(Node* nodeA, Node* nodeB, QueryResults& QList);
//		void SplitNodes(Node* nodeA, Node* nodeB, QueryResults& QList);
//		void FilloutData(std::vector<SpatialPartitionQueryData>& results, Node* node, int depth) const;
//
//
//		//variables
//		std::map<unsigned int, Node*> map_keyData;
//		std::vector<unsigned int> keyRemove;
//
//		std::vector<std::vector<Node>> m_NodeStorage;
//
//		Node* m_Root = nullptr;
//		Node* m_Last = nullptr;
//	};
//}