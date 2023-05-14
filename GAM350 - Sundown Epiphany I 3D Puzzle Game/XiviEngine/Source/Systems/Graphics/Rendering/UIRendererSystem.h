/*****************************************************************
*\file         UIRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Graphics/Helper/ConversionHelper.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
#include "Xivi/GUI/GUIManager.h"
#include "Components/GUIComponents.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
namespace Xivi::Graphics
{
struct UIRendererSystem : Xivi::ECS::System::Instance
{
  std::function<bool( Entity )> m_canvas_is_active;
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::RendererUI>
  {
    .m_id = 211,
    .m_name = "UI Renderer System"
  };

  void OnCreate() noexcept
  {
    m_canvas_is_active = [&]( Entity ent ) -> bool
    {
      if ( HaveComponent<GUI::CanvasTag>( ent ) && HaveComponent<GUI::GUIActiveTag>( ent ) )
        return true;
      if ( !HaveComponent<Parent>( ent ) )
        return false;
      return m_canvas_is_active( GetComponent<Parent>( ent ).m_entity );
    };
  }

  void OnPreUpdate() noexcept
  {
    auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
    if ( canvas_stack.Empty() || canvas_stack.Size() == 1 )
      return;

    auto device = Service::Locator::Get<Input::Manager>().GetDevice( "Gamepad[0]" );
    if ( device )
    {
      auto &gamepad = *static_cast<Ptr<Input::Device::Gamepad>>( device );
      if ( gamepad.AnyKey().Value() )
      {
        //Entity entity = canvas_stack.Pop();
        //auto &audio = GetComponent<AudioSource>( entity );
        //audio.Stop();
        ////AudioEngine::StopChannel(audio.m_channel);
        //audio.Play( "Assets/Sounds/GameplayBGM.wav" );
        //audio.m_loop = true;
      }
    }
    else
    {
      auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
      if ( keyboard.Space().Triggered() )
      {
        //Entity entity = canvas_stack.Pop();
        //auto &audio = GetComponent<AudioSource>( entity );
        //audio.Stop();
        ////AudioEngine::StopChannel(audio.m_channel);
        //audio.Play( "Assets/Sounds/GameplayBGM.wav" );
        //audio.m_loop = true;
      }
    }
  }

  void OnUpdate() noexcept
  {
    auto &canvas_container = Service::Locator::Get<GUI::Manager>();
    auto &frameBufferManager = Service::Locator::Get<Graphics::M_FrameBuffer>();
    auto &shdrpgm = Service::Locator::Get<Graphics::Manager>().GetShaders( ShaderOption::UI );
    #ifndef XV_EDITOR
    auto &gs = Service::Locator::Get<GSM::GSManager>();
    #endif
    auto &camera = frameBufferManager.CameraUI();
    #ifndef XV_EDITOR
    if ( gs.cutSceneFunc.isActive )
    {
      if ( gs.cutSceneFunc.GetIndex() == 0 )
        return;
      ECS::Query::Instance uiCutsceneOnly;
      uiCutsceneOnly.AllOf<CutSceneUI>();
      ForEach( uiCutsceneOnly, [&](
        ECS::Entity &entity, RenderSettings &renderSettings, Colour &colour, Material &material, Transform &transform, StaticMeshComponent &mesh )
      {
        if ( !renderSettings.onRender )
          return;
        shdrpgm.Use();
        Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
        shdrpgm.SetUniform( "uColor", colour.colour );
        shdrpgm.SetUniform( "material.diffuse", 0 );
        Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, shdrpgm );
        Gfx::Renderer::OpenGLBindTexture( GL_TEXTURE0, material.diffuse->pTex );
        Gfx::Renderer::BindTexture( material.diffuse->pTex );
        Gfx::Renderer::SetTransform( transform.LocalToWorld, camera.ViewMat_3D(), camera.ProjMat_3D(), shdrpgm );
        mesh.model->Draw();
        shdrpgm.UnUse();
        Gfx::Renderer::UnBindTexture();
        glClear( GL_DEPTH_BUFFER_BIT );
      } );
    }
    #endif

    for ( auto i = 0; i < canvas_container.Size(); ++i )
    {
      auto &entity = canvas_container[i];
      if ( !HaveComponent<Material>( entity ) || !HaveComponent<GUI::GUIActiveTag>( entity ) || !m_canvas_is_active( entity ) )
        continue;
      Position &pos = GetComponent<Position>( entity );
      Transform &transform = GetComponent<Transform>( entity );
      StaticMeshComponent &mesh = GetComponent<StaticMeshComponent>( entity );
      Material &material = GetComponent<Material>( entity );
      Colour &colour = GetComponent<Colour>( entity );
      RenderSettings &renderSettings = GetComponent<RenderSettings>( entity );
      UI &ui = GetComponent<UI>( entity );

      #ifdef XV_EDITOR
      auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
      if ( keyboard.Space().Triggered() )
      {
        camera.SetCameraEye( Math::Vector3( 0.f, 100.f, 100.f ) );
      }
      if ( keyboard.B().Triggered() )
      {
        camera.SetCameraEye( Math::Vector3( 0.f ) );
      }
      #endif

      Vec3f lightPos;
      shdrpgm.Use();
      Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
      shdrpgm.SetUniform( "uColor", colour.colour );
      shdrpgm.SetUniform( "material.diffuse", 0 );
      Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, shdrpgm );
      Gfx::Renderer::OpenGLBindTexture( GL_TEXTURE0, material.diffuse->pTex );
      Gfx::Renderer::BindTexture( material.diffuse->pTex );
      Gfx::Renderer::SetTransform( transform.LocalToWorld, camera.ViewMat_3D(), camera.ProjMat_3D(), shdrpgm );
      mesh.model->Draw();
      shdrpgm.UnUse();
      Gfx::Renderer::UnBindTexture();
      glClear( GL_DEPTH_BUFFER_BIT );
    }
  }

  XV_INLINE void OnPostUpdate() noexcept
  {}
};
}
