/*****************************************************************
*\file         Frustum.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "pch.h"

//
//namespace Xivi::Graphics
//{
//	using Vector3 = Vec3f;
//	using Matrix4 = Mat4x4;
//
//
//	struct Plane
//	{
//		// unit vector
//		Vec3f normal = Vec3f{ 0.f, 1.f, 0.f };
//
//		// distance from origin to the nearest point in the plan
//		float     distance = 0.f;
//		Plane() {}
//
//		Plane(const Vec3f& p1, const Vec3f& norm)
//			: normal(norm.normalize()),
//			distance(dot(normal, p1)) // check dot
//		{}
//
//		float getSignedDistanceToPlan(const Vec3f& point) const
//		{
//			return Math::dot(normal, point) - distance;
//		}
//	};
//
//	struct Frustum
//	{
//		Plane topFace;
//		Plane bottomFace;
//
//		Plane rightFace;
//		Plane leftFace;
//
//		Plane farFace;
//		Plane nearFace;
//
//
//		Frustum() {}
//		//int mLastX; int mLastY;
//		//float mPhi; float mTheta; float mRadius;
//		//bool mRightMouseIsDown;
//		//float mFov; float mNearPlane; float mFarPlane;
//		//Vector3 mTarget; Vector3 mTranslation; //camera eye;
//		//Matrix4 mView3D;
//		//Matrix4 mProj3D;
//
//
//		//float viewPort_x;
//		//float viewPort_y;
//		//float viewPort_Size_X;
//		//float viewPort_Size_Y;
//		//float aspectRatio;
//
//	//public:
//
//
//
//		//Camera3D(
//		//	const float viewPortPosX,
//		//	const float viewPortPosY,
//		//	const float viewPortSizeX,
//		//	const float viewPortSizeY,
//		//	const float fov,
//		//	const float nearp,
//		//	const float farp);
//
//
//		//void SetupCamera3D(
//		//	const float fov = 45.f,
//		//	const float near = 0.1f,
//		//	const float far = 10000.f);
//
//
//		//Matrix4 ViewTransform();
//		//Matrix4 OrthoTransform(float width, float height);
//		//Matrix4 FPSViewTransfrom();
//		//Matrix4 PlayerViewTransfrom(Vec3f cameraDir);
//		//Matrix4 OrbitViewTransform();
//		//Matrix4 PerspectiveTransform3D(float width, float height);
//		//Matrix4 BuildCameraMatrix(
//		//	const Vector3& forward, const Vector3& up,
//		//	const Vector3& right, const Vector3& translation);
//
//		//void ProcessMouseInput(unsigned int button, int x, int y, bool active, float sentivity = 0.01f);
//		//void ProcessGameMouseInput(int x, int y, bool active, float sentivity = 0.01f);
//		//void FPSInputLeft(bool active, float  mSensitivity);
//		//void FPSInputRight(bool active, float  mSensitivity);
//		//void OrbitInputLeft(bool active, float  mSensitivity);
//		//void OrbitInputRight(bool active, float  mSensitivity);
//
//		//Vector3 ComputeDirection();
//		//Vector3 ComputeCrossDirection();
//		//Vector3& CameraEye() { return mTranslation; }
//		//Vector3& CameraTarget() { return mTarget; }
//		//void SetCameraEye(const Math::Vector3 pos) { mTranslation = pos; }
//		//void SetPlayerCameraEye(const Math::Vector3 pos, const float scaleY)
//		//{
//		//	auto dir = CameraDirection();
//		//	dir.normalize();
//		//	dir *= 3.f;
//		//	mTranslation = pos - Math::Vector3(dir.getX(), -(scaleY * 0.5f), dir.getZ());
//		//}
//
//
//		//void SetCameraTarget(const Math::Vector3 pos) { mTarget = pos; }
//
//		//Vector3 CameraDirectionNormalized() { return Math::normalize(mTarget - mTranslation); }
//		//Vector3 CameraDirection() { return mTarget - mTranslation; }
//		//Matrix4& ViewMat_3D() { return mView3D; }
//		//Matrix4& ProjMat_3D() { return mProj3D; }
//		//void AssignDir(float phi, float theta)
//		//{
//		//	mPhi = phi; mTheta = theta;
//		//}
//
//		//float& GetNearPlane() { return mNearPlane; }
//		//float& GetFarPlane() { return mFarPlane; }
//
//		//float& GetViewportX() { return viewPort_Size_X; }
//		//float& GetViewportY() { return viewPort_Size_Y; }
//
//		//float& GetAspectRatio() { return aspectRatio; }
//		//float& GetFOV() { return mFov; }
//
//		//Vector3 MouseTo3DWorldDriection(
//		//	float mouseX, float mouseY,
//		//	float width, float height);
//		//Vector3 MouseToWorldPosition(
//		//	float mouseX, float mouseY,
//		//	float width, float height,
//		//	float distance);
//
//		//void Viewport();
//		//void UpdateViewport(float postionX, float positionY, float width, float height);
//		//void PrintViewPort();
//		//void SetAspectRatio(const float width, const float height);
//
//		////float mSensitivity;
//		//bool mToggleFreeCam;
//		//enum Mode { Orbit, Fps };
//		//Mode mMode;
//		//void SetMode(const int& mode);
//		//int GetMode() { return (int)mMode; }
//		//void ToggleMode();
//		//float GetPhi() { return mPhi; }
//		//float GetTheta() { return mTheta; }
//		//void ResetEye()
//		//{
//		//	mTranslation = { 0.f, 0.f, 0.f };
//		//	mTarget = { 0.f, 0.f, 0.f };
//		//}
//	};
//}
////namespace Xivi::Graphics::Camera::Reflect
////{
////	RTTR_REGISTRATION
////	{
////		// Camera Mode
////		rttr::registration::enumeration<Graphics::Camera3D::Mode>("Camera Mode")
////		(
////		rttr::value("Orbit", Graphics::Camera3D::Mode::Orbit),
////		rttr::value("Fps", Graphics::Camera3D::Mode::Fps)
////		);
////	}
////}