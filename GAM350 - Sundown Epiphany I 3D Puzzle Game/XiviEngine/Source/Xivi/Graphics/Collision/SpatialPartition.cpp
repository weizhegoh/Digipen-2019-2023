/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#include "pch.h"
//#include "SpatialPartition.h"
//
//namespace Xivi
//{
//    //-----------------------------------------------------------------------------SpatialPartitionKey
//    SpatialPartitionKey::SpatialPartitionKey()
//    {
//        mVoidKey = NULL;
//    }
//
//    //-----------------------------------------------------------------------------SpatialPartitionData
//    SpatialPartitionData::SpatialPartitionData()
//    {
//        mClientData = NULL;
//    }
//
//    SpatialPartitionData::SpatialPartitionData(void* clientData, const Aabb& aabb)
//    {
//        mClientData = clientData;
//        mAabb = aabb;
//    }
//
//    SpatialPartitionData::SpatialPartitionData(void* clientData, const Sphere& sphere)
//    {
//        mClientData = clientData;
//        mBoundingSphere = sphere;
//    }
//
//    bool SpatialPartitionData::operator<(const SpatialPartitionData& rhs) const
//    {
//        return mClientData < rhs.mClientData;
//    }
//
//    //-----------------------------------------------------------------------------CastResults
//    SpatialPartitionQueryData::SpatialPartitionQueryData()
//    {
//        mDepth = 0;
//    }
//
//    SpatialPartitionQueryData::SpatialPartitionQueryData(const SpatialPartitionData& base) : SpatialPartitionData(base)
//    {
//        mDepth = 0;
//    }
//
//    //-----------------------------------------------------------------------------CastResults
//    CastResult::CastResult()
//    {
//        mTime = -1;
//        mClientData = nullptr;
//    }
//
//    CastResult::CastResult(void* clientData)
//    {
//        mClientData = clientData;
//        mTime = -1;
//    }
//
//    CastResult::CastResult(void* clientData, float time)
//    {
//        mClientData = clientData;
//        mTime = time;
//    }
//
//    bool CastResult::operator<(const CastResult& rhs) const
//    {
//        // Sort first by time and then resolve conflicts by the client data
//        if (mTime == rhs.mTime)
//            return mClientData < rhs.mClientData;
//        return mTime < rhs.mTime;
//    }
//
//    //-----------------------------------------------------------------------------CastResults
//    void CastResults::AddResult(const CastResult& result)
//    {
//        mResults.push_back(result);
//
//        // As long as the result before has a larger T value then this new item should be before it
//        size_t index = mResults.size() - 1;
//        while (index > 0 && mResults[index - 1].mTime > result.mTime)
//        {
//            mResults[index] = mResults[index - 1];
//            --index;
//        }
//
//        mResults[index] = result;
//    }
//
//    //-----------------------------------------------------------------------------QueryResult
//    QueryResult::QueryResult(void* clientData0, void* clientData1)
//    {
//        // Verify a consistent order for pairs, mainly for unit tests
//        if (clientData0 < clientData1)
//        {
//            mClientData0 = clientData0;
//            mClientData1 = clientData1;
//        }
//        else
//        {
//            mClientData0 = clientData1;
//            mClientData1 = clientData0;
//        }
//    }
//
//    bool QueryResult::operator==(const QueryResult& rhs) const
//    {
//        return mClientData0 == rhs.mClientData0 && mClientData1 == rhs.mClientData1;
//    }
//
//    bool QueryResult::operator<(const QueryResult& rhs) const
//    {
//        // Lexicographic sort by the Data0 then Data1
//        if (mClientData0 == rhs.mClientData0)
//            return mClientData1 < rhs.mClientData1;
//        return mClientData0 < rhs.mClientData0;
//    }
//
//    //-----------------------------------------------------------------------------QueryResults
//    void QueryResults::AddResult(const QueryResult& result)
//    {
//        mResults.push_back(result);
//    }
//}