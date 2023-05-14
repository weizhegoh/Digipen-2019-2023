/*****************************************************************
*\file         ConsolePanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ConsolePanel.h"

#include "Editor/ImGui/TerminalCommands.h"
#include "imterm/terminal.hpp"

namespace Xivi::Editor
{
ConsolePanel::ConsolePanel( std::string name,
                            Ptr<ECS::World::Instance> world,
                            Ptr<Config> configs,
                            Ptr<EntityContext> entity_context,
                            Ptr<FileContext> file_context,
                            Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  terminal_cmd = MakeShared<custom_command_struct>();
  terminal = MakeShared<ImTerm::terminal<terminal_commands>>( *terminal_cmd, "Console" );

  // setting color for LE spdlog
  terminal->theme().log_level_colors[ImTerm::message::severity::trace] = ImTerm::theme::constexpr_color { 1.0f, 1.0f, 1.0f, 1.0f };
  terminal->theme().log_level_colors[ImTerm::message::severity::info] = ImTerm::theme::constexpr_color { 0.0f, 1.0f, 0.0f, 1.0f };
  terminal->theme().log_level_colors[ImTerm::message::severity::warn] = ImTerm::theme::constexpr_color { 1.0f, 1.0f, 0.0f, 1.0f };
  terminal->theme().log_level_colors[ImTerm::message::severity::err] = ImTerm::theme::constexpr_color { 1.0f, 0.0f, 0.0f, 1.0f };
  terminal->theme().log_level_colors[ImTerm::message::severity::critical] = ImTerm::theme::constexpr_color { 1.0f, 0.0f, 0.0f, 1.0f };
  terminal->set_max_log_len( 50 );
  auto terminal_helper = terminal->get_terminal_helper();
  terminal_helper->set_pattern( "%^[%T] %n (%l): %v%$" );
  Log::GetAppLogger()->sinks().push_back( terminal_helper );
  Log::GetCoreLogger()->sinks().push_back( terminal_helper );
}

ConsolePanel::~ConsolePanel()
{
  m_enabled = false;
}

void ConsolePanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  if ( showing_term )
    m_enabled = showing_term = terminal->show();
  else if ( m_enabled != showing_term )
    showing_term = m_enabled;
}
}
#endif