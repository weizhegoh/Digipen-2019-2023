/*****************************************************************
*\file         Application.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <Core.h>
#include <Types/Base.h>
#include <Utils/NonCopyable.h>
#include "Config.h"
#include "Xivi/ECS/XVECS.h"
#include "Declaration.h"

namespace Xivi
{
class Application : public NonCopyable
{
public:

  struct Config
  {
    WindowConfig window_config {};
  };

  enum class AppState
  {
    Running,
    Loading,
    Paused,
    Stopped
  };

  Application( const Config &config = Config() );
  virtual ~Application();

  void InitialiseEngineComponents();
  void InitialiseEngineEvents();
  void InitialiseEngineGlobalSystems();
  void InitialiseEngineUpdateSystems();

  //void OpenProject( const std::string &path );
  //void NewProject( const std::string &path );

  XV_INLINE void Running( bool b )
  {
    m_running = b;
  }

  XV_INLINE bool Running()
  {
    return m_running;
  }

  XV_INLINE void State( AppState state )
  {
    m_state = state;
  }

  XV_INLINE AppState State()
  {
    return m_state;
  }

  XV_INLINE std::vector<std::filesystem::path> &DragDropFiles()
  {
    return m_dragdrop_files;
  }

  static Application &Get()
  {
    return *s_application;
  }

  static void Release()
  {
    if ( s_application )
      delete s_application;
    s_application = nullptr;
  }

  void Run();

  virtual void Initialise()
  {};
  virtual void Shutdown()
  {};
  virtual void Update( f32 dt )
  {
    (void) dt;
  };

protected:
  AppState m_state;
  Ptr<ECS::World::Instance> m_world;
  Ptr<Window> m_window;
  Ptr<RM> m_resource_manager;
  Ptr<Input::Manager> m_input_manager;
  Ptr<TimerEngine> m_timer;
  Ptr<GUI::Manager> m_canvas_stack;
  bool m_running;

private:
  std::vector<std::filesystem::path> m_dragdrop_files;
  static Ptr<Application> s_application;
};

// Defined in Application Projects
Ptr<Application> CreateApplication();
}