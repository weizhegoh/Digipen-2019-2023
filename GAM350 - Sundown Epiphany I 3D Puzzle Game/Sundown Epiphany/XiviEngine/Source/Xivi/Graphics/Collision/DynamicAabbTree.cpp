///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <DynamicAabbTree.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_3>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 14 March 2021
//End Header -------------------------------------------------------*/
//
//#include "pch.h"
//#include "DebugDraw.h"
//#include "DynamicAabbTree.h"
//#include "Geometry.h"
//#include "SimpleNSquared.h"
//
//namespace Xivi
//{
//
//	const float DynamicAabbTree::mFatteningFactor = 1.1f;
//
//	DynamicAabbTree::DynamicAabbTree()
//	{
//		mType = SpatialPartitionTypes::AabbTree;
//	}
//
//	DynamicAabbTree::~DynamicAabbTree()
//	{
//		m_NodeStorage.clear();
//	}
//
//	void DynamicAabbTree::InsertData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		Node* newNode = nullptr;
//
//		if (keyRemove.empty())
//			key.mUIntKey = (unsigned)map_keyData.size();
//		else
//		{
//			key.mUIntKey = keyRemove.back();
//			keyRemove.pop_back();
//		}
//
//		newNode = GetNewNode();
//
//		newNode->m_Aabb = data.mAabb;
//		Xivi::Math::Vector3 newNode_vec = newNode->m_Aabb.GetHalfSize();
//		newNode_vec = newNode_vec * mFatteningFactor;
//		newNode->m_Aabb = Aabb::BuildFromCenterAndHalfExtents(newNode->m_Aabb.GetCenter(), newNode_vec);
//		newNode->m_ClientData = data.mClientData;
//
//		map_keyData[key.mUIntKey] = newNode;
//		insert_Rec(newNode, m_Root, key, data);
//	}
//
//	void DynamicAabbTree::UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		//find if key exist and key data does not contain data
//		if (!map_keyData.find(key.mUIntKey)->second->m_Aabb.Contains(data.mAabb))
//		{
//			RemoveData(key);
//			InsertData(key, data);
//		}
//	}
//
//	void DynamicAabbTree::RemoveData(SpatialPartitionKey& key)
//	{
//		auto found = map_keyData.find(key.mUIntKey);
//		if (found == map_keyData.end())
//			return;
//
//		//key to be removed
//		auto selection = found->second;
//		Node* parent = selection->m_Parent;
//		if (parent == nullptr)
//		{
//			//if the node for deletion's parent is nullptr, it shows that
//			//the node to be deleted is the root node, hence remove the root node
//			m_Root = nullptr;
//			RemoveDataKey(key);
//			return;
//		}
//
//		Node* grandParent = parent->m_Parent;
//		bool isLeftDir = (parent->m_Left == selection);
//
//		// if parent is left
//		if (isLeftDir)
//		{
//			if (grandParent == nullptr)
//			{
//				m_Root = parent->m_Right;
//				parent->m_Right->m_Parent = nullptr;
//			}
//			else if (grandParent->m_Left == parent)
//			{
//				parent->m_Parent->m_Left = parent->m_Right;
//				parent->m_Right->m_Parent = parent->m_Parent;
//			}
//			else
//			{
//				parent->m_Parent->m_Right = parent->m_Right;
//				parent->m_Right->m_Parent = parent->m_Parent;
//			}
//		}
//		// if parent is right
//		else
//		{
//			if (grandParent == nullptr)
//			{
//				m_Root = parent->m_Left;
//				parent->m_Left->m_Parent = nullptr;
//			}
//			else if (grandParent->m_Left == parent)
//			{
//				parent->m_Parent->m_Left = parent->m_Left;
//				parent->m_Left->m_Parent = parent->m_Parent;
//			}
//			else
//			{
//				parent->m_Parent->m_Right = parent->m_Left;
//				parent->m_Left->m_Parent = parent->m_Parent;
//			}
//		}
//
//		if (parent)
//		{
//			//balance, update height and update AABB
//			UpdateUpwards(parent->m_Parent);
//			freeNode(parent);
//		}
//
//		freeNode(selection);
//		RemoveDataKey(key);
//	}
//
//	void DynamicAabbTree::DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color, int bitMask)
//	{
//		DebugDraw(level, transform, color, bitMask, m_Root, 0);
//	}
//
//	void DynamicAabbTree::CastRay(const Ray& ray, CastResults& results)
//	{
//		CastRay(ray, results, m_Root);
//	}
//
//	void DynamicAabbTree::CastFrustum(const Frustum& frustum, CastResults& results)
//	{
//		if (m_Root)
//			CastFrustum(frustum, results, m_Root, false);
//	}
//
//	void DynamicAabbTree::SelfQuery(QueryResults& results)
//	{
//		SelfQuery(m_Root, results);
//	}
//
//	void DynamicAabbTree::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
//	{
//		FilloutData(results, m_Root, 0);
//	}
//
//	DynamicAabbTree::Node* DynamicAabbTree::GetNewNode()
//	{
//		size_t allocateSize = 96;
//
//		if (!m_Last)
//		{
//			m_NodeStorage.emplace_back(std::vector<Node>(allocateSize));
//			auto nodeArray = m_NodeStorage.back().data();
//			auto& node = m_Last;
//
//			for (size_t i = 0; i < allocateSize; ++i)
//			{
//				nodeArray[i].m_Parent = node;
//				node = nodeArray + i;
//			}
//		}
//
//		Node* newNode = m_Last;
//		m_Last = m_Last->m_Parent;
//		new(newNode) Node;
//		return newNode;
//	}
//
//	void DynamicAabbTree::insert_Rec(Node* insertingNode, Node* node, SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		if (!node)
//		{
//			m_Root = insertingNode;
//		}
//		else if (node->m_Right && node->m_Left)
//		{
//			//selecting the correct insertion position
//			Node* selection = surfaceArea_Selection(insertingNode, node->m_Left, node->m_Right);
//			insert_Rec(insertingNode, selection, key, data);
//		}
//		// if it is a leaf node
//		else
//		{
//			Node* parent = GetNewNode();
//			//if the node to be added, parent is nullptr, it shows that
//			//the node is the root node
//			if (node->m_Parent == nullptr)
//				m_Root = parent;
//			else
//			{
//				parent->m_Parent = node->m_Parent;
//
//				//promoting parent node
//				if (node->m_Parent->m_Left == node)
//				{
//					node->m_Parent->m_Left = parent;
//				}
//				else if (node->m_Parent->m_Right == node)
//				{
//					node->m_Parent->m_Right = parent;
//				}
//			}
//
//			//inserting values into parent node
//			parent->m_Left = node;
//			parent->m_Right = insertingNode;
//
//			parent->m_Left->m_Parent = parent;
//			parent->m_Right->m_Parent = parent;
//
//			//set child nodes to 0 height
//			parent->m_Left->m_Height = 0;
//			parent->m_Right->m_Height = 0;
//
//			//set promoted parent node to 1 height
//			parent->m_Height = 1;
//
//			//balance , update height and update aabb when inserted new node
//			UpdateUpwards(node);
//		}
//	}
//
//	DynamicAabbTree::Node* DynamicAabbTree::surfaceArea_Selection(Node* insertingNode, Node* left, Node* right)
//	{
//		//combining the left and right node with the inserting node
//		Aabb leftInserting = Aabb::Combine(insertingNode->m_Aabb, left->m_Aabb);
//		Aabb rightInserting = Aabb::Combine(insertingNode->m_Aabb, right->m_Aabb);
//
//		float left_inc = std::abs(leftInserting.GetSurfaceArea() - left->m_Aabb.GetSurfaceArea());
//		float right_inc = std::abs(rightInserting.GetSurfaceArea() - right->m_Aabb.GetSurfaceArea());
//
//		//find which node to select for insertion
//		return (left_inc < right_inc) ? left : right;
//	}
//
//	void DynamicAabbTree::Balance(Node* node)
//	{
//		if (node->m_Left != nullptr && node->m_Right != nullptr)
//		{
//			int heightDiff = std::abs(node->m_Left->m_Height - node->m_Right->m_Height);
//			if (heightDiff > 1)
//			{
//
//				//1. identify pivot and small children
//				Node* pivot = (node->m_Left->m_Height) > (node->m_Right->m_Height) ? node->m_Left : node->m_Right;
//				Node* smallChild = (pivot->m_Right->m_Height > pivot->m_Left->m_Height) ? pivot->m_Left : pivot->m_Right;
//				Node* oldParent = pivot->m_Parent;
//
//				//2. detach small child from pivot and pivot from the old parent
//
//				if (pivot->m_Right->m_Height > pivot->m_Left->m_Height)
//					pivot->m_Left = oldParent;
//				else
//					pivot->m_Right = oldParent;
//
//				smallChild->m_Parent = nullptr;
//
//				//3. Replace the grand-parent connection of the old parent with the pivot
//				if (oldParent->m_Parent)
//				{
//					pivot->m_Parent = oldParent->m_Parent;
//					oldParent->m_Parent = pivot;
//
//					if (pivot->m_Parent->m_Left == oldParent)
//						pivot->m_Parent->m_Left = pivot;
//					else
//						pivot->m_Parent->m_Right = pivot;
//				}
//				else
//				{
//					pivot->m_Parent = nullptr;
//					m_Root = pivot;
//					oldParent->m_Parent = pivot;
//				}
//
//				//4. Attach the old parent under the pivot where the small child was
//				if (node->m_Right->m_Height > node->m_Left->m_Height)
//					oldParent->m_Right = smallChild;
//				else
//					oldParent->m_Left = smallChild;
//				smallChild->m_Parent = oldParent;
//
//				CalculateHeight(oldParent);
//				CalculateHeight(pivot);
//
//				oldParent->m_Aabb = Aabb::Combine(oldParent->m_Right->m_Aabb, oldParent->m_Left->m_Aabb);
//				pivot->m_Aabb = Aabb::Combine(pivot->m_Right->m_Aabb, pivot->m_Left->m_Aabb);
//
//				CalculateHeight(node);
//			}
//		}
//	}
//
//	void DynamicAabbTree::CalculateHeight(Node* node)
//	{
//		if (node)
//		{
//			Node* right = node->m_Right;
//			Node* left = node->m_Left;
//			if (right && left)
//				node->m_Height = std::max(right->m_Height, left->m_Height) + 1;
//			else if (right && !left)
//				node->m_Height = node->m_Right->m_Height + 1;
//			else if (left && !right)
//				node->m_Height = node->m_Left->m_Height + 1;
//			else
//				node->m_Height = 0;
//		}
//	}
//
//	void DynamicAabbTree::UpdateUpwards(Node* node)
//	{
//		while (node != nullptr)
//		{
//			Balance(node);
//			CalculateHeight(node);
//			if (node->m_Right && node->m_Left)
//				node->m_Aabb = Aabb::Combine(node->m_Right->m_Aabb, node->m_Left->m_Aabb);
//
//			node = node->m_Parent;
//		}
//	}
//
//	void DynamicAabbTree::freeNode(Node* node)
//	{
//		new(node) Node;
//		node->m_Parent = m_Last;
//		m_Last = node;
//	}
//
//	void DynamicAabbTree::RemoveDataKey(SpatialPartitionKey& key)
//	{
//		//removing key
//		map_keyData.erase(key.mUIntKey);
//
//		//pushing key into keyRemove
//		keyRemove.emplace_back(key.mUIntKey);
//	}
//
//	void DynamicAabbTree::DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color, int bitMask, Node* node, int currDepth)
//	{
//		if (node)
//		{
//			if (currDepth == level || level == -1)
//			{
//				//tmp draw value
//				DebugShape& tmpShape = node->m_Aabb.DebugDraw();
//				tmpShape.SetTransform(transform);
//				tmpShape.Color(color);
//				tmpShape.SetMaskBit(bitMask);
//			}
//			DebugDraw(level, transform, color, bitMask, node->m_Left, currDepth + 1);
//			DebugDraw(level, transform, color, bitMask, node->m_Right, currDepth + 1);
//		}
//	}
//
//	void DynamicAabbTree::CastRay(const Ray& ray, CastResults& results, Node* node)
//	{
//		if (node)
//		{
//			float t = 0.0f;
//			if (RayAabb(ray.mStart, ray.mDirection, node->m_Aabb.GetMin(), node->m_Aabb.GetMax(), t))
//			{
//				//check whether if it is a leaf node
//				if (!node->m_Height)
//				{
//
//					CastResult tmpCast(node->m_ClientData, t);
//					//Adding results
//					results.AddResult(tmpCast);
//				}
//				else
//				{
//					CastRay(ray, results, node->m_Left);
//					CastRay(ray, results, node->m_Right);
//				}
//			}
//		}
//	}
//
//	void DynamicAabbTree::CastFrustum(const Frustum& frustum, CastResults& results, Node* node, bool addInside)
//	{
//		if (node)
//		{
//			//adding inside
//			if (addInside)
//			{
//				//if it is leaf node
//				if (!node->m_Height)
//				{
//					CastResult tmpCast(node->m_ClientData);
//					results.AddResult(tmpCast);
//				}
//				CastFrustum(frustum, results, node->m_Left, true);
//				CastFrustum(frustum, results, node->m_Right, true);
//			}
//			else
//			{
//				IntersectionType::Type intersectionType = FrustumAabb(frustum.GetPlanes(), node->m_Aabb.GetMin(), node->m_Aabb.GetMax(), node->m_LastAxis);
//
//				if (intersectionType == IntersectionType::Outside)
//					return;
//				else if (intersectionType == IntersectionType::Inside)
//					CastFrustum(frustum, results, node, true);
//				else
//				{
//					//adding outside
//					if (node->m_Right && node->m_Left)
//					{
//						CastFrustum(frustum, results, node->m_Left, addInside);
//						CastFrustum(frustum, results, node->m_Right, addInside);
//					}
//					else
//						//overlap , since dont use coplanar
//						CastFrustum(frustum, results, node, true);
//				}
//
//			}
//		}
//	}
//
//
//	void DynamicAabbTree::SelfQuery(Node* node, QueryResults& QList)
//	{
//		//return if node is leaf
//		if (!node || !node->m_Height)
//			return;
//
//		//check the two children against each other
//		SelfQuery(node->m_Left, node->m_Right, QList);
//		//recurse left and right
//		SelfQuery(node->m_Left, QList);
//		SelfQuery(node->m_Right, QList);
//
//	}
//
//	void DynamicAabbTree::SelfQuery(Node* nodeA, Node* nodeB, QueryResults& QList)
//	{
//		//return if not overlap
//		if (!AabbAabb(nodeA->m_Aabb.GetMin(), nodeA->m_Aabb.GetMax(),
//			nodeB->m_Aabb.GetMin(), nodeB->m_Aabb.GetMax()))
//			return;
//
//		//if node A and B are leaf node
//		if (!nodeA->m_Height && !nodeB->m_Height)
//		{
//			QueryResult add(nodeA->m_ClientData, nodeB->m_ClientData);
//			QList.AddResult(add);
//		}
//		else
//			//find leaf nodes if not leaf
//			SplitNodes(nodeA, nodeB, QList);
//	}
//
//	void DynamicAabbTree::SplitNodes(Node* nodeA, Node* nodeB, QueryResults& QList)
//	{
//		//if nodeA is a leaf node
//		if (!nodeA->m_Height)
//		{
//			SelfQuery(nodeA, nodeB->m_Left, QList);
//			SelfQuery(nodeA, nodeB->m_Right, QList);
//		}
//		else if (!nodeB->m_Height)
//		{
//			SelfQuery(nodeB, nodeA->m_Left, QList);
//			SelfQuery(nodeB, nodeA->m_Right, QList);
//		}
//		else
//		{
//			//split node with the largest volume (according to slides)
//			if (nodeA->m_Aabb.GetVolume() < nodeB->m_Aabb.GetVolume())
//			{
//				SelfQuery(nodeA, nodeB->m_Left, QList);
//				SelfQuery(nodeA, nodeB->m_Right, QList);
//			}
//			else
//			{
//				SelfQuery(nodeA->m_Right, nodeB, QList);
//				SelfQuery(nodeA->m_Left, nodeB, QList);
//			}
//		}
//	}
//
//	void DynamicAabbTree::FilloutData(std::vector<SpatialPartitionQueryData>& results, Node* node, int depth) const
//	{
//		if (node)
//		{
//			SpatialPartitionQueryData tmpData;
//			tmpData.mClientData = node->m_ClientData;
//			tmpData.mAabb = node->m_Aabb;
//			tmpData.mDepth = depth;
//			results.emplace_back(tmpData);
//
//			FilloutData(results, node->m_Left, depth + 1);
//			FilloutData(results, node->m_Right, depth + 1);
//		}
//	}
//}