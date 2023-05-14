/*****************************************************************
*\file         GridPanel.h
*\author(s)    Chng Yong Quan, Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"
#include "../Sundown/Source/NComponents/Entity/EntityType.h"

namespace Xivi
{
struct GameGrid; // Forward declare
}

namespace Xivi::Editor
{
struct GridPanel : public IPanel
{
  GridPanel( std::string name,
             Ptr<ECS::World::Instance> world,
             Ptr<Config> configs,
             Ptr<EntityContext> entity_context,
             Ptr<FileContext> file_context,
             Ptr<ToolContext> tool_context );

  ~GridPanel();

  void Render() noexcept override;

private:
  Ptr<System> m_editor_sys { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };

  std::function<void( Entity )> m_recur_push;
  std::function<void( Entity )> m_recur_update_animator;

  void ModalPopupInstanceCopy( Entity &prefab, const Vec3f &pos, const Vec3i &grid_pos, const GameType &type ) noexcept;
};
}
#endif