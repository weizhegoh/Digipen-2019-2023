/////////////////////////////////////////////////////////////////////////////////
/////
///// Feel free to reorganize your application's logic
///// This is nothing more than an incredibly simple framework to get you started
/////
///// Authors: Trevor Sundberg
///// Copyright 2013, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#include "GraphicPCH.h"
//
//class Level1 : public Level
//{
//  void Load(Application* application) override
//  {
//    application->CreateObject("Cube1", application->mMeshes[0], Vector3(1.0f), Math::Quaternion::cIdentity, Vector3(0.5f, 0.5f, 0));
//    application->CreateObject("Cube2", application->mMeshes[0], Vector3(1.0f), Math::Quaternion::cIdentity, Vector3(0,-0.5f, 0));
//  }
//
//  std::string GetName() const override
//  {
//    return "Level1";
//  }
//};
//
//class BigLevel : public Level
//{
//  void Load(Application* application) override
//  {
//    size_t width = 10;
//    size_t height = 10;
//    size_t depth = 10;
//
//    Vector3 scale(0.5f);
//    Quaternion rotation = Quaternion::cIdentity;
//
//    char buffer[200];
//
//    for(size_t z = 0; z < depth; ++z)
//    {
//      for(size_t y = 0; y < height; ++y)
//      {
//        for(size_t x = 0; x < width; ++x)
//        {
//          size_t index = x + y * height + z * (height * depth);
//
//          _itoa_s(index, buffer, 10);
//          std::string objName = "Cube" + std::string(buffer);
//
//          Vector3 translation((float)x, (float)y, (float)z);
//          translation *= 4 * scale;
//          application->CreateObject(objName, application->mMeshes[0], scale, rotation, translation);
//        }
//      }
//    }
//  }
//
//  std::string GetName() const override
//  {
//    return "BigLevel";
//  }
//};
//
//Statistics::Statistics()
//{
//  mFps = 1 / 60.0f;
//  Clear();
//}
//
//void Statistics::Clear()
//{
//  mAabbAabbTests = 0;
//  mRayAabbTests = 0;
//  mSphereSphereTests = 0;
//  mRaySphereTests = 0;
//  mPlaneSphereTests = 0;
//  mPlaneAabbTests = 0;
//  mSelfCollisionsCount = 0;
//
//  mRayPlaneTests = 0;
//  mRayTriangleTests = 0;
//  mPlaneTriangleTests = 0;
//  mFrustumTriangleTests = 0;
//  mFrustumSphereTests = 0;
//  mFrustumAabbTests = 0;
//}
//
//void Statistics::DisplayProperties(TwBar* bar)
//{
//  TwAddVarRO(bar, "Fps", TW_TYPE_FLOAT, &mFps, "");
//
//  TwAddVarRO(bar, "RayPlaneTests", TW_TYPE_INT32, &mRayPlaneTests, "");
//  TwAddVarRO(bar, "RayTriangleTests", TW_TYPE_INT32, &mRayTriangleTests, "");
//  TwAddVarRO(bar, "RayAabbTests", TW_TYPE_INT32, &mRayAabbTests, "");
//  TwAddVarRO(bar, "RaySphereTests", TW_TYPE_INT32, &mRaySphereTests, "");
//  TwAddVarRO(bar, "PlaneTriangleTests", TW_TYPE_INT32, &mPlaneTriangleTests, "");
//  TwAddVarRO(bar, "PlaneSphereTests", TW_TYPE_INT32, &mPlaneSphereTests, "");
//  TwAddVarRO(bar, "PlaneAabbTests", TW_TYPE_INT32, &mPlaneAabbTests, "");
//  TwAddVarRO(bar, "FrustumTriangleTests", TW_TYPE_INT32, &mFrustumTriangleTests, "");
//  TwAddVarRO(bar, "FrustumSphereTests", TW_TYPE_INT32, &mFrustumSphereTests, "");
//  TwAddVarRO(bar, "FrustumAabbTests", TW_TYPE_INT32, &mFrustumAabbTests, "");
//  TwAddVarRO(bar, "AabbAabbTests", TW_TYPE_INT32, &mAabbAabbTests, "");
//  TwAddVarRO(bar, "SphereSphereTests", TW_TYPE_INT32, &mSphereSphereTests, "");
//  TwAddVarRO(bar, "SelfCollisions", TW_TYPE_INT32, &mSelfCollisionsCount, "");
//}
//
////-----------------------------------------------------------------------------
//Statistics Application::mStatistics = Statistics();
//
//Application::Application()
//{
//  mAssignmentNumber = 0;
//  gDebugDrawer->mApplication = this;
//  mMaxIterations = 20;
//  mLightPosition = Vector3(0, 5, 0);
//  mLightAmbient  = Vector4(1, 1, 1, 1);
//  mLightDiffuse  = Vector4(1, 1, 1, 1);
//  mLightSpecular = Vector4(1, 1, 1, 1);
//  mDragging = false;
//  mMouseDown = false;
//  mDynamicDebugDrawLevel = 0;
//  mDebuggingIndex = -1;
//  mRefineCasts = false;
//  mDebugDraw = true;
//  mDrawGjk = false;
//  mRunGjk = false;
//  mBoundSphereType = BoundingSphereType::Centroid;
//  mFrustumCull = true;
//
//  mGizmos.push_back(
// TranslationGizmo());
//  mActiveGizmo = mGizmos[0];
//
//
//  mDynamicBroadphase = new NSquaredSpatialPartition();
//
//  mSelectionBar = TwNewBar("Selection");
//  TwDefine(" Selection refresh = 0.01 size='200 255' position='0 346'");
//
//  mStatisticsBar = TwNewBar("Statistics");
//  TwDefine(" Statistics visible=false");
//
//  mBar = TwNewBar("Application");
//  TwDefine(" Application size='200 345' position='0 0'");
//
//  TwAddVarRW(mBar, "LightPosition.X", TW_TYPE_FLOAT, &mLightPosition.x, "");
//  TwAddVarRW(mBar, "LightPosition.Y", TW_TYPE_FLOAT, &mLightPosition.y, "");
//  TwAddVarRW(mBar, "LightPosition.Z", TW_TYPE_FLOAT, &mLightPosition.z, "");
//  TwAddVarRW(mBar, "LightAmbient", TW_TYPE_COLOR4F, &mLightAmbient, "");
//  TwAddVarRW(mBar, "LightDiffuse", TW_TYPE_COLOR4F, &mLightDiffuse, "");
//  TwAddVarRW(mBar, "LightSpecular", TW_TYPE_COLOR4F, &mLightSpecular, "");
//
//  mCamera.DisplayProperties(mBar);
//
//  // Add a property that is which assignment is being tested. This swaps a bunch of
//  // properties to hopefully make testing everything easier for that assignment.
//  TwType assignmentNumberType = TwDefineEnumFromString("AssignmentNumber", "1,2,3,4,5");
//  BindPropertyInGroup(mBar, Application, AssignmentNumber, int, assignmentNumberType, "");
//
//  // Bind misc. tweakables (these are auto-changed when the assignment number is changed but can be further tweaked if desired)
//  const char* miscPropertiesGroup = "group=MiscProperties";
//  // Bind what spatial partion is being used
//  TwType spatialPartitionType = TwDefineEnumFromString("SpatialPartitionType", "NSquared,NSquaredSphere,NSquaredAabb,DynamicAabbTree");
//  BindPropertyInGroup(mBar, Application, BroadphaseType, int, spatialPartitionType, miscPropertiesGroup);
//  // Bind what method of bounding sphere computation is used
//  mBoundingSphereTypeEnum = TwDefineEnumFromString("BoundingSphereType", "Centroid,Ritter,PCA");
//  BindPropertyInGroup(mBar, Application, BoundingSphereType, int, mBoundingSphereTypeEnum, miscPropertiesGroup);
//
//  // Bind other misc. properties about whether or not to perform a certain task
//  TwAddVarRW(mBar, "Debugging Index", TW_TYPE_INT32, &mDebuggingIndex, miscPropertiesGroup);
//  TwAddVarRW(mBar, "RefineCasts", TW_TYPE_BOOLCPP, &mRefineCasts, miscPropertiesGroup);
//  TwAddVarRW(mBar, "MaxIterations", TW_TYPE_INT32, &mMaxIterations, miscPropertiesGroup);
//  TwAddVarRW(mBar, "FrustumCulling", TW_TYPE_BOOLCPP, &mFrustumCull, miscPropertiesGroup);
//  TwAddVarRW(mBar, "Gjk", TW_TYPE_BOOLCPP, &mRunGjk, miscPropertiesGroup);
//  // Put all of these properties under a group that is closed by default
//  TwDefine("Application/MiscProperties label=MiscProperties opened=false");
//
//  // Bind various debug drawing properties
//  const char* debugDrawPropertiesGroup = "group=DebugDrawProperties";
//  TwAddVarRW(mBar, "DebugDraw", TW_TYPE_BOOLCPP, &mDebugDraw, debugDrawPropertiesGroup);
//  TwAddVarRW(mBar, "DrawLevel", TW_TYPE_INT32, &mDynamicDebugDrawLevel, debugDrawPropertiesGroup);
//  TwAddVarRW(mBar, "DrawGjk", TW_TYPE_BOOLCPP, &mDrawGjk, debugDrawPropertiesGroup);
//  // Put all of these properties under a group that is closed by default
//  TwDefine("Application/DebugDrawProperties label=DebugDrawProperties opened=false");
//
//  BindMethod(mBar, Application, "CreateCube", OnCreateCube, "");
//
//  mStatistics.DisplayProperties(mStatisticsBar);
//
//  LoadMeshes();
//
//  mLevels.push_back(new Level1());
//  mLevels.push_back(new BigLevel());
//
//  // Add all of the unit test levels
//  for(size_t i = 0; i < mTestFns.size(); ++i)
//  {
//    for(size_t j = 0; j < mTestFns[i].size(); ++j)
//    {
//      UnitTestWrapper& wrapper = mTestFns[i][j];
//      mLevels.push_back(new SimpleUnitTestLevel(wrapper.mTestName, wrapper.mFn));
//    }
//  }
//
//  std::string levels;
//  for(size_t i = 0; i < mLevels.size(); ++i)
//    levels += mLevels[i]->GetName() + ",";
//  mLevelTypesEnum = TwDefineEnumFromString("Level", levels.c_str());
//
//  BindPropertyInGroup(mBar, Application, Level, int, mLevelTypesEnum, "");
//
//  ChangeLevel(0);
//
//  SetAssignmentNumber(0);
//}
//
////-----------------------------------------------------------------------------
//Application::~Application()
//{
//  delete mDynamicBroadphase;
//  for(size_t i = 0; i < mLevels.size(); ++i)
//    delete mLevels[i];
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//    delete mGameObjects[i];
//  for(size_t i = 0; i < mMeshes.size(); ++i)
//    delete mMeshes[i];
//
//  for(size_t i = 0; i < mGizmos.size(); ++i)
//    delete mGizmos[i];
//  delete gDebugDrawer;
//}
//
//std::string Application::GetApplicationDirectory()
//{
//  char temp[MAX_PATH+1];
//  GetModuleFileName(NULL, temp, MAX_PATH);
//  std::string fileName = std::string(temp);
//
//  size_t index = fileName.find_last_of('\\');
//  if(index == std::string::npos)
//    return std::string();
//
//  std::string applicationDir = fileName.substr(0, index);
//  return applicationDir;
//}
//
//void Application::CreateCubeMesh()
//{
//  Vector3 rub(+1, +1, -1);
//  Vector3 lub(-1, +1, -1);
//  Vector3 rdb(+1, -1, -1);
//  Vector3 ldb(-1, -1, -1);
//  Vector3 ruf(+1, +1, +1);
//  Vector3 luf(-1, +1, +1);
//  Vector3 rdf(+1, -1, +1);
//  Vector3 ldf(-1, -1, +1);
//
//  Mesh* cubeMesh = new Mesh();
//  cubeMesh->mVertices.push_back(rub);//0
//  cubeMesh->mVertices.push_back(lub);//1
//  cubeMesh->mVertices.push_back(rdb);//2
//  cubeMesh->mVertices.push_back(ldb);//3
//  cubeMesh->mVertices.push_back(ruf);//4
//  cubeMesh->mVertices.push_back(luf);//5
//  cubeMesh->mVertices.push_back(rdf);//6
//  cubeMesh->mVertices.push_back(ldf);//7
//  cubeMesh->mIndices.push_back(5); cubeMesh->mIndices.push_back(7); cubeMesh->mIndices.push_back(6);
//  cubeMesh->mIndices.push_back(5); cubeMesh->mIndices.push_back(6); cubeMesh->mIndices.push_back(4);
//  cubeMesh->mIndices.push_back(0); cubeMesh->mIndices.push_back(2); cubeMesh->mIndices.push_back(3);
//  cubeMesh->mIndices.push_back(0); cubeMesh->mIndices.push_back(3); cubeMesh->mIndices.push_back(1);
//  cubeMesh->mIndices.push_back(1); cubeMesh->mIndices.push_back(3); cubeMesh->mIndices.push_back(7);
//  cubeMesh->mIndices.push_back(1); cubeMesh->mIndices.push_back(7); cubeMesh->mIndices.push_back(5);
//  cubeMesh->mIndices.push_back(2); cubeMesh->mIndices.push_back(0); cubeMesh->mIndices.push_back(6);
//  cubeMesh->mIndices.push_back(6); cubeMesh->mIndices.push_back(0); cubeMesh->mIndices.push_back(4);
//  cubeMesh->mIndices.push_back(1); cubeMesh->mIndices.push_back(5); cubeMesh->mIndices.push_back(4);
//  cubeMesh->mIndices.push_back(1); cubeMesh->mIndices.push_back(4); cubeMesh->mIndices.push_back(0);
//  cubeMesh->mIndices.push_back(7); cubeMesh->mIndices.push_back(3); cubeMesh->mIndices.push_back(6);
//  cubeMesh->mIndices.push_back(6); cubeMesh->mIndices.push_back(3); cubeMesh->mIndices.push_back(2);
//  cubeMesh->mType = mMeshes.size();
//  cubeMesh->mName = "Cube";
//
//  mMeshes.push_back(cubeMesh);
//}
//
//void Application::CreateCylinderMesh()
//{
//  Mesh* mesh = new Mesh();
//
//  size_t slices = 12;
//  float height = 2;
//
//  mesh->mVertices.resize(2 + slices * 2);
//  mesh->mVertices[0] = Vector3(0, height, 0);
//  mesh->mVertices[1] = Vector3(0,-height, 0);
//  for(size_t i = 0; i < slices;  ++i)
//  {
//    float radians = (Xivi::Math::s_PI * 2) * (i / (float)slices);
//
//    float sin = Math::Sin(radians);
//    float cos = Math::Cos(radians);
//
//    mesh->mVertices[2 + i] = Vector3(sin, height, cos);
//    mesh->mVertices[2 + i + slices] = Vector3(sin,-height, cos);
//  }
//
//
//  for(size_t i = 0; i < slices;  ++i)
//  {
//    size_t tl = i + 2;
//    size_t tr = ((i + 1) % slices) + 2;
//    size_t bl = i + 2 + slices;
//    size_t br = ((i + 1) % slices) + 2 + slices;
//
//    mesh->mIndices.push_back(0);
//    mesh->mIndices.push_back(tl);
//    mesh->mIndices.push_back(tr);
//
//    mesh->mIndices.push_back(1);
//    mesh->mIndices.push_back(br);
//    mesh->mIndices.push_back(bl);
//
//    mesh->mIndices.push_back(tl);
//    mesh->mIndices.push_back(bl);
//    mesh->mIndices.push_back(br);
//
//    mesh->mIndices.push_back(tl);
//    mesh->mIndices.push_back(br);
//    mesh->mIndices.push_back(tr);
//  }
//  mesh->mType = mMeshes.size();
//  mesh->mName = "Cylinder";
//
//  mMeshes.push_back(mesh);
//}
//
//void Application::LoadMesh(const std::string& path, const std::string& meshName, const std::string& ext)
//{
//  const size_t bufferSize = 1000;
//  char buffer[bufferSize];
//  size_t size;
//
//  std::string fileData;
//  std::string fileName = (path + "\\" + meshName + ext);
//  FILE* file;
//  fopen_s(&file, fileName.c_str(), "r");
//  if(file == NULL)
//    return;
//
//  do
//  {
//    size = fread_s(buffer, bufferSize - 1, 1, bufferSize - 1, file);
//    buffer[size] = NULL;
//    fileData += buffer;
//  }while(size != 0);
//
//  Mesh* mesh = new Mesh();
//  mesh->mName = meshName;
//  mesh->mType = mMeshes.size();
//  Helpers::LoadObjFile(fileData, mesh->mVertices, mesh->mIndices);
//  mMeshes.push_back(mesh);
//
//  fclose(file);
//}
//
//void Application::LoadDataFiles()
//{
//  char path[MAX_PATH];
//  HANDLE handle;
//  WIN32_FIND_DATA findData;
//
//  std::string appDir = GetApplicationDirectory();
//  size_t size = appDir.size();
//  strncpy_s(path, MAX_PATH, appDir.c_str(), appDir.size());
//
//  // Check for trailing slash and add if not there
//  if(path[size-1] != '\\')
//    strcat_s(path, MAX_PATH, "\\");
//
//  // Add the wildcard to get all files in directory
//  strcat_s(path, MAX_PATH, "*");
//
//  handle = FindFirstFile(path, &findData);
//
//  if(handle == INVALID_HANDLE_VALUE)
//    return;
//
//  do
//  {
//    //Skip rid of "." and ".." directory results.
//    if(strcmp(findData.cFileName,".") == 0)
//      continue;
//    if(handle && strcmp(findData.cFileName,"..") == 0)
//      continue;
//
//    std::string filePath = findData.cFileName;
//    size_t index = filePath.find_last_of('.');
//    if(index == std::string::npos)
//      continue;
//
//    std::string ext = filePath.substr(index, filePath.size() - index);
//    if(ext != ".txt")
//      continue;
//
//
//    //the file name already doesn't contain the path
//    std::string fileName = filePath.substr(0, index);
//    LoadMesh(appDir, fileName, ext);
//
//  }while(FindNextFile(handle, &findData) != 0);
//}
//
//void Application::LoadMeshes()
//{
//  CreateCubeMesh();
//  CreateCylinderMesh();
//  LoadDataFiles();
//
//
//  std::string meshTypes;
//  for(size_t i = 0; i < mMeshes.size(); ++i)
//    meshTypes += mMeshes[i]->mName + ",";
//  mMeshTypesEnum = TwDefineEnumFromString("MeshType", meshTypes.c_str());
//}
//
//Ray Application::GetRayFromScreenCoords(const Math::Vector2& screenPos)
//{
//  GLint viewport[4];
//  GLdouble modelview[16];
//  GLdouble projection[16];
//  glGetIntegerv(GL_VIEWPORT, viewport);
//  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//  glGetDoublev(GL_PROJECTION_MATRIX, projection);
//
//  GLfloat z_cursor;
//  glReadPixels((GLint)screenPos.x, (GLint)screenPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);
//
//  // obtain the world coordinates
//
//  double rayX, rayY, rayZ;
//  gluUnProject(screenPos.x, screenPos.y, z_cursor, modelview, projection, viewport, &rayX, &rayY, &rayZ);
//
//  Ray ray;
//  ray.mStart = mCamera.mTranslation;
//  ray.mDirection = Vector3((float)rayX, (float)rayY, (float)rayZ) - ray.mStart;
//  ray.mDirection.AttemptNormalize();
//  return ray;
//}
//
////-----------------------------------------------------------------------------
//void Application::Update(float frameTime)
//{
//  mStatistics = Statistics();
//  mStatistics.mFps = 1 / frameTime;
//
//  gDebugDrawer->Update(frameTime);
//  mActiveGizmo->OnUpdate(this, frameTime);
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* gameObject = mGameObjects[i];
//    gameObject->Update(frameTime);
//  }
//
//  if (mDragging)
//  {
//    Math::Vector2 min = Math::Min(mStart, mEnd);
//    Math::Vector2 max = Math::Max(mStart, mEnd);
//
//    Ray bl = GetRayFromScreenCoords(min);
//    Ray br = GetRayFromScreenCoords(Math::Vector2(max.x, min.y));
//    Ray tl = GetRayFromScreenCoords(Math::Vector2(min.x, max.y));
//    Ray tr = GetRayFromScreenCoords(max);
//
//    Vector3 nbl = bl.GetPoint(mCamera.mNearPlane);
//    Vector3 nbr = br.GetPoint(mCamera.mNearPlane);
//    Vector3 ntr = tr.GetPoint(mCamera.mNearPlane);
//    Vector3 ntl = tl.GetPoint(mCamera.mNearPlane);
//
//    Vector3 fbl = bl.GetPoint(mCamera.mFarPlane);
//    Vector3 fbr = br.GetPoint(mCamera.mFarPlane);
//    Vector3 ftr = tr.GetPoint(mCamera.mFarPlane);
//    Vector3 ftl = tl.GetPoint(mCamera.mFarPlane);
//
//    Frustum frustum;
//    frustum.Set(nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl);
//
//    gDebugDrawer->DrawQuad(fbl, fbr, ftr, ftl);
//    CastFrustum(frustum);
//  }
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* gameObject = mGameObjects[i];
//    Model* model = gameObject->has(Model);
//    if(model != nullptr)
//      model->mOverlap = 0;
//  }
//
//  QueryResults results;
//  mDynamicBroadphase->SelfQuery(results);
//
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    for(size_t j = i + 1; j < results.mResults.size(); ++j)
//    {
//      QueryResult& r0 = results.mResults[i];
//      QueryResult& r1 = results.mResults[j];
//
//      if((r0.mClientData0 == r1.mClientData0 && r0.mClientData1 == r1.mClientData1) ||
//         (r0.mClientData0 == r1.mClientData1 && r0.mClientData1 == r1.mClientData0))
//         __debugbreak();
//    }
//  }
//
//
//  mStatistics.mSelfCollisionsCount = results.mResults.size();
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    QueryResult& result = results.mResults[i];
//    Model* model0 = static_cast<Model*>(result.mClientData0);
//    Model* model1 = static_cast<Model*>(result.mClientData1);
//
//    model0->mOverlap = 1;
//    model1->mOverlap = 1;
//
//    ModelSupportShape shape0;
//    shape0.mModel = model0;
//    ModelSupportShape shape1;
//    shape1.mModel = model1;
//
//    Gjk gjk;
//    Gjk::CsoPoint closestPoints;
//    float epsilon = 0.001f;
//    if(mRunGjk && gjk.Intersect(&shape0, &shape1, mMaxIterations, closestPoints, epsilon, mDebuggingIndex, mDrawGjk))
//    {
//      model0->mOverlap = model1->mOverlap = 2;
//    }
//  }
//
//  TwRefreshBar(mBar);
//  int value = TwRefreshBar(mStatisticsBar);
//}
//
//int Application::GetAssignmentNumber()
//{
//  return mAssignmentNumber;
//}
//
//void Application::SetAssignmentNumber(const int& assignmentNumber)
//{
//  mAssignmentNumber = assignmentNumber;
//  // Intersection Library
//  if(mAssignmentNumber == 0)
//  {
//    mFrustumCull = false;
//    mRefineCasts = false;
//    mDrawGjk = false;
//    mRunGjk = false;
//    SetBroadphaseType(SpatialPartitionTypes::NSquared);
//  }
//  // Basic spatial partitions, bounding volume calculations, debug drawing
//  else if(mAssignmentNumber == 1)
//  {
//    mRefineCasts = false;
//    mFrustumCull = false;
//    mDrawGjk = false;
//    mRunGjk = false;
//    SetBroadphaseType(SpatialPartitionTypes::NSquaredSphere);
//  }
//  // aabb tree, frustum culling
//  else if(mAssignmentNumber == 2)
//  {
//    mFrustumCull = true;
//    mRefineCasts = true;
//    mDrawGjk = false;
//    mRunGjk = false;
//    SetBroadphaseType(SpatialPartitionTypes::AabbTree);
//  }
//  // bsp-tree with csg operations
//  else if(mAssignmentNumber == 3)
//  {
//    mFrustumCull = true;
//    mRefineCasts = true;
//    mDrawGjk = false;
//    mRunGjk = false;
//    SetBroadphaseType(SpatialPartitionTypes::AabbTree);
//  }
//  //gjk
//  else if(mAssignmentNumber == 4)
//  {
//    mFrustumCull = true;
//    mRefineCasts = true;
//    mDrawGjk = true;
//    mRunGjk = true;
//    SetBroadphaseType(SpatialPartitionTypes::AabbTree);
//  }
//}
//
//int Application::GetBroadphaseType()
//{
//  return (int)mDynamicBroadphase->mType;
//}
//
//void Application::SetBroadphaseType(const int& type)
//{
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* gameObject = mGameObjects[i];
//    Model* model = gameObject->has(Model);
//    if(model != nullptr)
//      mDynamicBroadphase->RemoveData(model->mSpatialPartitionKey);
//  }
//
//  delete mDynamicBroadphase;
//
//  if(type == SpatialPartitionTypes::NSquared)
//    mDynamicBroadphase = new NSquaredSpatialPartition();
//  else if(type == SpatialPartitionTypes::NSquaredSphere)
//    mDynamicBroadphase = new BoundingSphereSpatialPartition();
//  else if(type == SpatialPartitionTypes::AabbTree)
//    mDynamicBroadphase = new DynamicAabbTree();
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* gameObject = mGameObjects[i];
//    Model* model = gameObject->has(Model);
//    if(model != nullptr)
//    {
//      SpatialPartitionData data;
//      data.mAabb = model->mAabb;
//      data.mBoundingSphere = model->mBoundingSphere;
//      data.mClientData = model;
//      mDynamicBroadphase->InsertData(model->mSpatialPartitionKey, data);
//    }
//  }
//}
//
//int Application::GetBoundingSphereType()
//{
//  return mBoundSphereType;
//}
//
//void Application::SetBoundingSphereType(const int& type)
//{
//  mBoundSphereType = type;
//
//  // Update all game object's bounding volumes
//  // (could try to update only the bounding sphere but whatever)
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* gameObject = mGameObjects[i];
//    Model* model = gameObject->has(Model);
//    if(model != nullptr)
//      model->UpdateBoundingVolumes(true);
//  }
//}
//
//int Application::GetLevel()
//{
//  return mCurrentLevelIndex;
//}
//
//void Application::SetLevel(const int& levelNumber)
//{
//  ChangeLevel(levelNumber);
//}
//
////-----------------------------------------------------------------------------
//void Application::Draw()
//{
//  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//  glLoadIdentity();
//
//  glEnable(GL_LIGHTING);
//  glLightfv(GL_LIGHT0, GL_POSITION, mLightPosition.ToFloats());
//  glLightfv(GL_LIGHT0, GL_AMBIENT, mLightAmbient.ToFloats());
//  glLightfv(GL_LIGHT0, GL_DIFFUSE, mLightDiffuse.ToFloats());
//  glLightfv(GL_LIGHT0, GL_SPECULAR, mLightSpecular.ToFloats());
//  glEnable(GL_LIGHT0);
//
//  glLoadIdentity();
//
//  mCamera.SetMatrix();
//
//  //glRotatef(mRotation, 0.0f, 1.0f, 0.0f);
//  //glRotatef(mRotationX, 1.0f, 0.0f, 0.0f);
//
//  if(mFrustumCull)
//  {
//    Frustum worldFrustum = BuildFrustum(Vector2(0, 0), mSize - Vector2(1, 1));
//    worldFrustum.DebugDraw();
//
//    CastResults results;
//    mDynamicBroadphase->CastFrustum(worldFrustum, results);
//    for(size_t i = 0; i < results.mResults.size(); ++i)
//    {
//      Model* model = static_cast<Model*>(results.mResults[i].mClientData);
//      GameObject* gameObject = model->mOwner;
//      model->Draw();
//
//      if(mDebugDraw)
//        gameObject->DebugDraw();
//    }
//  }
//  else
//  {
//    for(size_t i = 0; i < mGameObjects.size(); ++i)
//    {
//      GameObject* gameObject = mGameObjects[i];
//
//      Model* model = gameObject->has(Model);
//      if(model != nullptr)
//        model->Draw();
//
//      if(mDebugDraw)
//        gameObject->DebugDraw();
//    }
//  }
//
//  if(mDebugDraw)
//  {
//    Matrix4 transform = Matrix4::cIdentity;
//    mDynamicBroadphase->DebugDraw(mDynamicDebugDrawLevel, transform);
//  }
//
//  glDisable (GL_LIGHTING);
//  gDebugDrawer->Draw();
//
//  PrintOpenGLErrors();
//}
//
////-----------------------------------------------------------------------------
//void Application::Reshape(int width, int height, float aspectRatio)
//{
//  // Use the Projection Matrix
//  glMatrixMode(GL_PROJECTION);
//
//  // Load the identity matrix over the top of the current projection
//  glLoadIdentity();
//
//  mCamera.mNearPlane = 0.1f;
//  mCamera.mFarPlane = 1000.0f;
//  // Set the correct perspective
//  gluPerspective(45.0f, aspectRatio, mCamera.mNearPlane, mCamera.mFarPlane);
//  mSize = Math::Vector2((float)width, (float)height);
//
//  // Get back to the Modelview
//  glMatrixMode(GL_MODELVIEW);
//}
//
//GameObject* Application::CreateObject(const std::string& name, Mesh* mesh, const Vector3& scale, const Math::Quaternion& rotation, const Vector3& translation)
//{
//  GameObject* obj = new GameObject(this);
//  obj->mName = name;
//
//  Transform* transform = new Transform();
//  transform->mTranslation = translation;
//  transform->mScale = scale;
//  transform->mRotation = rotation;
//  obj->Add(transform);
//
//  Model* model = new Model();
//  model->mMesh = mesh;
//  obj->Add(model);
//
//  AddGameObject(obj);
//  return obj;
//}
//
//GameObject* Application::CreateEmptyObject(const std::string& name)
//{
//  GameObject* obj = new GameObject(this);
//  obj->mName = name;
//
//  AddGameObject(obj);
//  return obj;
//}
//
//void Application::AddGameObject(GameObject* gameObject)
//{
//  gameObject->mApplication = this;
//  mGameObjects.push_back(gameObject);
//
//  Transform* transform = gameObject->has(Transform);
//  if(transform != nullptr)
//    transform->mRotation.Normalize();
//
//  Model* model = gameObject->has(Model);
//  if(model != nullptr)
//  {
//    model->ComputeAabb();
//    model->ComputeBoundingSphere();
//    model->UpdateAabb();
//    model->UpdateBoundingSphere();
//
//    SpatialPartitionData data;
//    data.mAabb = model->mAabb;
//    data.mBoundingSphere = model->mBoundingSphere;
//    data.mClientData = model;
//    mDynamicBroadphase->InsertData(model->mSpatialPartitionKey, data);
//  }
//}
//
//void Application::UpdateGameObject(GameObject* gameObject)
//{
//  Model* model = gameObject->has(Model);
//  if(model != nullptr)
//  {
//    SpatialPartitionData data;
//    data.mAabb = model->mAabb;
//    data.mBoundingSphere = model->mBoundingSphere;
//    data.mClientData = model;
//    mDynamicBroadphase->UpdateData(model->mSpatialPartitionKey, data);
//  }
//}
//
//void Application::DestroyGameObject(GameObject* gameObject)
//{
//  TwRemoveAllVars(mSelectionBar);
//  mActiveGizmo->Select(nullptr);
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* obj = mGameObjects[i];
//    if(obj == gameObject)
//    {
//      mGameObjects[i] = mGameObjects[mGameObjects.size() - 1];
//      mGameObjects.pop_back();
//      break;
//    }
//  }
//
//  Model* model = gameObject->has(Model);
//  if(model != nullptr)
//    mDynamicBroadphase->RemoveData(model->mSpatialPartitionKey);
//
//  delete gameObject;
//}
//
//void Application::DisplayCastResult(GameObject* gameObject)
//{
//  std::string name = gameObject->mName;
//  std::string groupName = "group=" + name;
//
//  gameObject->DisplayComponentProperties(mSelectionBar);
//}
//
//void Application::DisplayCastResults(const Ray& worldRay, CastResults& results)
//{
//  TwRemoveAllVars(mSelectionBar);
//  mActiveGizmo->Select(NULL);
//
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    Model* model = static_cast<Model*>(results.mResults[i].mClientData);
//
//    if(mRefineCasts)
//    {
//      CastResult castInfo;
//      if(model->CastRay(worldRay, castInfo) == false)
//        continue;
//    }
//
//    DisplayCastResult(model->mOwner);
//  }
//
//  if(!results.mResults.empty())
//  {
//    Model* model = static_cast<Model*>(results.mResults[0].mClientData);
//    mActiveGizmo->Select(model->mOwner);
//  }
//}
//
//void Application::DisplayCastResults(const Frustum& worldFrustum, CastResults& results)
//{
//  TwRemoveAllVars(mSelectionBar);
//  mActiveGizmo->Select(NULL);
//
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    Model* model = static_cast<Model*>(results.mResults[i].mClientData);
//
//    if(mRefineCasts)
//    {
//      //CastInformation castInfo;
//      //if(model->CastFrustum(worldFrustum, castInfo) == false)
//      //  continue;
//    }
//
//    DisplayCastResult(model->mOwner);
//  }
//}
//
//void Application::CastRay(Ray& worldRay, CastResults& results)
//{
//  mDynamicBroadphase->CastRay(worldRay, results);
//}
//
//void Application::CastRay(Ray& worldRay)
//{
//  CastResults results;
//  CastRay(worldRay, results);
//
//  DisplayCastResults(worldRay, results);
//}
//
//void Application::CastFrustum(Frustum& worldFrustum)
//{
//  CastResults results;
//  mDynamicBroadphase->CastFrustum(worldFrustum, results);
//
//  worldFrustum.DebugDraw();
//  DisplayCastResults(worldFrustum, results);
//}
//
//void Application::FindPotentialIntersections(GameObject* gameObject, std::vector<GameObject*>& hitObjects)
//{
//  QueryResults results;
//  mDynamicBroadphase->SelfQuery(results);
//
//  for(size_t i = 0; i < results.mResults.size(); ++i)
//  {
//    QueryResult& result = results.mResults[i];
//    GameObject* obj0 = static_cast<Model*>(result.mClientData0)->mOwner;
//    GameObject* obj1 = static_cast<Model*>(result.mClientData1)->mOwner;
//
//    if(obj0 == gameObject)
//      hitObjects.push_back(obj1);
//    else if(obj1 == gameObject)
//      hitObjects.push_back(obj0);
//  }
//}
//
//void Application::Union(GameObject* gameObjectA, GameObject* gameObjectB)
//{
//  Model* model0 = gameObjectA->has(Model);
//  Model* model1 = gameObjectB->has(Model);
//
//  if(model0 != nullptr && model1 != nullptr)
//  {
//    float splitWeight = 0.8f;
//    float epsilon = 0.0001f;
//
//    TriangleList worldTriangles0 = model0->GetWorldTriangles();
//    BspTree tree0;
//    tree0.Construct(worldTriangles0, splitWeight, epsilon);
//
//    TriangleList worldTriangles1 = model1->GetWorldTriangles();
//    BspTree tree1;
//    tree1.Construct(worldTriangles1, splitWeight, epsilon);
//
//    tree1.Union(&tree0, splitWeight, epsilon);
//
//    std::vector<Triangle> results;
//    tree1.AllTriangles(results);
//
//    model1->SetWorldTriangles(results);
//    model1->UpdateBoundingVolumes(true);
//  }
//}
//
//void Application::Intersection(GameObject* gameObjectA, GameObject* gameObjectB)
//{
//  Model* model0 = gameObjectA->has(Model);
//  Model* model1 = gameObjectB->has(Model);
//
//  if(model0 != nullptr && model1 != nullptr)
//  {
//    float splitWeight = 0.8f;
//    float epsilon = 0.0001f;
//
//    TriangleList worldTriangles0 = model0->GetWorldTriangles();
//    BspTree tree0;
//    tree0.Construct(worldTriangles0, splitWeight, epsilon);
//
//    TriangleList worldTriangles1 = model1->GetWorldTriangles();
//    BspTree tree1;
//    tree1.Construct(worldTriangles1, splitWeight, epsilon);
//
//    tree1.Intersection(&tree0, splitWeight, epsilon);
//
//    std::vector<Triangle> results;
//    tree1.AllTriangles(results);
//
//    model1->SetWorldTriangles(results);
//    model1->UpdateBoundingVolumes(true);
//  }
//}
//
//void Application::Subtract(GameObject* gameObjectA, GameObject* gameObjectB)
//{
//  Model* model0 = gameObjectA->has(Model);
//  Model* model1 = gameObjectB->has(Model);
//
//  if(model0 != nullptr && model1 != nullptr)
//  {
//    float splitWeight = 0.8f;
//    float epsilon = 0.0001f;
//
//    TriangleList worldTriangles0 = model0->GetWorldTriangles();
//    BspTree tree0;
//    tree0.Construct(worldTriangles0, splitWeight, epsilon);
//
//    TriangleList worldTriangles1 = model1->GetWorldTriangles();
//    BspTree tree1;
//    tree1.Construct(worldTriangles1, splitWeight, epsilon);
//
//    tree1.Subtract(&tree0, splitWeight, epsilon);
//
//    std::vector<Triangle> results;
//    tree1.AllTriangles(results);
//
//    model1->SetWorldTriangles(results);
//    model1->UpdateBoundingVolumes(true);
//  }
//}
//
//void Application::Union(GameObject* gameObject)
//{
//  std::vector<GameObject*> hitObjects;
//  FindPotentialIntersections(gameObject, hitObjects);
//  for(size_t i = 0; i < hitObjects.size(); ++i)
//  {
//    Union(gameObject, hitObjects[i]);
//  }
//}
//
//void Application::Intersection(GameObject* gameObject)
//{
//  std::vector<GameObject*> hitObjects;
//  FindPotentialIntersections(gameObject, hitObjects);
//  for(size_t i = 0; i < hitObjects.size(); ++i)
//  {
//    Intersection(gameObject, hitObjects[i]);
//  }
//}
//
//void Application::Subtract(GameObject* gameObject)
//{
//  std::vector<GameObject*> hitObjects;
//  FindPotentialIntersections(gameObject, hitObjects);
//  for(size_t i = 0; i < hitObjects.size(); ++i)
//  {
//    Subtract(gameObject, hitObjects[i]);
//  }
//}
//
//Frustum Application::BuildFrustum(const Vector2& start, const Vector2& end)
//{
//  Math::Vector2 min = Math::Min(start, end);
//  Math::Vector2 max = Math::Max(start, end);
//
//  Ray bl = GetRayFromScreenCoords(min);
//  Ray br = GetRayFromScreenCoords(Math::Vector2(max.x, min.y));
//  Ray tl = GetRayFromScreenCoords(Math::Vector2(min.x, max.y));
//  Ray tr = GetRayFromScreenCoords(max);
//
//  Vector3 nbl = bl.GetPoint(mCamera.mNearPlane);
//  Vector3 nbr = br.GetPoint(mCamera.mNearPlane);
//  Vector3 ntr = tr.GetPoint(mCamera.mNearPlane);
//  Vector3 ntl = tl.GetPoint(mCamera.mNearPlane);
//
//  Vector3 fbl = bl.GetPoint(mCamera.mFarPlane);
//  Vector3 fbr = br.GetPoint(mCamera.mFarPlane);
//  Vector3 ftr = tr.GetPoint(mCamera.mFarPlane);
//  Vector3 ftl = tl.GetPoint(mCamera.mFarPlane);
//
//  Frustum frustum;
//  frustum.Set(nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl);
//  return frustum;
//}
//
//void Application::OnKeyDown(unsigned int key, int x, int y)
//{
//  mCamera.ProcessKeyboardInput(key, x, y);
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//    mGameObjects[i]->ProcessKeyboardInput(key, x, y);
//}
//
//void Application::OnKeyUp(unsigned int key, int x, int y)
//{
//  mCamera.ProcessKeyUp(key, x, y);
//
//  int index = (int)key - '0';
//  if(index >= 0 && index <= 9)
//  {
//    int mask = 1 << index;
//    if(gDebugDrawer->mActiveMask & mask)
//      gDebugDrawer->mActiveMask &= ~mask;
//    else
//      gDebugDrawer->mActiveMask |= mask;
//  }
//
//  if(key == SDLK_PLUS || key == SDLK_KP_PLUS || key == SDLK_EQUALS)
//    ChangeLevel((mCurrentLevelIndex + 1) % mLevels.size());
//  else if(key == SDLK_MINUS || key == SDLK_KP_MINUS)
//    ChangeLevel((mCurrentLevelIndex + mLevels.size() - 1) % mLevels.size());
//}
//
//Math::Vector2 GetScreenPosition(int x, int y)
//{
//  GLint viewport[4];
//  glGetIntegerv(GL_VIEWPORT, viewport);
//
//  Math::Vector2 screenPos;
//  screenPos.x = (float)x;
//  screenPos.y = (float)viewport[3]-(float)y;
//  return screenPos;
//}
//
//void Application::OnMouseInput(unsigned int button, bool isDown, int x, int y)
//{
//  Math::Vector2 screenPos = GetScreenPosition(x, y);
//  Ray worldRay = GetRayFromScreenCoords(screenPos);
//
//  mCamera.ProcessMouseInput(button, (int)isDown, x, y);
//
//  bool handled = mActiveGizmo->OnMouseInput(this, button, isDown, worldRay);
//  if(handled == true)
//    return;
//
//  if(button != SDL_BUTTON_LEFT)
//    return;
//
//  if(isDown)
//  {
//    mStart = screenPos;
//    mEnd = screenPos;
//    mMouseDown = true;
//
//    return;
//  }
//
//  mMouseDown = false;
//
//  mEnd = screenPos;
//  if(mDragging == true)
//  {
//    mDragging = false;
//    return;
//  }
//
//  CastRay(worldRay);
//}
//
////-----------------------------------------------------------------------------
//void Application::OnMouseMove(int x, int y)
//{
//  Math::Vector2 screenPos = GetScreenPosition(x, y);
//  Ray worldRay = GetRayFromScreenCoords(screenPos);
//
//  bool handled = mActiveGizmo->OnMouseMove(this, worldRay);
//  if(handled == true)
//    return;
//
//  mCamera.ProcessMouseMovement(x, y);
//
//  if(mMouseDown)
//  {
//    mEnd = screenPos;
//
//    if(Math::Length(mStart - mEnd) > 10)
//      mDragging = true;
//  }
//}
//
//void Application::OnMouseScroll(int x, int y)
//{
//  bool handled = mActiveGizmo->OnMouseScroll(this, x, y);
//  if(handled == true)
//    return;
//
//  mCamera.OnMouseScroll(x, y);
//}
//
//void Application::OnCreateCube()
//{
//  size_t index = mGameObjects.size();
//  std::string name = FormatString("Cube%d", index + 1);
//  CreateObject(name, mMeshes[0], Vector3(1.0f), Math::Quaternion::cIdentity, Vector3(0, 0, 0));
//}
//
//void Application::ChangeLevel(int levelIndex)
//{
//  if(mActiveGizmo != NULL)
//    mActiveGizmo->Select(NULL);
//
//  TwRemoveAllVars(mSelectionBar);
//
//  for(size_t i = 0; i < mGameObjects.size(); ++i)
//  {
//    GameObject* obj = mGameObjects[i];
//    Model* model = obj->has(Model);
//    if(model != nullptr)
//      mDynamicBroadphase->RemoveData(model->mSpatialPartitionKey);
//    delete obj;
//  }
//  mGameObjects.clear();
//
//
//  mCurrentLevelIndex = levelIndex;
//  mLevels[mCurrentLevelIndex]->Load(this);
//}

#include "pch.h"