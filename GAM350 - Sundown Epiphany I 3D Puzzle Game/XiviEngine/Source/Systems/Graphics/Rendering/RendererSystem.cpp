/*****************************************************************
*\file         RendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "RendererSystem.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include <Resource/Manager.h>
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Gfx/Gfx.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
#include "Xivi/Graphics/Helper/ConversionHelper.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
namespace Xivi::Graphics
{
void RendererSystem::OnCreate() noexcept
{
  //quad = new
  m_resource_manager = &Service::Locator::Get<RM>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  #ifdef XV_EDITOR
  auto &arch = GetOrCreateArchetype<SkyBoxType>();
  arch.CreateEntity( [&]( Transform &xform, StaticMeshComponent &mesh, SkyBoxMaterial &material ) noexcept
  {
    xform.scale = Vec3f { 10.f,10.f,10.f };
    mesh.model = m_resource_manager->Get<Graphics::StaticModel>( "Assets/Models/binary/Object/skybox.BinaryObj" );
    material.skybox_LT = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_rt.dds" );
    material.skybox_RT = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_lf.dds" );
    material.skybox_DN = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_up.dds" );
    material.skybox_UP = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_dn.dds" );
    material.skybox_BK = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_bk.dds" );
    material.skybox_FT = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_ft.dds" );

    //material.skybox_LT = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/rt.dds");
    //material.skybox_RT = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/lf.dds");
    //material.skybox_DN = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/up.dds");
    //material.skybox_UP = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/dn.dds");
    //material.skybox_BK = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/bk.dds");
    //material.skybox_FT = m_resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/ft.dds");
  } );
  #endif

      //auto& gsm = Service::Locator::Get<GSM::GSManager>();
      //auto& cs = gsm.cutSceneFunc;
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_1.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_2.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_3.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_4.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_5.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_6.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_7.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_8.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_9.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_10.dds");
      //cs.AddScene("Scene1", "Assets/Textures/CutScene_11.dds");
}

void RendererSystem::OnPreUpdate() noexcept
{
  #ifndef XV_EDITOR
  //auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
  //auto &gsm = Service::Locator::Get<GSM::GSManager>();
  //if (keyboard.F3().Released())
  //	gsm.NextStage();
  //if (keyboard.F4().Released())
  //	gsm.cutSceneFunc.SkipScene();
  //if ( keyboard.R().Pressed() )
    //gsm.ChangeState( GSM::GS_RESTART );
  #endif
}

void RendererSystem::OnUpdate() noexcept
{
  auto &graphicManager = *m_graphic_manager;
  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &cs = gsm.cutSceneFunc;

  //if ( cs.isActive)
  //{
  //	return;
  //}
  if ( !gsm.runOnce && cs.isActive )
  {
    auto &frameBuffersManger = *m_framebuffer;
    auto &UIFrameBuffer = frameBuffersManger.FrameBufferUI();
    Graphics::Gfx::Renderer::ClearBitBuffer( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    UIFrameBuffer.Bind();
    glColorMask( TRUE, TRUE, TRUE, TRUE );
    glClearColor( 0, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    Graphics::Gfx::Renderer::ToggleBlender( true );
    UIFrameBuffer.Viewport( 0 );
    SendEvent<RendererUI>( this );
    UIFrameBuffer.Unbind();
    glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
    Graphics::Gfx::Renderer::ToggleBlender( false );
    return;
  }

  #endif
  auto &frameBuffersManger = *m_framebuffer;
  auto &frameBuffers = frameBuffersManger.GetList();
  auto &frameBuffer = graphicManager.frameBuffer;
  auto &camera = graphicManager.camera;

  for ( int i = 0; i < frameBuffers.size(); ++i )
  {
    //glEnable(GL_FRAMEBUFFER_SRGB);

    glColorMask( TRUE, TRUE, TRUE, TRUE );
    Graphics::Gfx::Renderer::ToggleBlender( true );
    frameBuffer = &frameBuffers[i];
    Graphics::Gfx::Renderer::ClearBitBuffer( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    frameBuffer->Bind();
    auto &camera_List = frameBuffer->CameraList();
    SendEvent<BuildShadowEvent>( this );
    glEnable( GL_DEPTH_TEST );
    for ( int j = 0; j < camera_List.size(); ++j )
    {
      camera = &camera_List[j];
      frameBuffer->Viewport( j );
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
      glFrontFace( GL_CCW );
      glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBuffer->GetFrameBufferID() );
      SendEvent<SkyBoxEvent>( this );
      if ( !graphicManager.wireFrameMode )
        SendEvent<ChildEvent>( this );
      else
        SendEvent<WireFrameChildEvent>( this );
      #ifdef XV_EDITOR
      SendEvent<DebugDrawEvent>( this );
      #endif
      glDisable( GL_CULL_FACE );
    }
    frameBuffer->Unbind();
    Graphics::Gfx::Renderer::ToggleBlender( false );

    //glDisable(GL_FRAMEBUFFER_SRGB);
  }

  glfwWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE );
  auto &UIFrameBuffer = frameBuffersManger.FrameBufferUI();
  Graphics::Gfx::Renderer::ClearBitBuffer( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  UIFrameBuffer.Bind();

  glColorMask( TRUE, TRUE, TRUE, TRUE );
  glClearColor( 0, 0, 0, 0 );
  glClear( GL_COLOR_BUFFER_BIT );
  Graphics::Gfx::Renderer::ToggleBlender( true );
  UIFrameBuffer.Viewport( 0 );
  SendEvent<RendererUI>( this );
  UIFrameBuffer.Unbind();
  glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
  Graphics::Gfx::Renderer::ToggleBlender( false );
}

void RendererSystem::OnPostUpdate() noexcept
{
  #ifndef XV_EDITOR
  auto windowWidth = (float) Service::Locator::Get<GLFW_Window>().Width();
  auto windowHeight = (float) Service::Locator::Get<GLFW_Window>().Height();
  auto &gm = *m_graphic_manager;
  auto &timer = Service::Locator::Get<TimerEngine>();
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  std::vector<unsigned int> image;
  auto &cs = gsm.cutSceneFunc;

  if ( cs.isActive )
  {
    // Controller if any
    auto device1 = Service::Locator::Get<Input::Manager>().GetDevice( "Gamepad[0]" );
    bool prev_cs = false;
    bool next_cs = false;
    if ( device1 )
    {
      auto &gamepad = *static_cast<Ptr<Input::Device::Gamepad>>( device1 );
      prev_cs = gamepad.DPadLeft().Released();
      next_cs = gamepad.DPadRight().Released() || gamepad.East().Released();
    }

    auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
    cs.PrevScene( keyboard.Left().Released() || prev_cs, timer.GetScaledDeltaTime() );
    cs.NextScene( keyboard.Right().Released() || keyboard.Space().Released() || next_cs, timer.GetScaledDeltaTime() );
    cs.GetCutScene( image, timer.GetScaledDeltaTime() );

    bool addscene = false;
    ECS::Query::Instance uiCutsceneOnly;
    uiCutsceneOnly.AllOf<CutSceneUI>();
    ForEach( uiCutsceneOnly, [&]( RenderSettings &renderSettings )
    {
      if ( renderSettings.onRender )
      {
        addscene = true;
        return;
      }
    } );
    if ( addscene )
      image.push_back( m_framebuffer->ImageUI( 0 ) );
  }
  else
  {
    image.push_back( m_framebuffer->ImageGame( 0 ) );
    image.push_back( m_framebuffer->ImageUI( 0 ) );
  }
  //auto& brightContrast = GetOrCreateSingletonComponent<BrightnessContrast>();
  //brightContrast.AverageLuminance = { 1.f, 1.f,1.f,1.f };
  //auto& keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
  //if (keyboard.B().Triggered())
  //	brightContrast.bright += .1f;
  //if (keyboard.N().Triggered())
  //	brightContrast.bright -= .1f;
  //if (keyboard.K().Triggered())
  //	brightContrast.contrast += .1f;
  //if (keyboard.L().Triggered())
  //	brightContrast.contrast -= .1f;

  Graphics::Gfx::Renderer::Render(
    gm.GetShaders( ShaderOption::QUAD ),
    image, windowWidth, windowHeight, gm.Quad,
    m_graphic_manager->bright, m_graphic_manager->contrast, m_graphic_manager->AverageLuminance );
  #endif
}
}