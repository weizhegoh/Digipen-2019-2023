/*****************************************************************
*\file         AnimationStateSystem.h
*\author(s)    Spencil Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct EventRelinkComponentSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::GlobalEvent<RelinkComponentEvent>
  {
    .m_name = "Relink Component System"
  };

  Ptr<RM> m_resource_manager;

  XV_INLINE void OnCreate() noexcept
  {
    m_resource_manager = &Service::Locator::Get<RM>();
  }

  XV_INLINE void OnEvent() noexcept
  {
    ECS::Query::Instance prefab;
    prefab.AllOf<PrefabTag>();

      // Static Mesh
    m_world.ForEach( [&]( StaticMeshComponent &mesh )
    {
      mesh.model = m_resource_manager->Get<Graphics::StaticModel>( mesh.model.Key() );
    } );

    m_world.ForEach( prefab, [&]( StaticMeshComponent &mesh )
    {
      mesh.model = m_resource_manager->Get<Graphics::StaticModel>( mesh.model.Key() );
    } );

// Animation
    m_world.ForEach( [&]( Entity &entity, AnimationMeshComponent &anim, AnimatorComponent &animator )
    {
      anim.model = m_resource_manager->Get<Graphics::AnimationModel>( anim.model.Key() );
      animator.animator.PlayAnimation( &anim.model->animation );
      anim.unitScales = Math::Vector3( 1.f );
       //XV_CORE_WARN("{}",anim.model.File());

      if ( !HaveComponent<Player>( entity ) )
      {
        animator.UpdateAnimation( 0 );
        animator.isActive = false;
      }
    } );

    m_world.ForEach( prefab, [&]( Entity &entity, AnimationMeshComponent &anim, AnimatorComponent &animator )
    {
      anim.model = m_resource_manager->Get<Graphics::AnimationModel>( anim.model.Key() );
      animator.animator.PlayAnimation( &anim.model->animation );
      anim.unitScales = Math::Vector3( 1.f );
      //XV_CORE_WARN("{}", m_world.m_entity_manager.m_entities[entity.Index()].m_name);

      if ( !HaveComponent<Player>( entity ) )
      {
        animator.UpdateAnimation( 0 );
        animator.isActive = false;
      }
    } );

// Material
    m_world.ForEach( [&]( Material &material )
    {
      material.diffuse = m_resource_manager->Get<Graphics::Texture>( material.diffuse.Key() );
      material.specular = m_resource_manager->Get<Graphics::Texture>( material.specular.Key() );
      material.normal = m_resource_manager->Get<Graphics::Texture>( material.normal.Key() );
      material.bump = m_resource_manager->Get<Graphics::Texture>( material.bump.Key() );
    } );

    m_world.ForEach( prefab, [&]( Material &material )
    {
      material.diffuse = m_resource_manager->Get<Graphics::Texture>( material.diffuse.Key() );
      material.specular = m_resource_manager->Get<Graphics::Texture>( material.specular.Key() );
      material.normal = m_resource_manager->Get<Graphics::Texture>( material.normal.Key() );
      material.bump = m_resource_manager->Get<Graphics::Texture>( material.bump.Key() );
    } );

// Skybox Material
    m_world.ForEach( [&]( SkyBoxMaterial &sbmaterial )
    {
      sbmaterial.skybox_LT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_LT.Key() );
      sbmaterial.skybox_RT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_RT.Key() );
      sbmaterial.skybox_DN = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_DN.Key() );
      sbmaterial.skybox_UP = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_UP.Key() );
      sbmaterial.skybox_BK = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_BK.Key() );
      sbmaterial.skybox_FT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_FT.Key() );
    } );
    m_world.ForEach( [&]( ParticleEmitter &emitter )
    {
      emitter.model = m_resource_manager->Get<Graphics::StaticModel>( emitter.model.Key() );
      emitter.diffuse = m_resource_manager->Get<Graphics::Texture>( emitter.diffuse.Key() );
    } );

    m_world.ForEach( prefab, [&]( SkyBoxMaterial &sbmaterial )
    {
      sbmaterial.skybox_LT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_LT.Key() );
      sbmaterial.skybox_RT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_RT.Key() );
      sbmaterial.skybox_DN = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_DN.Key() );
      sbmaterial.skybox_UP = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_UP.Key() );
      sbmaterial.skybox_BK = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_BK.Key() );
      sbmaterial.skybox_FT = m_resource_manager->Get<Graphics::Texture>( sbmaterial.skybox_FT.Key() );
    } );
    m_world.ForEach( prefab, [&]( ParticleEmitter &emitter )
    {
      emitter.model = m_resource_manager->Get<Graphics::StaticModel>( emitter.model.Key() );
      emitter.diffuse = m_resource_manager->Get<Graphics::Texture>( emitter.diffuse.Key() );
    } );

// Point light
    m_world.ForEach( [&]( Position &pos_pl, LightProperty &lightProperty, QuadraticFunction &qFunc )
    {
      auto &frameBuffer = Service::Locator::Get<Graphics::M_FrameBuffer>().GameFB()[0];
      frameBuffer.CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer.GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer.GetDepthMap();
    } );

    m_world.ForEach( prefab, [&]( Position &pos_pl, LightProperty &lightProperty, QuadraticFunction &qFunc )
    {
      auto &frameBuffer = Service::Locator::Get<Graphics::M_FrameBuffer>().GameFB()[0];
      frameBuffer.CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer.GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer.GetDepthMap();
    } );

    // Direction light
    m_world.ForEach( [&]( Position &pos_dl, LightProperty &lightProperty, Direction &dir )
    {
      auto &frameBuffer = Service::Locator::Get<Graphics::M_FrameBuffer>().GameFB()[0];
      frameBuffer.CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer.GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer.GetDepthMap();
    } );

    m_world.ForEach( prefab, [&]( Position &pos_dl, LightProperty &lightProperty, Direction &dir )
    {
      auto &frameBuffer = Service::Locator::Get<Graphics::M_FrameBuffer>().GameFB()[0];
      frameBuffer.CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer.GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer.GetDepthMap();
    } );
  }
};
}