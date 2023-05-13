/*****************************************************************
*\file         GLFW_Window.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include <gl/glew.h>
#include "Xivi/Graphics/Graphics.h"
#include "GLFW_Window.h"
#include "GLFW_KeyCodes.h"
#include "Xivi/Core/Application.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi
{
static i32 s_glfw_windows_count = 0;
static bool s_glfw_initialised = false;

void ControllerSetup( i32 joy, i32 event )
{
  auto gamepads = Service::Locator::Get<Input::Manager>().GetDevices<Input::Device::Gamepad>();
  for ( auto &gamepad : gamepads )
    if ( gamepad->Index() == joy )
    {
      if ( event == GLFW_CONNECTED )
      {
        XV_CORE_TRACE( "({}) Connected!", gamepad->DeviceName() );
        gamepad->Enable();
        gamepad->Connect();
      }
      else
      {
        XV_CORE_TRACE( "({}) Disconnected!", gamepad->DeviceName() );
        gamepad->Disable();
        gamepad->Disconnect();
      }
      return;
    }

  auto gamepad = Service::Locator::Get<Input::Manager>().AddDevice( Input::Device::Type::Gamepad, "Gamepad[" + std::to_string( joy ) + "]" );
  XV_CORE_TRACE( "({}) Connected!", gamepad->DeviceName() );

  auto name = std::string( glfwGetGamepadName( joy ) );
  if ( name.find( "PS4" ) != std::string::npos )
    std::bit_cast<Ptr<Input::Device::Gamepad>>( gamepad )->API( Input::Device::Gamepad::InputAPI::PS4 );
}

GLFW_Window::GLFW_Window( const WindowConfig &c ) : Window( c )
{
  XV_CORE_INFO( "[GLFW_Windows] Creating..." );
  m_config.m_window = this;
  Initialise();
}

GLFW_Window::~GLFW_Window()
{
  XV_CORE_INFO( "[GLFW_Windows] Destroying..." );
  glfwDestroyWindow( m_handle );
  XV_CORE_TRACE( "[GLFW_Windows] Window destroyed" );
  if ( --s_glfw_windows_count < 1 )
  {
    XV_CORE_TRACE( "[GLFW_Windows] Terminating GLFW..." );
    s_glfw_initialised = false;
    glfwTerminate();
    XV_CORE_TRACE( "[GLFW_Windows] GLFW Terminated" );
  }
  XV_CORE_TRACE( "[GLFW_Windows] Destroyed" );
}

void GLFW_Window::Initialise()
{
  if ( !s_glfw_initialised )
  {
    XV_CORE_INFO( "[GLFW_Windows] Initialising..." );
    auto result = glfwInit();
    XV_CORE_ASSERT_MSG( result, "[GLFW_Windows] Failed to initialise GLFW" );
    glfwSetErrorCallback( []( i32 error, const char *description )
    {
      XV_CORE_ERROR( "[GLFW_Windows] GLFW error - {}: {}...", error, description );
    } );
    s_glfw_initialised = true;
  }
  ++s_glfw_windows_count;
  XV_CORE_TRACE( "[GLFW_Windows] Window count - {}", s_glfw_windows_count );

  #ifdef XV_RENDER_OPENGL
  if ( m_config.m_renderer_api == Graphics::RendererAPI::OpenGL )
  {
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE ); // window dimensions are static
    XV_CORE_TRACE( "[GLFW_Windows] Renderer - OpenGL" );
  }
  #endif
  #ifdef XV_RENDER_VULKAN
  if ( m_config.m_renderer_api == Graphics::RendererAPI::Vulkan )
    glfwWindowHi32( GLFW_CLIENT_API, GLFW_NO_API );
  #endif

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  f32 x_scale, y_scale;
  glfwGetMonitorContentScale( monitor, &x_scale, &y_scale );
  m_config.m_dpi = x_scale;
  XV_CORE_TRACE( "[GLFW_Windows] DPI - {}", m_config.m_dpi );

  Borderless( m_config.m_borderless );
  XV_CORE_TRACE( "[GLFW_Windows] Borderless - {}", m_config.m_borderless );
  XV_CORE_TRACE( "[GLFW_Windows] Fullscreen - {}", m_config.m_fullscreen );

  const GLFWvidmode *mode = glfwGetVideoMode( monitor );
  if ( m_config.m_fullscreen )
  {
    m_config.m_width = mode->width;
    m_config.m_height = mode->height;
  }
  glfwWindowHint( GLFW_RED_BITS, mode->redBits );
  glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
  glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
  glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

  XV_CORE_TRACE( "[GLFW_Windows] Window size - width({}), height({})", m_config.m_width, m_config.m_height );
  XV_CORE_TRACE( "[GLFW_Windows] Color bits - R({}), G({}), B({})", mode->redBits, mode->greenBits, mode->blueBits );
  XV_CORE_TRACE( "[GLFW_Windows] Referesh rate - {}", mode->refreshRate );

  XV_CORE_TRACE( "[GLFW_Windows]Creating window..." );
  if ( m_config.m_fullscreen && m_config.m_borderless )
    m_handle = glfwCreateWindow( m_config.m_width, m_config.m_height, m_config.m_title.c_str(), monitor, nullptr );
  else if ( m_config.m_fullscreen )
    m_handle = glfwCreateWindow( m_config.m_width, m_config.m_height, m_config.m_title.c_str(), nullptr, nullptr );
  else
    m_handle = glfwCreateWindow( m_config.m_width, m_config.m_height, m_config.m_title.c_str(), nullptr, nullptr );

  XV_CORE_ASSERT_MSG( m_handle, "[GLFW_Windows] Failed to create GLFW window" );

  glfwGetFramebufferSize( m_handle, &m_config.m_width, &m_config.m_height );

  #ifdef XV_RENDER_OPENGL
  if ( m_config.m_renderer_api == Graphics::RendererAPI::OpenGL )
  {
    XV_CORE_TRACE( "[GLFW_Windows] Making context current..." );
    glfwMakeContextCurrent( m_handle );
    XV_CORE_TRACE( "[GLFW_Windows] Initialising glew..." );
    const auto status = glewInit();
    if ( status != GLEW_OK )
      XV_CORE_ERROR( "[GLFW_Windows] Failed to initialise OpenGL context - {}", glewGetErrorString( status ) );
    else
      XV_CORE_TRACE( "[GLFW_Windows] GLEW version - {}", glewGetString( GLEW_VERSION ) );
  }
  #endif

  XV_CORE_TRACE( "[GLFW_Windows] Vsync - {}", m_config.m_vsync );
  if ( !m_config.m_vsync )
    glfwSwapInterval( 0 );

  #ifdef XV_PLATFORM_WINDOWS
      // SetIcon()
  #endif

  XV_CORE_TRACE( "[GLFW_Windows] Initialising GLFW callbacks..." );
  glfwSetWindowUserPointer( m_handle, &m_config );
  if ( glfwRawMouseMotionSupported() )
    glfwSetInputMode( m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE );
  glfwSetInputMode( m_handle, GLFW_STICKY_KEYS, true );
  glfwSetInputMode( m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
  m_config.m_focus = true;

  // Windows Callbacks
  glfwSetWindowCloseCallback( m_handle, []( Ptr<GLFWwindow> window )
  {
    WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );
    config.m_close = glfwWindowShouldClose( window );
  } );

  glfwSetWindowSizeCallback( m_handle, []( Ptr<GLFWwindow> window, i32 width, i32 height )
  {
    WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );

    glfwGetFramebufferSize( window, &config.m_width, &config.m_height );

    config.m_dpi = static_cast<f32>( config.m_width ) / static_cast<f32>( width );

    config.m_width = static_cast<i32>( static_cast<f32>( width ) * config.m_dpi );
    config.m_height = static_cast<i32>( static_cast<f32>( height ) * config.m_dpi );

    #ifndef XV_EDITOR
    auto &fm = Service::Locator::Get<Graphics::M_FrameBuffer>();
    auto &frameBuffers = fm.GetList();
    for ( int i = 0; i < frameBuffers.size(); ++i )
    {
      auto &camera_List = frameBuffers[i].CameraList();
      for ( int j = 0; j < camera_List.size(); ++j )
      {
        camera_List[j].GetViewportX() = static_cast<f32>( config.m_width );
        camera_List[j].GetViewportY() = static_cast<f32>( config.m_height );
      }
    }
    #endif
    glViewport( 0, 0, config.m_width, config.m_height );
  } );

  glfwSetWindowContentScaleCallback( m_handle, []( Ptr<GLFWwindow> window, f32 xscale, [[maybe_unused]] f32 yscale )
  {
    WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );
    config.m_window->DPIScale( xscale );
  } );

  glfwSetWindowIconifyCallback( m_handle, []( Ptr<GLFWwindow> window, i32 state )
  {
    WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );
    switch ( state )
    {
      case GL_TRUE:
      config.m_window->Focus( false );
      break;
      case GL_FALSE:
      config.m_window->Focus( true );
    }
  } );

  glfwSetWindowFocusCallback( m_handle, []( Ptr<GLFWwindow> window, i32 focused )
  {
    WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );
    config.m_window->Focus( static_cast<bool>( focused ) );
  } );

  glfwSetWindowRefreshCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window )
  {
  } );

  // Gamepad callback
  glfwSetJoystickCallback( ControllerSetup );

// Keys callback
  glfwSetKeyCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window,
                      i32 key,
                      [[maybe_unused]] i32 scancode,
                      i32 action,
                      [[maybe_unused]] i32 mods )
  {
    switch ( action )
    {
      case GLFW_PRESS:
      {
        Service::Locator::Get<Input::Manager>().Keyboard()->KeyUpdate( Input::GLFW::XVKey( key ), true );
        break;
      }
      case GLFW_REPEAT:
      {
        Service::Locator::Get<Input::Manager>().Keyboard()->KeyUpdate( Input::GLFW::XVKey( key ), true );
        break;
      }
      case GLFW_RELEASE:
      {
        Service::Locator::Get<Input::Manager>().Keyboard()->KeyUpdate( Input::GLFW::XVKey( key ), false );
      }
    }
  } );

  glfwSetCharCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, [[maybe_unused]] u32 keycode )
  {
  } );

  glfwSetMouseButtonCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, i32 button, i32 action, [[maybe_unused]] i32 mods )
  {
    switch ( action )
    {
      case GLFW_PRESS:
      {
        Service::Locator::Get<Input::Manager>().Mouse()->ButtonUpdate( Input::GLFW::XVMouse( button ), true );
        break;
      }
      case GLFW_RELEASE:
      {
        Service::Locator::Get<Input::Manager>().Mouse()->ButtonUpdate( Input::GLFW::XVMouse( button ), false );
      }
    }
  } );

  glfwSetCursorPosCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, f64 xPos, f64 yPos )
  {
    Service::Locator::Get<Input::Manager>().Mouse()->PositionUpdate( static_cast<f32>( xPos ), static_cast<f32>( yPos ) );
  } );

  glfwSetCursorEnterCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, [[maybe_unused]] i32 enter )
  {
    //WindowConfig &config = *static_cast<WindowConfig *>( ( glfwGetWindowUserPointer( window ) ) );
    //switch ( enter )
    //{
    //  //case GL_TRUE:
    //  //config.m_window->Focus( true );
    //  //break;
    //  //case GL_FALSE:
    //  //config.m_window->Focus( false );
    //}
  } );

  glfwSetScrollCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, f64 xOffset, f64 yOffset )
  {
    Service::Locator::Get<Input::Manager>().Mouse()->ScrollUpdate( static_cast<f32>( xOffset ), static_cast<f32>( yOffset ) );
  } );

  glfwSetDropCallback( m_handle, []( [[maybe_unused]] Ptr<GLFWwindow> window, i32 count, const char **paths )
  {
    auto &files = Application::Get().DragDropFiles();
    for ( auto i = 0; i < count; ++i )
      files.push_back( paths[i] );
  } );

  // Initial set-up to scan and connect existing controllers
  for ( i32 i = 0; i < GLFW_JOYSTICK_LAST; ++i )
    if ( glfwJoystickPresent( i ) == GLFW_TRUE )
      ControllerSetup( i, GLFW_CONNECTED );

  XV_CORE_TRACE( "[GLFW_Windows] GLFW version - {}", glfwGetVersionString() );

  if ( m_config.m_cursor )
    glfwSetInputMode( m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
  else
    glfwSetInputMode( m_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}

void GLFW_Window::Process()
{
  glfwPollEvents();

  auto gamepads = Service::Locator::Get<Input::Manager>().GetDevices<Input::Device::Gamepad>();
  for ( auto &gamepad : gamepads )
    if ( glfwJoystickPresent( gamepad->Index() ) == GLFW_TRUE )
    {
      i32 button_count;
      Ptr<const u8> buttons = glfwGetJoystickButtons( gamepad->Index(), &button_count );

      if ( buttons == nullptr || button_count == 0 )
        return;
      auto api = gamepad->GetAPI();
      for ( auto i = 0; i < button_count; ++i )
      {
        const i32 glfw_button_state = buttons[i];
        switch ( glfw_button_state )
        {
          case GLFW_PRESS:
          gamepad->ButtonUpdate( Input::GLFW::XVGamepadButton( i, api ), true );
          break;
          case GLFW_RELEASE:
          gamepad->ButtonUpdate( Input::GLFW::XVGamepadButton( i, api ), false );
        }
      }

      i32 axis_count;
      Ptr<const f32> axes = glfwGetJoystickAxes( gamepad->Index(), &axis_count );
      for ( auto i = 0; i < axis_count; ++i )
      {
        if ( Input::GLFW::XVGamepadAxis( i, api ) == Input::GamepadAxis::LeftY || Input::GLFW::XVGamepadAxis( i, api ) == Input::GamepadAxis::RightY )
          gamepad->AxisUpdate( Input::GLFW::XVGamepadAxis( i, api ), -axes[i] );
        else
          gamepad->AxisUpdate( Input::GLFW::XVGamepadAxis( i, api ), axes[i] );
      }
    }
}

void GLFW_Window::CursorPosition( const Vec2f &pos )
{
  glfwSetCursorPos( m_handle, pos[0], pos[1] );
}
}