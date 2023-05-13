/*****************************************************************
*\file         PrefabPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

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
struct System;
struct PrefabPanel : public IPanel
{
  PrefabPanel( std::string name,
               Ptr<ECS::World::Instance> world,
               Ptr<Config> configs,
               Ptr<EntityContext> entity_context,
               Ptr<FileContext> file_context,
               Ptr<ToolContext> tool_context );

  ~PrefabPanel();

  void Render() noexcept override;
  void AddPrefabFromFile() noexcept;
  void RenderPrefab( Entity prefab, const std::string prefab_name ) noexcept;
  void ModalPopup() noexcept;
  void RemovePrefab( Entity entity ) noexcept;

private:
  void CreatePrefab( const std::string filename ) noexcept;

  bool m_open_confirmation { false };
  Entity m_prefab_to_del;
  std::string prefab_path;
  bool m_override_prefab { false };
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };
  Ptr<System> m_editor_sys { nullptr };
  std::function<void( Entity )> m_render_prefab_components;
};
}
#endif