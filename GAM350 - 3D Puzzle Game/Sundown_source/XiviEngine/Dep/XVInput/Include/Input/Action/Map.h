#pragma once

namespace Xivi
{

  struct ActionMap
  {

    ActionMap() = default;
    ActionMap(std::string name) : m_name(name) {}

    using InputActionList = std::vector<InputAction>;
    using InputBindingList = std::vector<InputBinding>;
    using InputDeviceReferenceList = std::vector<InputDeviceReference>;
    using ActionMapID = GUID<ActionMap>;

    std::string m_name;
    ActionMapID m_id;

    Ptr<InputActionConfiguration> m_action_configuration;
    InputActionList m_actions;
    InputBindingList m_bindings;
    InputActionBindingGroupMask m_binding_mask{InputActionBindingGroupMask::Default};
    InputDeviceReferenceList m_devices;

    // TODO : Add internal state memory block for input tracking

    const std::string& Name() const
    {
      return m_name;
    }

    ActionMapID ID() const
    {
      return m_id;
    }

    const InputActionList &ListInputAction() const
    {
      return m_actions;
    }

    const InputBindingList &ListBinding() const
    {
      return m_bindings;
    }

    Ptr<InputActionConfiguration> Configuration()
    {
      return m_action_configuration;
    }

    Ptr<InputAction> GetAction(const std::string &name)
    {
      auto it = std::find_if(m_actions.begin(), m_actions.end(), [&](InputAction &action)
                             { return action.m_name == name; });
      return it != m_actions.end() ? it : nullptr;
    }

    Ptr<InputAction> GetAction(const InputActionID id)
    {
      auto it = std::find_if(m_actions.begin(), m_actions.end(), [&](InputAction &action)
                             { return action.m_id == id; });
      return it != m_actions.end() ? it : nullptr;
    }

    using InputBindingAndAction = std::pair<i32, Ptr<InputAction>>;
    InputBindingAndAction GetBinding(InputBinding binding)
    {
      for (auto& bind : m_bindings )
        if (bind.m_key == binding.m_key)
          return {bind.index, &bind.m_action};
        else if (bind.m_action == binding.m_action)
          return {bind.index, &bind.m_action};
        else if (bind.m_group_mask == binding.m_group_mask)
          return {bind.index, &bind.m_action};
      return {-1, nullptr};
    }

    bool BindableWithDevice(Ptr<InputDevice> device)
    {
      for (auto& bind : m_bindings)
        if (bind.BindableWithDevice(device))
          return true;
      return false;
    }

    bool Active()
    {
      for (auto &action : m_actions)
        if (action.m_enabled)
          return true;
      return false;
    }

    void Disable()
    {
      for (auto &action : m_actions)
        action.m_enabled = false;
    }

    void Enable()
    {
      for (auto &action : m_actions)
        action.m_enabled = true;
    }


  };

}