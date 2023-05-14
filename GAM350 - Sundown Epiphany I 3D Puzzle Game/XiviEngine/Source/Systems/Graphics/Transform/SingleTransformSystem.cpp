/*****************************************************************
*\file         SingleTransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "SingleTransformSystem.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
namespace Xivi::Graphics
{
void SingleTransformSystem::operator()(
  ECS::Entity &entity,
  Position &pos,
  Transform &transform ) noexcept
{
  auto &m_graphicManager = Service::Locator::Get<Graphics::Manager>();
  #ifndef XV_EDITOR
  auto &gs = Service::Locator::Get<GSM::GSManager>();
  if ( gs.cutSceneFunc.isActive )
    return;
  #endif;
  Math::Matrix4 model( 1.f );
  Math::Matrix4 scale( 1.f );
  Math::Matrix4 rotate( 1.f );
  Math::Vector3 offset = ( pos.shaky ) ? Math::Helper::RandFunction::RandNumberBetween( Math::Vector3( 1.f, 1.f, 1.f ), Math::Vector3( -1.f, -1.f, -1.f ) ) : Math::Vector3( 0.f, 0.f, 0.f );
  Math::Vector3 posTemp = pos.m_position + offset;
  Math::Vector3 scaleTemp = transform.scale;
  model = model.translate( posTemp.getX(), posTemp.getY(), posTemp.getZ() );
  Vec3f axisRad = Vec3f(
    Math::DegToRad( transform.axis.getX() ),
    Math::DegToRad( transform.axis.getY() ),
    Math::DegToRad( transform.axis.getZ() ) );
  rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
  scale = scale.scale( scaleTemp.getX(), scaleTemp.getY(), scaleTemp.getZ() );
  transform.LocalToWorld = ( model * rotate * scale );

  //Math::Matrix4 rotate(1.f);
  //Math::Vector3 position = pos.m_position;
  //Math::Vector3 scaleTemp = transform.scale;

  ////model = model.translate(position.getX(), position.getY(), position.getZ());
  //Vec3f axisRad = Vec3f(
  //	Math::DegToRad(transform.axis.getX()),
  //	Math::DegToRad(transform.axis.getY()),
  //	Math::DegToRad(transform.axis.getZ()));
  //rotate = Math::Helper::MathFunction::ToRotationMatrix(axisRad);

  //glm::quat orientation = glm::quat(Math::Helper::GLMHelpers::GetGLMVec(axisRad));
  //glm::mat4 model = glm::translate(glm::toMat4(orientation), Math::Helper::GLMHelpers::GetGLMVec(position));
  //glm::mat4 rot = Math::Helper::GLMHelpers::ConvertMatrixToGLMFormat(rotate);
  ////glm::mat4 rot = glm::toMat4(orientation);
  //glm::mat4 trans = glm::translate(glm::mat4(1.0f), Math::Helper::GLMHelpers::GetGLMVec(position));
  //glm::mat4 scal = glm::scale(glm::mat4(1.0f), Math::Helper::GLMHelpers::GetGLMVec(scaleTemp));
  //glm::mat4 final = trans * rot * scal;
  //transform.LocalToWorld = Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat(final);
}
}