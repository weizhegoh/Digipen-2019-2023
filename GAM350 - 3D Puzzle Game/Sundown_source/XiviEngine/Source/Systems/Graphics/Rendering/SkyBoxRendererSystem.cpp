/*****************************************************************
*\file         SkyBoxRendererSystem.cpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "SkyBoxRendererSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void SkyBoxRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_skybox_shader = &m_graphic_manager->GetShaders( ShaderOption::SKYBOX_SHADER );
}

void  SkyBoxRendererSystem::operator()(
  Transform &transform,
  StaticMeshComponent &mesh,
  SkyBoxMaterial &material ) noexcept
{
  glDisable( GL_CULL_FACE );
  auto &graphicManager = *m_graphic_manager;
  auto &frameBuffer = graphicManager.frameBuffer;
  auto &camera = graphicManager.camera;
  m_skybox_shader->Use();
  m_skybox_shader->SetUniform( "gamma", m_graphic_manager->gamma );
  m_skybox_shader->SetUniform( "sky_lt", 0 );
  m_skybox_shader->SetUniform( "sky_rt", 1 );
  m_skybox_shader->SetUniform( "sky_dn", 2 );
  m_skybox_shader->SetUniform( "sky_up", 3 );
  m_skybox_shader->SetUniform( "sky_bk", 4 );
  m_skybox_shader->SetUniform( "sky_ft", 5 );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_LT->pTex );
  glActiveTexture( GL_TEXTURE1 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_RT->pTex );
  glActiveTexture( GL_TEXTURE2 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_DN->pTex );
  glActiveTexture( GL_TEXTURE3 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_UP->pTex );
  glActiveTexture( GL_TEXTURE4 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_BK->pTex );
  glActiveTexture( GL_TEXTURE5 );
  glBindTexture( GL_TEXTURE_2D, material.skybox_FT->pTex );

  Mat4x4 view = camera->ViewMat_3D();
  Mat4x4 proj = camera->ProjMat_3D();
  Mat4x4 pv = proj ^ view;
  {
    Mat4x4 _trans = Mat4x4(
      10000.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 10000.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 10000.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f );

    auto cameraEye = camera->CameraEye();
    _trans[3][0] = cameraEye.getX();
    _trans[3][1] = cameraEye.getY();
    _trans[3][2] = cameraEye.getZ();

    Mat4x4 _mvp = pv ^ _trans;
    Mat4x4 invTrans;
    Math::SIMD::inverse( _trans, invTrans );
    m_skybox_shader->SetUniform( "MVP", _mvp );
    m_skybox_shader->SetUniform( "InvModel", invTrans );
    mesh.model->Draw();
    m_skybox_shader->UnUse();
    Gfx::Renderer::UnBindTexture();
  }
  glEnable( GL_CULL_FACE );
  glCullFace( GL_BACK );
  glFrontFace( GL_CCW );
}
}