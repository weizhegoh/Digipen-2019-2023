/*****************************************************************
*\file         Sundown.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Sundown.h"

#include "XiviEngine.h"
#include "Components/Components.h" // Must keep
#include "NEvents.h"
#include "NComponents.h"
#include "NSystems.h"
#include "HelperFunctions/InitFuncMapping.h"

namespace Xivi
{
class Sundown : public Application
{
  void Initialise() override
  {
    InitialiseEngineComponents();
    m_world->RegisterComponents<Sundown_NCList::Append<Game::StateComponent>>();
    m_world->RegisterComponents<Sundown_CList>();
    InitialiseEngineEvents();
    m_world->RegisterGlobalEvents<Sundown_NEList>();
    m_world->RegisterSystems<Sundown_NSList_Global>();
    m_world->RegisterSystems<Sundown_NSList_Notifier>();
    InitialiseEngineGlobalSystems();
    m_world->RegisterSystems<Sundown_NSList_Update>();
    InitialiseEngineUpdateSystems();
    FuncMapping::InitFuncMapping( *m_world, *m_canvas_stack, *m_resource_manager, Audio::AudioManager::Instance() );
  }
};

Ptr<Application> CreateApplication()
{
  return new Sundown();
}
}

void CreateDebugConsole() noexcept
{
  #ifndef XV_RELEASE
  AllocConsole();

  // Open the output stream
  freopen_s( (FILE **) stdout, "CONOUT$", "w", stdout );
  #endif
}

void FreeDebugConsole() noexcept
{
  #ifndef XV_RELEASE
  FreeConsole();
  #endif
}

void SundownEXE::Launch()
{
  CreateDebugConsole();
  auto app = Xivi::CreateApplication();
  app->Run();
  Xivi::Application::Release();
  FreeDebugConsole();
}