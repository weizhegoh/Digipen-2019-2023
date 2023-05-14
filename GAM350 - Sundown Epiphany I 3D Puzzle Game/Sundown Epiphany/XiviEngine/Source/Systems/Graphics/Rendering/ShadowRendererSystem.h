/*****************************************************************
*\file         ShadowRendererSystem.h
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi::Graphics
{
struct ShadowRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::BuildShadowEvent>
  {
    .m_id = 208,
    .m_name = "Shadow Renderer System"
  };

  f32 near_plane { 1.f };
  f32 far_plane { 200.f };
  f32 bound { 100.f };
  Vec3f v3f_view { 0.f,0.f,0.f };

  Ptr<Graphics::Manager> m_graphic_manager;
  Ptr<Graphics::M_FrameBuffer> m_framebuffer;
  Ptr<Graphics::GLSLShader> m_shader_texture;
  Ptr<Graphics::GLSLShader> m_shader_animation;
  Ptr<Graphics::GLSLShader> m_shader_shadow;;

  //Vec3f lightPos;
  //Mat4x4 lightView, lightSpaceMatrix;

  void OnCreate() noexcept;

  void OnUpdate() noexcept;
  //void OnUpdate() noexcept;

  /*void operator()(
    Position& pos,
    Transform& transform,
    Entity& entity,
    Material& material,
    Colour& colour,
    RenderSettings& renderSettings) noexcept;*/
};
}