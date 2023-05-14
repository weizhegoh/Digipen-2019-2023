/////////////////////////////////////////////////////////////////////////////////
/////
///// Authors: Joshua Davis
///// Copyright 2015, DigiPen Institute of Technology
/////
/////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
//#include "GraphicPCH.h"
//
//class GameObject;
//class Application;
//
//class Gizmo
//{
//public:
//  GameObject* mSelectedObject;
//  Gizmo() {mSelectedObject = nullptr;}
//
//  void Select(GameObject* object)
//  {
//    mSelectedObject = object;
//  }
//
//  virtual void OnUpdate(Application* application, float dt) = 0;
//  virtual bool OnMouseInput(Application* application, unsigned int button, bool isDown, const Ray& ray) = 0;
//  virtual bool OnMouseMove(Application* application, const Ray& ray) = 0;
//  virtual bool OnMouseScroll(Application* application, int x, int y) = 0;
//
//  void GetBasis(Vector3& pos, Vector3& basisX, Vector3& basisY, Vector3& basisZ);
//};
//
//class TranslationGizmo : public Gizmo
//{
//public:
//  TranslationGizmo();
//  void OnUpdate(Application* application, float dt) override;
//  bool OnMouseInput(Application* application, unsigned int button, bool isDown, const Ray& ray) override;
//  bool OnMouseMove(Application* application, const Ray& ray) override;
//  bool OnMouseScroll(Application* application, int x, int y) override;
//
//  int mHightlightedAxis;
//  bool mCaptured;
//  Vector3 mLocalPoint;
//  float mLocalOffset;
//
//  Vector3 mPlaneNormal;
//};
