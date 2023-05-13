/*****************************************************************
*\file         Camera3D.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Camera3D.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
#include "Xivi/Graphics/Helper/ConversionHelper.h"

namespace Xivi::Graphics
{
using Vector3 = Math::Vector3;
using Matrix4 = Math::Matrix4;

Camera3D::Camera3D()
{
  viewPort_Size_X = 0.f;
  viewPort_Size_Y = 0.f;
  viewPort_x = 0.f;
  viewPort_y = 0.f;
  mTarget = Vector3 {};
  mTranslation = Vector3 {};
  mRightMouseIsDown = false;
  mPhi = Math::s_PI * 0.5f;
  mTheta = 0;
  mRadius = 1;
  mLastX = mLastY = 0;
  mMode = Mode::Fps;
  mFov = 45.f;
  mNearPlane = 0.1f;
  mFarPlane = 1000.f;
  mView3D = Matrix4( 1.f );
  mProj3D = Matrix4( 1.f );

  //_frustum = Frustum();
}

Camera3D::Camera3D(
  const float viewPortPosX,
  const float viewPortPosY,
  const float viewPortSizeX,
  const float viewPortSizeY,
  const float fov,
  const float nearp,
  const float farp )
{
  viewPort_x = viewPortPosX;
  viewPort_y = viewPortPosY;
  viewPort_Size_X = viewPortSizeX;
  viewPort_Size_Y = viewPortSizeY;
  mFov = fov;
  mNearPlane = nearp;
  mFarPlane = farp;
  //mSensitivity = sensitivity;
  //default
  mMode = Mode::Fps;
  mTarget = Vector3 {};
  mTranslation = Vector3 {};
  mRightMouseIsDown = false;
  mPhi = Math::s_PI * 0.5f;
  mTheta = 0;
  mRadius = 1;
  mLastX = mLastY = 0;
  mView3D = Matrix4( 1.f );
  mProj3D = Matrix4( 1.f );
}

void Camera3D::SetupCamera3D(
  const float fov,
  const float nearp,
  const float farp )
{
  mFov = fov;
  mNearPlane = nearp;
  mFarPlane = farp;
  //default
  mMode = Mode::Fps;
  mTarget = Vector3 {};
  mTranslation = Vector3 {};
  mRightMouseIsDown = false;
  mPhi = Math::s_PI * 0.5f;
  mTheta = 0;
  mRadius = 1;
  mLastX = mLastY = 0;
  mView3D = Matrix4( 1.f );
  mProj3D = Matrix4( 1.f );
}

Matrix4 Camera3D::BuildCameraMatrix( const Vector3 &forward, const Vector3 &up, const Vector3 &right, const Vector3 &translation )
{
  Vector3 f = -forward.normalize();
  Vector3 u = up.normalize();
  Vector3 r = right.normalize();

  Matrix4 transform;
  transform.setBasis( 0, getX( r ), getY( r ), getZ( r ), 0 );
  transform.setBasis( 1, getX( u ), getY( u ), getZ( u ), 0 );
  transform.setBasis( 2, getX( f ), getY( f ), getZ( f ), 0 );
  transform.setBasis( 3, 0, 0, 0, 1 );

  Vector3 t = Math::SIMD::transposedTransform( Math::SIMD::toMat3x3( transform ), -translation );
  transform[3].set( getX( t ), getY( t ), getZ( t ), 1 );
  return transform;
}

Matrix4 Camera3D::ViewTransform()
{
  return ( mMode == Mode::Orbit ) ? OrbitViewTransform() : FPSViewTransfrom();
}

Matrix4 Camera3D::PerspectiveTransform3D( float width, float height )
{
  SetAspectRatio( width, height );
  mProj3D = Math::Helper::MathFunction::Perspective(
    Math::DegToRad( mFov ),
    aspectRatio,
    mNearPlane, mFarPlane );
  return mProj3D;
}

Matrix4 Camera3D::OrthoTransform( float width, float height )
{
  SetAspectRatio( width, height );
  mProj3D = Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( glm::ortho( 0.f, width, 0.f, height, 0.1f, 100.f ) );
  return mProj3D;
}

void Camera3D::SetAspectRatio( const float width, const float height )
{
  aspectRatio = width / height;
}

void Camera3D::Viewport()
{
  glViewport( (GLint) viewPort_x, (GLint) viewPort_y, (GLsizei) viewPort_Size_X, (GLsizei) viewPort_Size_Y );
}

void Camera3D::UpdateViewport( float postionX, float positionY, float width, float height )
{
  viewPort_x = postionX;
  viewPort_y = postionX;
  viewPort_Size_X = width;
  viewPort_Size_Y = height;
}

void Camera3D::PrintViewPort()
{
  std::cout
    << "ViewPort_sizeX " << viewPort_Size_X << " \n"
    << "viewPort_Size_Y " << viewPort_Size_Y << " \n"
    << "viewPort_x " << viewPort_x << " \n"
    << "viewPort_y " << viewPort_y << " \n"
    << std::endl;
}

void Camera3D::FPSInputLeft( bool active, float  mSensitivity )
{
  if ( active )
  {
    Vector3 worldUp = Vector3( 0, 1, 0 );
    Vector3 cameraDir = ComputeDirection();
    Vector3 forward = cameraDir;
    forward.normalize();
    Vector3 right = forward.cross( worldUp );;
    right.normalize();
    Math::Vector3 movementW = Math::Vector3 { 0.0f,0.0f,0.0f };

    auto key = Service::Locator::Get<Input::Manager>().Keyboard();
    auto key_t = key->T();
    auto key_g = key->G();
    auto key_f = key->F();
    auto key_h = key->H();

    if ( key_t.Pressed() )
      movementW += forward;
    else if ( key_g.Pressed() )
      movementW -= forward;
    if ( key_f.Pressed() )
      movementW -= right;
    else if ( key_h.Pressed() )
      movementW += right;

    movementW *= mSensitivity;
    mTranslation += movementW;
  }
}
void Camera3D::FPSInputRight( bool active, float  mSensitivity )
{
  if ( active )
  {
    Vector3 worldUp = Vector3( 0, 1, 0 );
    Vector3 cameraDir = ComputeDirection();
    Vector3 forward = cameraDir;
    forward.normalize();
    Vector3 right = forward.cross( worldUp );;
    right.normalize();
    Math::Vector3 movementW = Math::Vector3 { 0.0f,0.0f,0.0f };

    auto key = Service::Locator::Get<Input::Manager>().Keyboard();
    auto mouse = Service::Locator::Get<Input::Manager>().Mouse();
    auto key_up = key->Up();
    auto key_down = key->Down();
    auto key_left = key->Left();
    auto key_right = key->Right();
    auto key_w = key->W();
    auto key_s = key->S();
    auto key_a = key->A();
    auto key_d = key->D();

    if ( key_up.Pressed() || ( mouse->RightButton().Pressed() && key_w.Pressed() ) )
      movementW += forward;
    else if ( key_down.Pressed() || ( mouse->RightButton().Pressed() && key_s.Pressed() ) )
      movementW -= forward;
    if ( key_left.Pressed() || ( mouse->RightButton().Pressed() && key_a.Pressed() ) )
      movementW -= right;
    else if ( key_right.Pressed() || ( mouse->RightButton().Pressed() && key_d.Pressed() ) )
      movementW += right;

    movementW = movementW * mSensitivity;
    mTranslation += movementW;
  }
}
Matrix4 Camera3D::FPSViewTransfrom()
{
  Vector3 worldUp = Vector3( 0, 1, 0 );
  Vector3 cameraDir = ComputeDirection();
  Vector3 forward = cameraDir;
  forward = forward.normalize();
  Vector3 right = forward.cross( worldUp );;
  right = right.normalize();
  Math::Vector3 up = Math::SIMD::cross( right, forward );
  up = Math::SIMD::normalize( up );

  mTarget = mTranslation + forward;
  mView3D = Math::Helper::MathFunction::LookAt( mTranslation, mTarget, worldUp );
  createFrustumFromCamera( forward, worldUp );
  return mView3D;
}
Matrix4 Camera3D::PlayerViewTransfrom( Vec3f cameraDir )
{
  Vector3 worldUp = Vector3( 0, 1, 0 );
  Vector3 forward = cameraDir;
  forward = forward.normalize();
  Vector3 right = forward.cross( worldUp );;
  right = right.normalize();
  Math::Vector3 up = Math::SIMD::cross( right, forward );
  up = Math::SIMD::normalize( up );
  mTarget = mTranslation + forward;
  mView3D = Math::Helper::MathFunction::LookAt( mTranslation, mTarget, up );
  createFrustumFromCamera( forward, worldUp );
  return mView3D;
}

void Camera3D::OrbitInputLeft( bool active, float  mSensitivity )
{
  Vector3 worldUp = Vector3( 0, 1, 0 );
  Vector3 cameraDir = ComputeDirection();
  Vector3 forward = -cameraDir;
  Vector3 movementForward = forward;
  movementForward.setY( 0 );
  movementForward = movementForward.normalize();
  Vector3 movementRight = Vector3( -movementForward.getZ(), 0.0f, movementForward.getX() );
  Vector3 movementW;
  movementW.setToZero();

  auto key = Service::Locator::Get<Input::Manager>().Keyboard();
  auto key_w = key->W();
  auto key_a = key->A();
  auto key_s = key->S();
  auto key_d = key->D();

  if ( key_w.Pressed() )
    movementW -= movementForward;
  else if ( key_s.Pressed() )
    movementW += movementForward;
  if ( key_a.Pressed() )
    movementW -= movementRight;
  else if ( key_d.Pressed() )
    movementW += movementRight;

  movementW = movementW * 0.1f;
  Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );
  Vector3 right = forward.cross( up );;
  right = right.normalize();

  up = Math::SIMD::cross( right, forward );
  up = Math::SIMD::normalize( up );
  mTarget += movementW;
  Vector3 eye = cameraDir + mTarget;
  mTranslation = eye;
}
void Camera3D::OrbitInputRight( bool active, float  mSensitivity )
{
  Vector3 worldUp = Vector3( 0, 1, 0 );
  Vector3 cameraDir = ComputeDirection();
  Vector3 forward = -cameraDir;

  // Compute the forward and right vector of the camera in world space
  Vector3 movementForward = forward;
  movementForward.setY( 0 );
  movementForward = movementForward.normalize();
  Vector3 movementRight = Vector3( -movementForward.getZ(), 0.0f, movementForward.getX() );

  // Build up the total world space movement of the camera
  Vector3 movementW;
  movementW.setToZero();

  auto key = Service::Locator::Get<Input::Manager>().Keyboard();
  auto key_up = key->Up();
  auto key_down = key->Down();
  auto key_left = key->Left();
  auto key_right = key->Right();

  if ( key_up.Pressed() )
    movementW -= movementForward;
  else if ( key_down.Pressed() )
    movementW += movementForward;
  if ( key_left.Pressed() )
    movementW -= movementRight;
  else if ( key_right.Pressed() )
    movementW += movementRight;

  movementW = movementW * 0.1f;
  Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );
  Vector3 right = forward.cross( up );;
  right = right.normalize();

  up = Math::SIMD::cross( right, forward );
  up = Math::SIMD::normalize( up );
  mTarget += movementW;
  Vector3 eye = cameraDir + mTarget;
  mTranslation = eye;
}
Matrix4 Camera3D::OrbitViewTransform()
{
  //Vector3 cameraDir = ComputeDirection();
  Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );
  //mTranslation = -cameraDir + mTarget;;
  mView3D = Math::Helper::MathFunction::LookAt( mTranslation, mTarget, up );
  return mView3D;
}

Vector3 Camera3D::ComputeDirection()
{
  Math::Vector3 cameraDir;
  setX( cameraDir, mRadius * std::cos( mTheta ) * std::sin( mPhi ) );
  setY( cameraDir, mRadius * std::cos( mPhi ) );
  setZ( cameraDir, mRadius * std::sin( mTheta ) * std::sin( mPhi ) );
  return cameraDir;
}

Vector3 Camera3D::ComputeCrossDirection()
{
  return ComputeDirection().cross( Vec3f( 0.f, 1.f, 0.f ) );
}

void Camera3D::ProcessMouseInput( unsigned int button, int x, int y, bool active, float sentivity )
{
  if ( active )
  {
    auto &mouse = *Service::Locator::Get<Input::Manager>().Mouse();
    mRightMouseIsDown = ( mouse[static_cast<Input::MouseButton>( button )].Pressed() );
    if ( mRightMouseIsDown == true )
    {
      int deltaX = x - mLastX;
      int deltaY = y - mLastY;
      if ( mMode == Mode::Orbit )
        deltaY *= -1;

      mPhi = Math::Clamp( mPhi + deltaY * 0.01f, Math::DegToRad( 5 ), Math::DegToRad( 175 ) );
      mTheta += deltaX * sentivity;
    }
    mLastX = x;
    mLastY = y;
  }
}

void Camera3D::ProcessGameMouseInput( int x, int y, float &phi, float &theta, bool active, float sentivity )
{
  if ( active )
  {
    int deltaX = x - mLastX;
    int deltaY = y - mLastY;
    if ( mMode == Mode::Orbit )
      deltaY *= -1;
    mPhi = Math::Clamp( Math::DegToRad( phi ) + deltaY * sentivity, Math::DegToRad( 5 ), Math::DegToRad( 175 ) );
    mTheta += deltaX * sentivity;
    phi = Math::RadToDeg( mPhi );
    theta = Math::RadToDeg( mTheta );

    mLastX = x;
    mLastY = y;
  }
}

void Camera3D::ToggleMode()
{
  if ( mMode == Orbit )
    SetMode( Mode::Fps );
  else
    SetMode( Mode::Orbit );
}

void Camera3D::SetMode( const int &mode )
{
  int newMode = mode;
  if ( newMode == mMode )
    return;

  mMode = (Mode) newMode;
  mPhi = Math::s_PI - mPhi;
  mTheta = Math::s_PI - mTheta;
  if ( newMode == Mode::Orbit )
    mTarget = mTranslation - ComputeDirection();
}

Vector3 Camera3D::MouseTo3DWorldDriection( float mouseX, float mouseY, float width, float height )
{
  Vec4f ray_nds = Math::Helper::MathFunction::GetNormalisedDeviceCoordinates( width, height, mouseX, mouseY );
  Vec4f ray_eye = Math::Helper::MathFunction::ToViewCoords( mProj3D, ray_nds );
  Vec4f  eye_coord = Vec4f( ray_eye.getX(), ray_eye.getY(), -1.f, 0.f );
  Vec3f world_coords = Math::Helper::MathFunction::ToWorldCoords( mView3D, eye_coord );
  world_coords.normalize();
  return world_coords;
}

Vector3 Camera3D::MouseToWorldPosition( float mouseX, float mouseY, float width, float height, float distance )
{
  return CameraEye() + MouseTo3DWorldDriection( mouseX, mouseY, width, height ) * distance;
}

Shapes::Frustum Camera3D::GetFrustum()
{
  return _frustum;
}

void Camera3D::createFrustumFromCamera( Vector3 forward, Vector3 worldUp )
{
  Math::Vector3 cameraDir = ComputeDirection();
  Math::Vector3 front = cameraDir.normalize();

  const float halfVSide = mFarPlane * tanf( Math::RadToDeg( 45.f * 0.5 ) );
  const float halfHSide = halfVSide * aspectRatio;
  const Vec3f frontMultFar = mFarPlane * front;

  Vector3 _right = forward.cross( worldUp );
  _right = _right.normalize();

  Shapes::Plane nearFace = Shapes::Plane( front, CameraEye() + mNearPlane * front );
  Shapes::Plane farFace = Shapes::Plane( -front, CameraEye() + frontMultFar );
  Shapes::Plane rightFace = Shapes::Plane( Math::cross( worldUp, frontMultFar + _right * halfHSide ), CameraEye() );
  Shapes::Plane leftFace = Shapes::Plane( Math::cross( frontMultFar - _right * halfHSide, worldUp ), CameraEye() );
  Shapes::Plane topFace = Shapes::Plane( Math::cross( _right, frontMultFar - worldUp * halfVSide ), CameraEye() );
  Shapes::Plane bottomFace = Shapes::Plane( Math::cross( frontMultFar + worldUp * halfVSide, _right ), CameraEye() );

  _frustum.Set( leftFace, rightFace, topFace, bottomFace, nearFace, farFace );
}
}