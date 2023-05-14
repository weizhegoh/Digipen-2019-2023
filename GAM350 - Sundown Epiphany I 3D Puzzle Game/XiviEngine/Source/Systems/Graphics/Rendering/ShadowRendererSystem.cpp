/*****************************************************************
*\file         ShadowRendererSystem.cpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include <Resource/Manager.h>
#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
//Asset
#include "Xivi/Graphics/Model/Model.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Model/Animation/Animation.h"
#include "Xivi/Resources/EngineResource.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Core/Application.h"
#include "RendererSystem.h"
#include "ShadowRendererSystem.h"
#include "Components/Particles/ParticleContainer.h"
#include "Components/Particles/ParticleSettings.h"

namespace Xivi::Graphics
{
void ShadowRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_shader_texture = &m_graphic_manager->GetShaders( ShaderOption::TEXTURE );
  m_shader_animation = &m_graphic_manager->GetShaders( ShaderOption::ANIMATION );
  m_shader_shadow = &m_graphic_manager->GetShaders( ShaderOption::SHADOW_MAPPING );
}

void ShadowRendererSystem::OnUpdate() noexcept
{
  auto &frameBuffer = m_graphic_manager->frameBuffer;
  auto &camera = m_graphic_manager->camera;

  if ( !m_graphic_manager->shadowRender )
    return;
  else
  {
    //glEnable(GL_DEPTH_TEST);
    m_shader_shadow->Use();

    ForEach( [&]( Entity &entity_pl, Position &pos_pl, LightProperty &lightProperty, Direction &dir )
    {
      //if (!lightProperty.toRenderShadow)
      //	return;

      glBindFramebuffer( GL_FRAMEBUFFER, lightProperty.frameBufferID );
      glViewport( 0, 0, 1024, 1024 );
      glClear( GL_DEPTH_BUFFER_BIT );

      Vec3f lightInvDir = pos_pl.m_position;
      glm::mat4 tempLightProjection;
      tempLightProjection = glm::ortho( -lightProperty.bound, lightProperty.bound, -lightProperty.bound, lightProperty.bound, lightProperty.near_plane, lightProperty.far_plane );
      //tempLightProjection = glm::perspective(glm::radians(90.f), 1.f, near_plane, far_plane);
      auto lightProjection = Math::Helper::GLMHelpers::ConvertMatrixToSIMDFormat( tempLightProjection );
      auto lightView = Math::Helper::MathFunction::LookAt( lightInvDir, lightProperty.view_at, lightProperty.view_up );
      auto lightSpaceMatrix = lightProjection ^ lightView;

      ForEach( [&](
        Entity &entity,
        Transform &transform,
        StaticMeshComponent *mesh,
        AnimationMeshComponent *amesh,
        AnimatorComponent *animator,
        Material &material, AABB &aabb, LightData &lightData )
      {
        if ( HaveComponent<RenderSettings>( entity ) )
        {
          auto &rs = GetComponent<RenderSettings>( entity );
          if ( !rs.onRender )
            return false;
        }

        lightData.isValid = lightProperty.toRenderShadow;
        if ( !lightProperty.toRenderShadow )
        {
          if ( lightData.lightEntitiesList.empty() )
            return true;
          else
          {
            lightData.lightEntitiesList.erase( std::remove( lightData.lightEntitiesList.begin(), lightData.lightEntitiesList.end(), entity_pl ), lightData.lightEntitiesList.end() );
            lightData.lightData_map.erase( entity_pl );
            lightData.shadowMapID_map.erase( entity_pl );
            return true;
          }
        }

        if ( mesh || amesh )
        {
          m_shader_shadow->SetUniform( "lightSpaceMatrix", lightSpaceMatrix );
          Mat4x4 tmodel = transform.LocalToWorld.transpose();
          m_shader_shadow->SetUniform( "uModel", tmodel );

          //auto& checkAabb = GetComponent<AABB>(entity);
          {
            //if (Math::Helper::MathFunction::SphereAABB(checkAabb.AABB_mMin,
            //	checkAabb.AABB_mMax, pos_pl.m_position, lightProperty.radius))
            //if (HaveComponent<Direction>(entity_pl))
            //{
              // do something
            if ( lightData.lightEntitiesList.empty() )
            {
              lightData.lightEntitiesList.push_back( entity_pl );
              lightData.lightData_map.insert( { entity_pl, std::make_pair( pos_pl.m_position, lightSpaceMatrix ) } );
              lightData.shadowMapID_map.insert( { entity_pl, lightProperty.shadowMapID } );
            }
            else
            {
              auto it = std::find( lightData.lightEntitiesList.begin(), lightData.lightEntitiesList.end(), entity_pl );
              if ( it == lightData.lightEntitiesList.end() )
              {
                lightData.lightEntitiesList.push_back( entity_pl );
                lightData.lightData_map.insert( { entity_pl, std::make_pair( pos_pl.m_position, lightSpaceMatrix ) } );
                lightData.shadowMapID_map.insert( { entity_pl, lightProperty.shadowMapID } );
              }
            }
          //}
          //else
          //{
          //	lightData.isValid = false;
          //	return false;;
          //}
          }

          // Update light position and light space matrix
          lightData.lightData_map.find( entity_pl )->second.first = pos_pl.m_position;
          lightData.lightData_map.find( entity_pl )->second.second = lightSpaceMatrix;

          if ( mesh )
          {
            m_shader_shadow->SetUniform( "isSkeletalMesh", false );
            mesh->model->Draw();
          }
          else if ( amesh )
          {
            m_shader_shadow->SetUniform( "isSkeletalMesh", true );
            Gfx::Renderer::SetTransform( animator->animator.GetFinalBoneMatrices(), *m_shader_shadow );
            amesh->model->model.Draw();
          }
        }
        return false;
      } );

      glBindFramebuffer( GL_FRAMEBUFFER, 0 );
      glClear( GL_DEPTH_BUFFER_BIT );
    } );

    //glDisable(GL_DEPTH_TEST);
    m_shader_shadow->UnUse();
  }
}
}