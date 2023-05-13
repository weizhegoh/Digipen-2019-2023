/*****************************************************************
*\file         LightRenderer.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "../Gfx/Gfx.h"
#include "../Shader/glslshader.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
namespace Xivi::Graphics
{
void Gfx::LightRenderer::SetLightOn( bool lightOn, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "lightOn", lightOn );
}
void Gfx::LightRenderer::SetBlinnPhong( bool blinn, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "blinn", blinn );
}
void Gfx::LightRenderer::SetViewPos( Vec3f viewPos, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "viewPos", viewPos );
}
void Gfx::LightRenderer::SetPointLights( PointLights &pointLights, GLSLShader &shdrpgm )
{
  for ( int i = 0; i < pointLights.size(); ++i )
    SetPointLight( pointLights[i], i, shdrpgm );
  shdrpgm.SetUniform( "pointLightNum", (int) pointLights.size() );
}
void Gfx::LightRenderer::SetPointLight( PointLight &pointLight, int i, GLSLShader &shdrpgm )
{
  std::string number = std::to_string( i );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].position" ).c_str(), pointLight.position );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].constant" ).c_str(), pointLight.constant );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].linear" ).c_str(), pointLight.linear );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].quadratic" ).c_str(), pointLight.quadratic );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].ambient" ).c_str(), pointLight.ambient );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].diffuse" ).c_str(), pointLight.diffuse );
  shdrpgm.SetUniform( ( "pointLights[" + number + "].specular" ).c_str(), pointLight.specular );
}

void Gfx::LightRenderer::SetDirLights( DirLights &dirLights, GLSLShader &shdrpgm )
{
  for ( int i = 0; i < dirLights.size(); ++i )
    SetDirLight( dirLights[i], i, shdrpgm );
  shdrpgm.SetUniform( "dirLightNum", (int) dirLights.size() );
}

void Gfx::LightRenderer::SetDirLight( DirLight &dirLight, int i, GLSLShader &shdrpgm )
{
  std::string number = std::to_string( i );
  shdrpgm.SetUniform( ( "dirLights[" + number + "].direction" ).c_str(), dirLight.direction );
  //shdrpgm.SetUniform(("dirLights[" + number + "].position").c_str(), dirLight.position);
  shdrpgm.SetUniform( ( "dirLights[" + number + "].ambient" ).c_str(), dirLight.ambient );
  shdrpgm.SetUniform( ( "dirLights[" + number + "].diffuse" ).c_str(), dirLight.diffuse );
  shdrpgm.SetUniform( ( "dirLights[" + number + "].specular" ).c_str(), dirLight.specular );
}

void Gfx::LightRenderer::SetSpotLight( Vec3f campos, Vec3f camDir, SpotLight &spotLight, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "spotLight.position", campos );
  shdrpgm.SetUniform( "spotLight.direction", camDir );
  shdrpgm.SetUniform( "spotLight.cutOff", glm::cos( glm::radians( spotLight.cutOff ) ) );
  shdrpgm.SetUniform( "spotLight.outerCutOff", glm::cos( glm::radians( spotLight.outerCutOff ) ) );
  shdrpgm.SetUniform( "spotLight.constant", spotLight.constant );
  shdrpgm.SetUniform( "spotLight.linear", spotLight.linear );
  shdrpgm.SetUniform( "spotLight.quadratic", spotLight.quadratic );
  shdrpgm.SetUniform( "spotLight.ambient", spotLight.ambient );
  shdrpgm.SetUniform( "spotLight.diffuse", spotLight.ambient );
  shdrpgm.SetUniform( "spotLight.specular", spotLight.ambient );
}

void Gfx::LightRenderer::SetShinniess( float shinness, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "material.shininess", shinness );
}

//void Gfx::LightRenderer::SetMaterial(Material material, GLSLShader& shdrpgm)
//{
//	shdrpgm.SetUniform("material.shininess", material.shinness);
//	//shdrpgm.SetUniform("material.specular", material.specular);
//	//shdrpgm.SetUniform("material.diffuse", material.diffuse);
//}
}