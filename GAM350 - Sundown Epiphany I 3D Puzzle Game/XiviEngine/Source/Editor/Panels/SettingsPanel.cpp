/*****************************************************************
*\file         SettingsPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "SettingsPanel.h"

#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"

namespace Xivi::Editor
{
SettingsPanel::SettingsPanel( std::string name,
                              Ptr<ECS::World::Instance> world,
                              Ptr<Config> configs,
                              Ptr<EntityContext> entity_context,
                              Ptr<FileContext> file_context,
                              Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_timer_engine = &Service::Locator::Get<TimerEngine>();
}

SettingsPanel::~SettingsPanel()
{
  m_enabled = false;
}

void SettingsPanel::Render() noexcept
{
  if ( !m_enabled )
    return;
  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                 ImGui::GetIO().Framerate );

    bool engine_shown = false, graphic_shown = false, physics_shown = false, logic_shown = false;

    /*if (ImGui::BeginTable("##performance", 3, ImGuiTableFlags_Borders))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Editor:");
        for (auto& [id, data] : TimerEngine::Instance().m_system_fps)
        {
            if (id > 100 && id <= 199 && !engine_shown)
            {
                ImGui::TableNextRow();
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Engine:");
                engine_shown = true;
            }
            else if (id > 200 && id <= 299 && !graphic_shown)
            {
                ImGui::TableNextRow();
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Graphics:");
                graphic_shown = true;
            }
            else if (id > 300 && id <= 399 && !physics_shown)
            {
                ImGui::TableNextRow();
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Physic:");
                physics_shown = true;
            }
            else if (id > 400 && id <= 499 && !logic_shown)
            {
                ImGui::TableNextRow();
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Logic:");
                logic_shown = true;
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(data.second.c_str());
            ImGui::TableNextColumn();
            ImGui::ProgressBar(data.first / total_time);
        }
        ImGui::EndTable();
    }*/

    const auto &systems_performace = m_timer_engine->m_system_fps;

    const auto total_time = m_timer_engine->m_total_time;

    static const int column = 2;

    ImGuiHelper::Table( "##Perfomance_Editor", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "Editor:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id > 100 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );

    ImGuiHelper::Table( "##Performance_Engine", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "Engine:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id < 101 )
          continue;
        else if ( id > 200 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );

    ImGuiHelper::Table( "##Performance_Graphic", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "Graphic:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id < 201 )
          continue;
        else if ( id > 300 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );

    ImGuiHelper::Table( "##Performance_Physic", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "Physic:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id < 301 )
          continue;
        else if ( id > 400 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );

    ImGuiHelper::Table( "##Performance_Logic", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "Logic:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id < 401 )
          continue;
        else if ( id > 500 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );

    ImGuiHelper::Table( "##Performance_GUI", column, ImGuiTableFlags_BordersOuter, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text( "GUI:" );
      for ( auto &[id, data] : systems_performace )
      {
        if ( id < 501 )
          continue;
        else if ( id > 600 )
          break;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text( data.second.c_str() );
        ImGui::TableNextColumn();
        ImGui::ProgressBar( data.first / total_time );
      }
    } );
  } );
}
}
#endif