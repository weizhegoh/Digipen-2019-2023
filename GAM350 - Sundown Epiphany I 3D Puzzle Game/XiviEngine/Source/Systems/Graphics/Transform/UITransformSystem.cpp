/*****************************************************************
*\file         UITransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "UITransformSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Components/GraphicsComponents.h"
#include "Components/EngineComponents.h"
namespace Xivi::Graphics
{
void UITransformSystem::OnCreate() noexcept
{
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
}

void UITransformSystem::operator()(
  Entity &entity,
  Position &pos,
  Transform &transform,
  AABB &aabb ) noexcept
{
  //auto windowWidth = (float)Service::Locator::Get<GLFW_Window>().Width();
  //auto windowHeight = (float)Service::Locator::Get<GLFW_Window>().Height();
  //m_framebuffer->CameraOnUpdateUI(windowWidth, windowHeight);
  auto &camera = m_framebuffer->CameraUI();
  auto cameraDir = camera.CameraDirection();
  transform.axis.setY( Math::RadToDeg( -std::atan2( cameraDir.getX(), cameraDir.getZ() ) ) );
  Math::Vector3 scaleTemp = transform.scale;
  Math::Matrix4 model( 1.f );
  Math::Matrix4 scale( 1.f );
  Math::Matrix4 rotate( 1.f );
  model = model.translate( pos.m_position.getX(), pos.m_position.getY(), pos.m_position.getZ() );
  Vec3f axisRad = Vec3f(
    Math::DegToRad( transform.axis.getX() ),
    Math::DegToRad( transform.axis.getY() ),
    Math::DegToRad( transform.axis.getZ() ) );
  rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
  scale = scale.scale( scaleTemp.getX(), scaleTemp.getY(), scaleTemp.getZ() );
  transform.LocalToWorld = ( model * rotate * scale );
  ////transform.model = SIMDtestmodel.transpose();
  //auto& m_graphicManager = Service::Locator::Get<Graphics::Manager>();
  //auto& m_debugDrawer = m_graphicManager.GetDebugDraw();
  //m_debugDrawer.DrawAabb(Graphics::Shapes::Aabb(aabb.AABB_mMin, aabb.AABB_mMax));
}
}