/*****************************************************************
*\file         GUISystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/ECS/XVECS.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"

namespace Xivi::GUI
{
class Manager;
struct GUISystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
      .m_id = 501,
    .m_name = "GUI System"
  };

  ECS::Query::Instance m_canvas_query;

  struct ChildEvent : ECS::Event::Instance<>
  {};
/*struct GameGUIEvent : ECS::Event::Instance<> {};*/
  using Events = tuplet::tuple<ChildEvent/*, GameGUIEvent*/>;

  Ptr<Graphics::M_FrameBuffer> m_framebuffer;
  Ptr<Input::Manager> m_inputmanager;
  ECS::Query::Instance m_gui_function;
  ECS::Query::Instance m_canvas_tag;

  void OnCreate() noexcept;

  void OnPreUpdate() noexcept;

  void OnUpdate() noexcept;

  template <Traits::Concepts::InvokableFunction Func>
  void IsItemTrigger( AABB &aabb, Func func = EmptyLambda {} ) noexcept
  {
    if constexpr ( std::is_same_v<Func, EmptyLambda> )
      return;
    else
    {
      auto &mouse = *m_inputmanager->Mouse();
      auto pos = mouse.Position();
      if ( mouse.LeftButton().Triggered() )
      {
        #ifdef XV_EDITOR
        //	ImVec2 render_pos = /*ImGui::GetCursorScreenPos();*/
        //		//ImGui::GetCursorPos();
        //		ImGui::GetMousePos();
        //ImGui::getpos
        //	ImVec2 render_size = ImGui::GetContentRegionAvail();

        //	float cursorViewPortDistanceX = pos.X() - render_pos.x;
        //	float cursorViewPortDistanceY = pos.Y() - render_pos.y;
        //	float mousepoint_x = ((cursorViewPortDistanceX / render_size.x)) * (float)Service::Locator::Get<GLFW_Window>().Width();
        //	float mousepoint_y = ((cursorViewPortDistanceY / render_size.y)) * (float)Service::Locator::Get<GLFW_Window>().Height();

        float mousepoint_x = pos.X();
        float mousepoint_y = pos.Y();

        /*float mousepoint_x = Editor::ImGuiHelper::s_mouse_pos_offsetted.x;
        float mousepoint_y = Editor::ImGuiHelper::s_mouse_pos_offsetted.y;*/
        #else
        float mousepoint_x = pos.X();
        float mousepoint_y = pos.Y();
        #endif

        float t = -1.f;
        float min_t = 0.f;
        if ( m_framebuffer->GetPickingAABBCheck(
          mousepoint_x,
          mousepoint_y,
          (float) Service::Locator::Get<GLFW_Window>().Width(),
          (float) Service::Locator::Get<GLFW_Window>().Height(),
          aabb, t ) )
        {
          // if pressed
          if ( t < min_t )
          {
            func();
          }
        }
      }
    }

    //if constexpr (std::is_same_v<Func, EmptyLambda>)
    //	return;
    //else
    //{
    //	auto& mouse = *m_inputmanager->Mouse();
    //	auto pos = mouse.Position();
    //	if ( mouse.LeftButton().Triggered() )
    //	{
    //		float t = -1.f;
    //		float min_t = 0.f;
    //		if ( m_framebuffer->GetPickingAABBCheck(
    //			pos.X(),
    //			pos.Y(),
    //			(float) Service::Locator::Get<GLFW_Window>().Width(),
    //			(float) Service::Locator::Get<GLFW_Window>().Height(),
    //			aabb, t ) )
    //		{
    //			// if pressed
    //			if ( t < min_t )
    //			{
    //				func();
    //			}
    //		}
    //	}
    //}
  }

  template <Traits::Concepts::InvokableFunction Func>
  void IsItemHover( AABB &aabb, Func func = EmptyLambda {} ) noexcept
  {
    if constexpr ( std::is_same_v<Func, EmptyLambda> )
      return;
    else
    {
      auto &mouse = *m_inputmanager->Mouse();
      auto pos = mouse.Position();

      #ifdef XV_EDITOR
        //	ImVec2 render_pos = /*ImGui::GetCursorScreenPos();*/
        //		//ImGui::GetCursorPos();
        //		ImGui::GetMousePos();
        //ImGui::getpos
        //	ImVec2 render_size = ImGui::GetContentRegionAvail();

        //	float cursorViewPortDistanceX = pos.X() - render_pos.x;
        //	float cursorViewPortDistanceY = pos.Y() - render_pos.y;
        //	float mousepoint_x = ((cursorViewPortDistanceX / render_size.x)) * (float)Service::Locator::Get<GLFW_Window>().Width();
        //	float mousepoint_y = ((cursorViewPortDistanceY / render_size.y)) * (float)Service::Locator::Get<GLFW_Window>().Height();

      float mousepoint_x = pos.X();
      float mousepoint_y = pos.Y();

      /*float mousepoint_x = Editor::ImGuiHelper::s_mouse_pos_offsetted.x;
      float mousepoint_y = Editor::ImGuiHelper::s_mouse_pos_offsetted.y;*/

      #else
      float mousepoint_x = pos.X();
      float mousepoint_y = pos.Y();
      #endif

      float t = -1.f;
      float min_t = 0.f;
      if ( m_framebuffer->GetPickingAABBCheck(
        mousepoint_x,
        mousepoint_y,
        (float) Service::Locator::Get<GLFW_Window>().Width(),
        (float) Service::Locator::Get<GLFW_Window>().Height(),
        aabb, t ) )
      {
        // if pressed
        if ( t < min_t )
        {
          func();
        }
      }
    }
  }

  template <Traits::Concepts::InvokableFunction Func>
  void IsItemHold( AABB &aabb, Func func = EmptyLambda {} ) noexcept
  {
    if constexpr ( std::is_same_v<Func, EmptyLambda> )
      return;
    else
    {
      auto &mouse = *Service::Locator::Get<Input::Manager>().Mouse();
      auto pos = mouse.Position();
      if ( mouse.LeftButton().Pressed() )
      {
        float t = -1.f;
        float min_t = 0.f;
        if ( m_framebuffer->GetPickingAABBCheck(
          pos.X(),
          pos.Y(),
          (float) Service::Locator::Get<GLFW_Window>().Width(),
          (float) Service::Locator::Get<GLFW_Window>().Height(),
          aabb, t ) )
        {
          // if pressed
          if ( t < min_t )
          {
            func();
          }
        }
      }
    }
  }
};
};
