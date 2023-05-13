#pragma once

namespace Xivi::Input::Device
{

  enum class Type : u8
  {
    Mouse,
    Keyboard,
    Gamepad
  };

  enum class State : u8
  {
    Connected,
    Disconnected
  };

  using ID = GUID<struct InputDeviceID>;
  using Name = std::string;

  struct Instance
  {
    ID m_id;
    Name m_name;
    Type m_type;
    State m_state;
    u8 m_indexer;
    bool m_enabled;
    bool m_disconnected;
    bool m_any;

    Instance(Type type, const Name &name, u8 index) : m_id(Xivi::Hash::XXH3_64(name)),
                                                            m_name(name),
                                                            m_type(type),
                                                            m_state(State::Connected),
                                                            m_indexer(index),
                                                            m_enabled(true)
    {
    }

    virtual ~Instance() = default;

    ID GetID() const noexcept
    {
      return m_id;
    }

    u8 Index() const noexcept
    {
      return m_indexer;
    }

    Name DeviceName() const noexcept
    {
      return m_name;
    }

    Type DeviceType() const noexcept
    {
      return m_type;
    }

    bool Enabled() const noexcept
    {
      return m_enabled;
    }

    void Disable() noexcept
    {
      m_enabled = false;
    }

    void Enable() noexcept
    {
      m_enabled = true;
    }

    bool Disconnected() const noexcept
    {
      return m_disconnected;
    }

    void Disconnect() noexcept
    {
      m_disconnected = true;
    }

    void Connect() noexcept
    {
      m_disconnected = false;
    }

    virtual void Update(f32 dt) noexcept = 0;
    virtual void Clear() noexcept = 0;
  };

}

namespace Xivi::Input::Device::Concepts
{

template <typename T>
concept IsDevice = std::is_base_of_v<Device::Instance, T>;

}