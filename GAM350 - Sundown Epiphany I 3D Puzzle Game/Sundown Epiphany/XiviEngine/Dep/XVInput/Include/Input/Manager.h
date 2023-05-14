#pragma once

#include <array>
#include <vector>
#include <Types/GUID.h>

#ifdef XVMATH
// TBC
#else
namespace Xivi
{
struct Vec2f 
{
  f32 x,y; 
};
}
#endif


#include <Input/Device/Instance.h>
#include <Input/Device/Keyboard/Enum.h>
#include <Input/Device/Mouse/Enum.h>
#include <Input/Device/Gamepad/Enum.h>

#include <Input/Control/AnyKey.h> 
#include <Input/Control/Button.h>
#include <Input/Control/DPad.h>
#include <Input/Control/Integer.h>
#include <Input/Control/Float.h>
#include <Input/Control/Key.h>
#include <Input/Control/Vector2.h>

#include <Input/Device/Mouse.h>
#include <Input/Device/Keyboard.h>
#include <Input/Device/Gamepad.h>

namespace Xivi::Input
{

class Manager
{

using InputDevices = std::vector<UniquePtr<Device::Instance>>;
using InputDeviceList = std::vector<Ptr<Device::Instance>>;

 #ifdef XV_PLATFORM_WINDOWS
  Ptr<Device::Mouse> m_mouse;
  Ptr<Device::Keyboard> m_keyboard;
  #endif
  InputDevices m_devices;
  u8 m_mouse_count { 0 };
  u8 m_keyboard_count { 0 };
  u8 m_gamepad_count { 0 };

public:
  Manager()
  {
    #ifdef XV_PLATFORM_WINDOWS
    m_mouse = std::bit_cast<Ptr<Device::Mouse>>( AddDevice( Device::Type::Mouse, "DefaultMouse" ) );
    m_keyboard = std::bit_cast<Ptr<Device::Keyboard>>( AddDevice( Device::Type::Keyboard, "DefaultKeyboard" ) );
    #endif
  }

  void Update(f32 dt) noexcept
  {
    for ( auto &device : m_devices )
      device->Update(dt);
  }

  #ifdef XV_PLATFORM_WINDOWS
  Ptr<Device::Mouse> Mouse() noexcept
  {
    return m_mouse;
  }

  Ptr<Device::Keyboard> Keyboard() noexcept
  {
    return m_keyboard;
  }
  #endif

  Ptr<Device::Instance> AddDevice( Device::Type type, const Device::Name &name )
  {
    switch ( type )
    {
      case Device::Type::Mouse:
      return m_devices.emplace_back( MakeUnique<Device::Mouse>( type, name, m_mouse_count++ ) ).get();
      break;
      case Device::Type::Keyboard:
      return m_devices.emplace_back( MakeUnique<Device::Keyboard>( type, name, m_keyboard_count++ ) ).get();
      break;
      case Device::Type::Gamepad:
      return m_devices.emplace_back( MakeUnique<Device::Gamepad>( type, name, m_gamepad_count++ ) ).get();
      break;
    }
    return nullptr;
  }

  void RemoveDevice( const Device::Name &name )
  {
    RemoveDevice( Device::ID( Xivi::Hash::XXH3_64( name ) ) );
  }
  void RemoveDevice( Device::ID id )
  {
    std::erase_if( m_devices, [&]( UniquePtr<Device::Instance> &device )
    {
      return device->m_id == id;
    } );
  }

  void RemoveDisconnectedDevices()
  {
    std::erase_if( m_devices, []( UniquePtr<Device::Instance> &device )
    {
      return device->m_state == Device::State::Disconnected;
    } );
  }

  Ptr<Device::Instance> GetDevice( Device::Type type ) noexcept
  {
    for ( auto &device : m_devices )
      if ( device->m_type == type )
        return device.get();
    return nullptr;
  }

  Ptr<Device::Instance> GetDevice( const Device::Name &name ) noexcept
  {
    return GetDevice( Device::ID( Xivi::Hash::XXH3_64( name ) ) );
  }

  Ptr<Device::Instance> GetDevice( Device::ID id ) noexcept
  {
    for ( auto &device : m_devices )
      if ( device->m_id == id )
        return device.get();
    return nullptr;
  }

  InputDeviceList GetDevices( Device::Type type ) noexcept
  {
    InputDeviceList result;
    for ( auto &device : m_devices )
      if ( device->m_type == type )
        result.push_back( device.get() );
    return result;
  }

  template <Device::Concepts::IsDevice T>
  std::vector<Ptr<T>> GetDevices() noexcept
  {
    std::vector<Ptr<T>> result;
    for ( auto &device : m_devices )
    {
      if constexpr ( std::is_same_v<T, Device::Mouse> )
      {
        if ( device->m_type == Device::Type::Mouse )
          result.push_back( std::bit_cast<Ptr<T>>( device.get() ) );
      }
      else if constexpr ( std::is_same_v<T, Device::Keyboard> )
      {
        if ( device->m_type == Device::Type::Keyboard )
          result.push_back( std::bit_cast<Ptr<T>>( device.get() ) );
      }
      else if constexpr ( std::is_same_v<T, Device::Gamepad> )
      {
        if ( device->m_type == Device::Type::Gamepad )
          result.push_back( std::bit_cast<Ptr<T>>( device.get() ) );
      }
    }
    return result;
  }

  // void DisableAllInputActions();
  // void EnableAllInputActions();
  // InputActionList ListEnabledInputActions();

  InputDeviceList &ListDevices() noexcept
  {
    InputDeviceList result;
    for ( auto &device : m_devices )
      result.push_back( device.get() );
    return result;
  }

  InputDeviceList ListDisconnectedDevices() noexcept
  {
    InputDeviceList result;
    for ( auto &device : m_devices )
      if ( device->m_state == Device::State::Disconnected )
        result.push_back( device.get() );
    return result;
  }
};

// Explicit template instantiation
template std::vector<Ptr<Device::Mouse>> Manager::GetDevices<Device::Mouse>() noexcept;
template std::vector<Ptr<Device::Keyboard>> Manager::GetDevices<Device::Keyboard>() noexcept;
template std::vector<Ptr<Device::Gamepad>> Manager::GetDevices<Device::Gamepad>() noexcept;

}
