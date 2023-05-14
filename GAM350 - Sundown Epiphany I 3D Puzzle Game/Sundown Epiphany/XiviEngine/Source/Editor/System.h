/*****************************************************************
*\file         System.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Editor/Config.h"
#include "Editor/Contexts.h"
#include "Panels/IPanel.h"
#include "Editor/Command/CommandHistory.h"

namespace Xivi::Editor
{
namespace Concepts
{
template <typename TPanel>
concept IsPanel = std::derived_from<TPanel, IPanel>;
}

struct System : public Singleton<System>
{
  Config m_configs;
  EntityContext m_entity_context;
  FileContext m_file_context;
  ToolContext m_tool_context;
  CommandHistory m_command_history;

  void Update() noexcept;
  void Init() noexcept;
  void Free() noexcept;

  void AddEntity( Entity entity, bool is_prefab, bool is_ui ) noexcept;
  void RemoveEntity( Entity entity, bool is_prefab, bool is_ui ) noexcept;

  void OnPause() noexcept;
  void OnResume() noexcept;

  template <typename TPanel>
  Ptr<TPanel> GetPanel( std::string name ) noexcept
  {
    for ( auto &panel : m_panels )
    {
      if ( panel->GetName() == name )
        return std::bit_cast<Ptr<TPanel>>( panel.get() );
    }
    XV_CORE_ERROR( "No {} panel found", name );
    return nullptr;
  }

private:
  friend class Singleton<System>;

  System() = default;
  ~System() = default;

  std::vector<UniquePtr<IPanel>> m_panels;

  void PushAllPanels() noexcept;

  void InitBaseSystems() noexcept;
  void UpdateBaseSystems() noexcept;
};
}
#endif