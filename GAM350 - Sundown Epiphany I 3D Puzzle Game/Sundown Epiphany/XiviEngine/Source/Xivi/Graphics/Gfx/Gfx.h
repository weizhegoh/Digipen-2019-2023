/*****************************************************************
*\file		 Gfx.h
*\brief		 Header file declarations for Gfx.

*\author(s)   Lee Liang Ping

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
//#include <glslshader.h>
//#include <stb_image.h>
//#include <FreeImage.h>
//#include <GraphicTypes.h>
//#include <Model.h>

//#include "../Shader/glslshader.h"
#include "../GraphicTypes.h"
#include "../Light/LightProperties.h"
#include "../Model/Mesh.h"
#include "../Texture/Texture.h"
// #define STB_IMAGE_IMPLEMENTATION

namespace Xivi::Graphics
{
class GLSLShader;
struct Gfx
{
  struct Renderer
  {
    static void ToggleBlender( bool set );
    static void ClearBitBuffer( unsigned int set );

    static void SetTransform(
      Math::SIMD::Mat4x4 &modelMxt44,
      Math::SIMD::Mat4x4 &viewMxt44,
      Math::SIMD::Mat4x4 &projectionMxt44,
      GLSLShader &shdrpgm );
    static void SetTransformLine(
      Math::SIMD::Mat4x4 &viewMxt44,
      Math::SIMD::Mat4x4 &projectionMxt44,
      GLSLShader &shdrpgm );

    static void SetTransform( std::vector<glm::mat4> &transforms, GLSLShader &shdrpgm );
    static void SetTransform( std::array<Math::Matrix4, 60> &transforms, GLSLShader &shdrpgm );

    static void DrawTexure( Mesh model );
    static void DrawArray( Mesh model, DrawOption option );
    static void DrawElements( Mesh model, DrawOption option );
    static void BindTexture( GLuint texture );
    static void BindVertexArray( GLuint vaoid );
    static void DeleteVAO( GLuint id );
    static void BindMaterialTexture( TexturePtr diffuse, TexturePtr spec, float shininess, GLSLShader &shdrpgm );
    static void BindTransparencyClamp( float &trans, GLSLShader &shdrpgm );
    static void BindTransparency( float trans, GLSLShader &shdrpgm );
    static void UnBindTexture();
    static void SetTransparency( float transparent, GLSLShader &shdrpgm );
    static void DrawLine( const Math::Vector3 &from, const Math::Vector3 &to, const Math::Vector3 &color, unsigned int VAO, unsigned VBO );
    static void DrawLine( const Math::Vector3 &from, const Math::Vector3 &to, const Math::Vector3 &color, float lineWidth );
    static void DrawGrid( const Math::Vector3 &color, int size );

    static void Render( GLSLShader &shdrpgm, std::vector<unsigned int> &texture, float width, float height, Mesh &mesh, float brightness, float contrast, Math::SIMD::Vec4 averageLumi );
    static void OpenGLBindTexture( unsigned int loc, unsigned int sample );
    static void DrawElements( unsigned int vaoid, unsigned int eboid, unsigned int vboid, unsigned int draw_cnt );
    Renderer()
    {}
    virtual ~Renderer()
    {}

//void SetLight(PointLight& pointLight, glm::vec3 position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm);
//void SetLight(PointLight& pointLight, mathEngine::Vector3D position, glm::vec3& colour, int& lightIndex, GLSLShader& shdrpgm);
  private:
    static GLenum GetOption( DrawOption option );
  };

  struct LightRenderer
  {
    static void SetLightOn( bool lightOn, GLSLShader &shdrpgm );
    static void SetBlinnPhong( bool blinn, GLSLShader &shdrpgm );
    static void SetPointLights( PointLights &pointLights, GLSLShader &shdrpgm );
    static void SetPointLight( PointLight &pointLight, int i, GLSLShader &shdrpgm );
    static void SetDirLights( DirLights &dirLights, GLSLShader &shdrpgm );
    static void SetDirLight( DirLight &dirLight, int i, GLSLShader &shdrpgm );
    static void SetSpotLight( Vec3f campos, Vec3f camDir, SpotLight &spotLight, GLSLShader &shdrpgm );
    static void SetShinniess( float shinness, GLSLShader &shdrpgm );
    static void SetViewPos( Vec3f viewPos, GLSLShader &shdrpgm );
  };
};
}