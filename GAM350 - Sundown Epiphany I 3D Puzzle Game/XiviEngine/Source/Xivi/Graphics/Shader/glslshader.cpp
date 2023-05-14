/*****************************************************************
*\file		 glslshader.cpp
*\brief		 glslshader functions definitions.

*\author(s)   Lee Liang Ping	325 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "glslshader.h"

namespace Xivi::Graphics
{
GLint
GLSLShader::GetUniformLocation( GLchar const *name )
{
  return glGetUniformLocation( pgm_handle, name );
}

GLboolean
GLSLShader::FileExists( std::string const &file_name )
{
  std::ifstream infile( file_name ); return infile.good();
}

void
GLSLShader::DeleteShaderProgram()
{
  if ( pgm_handle > 0 )
  {
    glDeleteProgram( pgm_handle );
  }
}

GLboolean
GLSLShader::CompileLinkValidate( std::vector<std::pair<GLenum, std::string>> vec )
{
  for ( auto &elem : vec )
  {
    if ( GL_FALSE == CompileShaderFromFile( elem.first, elem.second.c_str() ) )
    {
      return GL_FALSE;
    }
  }
  if ( GL_FALSE == Link() )
  {
    return GL_FALSE;
  }
  if ( GL_FALSE == Validate() )
  {
    return GL_FALSE;
  }
  PrintActiveAttribs();
  PrintActiveUniforms();

  return GL_TRUE;
}

GLboolean
GLSLShader::CompileShaderFromFile( GLenum shader_type, const std::string &file_name )
{
  if ( GL_FALSE == FileExists( file_name ) )
  {
    *log_string = "File not found";
    return GL_FALSE;
  }
  if ( pgm_handle <= 0 )
  {
    pgm_handle = glCreateProgram();
    if ( 0 == pgm_handle )
    {
      *log_string = "Cannot create program handle";
      return GL_FALSE;
    }
  }

  std::ifstream shader_file( file_name, std::ifstream::in );
  if ( !shader_file )
  {
    *log_string = "Error opening file " + file_name;
    return GL_FALSE;
  }
  std::stringstream buffer;
  buffer << shader_file.rdbuf();
  shader_file.close();
  return CompileShaderFromString( shader_type, buffer.str() );
}

GLboolean
GLSLShader::CompileShaderFromString( GLenum shader_type,
                                     const std::string &shader_src )
{
  if ( pgm_handle <= 0 )
  {
    pgm_handle = glCreateProgram();
    if ( 0 == pgm_handle )
    {
      *log_string = "Cannot create program handle";
      return GL_FALSE;
    }
  }

  GLuint shader_handle = 0;
  switch ( shader_type )
  {
    case VERTEX_SHADER: shader_handle = glCreateShader( GL_VERTEX_SHADER ); break;
    case FRAGMENT_SHADER: shader_handle = glCreateShader( GL_FRAGMENT_SHADER ); break;
    case GEOMETRY_SHADER: shader_handle = glCreateShader( GL_GEOMETRY_SHADER ); break;
    case TESS_CONTROL_SHADER: shader_handle = glCreateShader( GL_TESS_CONTROL_SHADER ); break;
    case TESS_EVALUATION_SHADER: shader_handle = glCreateShader( GL_TESS_EVALUATION_SHADER ); break;
    default:
    *log_string = "Incorrect shader type";
    return GL_FALSE;
  }

  // load shader source code into shader object
  GLchar const *shader_code[] = { shader_src.c_str() };
  glShaderSource( shader_handle, 1, shader_code, NULL );

  // compile the shader
  glCompileShader( shader_handle );

  // check compilation status
  GLint comp_result;
  glGetShaderiv( shader_handle, GL_COMPILE_STATUS, &comp_result );
  if ( GL_FALSE == comp_result )
  {
    *log_string = "Vertex shader compilation failed\n";
    GLint log_len;
    glGetShaderiv( shader_handle, GL_INFO_LOG_LENGTH, &log_len );
    if ( log_len > 0 )
    {
      GLchar *log = new GLchar[log_len];
      GLsizei written_log_len;
      glGetShaderInfoLog( shader_handle, log_len, &written_log_len, log );
      *log_string += log;
      delete[] log;
    }
    return GL_FALSE;
  }
  else
  { // attach the shader to the program object
    glAttachShader( pgm_handle, shader_handle );
    return GL_TRUE;
  }
}

GLboolean GLSLShader::Link()
{
  if ( GL_TRUE == is_linked )
  {
    return GL_TRUE;
  }
  if ( pgm_handle <= 0 )
  {
    return GL_FALSE;
  }

  glLinkProgram( pgm_handle ); // link the various compiled shaders

  // verify the link status
  GLint lnk_status;
  glGetProgramiv( pgm_handle, GL_LINK_STATUS, &lnk_status );
  if ( GL_FALSE == lnk_status )
  {
    *log_string = "Failed to link shader program\n";
    GLint log_len;
    glGetProgramiv( pgm_handle, GL_INFO_LOG_LENGTH, &log_len );
    if ( log_len > 0 )
    {
      GLchar *log_str = new GLchar[log_len];
      GLsizei written_log_len;
      glGetProgramInfoLog( pgm_handle, log_len, &written_log_len, log_str );
      *log_string += log_str;
      delete[] log_str;
    }
    return GL_FALSE;
  }
  return is_linked = GL_TRUE;
}

void GLSLShader::Use()
{
  if ( pgm_handle > 0 && is_linked == GL_TRUE )
  {
    glUseProgram( pgm_handle );
  }
}

void GLSLShader::UnUse()
{
  glUseProgram( 0 );
}

GLboolean GLSLShader::Validate()
{
  if ( pgm_handle <= 0 || is_linked == GL_FALSE )
  {
    return GL_FALSE;
  }

  glValidateProgram( pgm_handle );
  GLint status;
  glGetProgramiv( pgm_handle, GL_VALIDATE_STATUS, &status );
  if ( GL_FALSE == status )
  {
    *log_string = "Failed to validate shader program for current OpenGL context\n";
    GLint log_len;
    glGetProgramiv( pgm_handle, GL_INFO_LOG_LENGTH, &log_len );
    if ( log_len > 0 )
    {
      GLchar *log_str = new GLchar[log_len];
      GLsizei written_log_len;
      glGetProgramInfoLog( pgm_handle, log_len, &written_log_len, log_str );
      *log_string += log_str;
      delete[] log_str;
    }
    return GL_FALSE;
  }
  else
  {
    return GL_TRUE;
  }
}

GLuint GLSLShader::GetHandle() const
{
  return pgm_handle;
}

GLboolean GLSLShader::IsLinked() const
{
  return is_linked;
}

std::string GLSLShader::GetLog() const
{
  return *log_string;
}

void GLSLShader::BindAttribLocation( GLuint index, GLchar const *name )
{
  glBindAttribLocation( pgm_handle, index, name );
}

void GLSLShader::BindFragDataLocation( GLuint color_number, GLchar const *name )
{
  glBindFragDataLocation( pgm_handle, color_number, name );
}

void GLSLShader::SetUniform( GLchar const *name, GLboolean val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform1i( loc, val );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, GLint val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform1i( loc, val );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, GLfloat val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform1f( loc, val );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, GLfloat x, GLfloat y )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform2f( loc, x, y );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, GLfloat x, GLfloat y, GLfloat z )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform3f( loc, x, y, z );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void
GLSLShader::SetUniform( GLchar const *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform4f( loc, x, y, z, w );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, glm::vec2 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform2f( loc, val.x, val.y );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, glm::vec3 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform3f( loc, val.x, val.y, val.z );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, glm::vec4 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform4f( loc, val.x, val.y, val.z, val.w );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, Vec3f &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform3f( loc, val.getX(), val.getY(), val.getZ() );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, Vec4f &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniform4f( loc, val.getX(), val.getY(), val.getZ(), val.getW() );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}
void GLSLShader::SetUniform( GLchar const *name, Mat3x3 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniformMatrix3fv( loc, 1, GL_FALSE, &val[0][0] );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}
void GLSLShader::SetUniform( GLchar const *name, Mat4x4 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniformMatrix4fv( loc, 1, GL_FALSE, &val[0][0] );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, glm::mat3 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniformMatrix3fv( loc, 1, GL_FALSE, &val[0][0] );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::SetUniform( GLchar const *name, glm::mat4 &val )
{
  GLint loc = glGetUniformLocation( pgm_handle, name );
  if ( loc >= 0 )
  {
    glUniformMatrix4fv( loc, 1, GL_FALSE, &val[0][0] );
  }
  else
  {
    std::string c = "Uniform variable " + std::string( name ) + " doesn't exist";
    XV_CORE_WARN( c );
  }
}

void GLSLShader::PrintActiveAttribs() const
{
  #if 1
  GLint max_length, num_attribs;
  glGetProgramiv( pgm_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length );
  glGetProgramiv( pgm_handle, GL_ACTIVE_ATTRIBUTES, &num_attribs );
  GLchar *pname = new GLchar[max_length];
  std::cout << "Index\t|\tName\n";
  std::cout << "----------------------------------------------------------------------\n";
  for ( GLint i = 0; i < num_attribs; ++i )
  {
    GLsizei written;
    GLint size;
    GLenum type;
    glGetActiveAttrib( pgm_handle, i, max_length, &written, &size, &type, pname );
    GLint loc = glGetAttribLocation( pgm_handle, pname );
    std::cout << loc << "\t\t" << pname << std::endl;
  }
  std::cout << "----------------------------------------------------------------------\n";
  delete[] pname;

  #else
  GLint numAttribs;
  glGetProgramInterfaceiv( pgm_handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs );
  GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
  std::cout << "Active attributes:" << std::endl;
  for ( GLint i = 0; i < numAttribs; ++i )
  {
    GLint results[3];
    glGetProgramResourceiv( pgm_handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results );

    GLint nameBufSize = results[0] + 1;
    GLchar *pname = new GLchar[nameBufSize];
    glGetProgramResourceName( pgm_handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, pname );
    std::cout << results[2] << " " << pname << " " << results[1] << std::endl;
    delete[] pname;
  }
  #endif
}

void GLSLShader::PrintActiveUniforms() const
{
  GLint max_length;
  glGetProgramiv( pgm_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length );
  GLchar *pname = new GLchar[max_length];
  GLint num_uniforms;
  glGetProgramiv( pgm_handle, GL_ACTIVE_UNIFORMS, &num_uniforms );
  std::cout << "Location\t|\tName\n";
  std::cout << "----------------------------------------------------------------------\n";
  for ( GLint i = 0; i < num_uniforms; ++i )
  {
    GLsizei written;
    GLint size;
    GLenum type;
    glGetActiveUniform( pgm_handle, i, max_length, &written, &size, &type, pname );
    GLint loc = glGetUniformLocation( pgm_handle, pname );
    std::cout << loc << "\t\t" << pname << std::endl;
  }
  std::cout << "----------------------------------------------------------------------\n";
  delete[] pname;
}
}