/////////////////////////////////////////////////////////////////////////////////
/////
///// Feel free to reorganize your application's logic
///// This is nothing more than an incredibly simple framework to get you started
/////
///// Authors: Trevor Sundberg
///// Copyright 2013, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
//// Includes
//#include "GraphicPCH.h"
//
//class Application;
//class Level
//{
//public:
//  virtual ~Level() {};
//  virtual void Load(Application* application) = 0;
//  virtual std::string GetName() const = 0;
//};
//
//// Simple statistics of frame-rate, how many collision tests, etc... are run both for you to
//// use as a sanity check and for me to use for unit tests/drivers.
//struct Statistics
//{
//  Statistics();
//  void Clear();
//
//  void DisplayProperties(TwBar* bar);
//
//  float mFps;
//
//  size_t mRayPlaneTests;
//  size_t mRayTriangleTests;
//  size_t mRayAabbTests;
//  size_t mRaySphereTests;
//  size_t mPlaneTriangleTests;
//  size_t mPlaneSphereTests;
//  size_t mPlaneAabbTests;
//  size_t mFrustumTriangleTests;
//  size_t mFrustumSphereTests;
//  size_t mFrustumAabbTests;
//  size_t mAabbAabbTests;
//  size_t mSphereSphereTests;
//
//  // The number of object pairs that made it through broad phase.
//  // Basically how many pairs would normally go to narrow-phase (collision detection).
//  size_t mSelfCollisionsCount;
//};
//
//namespace BoundingSphereType
//{
//  enum {Centroid, RitterSphere};
//  static const char* Names[3] = {"Centroid", "RitterSphere"};
//}//namespace BoundingSphereType
//
//// The main flow behind your application
//class Application
//{
//public:
//  // Constructor
//  Application();
//
//  // Destructor
//  ~Application();
//
//  std::string GetApplicationDirectory();
//  void CreateCubeMesh();
//  void CreateCylinderMesh();
//  void LoadMesh(const std::string& path, const std::string& meshName, const std::string& ext);
//  void LoadDataFiles();
//  void LoadMeshes();
//
//  Ray GetRayFromScreenCoords(const
//
//
//
//
// ::Vector2& screenPos);
//
//  // This function gets called every frame
//  void Update(float frameTime);
//
//  int GetAssignmentNumber();
//  void SetAssignmentNumber(const int& assignmentNumber);
//
//  int GetBroadphaseType();
//  void SetBroadphaseType(const int& type);
//
//  // What kind of computation is used to compute the bounding sphere.
//  int GetBoundingSphereType();
//  void SetBoundingSphereType(const int& type);
//
//  // What level we are currently running.
//  int GetLevel();
//  void SetLevel(const int& levelNumber);
//
//  // Draw the application in its current state
//  void Draw();
//
//  // Reshape our projection matrix since the window size changed
//  void Reshape(int width, int height, float aspectRatio);
//
//  GameObject* CreateObject(const std::string& name, Mesh* mesh, const Vector3& scale, const Math::Quaternion& rotation, const Vector3& translation);
//  GameObject* CreateEmptyObject(const std::string& name);
//
//  void AddGameObject(GameObject* gameObject);
//  void UpdateGameObject(GameObject* gameObject);
//  void DestroyGameObject(GameObject* gameObject);
//
//  void DisplayCastResult(GameObject* gameObject);
//  void DisplayCastResults(const Ray& worldRay, CastResults& results);
//  void DisplayCastResults(const Frustum& worldFrustum, CastResults& results);
//  void CastRay(Ray& worldRay, CastResults& results);
//  void CastRay(Ray& worldRay);
//  void CastFrustum(Frustum& worldFrustum);
//
//  void FindPotentialIntersections(GameObject* gameObject, std::vector<GameObject*>& hitObjects);
//  void Union(GameObject* gameObjectA, GameObject* gameObjectB);
//  void Intersection(GameObject* gameObjectA, GameObject* gameObjectB);
//  // Sets B = B - A
//  void Subtract(GameObject* gameObjectA, GameObject* gameObjectB);
//  void Union(GameObject* gameObject);
//  void Intersection(GameObject* gameObject);
//  void Subtract(GameObject* gameObject);
//
//  Frustum BuildFrustum(const Vector2& start, const Vector2& end);
//
//  void OnKeyDown(unsigned int key, int x, int y);
//  void OnKeyUp(unsigned int key, int x, int y);
//  void OnMouseInput(unsigned int button, bool isDown, int x, int y);
//  void OnMouseMove(int x, int y);
//  void OnMouseScroll(int x, int y);
//  void OnCreateCube();
//
//private:
//public:
//  std::vector<GameObject*> mGameObjects;
//
//  SpatialPartition* mDynamicBroadphase;
//
//  // The ui that represents our application
//  TwBar* mBar;
//  TwBar* mSelectionBar;
//  TwBar* mStatisticsBar;
//
//  Vector3 mLightPosition;
//  Vector4 mLightAmbient;
//  Vector4 mLightDiffuse;
//  Vector4 mLightSpecular;
//
//  Camera mCamera;
//
//  int mAssignmentNumber;
//  Vector2 mSize;
//  int mDynamicDebugDrawLevel;
//  int mDebuggingIndex;
//  bool mDragging;
//  bool mMouseDown;
//  bool mRefineCasts;
//  Math::Vector2 mStart;
//  Math::Vector2 mEnd;
//
//  std::vector<Mesh*> mMeshes;
//  std::vector<Gizmo*> mGizmos;
//  Gizmo* mActiveGizmo;
//
//  //std::vector<UnitTest*> mTests;
//  std::vector<Level*> mLevels;
//
//  TwType mMeshTypesEnum;
//  TwType mLevelTypesEnum;
//  TwType mBoundingSphereTypeEnum;
//  int mBoundSphereType;
//
//  int mMaxIterations;
//  bool mDebugDraw;
//  bool mDrawGjk;
//  bool mRunGjk;
//  int mCurrentLevelIndex;
//  void ChangeLevel(int levelIndex);
//
//  static Statistics mStatistics;
//  bool mFrustumCull;
//};
