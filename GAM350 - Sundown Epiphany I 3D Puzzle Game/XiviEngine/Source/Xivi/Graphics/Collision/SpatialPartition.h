/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
//#include "Shapes.h"
//#include <vector>
//
//namespace Xivi
//{
//    //-----------------------------------------------------------------------------SpatialPartitionKey
//    // This class is used for uniquely identifying (and quick finding) of any object
//    // in a spatial partition. When an object is inserted into the spatial partition
//    // this structure should be filled out with whatever information the spatial
//    // partitions needs.
//    class SpatialPartitionKey
//    {
//    public:
//        SpatialPartitionKey();
//
//        // Most spatial partitions can get away with a pointer or an integer.
//        // Use whichever makes the most sense for your spatial partition.
//        union
//        {
//            void* mVoidKey;
//            unsigned int mUIntKey;
//        };
//    };
//
//    //-----------------------------------------------------------------------------SpatialPartitionData
//    // Data that must be passed to a spatial partition to insert/update an object.
//    class SpatialPartitionData
//    {
//    public:
//        SpatialPartitionData();
//        SpatialPartitionData(void* clientData, const Aabb& aabb);
//        SpatialPartitionData(void* clientData, const Sphere& sphere);
//
//        // Comparison function used for unit testing
//        bool operator<(const SpatialPartitionData& rhs) const;
//
//        // Some data that the user wants to get back when querying the spatial partition.
//        // This project uses the model component's pointer for broadphases.
//        void* mClientData;
//
//        // The bounding aabb of the client data.
//        Aabb mAabb;
//        // The bounding sphere of the client data.
//        Sphere mBoundingSphere;
//    };
//
//    //-----------------------------------------------------------------------------SpatialPartitionQueryData
//    // Data returned from a spatial partition's FilloutData. Used to specify data in the
//    // spatial partition and a depth (mainly for trees) to verify the structure.
//    class SpatialPartitionQueryData : public SpatialPartitionData
//    {
//    public:
//        SpatialPartitionQueryData();
//        SpatialPartitionQueryData(const SpatialPartitionData& base);
//
//        // The depth of this item in the spatial partition (tree depth, etc..). If this doesn't make sense just set to 0.
//        int mDepth;
//    };
//
//    //-----------------------------------------------------------------------------CastResult
//    // The results from a cast through a spatial partition.
//    class CastResult
//    {
//    public:
//        CastResult();
//        explicit CastResult(void* clientData);
//        CastResult(void* clientData, float time);
//
//        // Comparison function used for unit testing
//        bool operator<(const CastResult& rhs) const;
//
//        // What time the object was hit in a raycast. In a frustum cast this left alone.
//        float mTime;
//        // The object that was hit from the cast (so the user can check the original object).
//        void* mClientData;
//    };
//
//    //-----------------------------------------------------------------------------CastResults
//    // A collection of cast results so that a spatial partition can return multiple hit objects.
//    class CastResults
//    {
//    public:
//        // Adds the result and sorts it based upon t so that all
//        // results are in proper lower t first order.
//        void AddResult(const CastResult& result);
//
//        typedef std::vector<CastResult> Results;
//        Results mResults;
//    };
//
//    //-----------------------------------------------------------------------------QueryResult
//    // A result from self query (a pair of objects)
//    class QueryResult
//    {
//    public:
//        QueryResult(void* clientData0, void* clientData1);
//
//        // Comparison functions used for unit testing
//        bool operator==(const QueryResult& rhs) const;
//        bool operator<(const QueryResult& rhs) const;
//
//        void* mClientData0;
//        void* mClientData1;
//    };
//
//    //-----------------------------------------------------------------------------QueryResults
//    // A collection of query results. These are not sorted in any specific order.
//    class QueryResults
//    {
//    public:
//        void AddResult(const QueryResult& result);
//
//        typedef std::vector<QueryResult> Results;
//        Results mResults;
//    };
//
//    namespace SpatialPartitionTypes
//    {
//        enum Types { NSquared, NSquaredSphere, AabbTree, Unknown };
//    }
//
//    //-----------------------------------------------------------------------------SpatialPartition
//    class SpatialPartition
//    {
//    public:
//        SpatialPartition() { mType = SpatialPartitionTypes::Unknown; }
//        virtual ~SpatialPartition() {};
//
//        // Insert the given data into the spatial partition and sets the key to
//        // be whatever data is needed to efficiently find this object for subsequent updates/removals
//        virtual void InsertData(SpatialPartitionKey& key, SpatialPartitionData& data) = 0;
//        // Update the object represented by the key with the new data. This happens when an object moves.
//        virtual void UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data) = 0;
//        // Remove the object represented by the key from this spatial partition.
//        virtual void RemoveData(SpatialPartitionKey& key) = 0;
//
//        // Debug draw this spatial partition with a transform. Level of -1 means draw the entire spatial partition.
//        // Otherwise the level signifies which level (height) of the tree to draw.
//        // The transform should be applied to any shape you draw (for mid-phase debug drawing). You should set the color and
//        // bit-mask on the debug shape returned from a debug drawing operation using ".Color(float4)" and ".MaskBit(size_t)".
//        // The mask bit is just a helper to allow run-time toggling of debug shapes.
//        virtual void DebugDraw(int level, const Math::Matrix4& transform, const Math::Vector4& color = Math::Vector4(1), int bitMask = 0) = 0;
//
//        // Finds out what objects in the spatial partition are hit by the given ray.
//        // Inserts all hits into the cast results (which sorts them by t automatically).
//        virtual void CastRay(const Ray& ray, CastResults& results) = 0;
//        // Finds out what objects hit the frustum. This test is expected to be a
//        // bit loose (as accurate frustum tests can be a bit expensive).
//        // Also the CastResult's time should be set to 0.
//        virtual void CastFrustum(const Frustum& frustum, CastResults& results) = 0;
//
//        // Returns pairs of all objects that are overlapping in this spatial partition.
//        // This represents what physics might do to determine overlapping pairs.
//        virtual void SelfQuery(QueryResults& results) = 0;
//
//        // Debug Interface
//        virtual void GetDataFromKey(const SpatialPartitionKey& key, SpatialPartitionData& data) const {};
//        // Fill out all contained data (whichever is relevant between sphere and aabb). If this is a tree then it should fill out the data in a pre-order depth first traversal.
//        virtual void FilloutData(std::vector<SpatialPartitionQueryData>& results) const {};
//
//        // What kind of spatial partition this is. Used for anttweakbar binding.
//        SpatialPartitionTypes::Types mType;
//    };
//}