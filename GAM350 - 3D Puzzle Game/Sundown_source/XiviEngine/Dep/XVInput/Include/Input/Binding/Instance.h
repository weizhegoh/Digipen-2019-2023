#pragma once

namespace Xivi
{

  struct InputBinding
  {
    Ptr<InputAction> m_action;
    InputActionBindingGroupMask m_binding_mask{InputActionBindingGroupMask::Default};
    std::string m_name;

    std::string GetActionName() const
    {
      return m_action.m_name;
    }

    InputActionID GetActionID() const
    {
      return m_action.m_id;
    }

    InputActionBindingGroupMask GetActionBindingGroupMask() const
    {
      return m_binding_mask;
    }

    void SetActionBindingGroupMask(InputActionBindingGroupMask mask)
    {
      m_binding_mask = mask;
    }

    InputActionBindingGroupMask AddActionBindingGroup(ActionBindingGroup group)
    {
      static_cast<u8 &>(m_binding_mask) |= static_cast<u8 &>(group);
    }

    void RemoveActionBindingGroup(ActionBindingGroup group)
    {
      static_cast<u8 &>(m_binding_mask) &= ~(static_cast<u8 &>(group));
    }
  }

}