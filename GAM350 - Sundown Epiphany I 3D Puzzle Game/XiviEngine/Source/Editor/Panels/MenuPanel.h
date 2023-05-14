/*****************************************************************
*\file         MenuPanel.h
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
struct MenuPanel : public IPanel
{
  MenuPanel( std::string name,
             Ptr<ECS::World::Instance> world,
             Ptr<Config> configs,
             Ptr<EntityContext> entity_context,
             Ptr<FileContext> file_context,
             Ptr<ToolContext> tool_context,
             Ptr<std::vector<UniquePtr<IPanel>>> panels );

  ~MenuPanel();

  void Render() noexcept override;
  void SaveLevelAs( Ptr<ECS::World::Instance> world ) noexcept;
  void SaveLevel( Ptr<ECS::World::Instance> world ) noexcept;
  void OpenLevel( Ptr<ECS::World::Instance> world ) noexcept;
  void NewFile( Ptr<ECS::World::Instance> world ) noexcept;
  void FileShortcut( Ptr<ECS::World::Instance> world ) noexcept;
  void FileMenu( Ptr<ECS::World::Instance> world ) noexcept;
  void CopyEntity() noexcept;
  void PasteEntity( Ptr<ECS::World::Instance> world ) noexcept;
  void DuplicateEntityF( Ptr<ECS::World::Instance> world ) noexcept;
  void RedoFn() noexcept;
  void EditShortcut( Ptr<ECS::World::Instance> world ) noexcept;
  void EditMenu( Ptr<ECS::World::Instance> world ) noexcept;
  void WindowMenu( Ptr<ECS::World::Instance> world ) noexcept;

private:
  std::string project_path;
  Ptr<std::vector<UniquePtr<IPanel>>> m_panels;
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };
  Ptr<RM> m_resource_manager { nullptr };
};
}
#endif