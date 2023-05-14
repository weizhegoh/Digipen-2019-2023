///* Start Header ------------------------------------------------------
//Copyright (C) 2021 DigiPen Institute of Technology.
//File Name: <SimpleNSquared.cpp>
//Purpose: <To specify the objectives of the assignment>
//Language: <C++17, Visual Studio>
//Platform: <Visual Studio 2019, Windows 10>
//Project: <weiliangkenric.tan_CS350_2>
//Author: <Kenric Tan Wei Liang, weiliangkenric.tan, 390001119>
//Creation date: 23 February 2021
//End Header -------------------------------------------------------*/
//
//
//#include "pch.h"
//#include "SimpleNSquared.h"
//#include "Geometry.h"
//#include "DebugDraw.h"
//#include "../Shape/Shape.h"
//
//namespace Xivi
//{
//	//-----------------------------------------------------------------------------NSquaredSpatialPartition
//	NSquaredSpatialPartition::NSquaredSpatialPartition()
//	{
//		mType = SpatialPartitionTypes::NSquared;
//	}
//
//	void NSquaredSpatialPartition::InsertData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		// Doing this lazily (and bad, but it's n-squared...).
//		// Just store as the key what the client data is so we can look it up later.
//		key.mVoidKey = data.mClientData;
//		mData.push_back(data.mClientData);
//	}
//
//	void NSquaredSpatialPartition::UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		// Nothing to do here, update doesn't do anything
//	}
//
//	void NSquaredSpatialPartition::RemoveData(SpatialPartitionKey& key)
//	{
//		// Find the key data and remove it
//		for (size_t i = 0; i < mData.size(); ++i)
//		{
//			if (mData[i] == key.mVoidKey)
//			{
//				mData[i] = mData.back();
//				mData.pop_back();
//				break;
//			}
//		}
//	}
//
//	void NSquaredSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color, int bitMask)
//	{
//		// Nothing to debug draw
//		(void)level;
//		(void)transform;
//		(void)color;
//		(void)bitMask;
//	}
//
//	void NSquaredSpatialPartition::CastRay(const Ray& ray, CastResults& results)
//	{
//		(void)ray;
//		// Add everything
//		for (size_t i = 0; i < mData.size(); ++i)
//		{
//			CastResult result;
//			result.mClientData = mData[i];
//			results.AddResult(result);
//		}
//	}
//
//	void NSquaredSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
//	{
//		(void)frustum;
//		// Add everything
//		for (size_t i = 0; i < mData.size(); ++i)
//		{
//			CastResult result;
//			result.mClientData = mData[i];
//			results.AddResult(result);
//		}
//	}
//
//	void NSquaredSpatialPartition::SelfQuery(QueryResults& results)
//	{
//		// Add everything
//		for (size_t i = 0; i < mData.size(); ++i)
//		{
//			for (size_t j = i + 1; j < mData.size(); ++j)
//			{
//				results.AddResult(QueryResult(mData[i], mData[j]));
//			}
//		}
//	}
//
//	void NSquaredSpatialPartition::GetDataFromKey(const SpatialPartitionKey& key, SpatialPartitionData& data) const
//	{
//		data.mClientData = key.mVoidKey;
//	}
//
//	void NSquaredSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
//	{
//		for (size_t i = 0; i < mData.size(); ++i)
//		{
//			SpatialPartitionQueryData data;
//			data.mClientData = mData[i];
//			results.push_back(data);
//		}
//	}
//
//	//-----------------------------------------------------------------------------BoundingSphereSpatialPartition
//	BoundingSphereSpatialPartition::BoundingSphereSpatialPartition()
//	{
//		mType = SpatialPartitionTypes::NSquaredSphere;
//	}
//
//	void BoundingSphereSpatialPartition::InsertData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		if (keyRemove.empty())
//		{
//			key.mUIntKey = (unsigned)map_keyData.size();
//		}
//		else
//		{
//			key.mUIntKey = keyRemove.back();
//			keyRemove.pop_back();
//		}
//		map_keyData[key.mUIntKey] = data;
//	}
//
//	void BoundingSphereSpatialPartition::UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data)
//	{
//		map_keyData[key.mUIntKey] = data;
//	}
//
//	void BoundingSphereSpatialPartition::RemoveData(SpatialPartitionKey& key)
//	{
//		//removing key
//		map_keyData.erase(key.mUIntKey);
//
//		//pushing key into keyRemove
//		keyRemove.emplace_back(key.mUIntKey);
//	}
//
//	void BoundingSphereSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color, int bitMask)
//	{
//		(void)level;
//		for (auto& map : map_keyData)
//		{
//			DebugShape& shape = map.second.mBoundingSphere.DebugDraw();
//			shape.Color(color);
//			shape.SetTransform(transform);
//			shape.SetMaskBit(bitMask);
//		}
//	}
//
//	void BoundingSphereSpatialPartition::CastRay(const Ray& ray, CastResults& results)
//	{
//		for (auto& map : map_keyData)
//		{
//			float t = 0.0f;
//			auto& data = map.second;
//
//			//Cheking for collision
//			if (RaySphere(ray.mStart, ray.mDirection, data.mBoundingSphere.mCenter, data.mBoundingSphere.mRadius, t))
//			{
//				CastResult tmpCast(map.second.mClientData, t);
//				//Adding results
//				results.AddResult(tmpCast);
//			}
//		}
//	}
//
//	void BoundingSphereSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
//	{
//		for (auto& map : map_keyData)
//		{
//			auto& data = map.second;
//			size_t lastAxis = 0;
//			//checking for collision
//			if (FrustumSphere(frustum.GetPlanes(), data.mBoundingSphere.mCenter, data.mBoundingSphere.mRadius, lastAxis) != IntersectionType::Outside)
//			{
//				CastResult tmpCast(map.second.mClientData);
//				//Adding results
//				results.AddResult(tmpCast);
//			}
//		}
//	}
//
//	void BoundingSphereSpatialPartition::SelfQuery(QueryResults& results)
//	{
//		auto end = map_keyData.end();
//
//		for (auto start = map_keyData.begin(); start != end; ++start)
//		{
//			for (auto start2 = start; start2 != end; ++start2)
//			{
//				if (start == start2)
//					continue;
//
//				Sphere& sphere1 = start->second.mBoundingSphere;
//				Sphere& sphere2 = start2->second.mBoundingSphere;
//				//Check the collision
//				if (SphereSphere(sphere1.mCenter, sphere1.mRadius, sphere2.mCenter, sphere2.mRadius))
//				{
//					QueryResult result{ start->second.mClientData, start2->second.mClientData };
//					results.AddResult(result);
//				}
//			}
//		}
//	}
//
//	void BoundingSphereSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
//	{
//		for (auto& map : map_keyData)
//		{
//			results.push_back(map.second);
//		}
//	}
//}