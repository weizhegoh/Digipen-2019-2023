/*****************************************************************
*\file         Application.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Application.h"

//Asset
#include "Xivi/Graphics/Model/Model.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Model/Animation/Animation.h"
#include <Resource/Manager.h>
#include "Xivi/Resources/EngineResource.h"

//Physics
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"

// Engine
#include "Xivi/Core/Timer/TimerEngine.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include "Xivi/Core/Utils/Filesystem/Watcher/SystemDirectory.h"
#include "Xivi/Core/Utils/Filesystem/Filesystem.h"
#include "Xivi/Core/Utils/Filesystem/FileDialog/FileDialog.h"

#include "Xivi/ECS/World.h"
#include <imgui.h>
#include <imgui_internal.h>

// System Engine
#include "Components/GUIComponents.h"
#include "EnginePackage.h"
#include "AudioPackage.h"
#include "GraphicsPackage.h"
#include "GUIPackage.h"

#ifdef XV_EDITOR
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include "Editor/System.h"
#include "Editor/EditorPackage.h"
#endif

namespace Xivi
{
#ifdef XV_EDITOR
using NotifierList =
Notifier_Editor_SList::Extend<
  Notifier_Graphics_SList>/*::Extend<
  Notifier_Physics_SList>*/::Extend<
  Notifier_Engine_SList>::Extend<
  Notifier_GUI_SList>;

using GlobalList =
Global_Editor_SList::Extend<
  Global_Graphics_SList>/*::Extend<
  Global_Physics_SList>*/::Extend<
  Global_Engine_SList>::Extend <
  Global_GUI_SList>;

using UpdateList =
Update_Engine_SList/*::Extend<
  Update_Physics_SList>*/::Extend<
  Update_Graphics_SList>::Extend<
  Update_Editor_SList>::Extend<
  Update_GUI_SList>;

using ComponentList = Engine_CList::Extend<Graphics_CList>/*::Extend<Physics_CList>*/::Extend<GUI_CList>::Extend<Audio_CList>::Extend<Particles_CList>;
using EventList = Engine_EList/*::Extend<Physics_EList>*/::Extend<Graphics_EList>::Extend<Editor_EList>::Extend<GUI_EList>::Extend<Audio_EList>;
#else
using NotifierList =
Notifier_Graphics_SList/*::Extend<
Notifier_Physics_SList>*/::Extend<
  Notifier_Engine_SList>::Extend<
  Notifier_GUI_SList>;

using GlobalList =
Global_Graphics_SList/*::Extend<
Global_Physics_SList>*/::Extend<
  Global_Engine_SList>::Extend<
  Global_GUI_SList>;

using UpdateList =
Update_Engine_SList/*::Extend<
Update_Physics_SList>*/::Extend <
  Update_Graphics_SList>::Extend <
  Update_GUI_SList>;

using ComponentList = Engine_CList::Extend<Graphics_CList>/*::Extend<Physics_CList>*/::Extend<GUI_CList>::Extend<Audio_CList>::Extend<Particles_CList>;
using EventList = Engine_EList::Extend<Graphics_EList>/*::Extend<Physics_EList>*/::Extend<GUI_EList>::Extend<Audio_EList>;
#endif

Ptr<Application> Application::s_application = nullptr;

void InitialiseDirectories() noexcept
{
  XV_CORE_INFO( "[Application] Initialising directories..." );
  // Create root folder
  if ( !Filesystem::dir_exists( "../SundownEpiphany" ) )
  {
    Filesystem::create_dir( "../SundownEpiphany" );
    XV_CORE_TRACE( "[Application] Created SundownEpiphany directory" );
  }
  // Make this folder the root
  Filesystem::root( "../SundownEpiphany" );
  XV_CORE_TRACE( "[Application] Made SundownEpiphany directory the root" );
  // Create Assets folders
  if ( !Filesystem::dir_exists( "Assets" ) )
  {
    Filesystem::create_dir( "Assets" );
    XV_CORE_TRACE( "[Application] Created Assets directory" );
  }
  if ( !Filesystem::dir_exists( "Assets/Shaders" ) )
  {
    Filesystem::create_dir( "Assets/Shaders" );
    XV_CORE_TRACE( "[Application] Created Shaders directory" );
  }
  if ( !Filesystem::dir_exists( "Assets/Models" ) )
  {
    Filesystem::create_dir( "Assets/Models" );
    XV_CORE_TRACE( "[Application] Created Models directory" );
  }
  if ( !Filesystem::dir_exists( "Assets/Textures" ) )
  {
    Filesystem::create_dir( "Assets/Textures" );
    XV_CORE_TRACE( "[Application] Created Textures directory" );
  }
  // Create Config folders
  if ( !Filesystem::dir_exists( "Config" ) )
  {
    Filesystem::create_dir( "Config" );
    XV_CORE_TRACE( "[Application] Created Config directory" );
  }
  // Mount to VFP
  Filesystem::Virtual::Instance().Mount( "Shaders", "Assets/Shaders" );
  Filesystem::Virtual::Instance().Mount( "Models", "Assets/Models" );
  Filesystem::Virtual::Instance().Mount( "Textures", "Assets/Textures" );
}

Application::Application( const Config &config )
{
  // Initialise logger and set default file sink for released mode
  Log::Init();
  #if defined(XV_DEV) && !defined(XV_EDITOR)
  Log::SetFileSink();
  #endif

  XV_CORE_INFO( "[Application] Logger initialised..." );
  XV_CORE_INFO( "[Application] Creating..." );
  XV_CORE_ASSERT_MSG( !s_application, "[Application] Application exists..." );
  s_application = this;

  InitialiseDirectories();

  XV_CORE_TRACE( "[Application] Registering timer engine" );
  m_timer = &Service::Locator::Register<TimerEngine>();

  XV_CORE_TRACE( "[Application] Registering input manager..." );
  m_input_manager = &Service::Locator::Register<Input::Manager>();
  #ifdef XV_PLATFORM_WINDOWS
  XV_CORE_TRACE( "[Application] Registering window..." );
  m_window = &Service::Locator::Register<GLFW_Window>( config.window_config );
  #endif

  XV_CORE_TRACE( "[Application] Registering resource manager..." );
  m_resource_manager = &Service::Locator::Register<RM>();

  XV_CORE_TRACE( "[Application] Registering graphics manager..." );
  Service::Locator::Register<Graphics::Manager>().InitManager( (float) m_window->Width(), (float) m_window->Height() );
  Service::Locator::Register<Graphics::M_FrameBuffer>().InitManager( (float) m_window->Width(), (float) m_window->Height() );

  auto model_loader = new ModelLoader;
  m_resource_manager->SetLoader<Graphics::StaticModel>( model_loader );
  model_loader->DefaultLoad( "Assets/Models/binary" );

  auto animation_model_loader = new AnimationModelLoader;
  m_resource_manager->SetLoader<Graphics::AnimationModel>( animation_model_loader );
  animation_model_loader->DefaultLoad( "Assets/Models/binary/Animation" );

  auto texture_loader = new TextureLoader;
  m_resource_manager->SetLoader<Graphics::Texture>( texture_loader );
  texture_loader->DefaultLoad( "Assets/Textures" );

  XV_CORE_TRACE( "[Application] Registering GUI manager..." );
  m_canvas_stack = &Service::Locator::Register<GUI::Manager>();

  XV_CORE_TRACE( "[Application] Registering ECS world..." );
  m_world = &Service::Locator::Register<ECS::World::Instance>();

  //XV_CORE_TRACE( "[Application] Initialising audio engine..." );
  //AudioEngine::Initialise();

  XV_CORE_TRACE( "[Application] Initialising audio engine..." );
  Audio::Init();

  #ifndef XV_EDITOR
  XV_CORE_TRACE( "[Application] Registering GSManager..." );
  Service::Locator::Register<GSM::GSManager>().InitManager( m_world, m_resource_manager, GSM::MAINMENU );
  #endif
}

Application::~Application()
{
  XV_CORE_INFO( "[Application] Destroying..." );
  XV_CORE_TRACE( "[Application] Shutting down audio engine..." );
  //AudioEngine::Shutdown();

  Audio::Quit();
  #ifdef XV_EDITOR
  XV_CORE_TRACE( "[Application] Freeing editor..." );
  Editor::System::Instance().Free();
  #endif
  XV_CORE_TRACE( "[Application] Releasing ECS world..." );
  Service::Locator::Shutdown();
}

void Application::InitialiseEngineComponents()
{
  m_world->RegisterComponents<ComponentList>();
  m_world->Initialise();
}

void Application::InitialiseEngineEvents()
{
  m_world->RegisterGlobalEvents<EventList>();
}

void Application::InitialiseEngineGlobalSystems()
{
  m_world->RegisterSystems<NotifierList>();
  m_world->RegisterSystems<GlobalList>();
}

void Application::InitialiseEngineUpdateSystems()
{
  m_world->RegisterSystems<UpdateList>();
}

void Application::Run()
{
  XV_CORE_INFO( "[Application] Run..." );
  XV_CORE_TRACE( "[Application] Initialising..." );
  Initialise();
  #ifdef XV_EDITOR
  XV_CORE_TRACE( "[Application] Initialising editor..." );
  Editor::System::Instance().Init();
  #endif
      //m_world->SendGlobalEvent<RelinkComponentEvent>();

  m_world->Start();
  #ifndef XV_EDITOR
  m_world->Resume();
  #endif

  auto glfw_window = std::bit_cast<Ptr<GLFWwindow>>( m_window->Handle() );
  m_timer->Initialise();
  glewInit();

  #ifdef XV_EDITOR
  while ( !glfwWindowShouldClose( glfw_window ) )
    #else
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &m_audio_manager = Audio::AudioManager::Instance();
  auto &m_GraphicManager = Service::Locator::Get<Graphics::Manager>();
  bool first = true;
  while ( !gsm.isQuit() )
    #endif
  {
    #ifndef XV_EDITOR
    if ( gsm.isRestart() )
    {
      gsm.once = false;
      gsm.SetCurrentNext_toPrev();
    }
    else
      gsm.once = true;

    m_world->Clear();
    m_world->DeserialiseWorld( gsm.GetCurrentStateFileName() );
    m_world->SendGlobalEvent<RelinkComponentEvent>();
    first = false;
    gsm.InitScene();
    if ( gsm.runOnce )
    {
      m_world->Run();
      gsm.runOnce = false;
    }

    while ( gsm.isSameState() )
    {
      if ( glfwWindowShouldClose( glfw_window ) )
        gsm.ChangeState( GSM::GS_QUIT );
      #endif
      m_timer->Update();
      m_input_manager->Update( m_timer->GetDeltaTime() );
      m_window->Process();
      m_world->Run();
      Audio::Update();

      #ifdef XV_EDITOR
      Editor::System::Instance().Update();
      #endif
      m_window->Update();
      XV_PROFILE_FRAMEMARKER_NAME( "Main Loop" );
      XV_PROFILE_FRAMEMARKER();
      m_timer->Reset();
      #ifndef XV_EDITOR
    }
    gsm.UpdateList();
    gsm.UpdateState();
    //m_audio_manager.StopAllAudio();
    #endif
  }

  XV_CORE_TRACE( "[Application] Stopping..." );
  m_world->Stop();
  Shutdown();
}
}