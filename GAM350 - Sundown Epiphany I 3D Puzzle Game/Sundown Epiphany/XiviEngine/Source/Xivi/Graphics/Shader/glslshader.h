/*****************************************************************
*\file		 glslshader.h
*\brief		 Header file declarations for glslshader.

*\author(s)   Lee Liang Ping	51 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLSLSHADER_H
#define GLSLSHADER_H
#include <iostream>
/*                                                                   includes
----------------------------------------------------------------------------- */

namespace Xivi::Graphics
{
    /*  _________________________________________________________________________ */
class GLSLShader
    /*! GLSLShader class.
    */
{
public:

    // default ctor required to initialize GLSLShader object to safe state
  GLSLShader() : pgm_handle( 0 ), is_linked( GL_FALSE )
  {
    log_string = new std::string;
    delete log_string;
  }

  GLboolean CompileLinkValidate( std::vector<std::pair<GLenum, std::string>> );

  GLboolean CompileShaderFromFile( GLenum shader_type, std::string const &file_name );

  GLboolean CompileShaderFromString( GLenum shader_type, std::string const &shader_src );

  GLboolean Link();

  void Use();

  void UnUse();

  GLboolean Validate();

  // return the handle to the shader program object
  GLuint GetHandle() const;

  // have the different object code linked into a shader program?
  GLboolean IsLinked() const;

  std::string GetLog() const;

  void BindAttribLocation( GLuint index, GLchar const *name );

  void BindFragDataLocation( GLuint color_number, GLchar const *name );

  // as the name implies, this function deletes a program object
  void DeleteShaderProgram();

  void SetUniform( GLchar const *name, GLboolean val );
  void SetUniform( GLchar const *name, GLint val );
  void SetUniform( GLchar const *name, GLfloat val );
  void SetUniform( GLchar const *name, GLfloat x, GLfloat y );
  void SetUniform( GLchar const *name, GLfloat x, GLfloat y, GLfloat z );
  void SetUniform( GLchar const *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  void SetUniform( GLchar const *name, glm::vec2 &val );
  void SetUniform( GLchar const *name, glm::vec3 &val );
  void SetUniform( GLchar const *name, glm::vec4 &val );
  void SetUniform( GLchar const *name, glm::mat3 &val );
  void SetUniform( GLchar const *name, glm::mat4 &val );
  void SetUniform( GLchar const *name, Vec3f &val );
  void SetUniform( GLchar const *name, Vec4f &val );
  void SetUniform( GLchar const *name, Mat3x3 &val );
  void SetUniform( GLchar const *name, Mat4x4 &val );

  // display the list of active vertex attributes used by vertex shader
  void PrintActiveAttribs() const;

  // display the list of active uniform variables
  void PrintActiveUniforms() const;

private:
  enum ShaderType
  {
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
  };

  GLuint pgm_handle = 0;  // handle to linked shader program object
  GLboolean is_linked = GL_FALSE; // has the program successfully linked?
  std::string *log_string; // log for OpenGL compiler and linker messages

private:
  GLint GetUniformLocation( GLchar const *name );
  GLboolean FileExists( std::string const &file_name );
};
}
#endif /* GLSLSHADER_H */
