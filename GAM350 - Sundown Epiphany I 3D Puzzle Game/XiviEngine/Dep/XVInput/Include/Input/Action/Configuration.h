#pragma once

#include <utility>
#include <string>
#include <vector>

namespace Xivi
{

  struct ActionConfiguration
  {
    using ActionMapList = std::vector<ActionMap>;

    struct InputDeviceReference
    {
      InputDeviceType m_type;
      Ptr<InputDevice> m_device;
    };
    using InputDeviceReferenceList = std::vector<InputDeviceReference>;

    ActionMapList m_action_maps;
    ActionBindingGroupMask m_binding_mask;
    InputDeviceReferenceList m_input_devices;

    const ActionMapList &ListActionMap() const
    {
      return m_action_maps;
    }

    std::vector<Ptr<const InputBindingList>> ListBinding() const
    {
      std::vector<Ptr<const InputBindingList>> result;
      for (auto &map : m_action_maps)
        result.push_back(&map.ListBinding());
      return result;
    }

    void AddActionMap(const std::string &name)
    {
      m_action_maps.emplace_back(name);
    }

    void AddActionMap(ActionMap &&map)
    {
      m_action_maps.push_back(std::move(map));
    }

    void AddActionMap(const ActionMap &map)
    {
      m_action_maps.push_back(map);
    }

    void RemoveActionMap(const std::string &name)
    {
      std::erase_if(m_action_maps, [](ActionMap &map)
                    { return map.m_name == name; });
    }

    void RemoveActionMap(const ActionMapID id)
    {
      std::erase_if(m_action_maps, [](ActionMap &map)
                    { return map.m_id = id; });
    }

    void RemoveActionMap(const ActionMap &map)
    {
      std::erase(m_action_maps, map);
    }

    Ptr<ActionMap> GetActionMap(const std::string &name)
    {
      auto it = std::find_if(m_action_maps.begin(), m_action_maps.end(), [&](ActionMap &map)
                             { return map.m_name == name; });
      return it != m_action_maps.end() ? it : nullptr;
    }

    Ptr<ActionMap> GetActionMap(ActionMapID id)
    {
      auto it = std::find_if(m_action_maps.begin(), m_action_maps.end(), [&](ActionMap &map)
                             { return map.m_id == id; });
      return it != m_action_maps.end() ? it : nullptr;
    }

    const ActionBindingGroupMask &GetActionBindingGroupMask() const
    {
      return m_binding_mask;
    }

    void SetActionBindingGroupMask(const ActionBindingGroupMask mask)
    {
      m_binding_mask = mask;
    }

    void AddActionBindingGroup(const ActionBindingGroup group)
    {
      static_cast<u8 &>(m_binding_mask) |= static_cast<u8 &>(group);
    }

    void RemoveActionBindingGroup(const ActionBindingGroup group)
    {
      static_cast<u8 &>(m_binding_mask) &= ~(static_cast<u8 &>(group));
    }

    void AddInputDevice(Ptr<InputDevice> device)
    {
      m_input_devices.push_back({device.m_type, device});
    }

    void RemoveInputDevice(Ptr<InputDevice> device)
    {
      std::erase(m_input_devices, {device->m_type, device});
    }

    bool BindableWithDevice(Ptr<InputDevice> device)
    {
      for (auto& map : m_action_maps)
        if (map.BindableWithDevice(device))
          return true;
      return false;
    }

    const InputDeviceReferenceList &ListDevices() const
    {
      return m_input_devices;
    }

    Ptr<InputAction> GetAction(const std::string &name)
    {
      for (auto &map : m_action_maps)
        if (auto it = map.GetAction(name); it)
          return it;
      return nullptr;
    }

    Ptr<InputAction> GetAction(const InputActionID id)
    {
      for (auto &map : m_action_maps)
        if (auto it = map.GetAction(id); it)
          return it;
      return nullptr;
    }

    using InputBindingAndAction = std::pair<InputBinding, InputAction>;
    InputBindingAndAction GetBinding(InputBinding binding)
    {
      for (auto &map : m_action_maps)
        if (auto result = map.GetBinding(binding); result.index >= 0)
          return result;
    }

    bool Active() const
    {
      for (auto &map : m_action_maps)
        if (map.Active())
          return true;
      return false;
    }

    void Disable()
    {
      for (auto &map : m_action_maps)
        map.Disable();
    }

    void Enable()
    {
      for (auto &map : m_action_maps)
        map.Enable();
    }
  }

}