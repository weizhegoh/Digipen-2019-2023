/*****************************************************************
*\file         ConsolePanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace ImTerm
{
template <typename T>
class terminal;
}

class terminal_commands;
struct custom_command_struct;

inline Xivi::SharedPtr<ImTerm::terminal<terminal_commands>> terminal;
inline Xivi::SharedPtr<custom_command_struct> terminal_cmd;

namespace Xivi::Editor
{
struct ConsolePanel : public IPanel
{
  ConsolePanel( std::string name,
                Ptr<ECS::World::Instance> world,
                Ptr<Config> configs,
                Ptr<EntityContext> entity_context,
                Ptr<FileContext> file_context,
                Ptr<ToolContext> tool_context );

  ~ConsolePanel();

  void Render() noexcept override;
private:
  bool showing_term = true;
};
}
#endif