/*****************************************************************
*\file         ViewportPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"

namespace Xivi::Audio
{
class AudioManager;
}

namespace Xivi::GUI
{
class Manager;
}

namespace Xivi::Editor
{
struct ViewportPanel : public IPanel
{
  enum class GizmoState : u8
  {
    Default,
    Manipulating,
    Manipulated,
    Duplicating,
    ManipulatingDuplicated
  };

  ViewportPanel( std::string name,
                 Ptr<ECS::World::Instance> world,
                 Ptr<Config> configs,
                 Ptr<EntityContext> entity_context,
                 Ptr<FileContext> file_context,
                 Ptr<ToolContext> tool_context );

  ~ViewportPanel();

  void Render() noexcept override;

private:
  Entity m_duplicate_ref;
  Editor::State m_prev_state;
  GizmoState m_gizmo_state { GizmoState::Default };
  Ptr<RM> m_resource_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_frame_buffer { nullptr };
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };
  Ptr<Input::Device::Mouse> m_mouse { nullptr };
  Ptr<Input::Device::Keyboard> m_keyboard { nullptr };
  Ptr<System> m_editor_sys { nullptr };

  Input::Control::Key m_key_w;
  Input::Control::Key m_key_e;
  Input::Control::Key m_key_r;
  Input::Control::Key m_key_lalt;

  Input::Control::Button m_mouse_lb;
  Input::Control::Button m_mouse_rb;

  Input::Control::Vector2 m_mouse_pos;

  Input::Control::Integer m_mouse_click;

  std::function<void( Entity )> m_recur_push;
  std::function<void( Entity )> m_recur_update_animator;

  void ModalPopupInstanceCopy( Entity &prefab, Entity &new_ent, const Vec3f &pos ) noexcept;
};
}
#endif