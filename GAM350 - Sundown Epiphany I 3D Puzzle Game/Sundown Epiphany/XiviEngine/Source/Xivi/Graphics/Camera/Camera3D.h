/*****************************************************************
*\file         Camera3D.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "pch.h"
#include <rttr/registration.h>
#include <rttr/type.h>
#include "../Shapes/Shapes.h"

namespace Xivi::Graphics
{
using Vector3 = Vec3f;
using Matrix4 = Mat4x4;
class Camera3D
{
  int mLastX; int mLastY;
  float mPhi; float mTheta; float mRadius;
  bool mRightMouseIsDown;
  float mFov; float mNearPlane; float mFarPlane;
  Vector3 mTarget; Vector3 mTranslation; //camera eye;
  Matrix4 mView3D;
  Matrix4 mProj3D;

  float viewPort_x;
  float viewPort_y;
  float viewPort_Size_X;
  float viewPort_Size_Y;
  float aspectRatio;

  Shapes::Frustum _frustum;

public:

  Camera3D();
  Camera3D(
    const float viewPortPosX,
    const float viewPortPosY,
    const float viewPortSizeX,
    const float viewPortSizeY,
    const float fov,
    const float nearp,
    const float farp );

  void SetupCamera3D(
    const float fov = 45.f,
    const float near = 0.1f,
    const float far = 10000.f );

  Matrix4 ViewTransform();
  Matrix4 OrthoTransform( float width, float height );
  Matrix4 FPSViewTransfrom();
  Matrix4 PlayerViewTransfrom( Vec3f cameraDir );
  Matrix4 OrbitViewTransform();
  Matrix4 PerspectiveTransform3D( float width, float height );
  Matrix4 BuildCameraMatrix(
    const Vector3 &forward, const Vector3 &up,
    const Vector3 &right, const Vector3 &translation );

  void ProcessMouseInput( unsigned int button, int x, int y, bool active, float sentivity = 0.01f );
  void ProcessGameMouseInput( int x, int y, float &phi, float &theta, bool active, float sentivity = 0.01f );
  void FPSInputLeft( bool active, float  mSensitivity );
  void FPSInputRight( bool active, float  mSensitivity );
  void OrbitInputLeft( bool active, float  mSensitivity );
  void OrbitInputRight( bool active, float  mSensitivity );

  Vector3 ComputeDirection();
  Vector3 ComputeCrossDirection();
  Vector3 &CameraEye()
  {
    return mTranslation;
  }
  Vector3 &CameraTarget()
  {
    return mTarget;
  }
  void SetCameraEye( const Math::Vector3 pos )
  {
    mTranslation = pos;
  }

  void SetCameraTarget( const Math::Vector3 pos )
  {
    mTarget = pos;
  }
  Vector3 CameraDirectionNormlizedOrbit()
  {
    return Math::normalize( mTranslation - mTarget );
  }
  Vector3 CameraDirectionNormalized()
  {
    return Math::normalize( mTarget - mTranslation );
  }
  Vector3 CameraDirection()
  {
    return mTarget - mTranslation;
  }
  Matrix4 &ViewMat_3D()
  {
    return mView3D;
  }
  Matrix4 &ProjMat_3D()
  {
    return mProj3D;
  }
  void AssignDir( float phi, float theta )
  {
    mPhi = phi; mTheta = theta;
  }

  float &GetNearPlane()
  {
    return mNearPlane;
  }
  float &GetFarPlane()
  {
    return mFarPlane;
  }

  float &GetViewportX()
  {
    return viewPort_Size_X;
  }
  float &GetViewportY()
  {
    return viewPort_Size_Y;
  }

  float &GetAspectRatio()
  {
    return aspectRatio;
  }
  float &GetFOV()
  {
    return mFov;
  }

  Vector3 MouseTo3DWorldDriection(
    float mouseX, float mouseY,
    float width, float height );
  Vector3 MouseToWorldPosition(
    float mouseX, float mouseY,
    float width, float height,
    float distance );

  void Viewport();
  void UpdateViewport( float postionX, float positionY, float width, float height );
  void PrintViewPort();
  void SetAspectRatio( const float width, const float height );

  //float mSensitivity;
  bool mToggleFreeCam;
  enum Mode
  {
    Orbit, Fps
  };
  Mode mMode;
  void SetMode( const int &mode );
  int GetMode()
  {
    return (int) mMode;
  }
  void ToggleMode();
  void ResetEye()
  {
    mTranslation = { 0.f, 0.f, 0.f };
    mTarget = { 0.f, 0.f, 0.f };
  }

  float GetFov()
  {
    return mFov;
  }
  void SetFov( float fov )
  {
    mFov = fov;
  }
  float GetFar()
  {
    return mFarPlane;
  }
  void SetFar( float farr )
  {
    mFarPlane = farr;
  }
  float GetNear()
  {
    return mNearPlane;
  }
  void SetNear( float nearr )
  {
    mNearPlane = nearr;
  }
  float GetPhi()
  {
    return mPhi;
  }
  void SetPhi( float Phi )
  {
    mPhi = Phi;
  }
  float GetTheta()
  {
    return mTheta;
  }
  void SetTheta( float Theta )
  {
    mTheta = Theta;
  }

//Frustum GetFrustum();
  Shapes::Frustum GetFrustum();
  void createFrustumFromCamera( Vector3 forward, Vector3 worldUp );
};
}
namespace Xivi::Graphics::Camera::Reflect
{
RTTR_REGISTRATION
{
  // Camera Mode
  rttr::registration::enumeration<Graphics::Camera3D::Mode>( "Camera Mode" )
  (
  rttr::value( "Orbit", Graphics::Camera3D::Mode::Orbit ),
  rttr::value( "Fps", Graphics::Camera3D::Mode::Fps )
  );
}
}