/*****************************************************************
*\file		 Gfx.cpp
*\brief		 Gfx functions definitions.

*\author(s)   Lee Liang Ping

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "../Gfx/Gfx.h"
#include "../Texture/Texture.h"
#include "../Shader/glslshader.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"

namespace Xivi::Graphics
{
void Gfx::Renderer::ToggleBlender( bool set )
{
  if ( set )
  {
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  }
  else
  {
    glDisable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  }
}

void Gfx::Renderer::ClearBitBuffer( unsigned int set )
{
  auto test = pow( 0.1f, 2.2f );
  glClearColor( test, test, test, 1.0f );
  glClear( set );
}

void Gfx::Renderer::BindTexture( GLuint texture )
{
  glBindTextureUnit( 6, texture );
  glTextureParameteri( texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTextureParameteri( texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTextureParameteri( texture, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTextureParameteri( texture, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

void Gfx::Renderer::BindVertexArray( GLuint vaoid )
{
  glBindVertexArray( vaoid );
}

void Gfx::Renderer::SetTransformLine(
  Math::SIMD::Mat4x4 &viewMxt44,
  Math::SIMD::Mat4x4 &projectionMxt44,
  GLSLShader &shdrpgm
)
{
  shdrpgm.SetUniform( "uView", viewMxt44 );
  shdrpgm.SetUniform( "uProjection", projectionMxt44 );
}

void Gfx::Renderer::SetTransform(
  Math::SIMD::Mat4x4 &modelMxt44,
  Math::SIMD::Mat4x4 &viewMxt44,
  Math::SIMD::Mat4x4 &projectionMxt44,
  GLSLShader &shdrpgm
)
{
  Math::Matrix4 model = modelMxt44.transpose();
  //Math::Matrix4 model = modelMxt44;
  shdrpgm.SetUniform( "uModel", model );
  shdrpgm.SetUniform( "uView", viewMxt44 );
  shdrpgm.SetUniform( "uProjection", projectionMxt44 );
}

void Gfx::Renderer::SetTransform( std::vector<glm::mat4> &transforms, GLSLShader &shdrpgm )
{
  for ( int i = 0; i < 60; ++i )
  {
    glm::mat4 glmMat4 = ( transforms[i] );
    std::string pass = "finalBonesMatrices[" + std::to_string( i ) + "]";
    shdrpgm.SetUniform( pass.c_str(), glmMat4 );
    //std::cout << glm::to_string(glmMat4) << std::endl;
  }
}

void Gfx::Renderer::SetTransform( std::array<Math::Matrix4, 60> &transforms, GLSLShader &shdrpgm )
{
  for ( int i = 0; i < 60; ++i )
  {
    //Math::Matrix4 trans = transforms[i].transpose();
    std::string pass = "finalBonesMatrices[" + std::to_string( i ) + "]";
    shdrpgm.SetUniform( pass.c_str(), transforms[i] );
  }
}

void Gfx::Renderer::DrawTexure( Mesh model )
{
  glDrawElements( model.primitive_type, model.draw_cnt, GL_UNSIGNED_SHORT, NULL );
}

void Gfx::Renderer::DrawArray( Mesh model, DrawOption option )
{
  GLenum pOption;
  if ( option == DrawOption::DEFAULT )
    pOption = model.primitive_type;
  else
    pOption = GetOption( option );
  glDrawArrays( pOption, NULL, model.draw_cnt );
}

void Gfx::Renderer::DrawElements( Mesh model, DrawOption option )
{
  GLenum pOption;
  if ( option == DrawOption::DEFAULT )
    pOption = model.primitive_type;
  else
    pOption = GetOption( option );
  glDrawElements( pOption, model.draw_cnt, GL_UNSIGNED_SHORT, NULL );
}

GLenum Gfx::Renderer::GetOption( DrawOption option )
{
  switch ( option )
  {
    case DrawOption::POINTS:
    return GL_POINTS;
    break;
    case DrawOption::LINE_STRIP:
    return GL_LINE_STRIP;
    break;
    case DrawOption::LINES:
    return GL_LINES;
    break;
    case DrawOption::DEFAULT:
    return GL_LINE_STRIP;
    break;
    default:
    return GL_NONE;
    break;
  }
}

void Gfx::Renderer::DeleteVAO( GLuint id )
{
  GLint max_vtx_attrib = 0;
  glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_vtx_attrib );
  glBindVertexArray( id );
  for ( int i = 0; i < max_vtx_attrib; ++i )
  {
    GLuint vbo_handle = 0;
    glGetVertexAttribIuiv( i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo_handle );
    if ( vbo_handle > 0 )
    {
      glDeleteBuffers( 1, &vbo_handle );
    }
  }
  glDeleteBuffers( 1, &id );
}
void Gfx::Renderer::BindTransparencyClamp( float &trans, GLSLShader &shdrpgm )
{
  Math::Helper::MathFunction::clamp( trans, 0.f, 1.f );
  shdrpgm.SetUniform( "transparency", trans );
}
void Gfx::Renderer::BindTransparency( float trans, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "transparency", trans );
}

void Gfx::Renderer::BindMaterialTexture(
  TexturePtr diffuse,
  TexturePtr spec,
  float shininess,
  GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "material.diffuse", 0 );
  OpenGLBindTexture( GL_TEXTURE0, diffuse->pTex );
  BindTexture( diffuse->pTex );

  shdrpgm.SetUniform( "material.specular", 1 );
  OpenGLBindTexture( GL_TEXTURE1, spec->pTex );
  shdrpgm.SetUniform( "material.shininess", shininess );
}
void Gfx::Renderer::UnBindTexture()
{
  OpenGLBindTexture( GL_TEXTURE0, 0 );
  OpenGLBindTexture( GL_TEXTURE1, 0 );
  OpenGLBindTexture( GL_TEXTURE2, 0 );
  OpenGLBindTexture( GL_TEXTURE3, 0 );
  OpenGLBindTexture( GL_TEXTURE4, 0 );
  OpenGLBindTexture( GL_TEXTURE5, 0 );
}

void Gfx::Renderer::OpenGLBindTexture( unsigned int loc, unsigned int sample )
{
  glActiveTexture( loc );
  glBindTexture( GL_TEXTURE_2D, sample );
}

void Gfx::Renderer::DrawElements( unsigned int vaoid, unsigned int eboid, unsigned int vboid, unsigned int draw_cnt )
{
  glBindVertexArray( vaoid );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eboid );
  glBindBuffer( GL_ARRAY_BUFFER, vboid );
  glDrawElements( GL_TRIANGLES, draw_cnt, GL_UNSIGNED_INT, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
}

void Gfx::Renderer::SetTransparency( float transparent, GLSLShader &shdrpgm )
{
  shdrpgm.SetUniform( "transparency", transparent );
}

void Gfx::Renderer::Render( GLSLShader &shdrpgm, std::vector<unsigned int> &texture, float width, float height, Mesh &mesh, float brightness, float contrast, Math::SIMD::Vec4 averageLumi )
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );
  glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
  shdrpgm.Use();
  shdrpgm.SetUniform( "screenTexture", 0 );
  shdrpgm.SetUniform( "Brightness", brightness );
  shdrpgm.SetUniform( "Contrast", contrast );
  shdrpgm.SetUniform( "AverageLuminance", averageLumi );
  for ( const auto &x : texture )
  {
    OpenGLBindTexture( GL_TEXTURE0, x );
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, x, 0 );
    glBindVertexArray( mesh.vaoid );
    //std::cout << mesh.vaoid << std::endl;

    glDrawElements( GL_TRIANGLES, mesh.draw_cnt, GL_UNSIGNED_SHORT, NULL );
  }

  glBindVertexArray( 0 );
  shdrpgm.UnUse();

  glDisable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_DEPTH_TEST );
}

void Gfx::Renderer::DrawLine( const Math::Vector3 &from, const Math::Vector3 &to, const Math::Vector3 &color,
                              unsigned int VAO, unsigned VBO )
{
  //// Vertex data
  GLfloat points[12];

  points[0] = from.getX();
  points[1] = from.getY();
  points[2] = from.getZ();
  points[3] = color.getX();
  points[4] = color.getY();
  points[5] = color.getZ();

  points[6] = to.getX();
  points[7] = to.getY();
  points[8] = to.getZ();
  points[9] = color.getX();
  points[10] = color.getY();
  points[11] = color.getZ();

  //GLuint VBO, VAO;
  //glDeleteBuffers(1, &VBO);
  //glDeleteVertexArrays(1, &VAO);
  //glGenBuffers(1, &VBO);
  //glGenVertexArrays(1, &VAO);
  //glBindVertexArray(VAO);
  //glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
  //glEnableVertexAttribArray(0);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  //glEnableVertexAttribArray(1);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  //glBindVertexArray(0);

  glBindVertexArray( VAO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points, GL_STATIC_DRAW );
  glLineWidth( 10.f );
  glDrawArrays( GL_LINES, 0, 2 );
  glBindVertexArray( 0 );
}

void Gfx::Renderer::DrawLine( const Math::Vector3 &from, const Math::Vector3 &to, const Math::Vector3 &color, float lineWidth )
{
  // Vertex data
  GLfloat points[12];

  points[0] = from.getX();
  points[1] = from.getY();
  points[2] = from.getZ();
  points[3] = color.getX();
  points[4] = color.getY();
  points[5] = color.getZ();

  points[6] = to.getX();
  points[7] = to.getY();
  points[8] = to.getZ();
  points[9] = color.getX();
  points[10] = color.getY();
  points[11] = color.getZ();

  GLuint VBO, VAO;
  glDeleteBuffers( 1, &VBO );
  glDeleteVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points, GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), 0 );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), (GLvoid *) ( 3 * sizeof( GLfloat ) ) );
  glBindVertexArray( 0 );

  glBindVertexArray( VAO );
  glLineWidth( lineWidth );
  glDrawArrays( GL_LINES, 0, 2 );
  glBindVertexArray( 0 );
}

void Gfx::Renderer::DrawGrid( const Math::Vector3 &color, int size )
{
  std::vector<glm::vec3> vertices;
  std::vector<glm::uvec4> indices;

  for ( int j = 0; j <= size; ++j )
  {
    for ( int i = 0; i <= size; ++i )
    {
      float x = (float) i / (float) size;
      float y = 0;
      float z = (float) j / (float) size;

      vertices.push_back( glm::vec3( x * 10, y, z * 10 ) );
    }
  }

  for ( int j = 0; j < size; ++j )
  {
    for ( int i = 0; i < size; ++i )
    {
      int row1 = j * ( size + 1 );
      int row2 = ( j + 1 ) * ( size + 1 );

      indices.push_back( glm::uvec4( row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1 ) );
      indices.push_back( glm::uvec4( row2 + i + 1, row2 + i, row2 + i, row1 + i ) );
    }
  }

  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  GLuint vbo;
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( glm::vec3 ), glm::value_ptr( vertices[0] ), GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

  GLuint ibo;
  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( glm::uvec4 ), glm::value_ptr( indices[0] ), GL_STATIC_DRAW );

  glBindVertexArray( 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  GLuint lenght = (GLuint) indices.size() * 4;

  glBindVertexArray( vao );
  glLineWidth( 10.f );
  glDrawElements( GL_LINES, lenght, GL_UNSIGNED_INT, NULL );
  glBindVertexArray( 0 );
}
}