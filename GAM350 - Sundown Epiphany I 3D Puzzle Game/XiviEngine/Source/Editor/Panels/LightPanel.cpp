/*****************************************************************
*\file         LightPanel.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "Xivi/ECS/XVECS.h"
#include "LightPanel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include "Systems/Graphics/Rendering/RendererSystem.h"
#include "Systems/Graphics/Rendering/ShadowRendererSystem.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Editor
{
LightPanel::LightPanel( std::string name,
                        Ptr<ECS::World::Instance> world,
                        Ptr<Config> configs,
                        Ptr<EntityContext> entity_context,
                        Ptr<FileContext> file_context,
                        Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{}

LightPanel::~LightPanel()
{
  m_enabled = false;
}

void LightPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    auto &ambient = m_world->GetOrCreateSingletonComponent<GlobalAmbient>().ambient;

    ImGui::Text( "Global Ambient" );
    ImGui::SameLine();
    ImGui::ColorEdit3( "##Global Ambient", ( Ptr<f32> ) & ambient );

    ImGui::Text( "Brightness" );
    ImGui::SameLine();
    ImGui::DragFloat( "##Brightness", &ambient[3] );
  } );
}
}
#endif