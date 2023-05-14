/*****************************************************************
*\file         MeshCreate.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "MeshCreate.h"
#include "../Model/Mesh.h"
#include "../Model/Model.h"
#include "../Buffers/OpenGLBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/IndexBuffer.h"
namespace Xivi::Graphics
{
void CreatePrimative( Model::StaticType type, std::vector<Mesh> &mesh )
{
  //StaticModel model;
  switch ( type )
  {
    case Model::StaticType::CUBE:
    mesh.push_back( CreateCube() );
    break;
    case Model::StaticType::PLANE:
    mesh.push_back( CreatePlane( 1, 1, Vector3( 0.f ) ) );
    break;
    case Model::StaticType::SKYBOX:
    mesh.push_back( CreateSkyBox() );
    break;
    case Model::StaticType::SQUARE:
    mesh.push_back( CreateSquare() );
    break;
  }
}

Mesh CreatePlane( int width, int height, const Vector3 &normal )
{
  XV_CORE_INFO( "Loading Primitive Plane" );
  (void) normal;
  VertexBuffer vertices;
  for ( int stack = 0; stack <= width; ++stack )
  {
    float row = (float) stack / width;

    for ( int slice = 0; slice <= height; ++slice )
    {
      float col = (float) slice / height;

      Vertex v;

      v.position = Vector3( col - 0.5f, 0.5f - row, 0.0f );
      v.normal = Vector3( 0.0f, 0.0f, 1.0f );
      Vector2 var;
      var.x = static_cast<float>( slice );
      var.y = static_cast<float>( width - stack );
      v.texture_coord = var;
      vertices.push_back( v );
    }
  }

  IndexBuffer indices;
  indices.BuildIndices( static_cast<int>( width ), static_cast<int>( height ), vertices.data() );
  ComputeTangentsBitangents( vertices, indices );
  XV_CORE_INFO( "Primitive Plane Loaded" );
  return Mesh( vertices, indices );
}

Mesh CreateCube()
{
  XV_CORE_INFO( "Loading Primitive Cube" );
  VertexBuffer vertices;
  vertices.AddSpace( 24 );
  vertices.data()[0].position = Vector3( .5f, .5f, .5f );
  vertices.data()[0].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[1].position = Vector3( -.5f, .5f, .5f );
  vertices.data()[1].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[2].position = Vector3( -.5f, -.5f, .5f );
  vertices.data()[2].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[3].position = Vector3( .5f, -.5f, .5f );
  vertices.data()[3].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[4].position = Vector3( .5f, .5f, .5f );
  vertices.data()[4].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[5].position = Vector3( .5f, -.5f, .5f );
  vertices.data()[5].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[6].position = Vector3( .5f, -.5f, -.5f );
  vertices.data()[6].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[7].position = Vector3( .5f, .5f, -.5f );
  vertices.data()[7].texture_coord = Vector2( 0.0f, .5f );
  vertices.data()[7].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[8].position = Vector3( .5f, .5f, .5f );
  vertices.data()[8].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[9].position = Vector3( .5f, .5f, -.5f );
  vertices.data()[9].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[10].position = Vector3( -.5f, .5f, -.5f );
  vertices.data()[10].texture_coord = Vector2( 0.0f, .5f );
  vertices.data()[10].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[11].position = Vector3( -.5f, .5f, .5f );
  vertices.data()[11].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[12].position = Vector3( -.5f, .5f, .5f );
  vertices.data()[12].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[13].position = Vector3( -.5f, .5f, -.5f );
  vertices.data()[13].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[14].position = Vector3( -.5f, -.5f, -.5f );
  vertices.data()[14].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[15].position = Vector3( -.5f, -.5f, .5f );
  vertices.data()[15].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[16].position = Vector3( -.5f, -.5f, -.5f );
  vertices.data()[16].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[17].position = Vector3( .5f, -.5f, -.5f );
  vertices.data()[17].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[18].position = Vector3( .5f, -.5f, .5f );
  vertices.data()[18].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[19].position = Vector3( -.5f, -.5f, .5f );
  vertices.data()[19].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[20].position = Vector3( .5f, -.5f, -.5f );
  vertices.data()[20].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[21].position = Vector3( -.5f, -.5f, -.5f );
  vertices.data()[21].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[22].position = Vector3( -.5f, .5f, -.5f );
  vertices.data()[22].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[23].position = Vector3( .5f, .5f, -.5f );
  vertices.data()[23].normal = Vector3( 0.0f, 0.0f, -.5f );

  for ( int i = 0; i < 6; i++ )
  {
    vertices.data()[i * 4 + 0].texture_coord = Vector2( 0.0f, 0.0f );
    vertices.data()[i * 4 + 1].texture_coord = Vector2( 1.0f, 0.0f );
    vertices.data()[i * 4 + 2].texture_coord = Vector2( 1.0f, 1.0f );
    vertices.data()[i * 4 + 3].texture_coord = Vector2( 0.0f, 1.0f );
  }

  IndexBuffer indices;
  indices.push_back( 0 );
  indices.push_back( 1 );
  indices.push_back( 2 );

  indices.push_back( 0 );
  indices.push_back( 2 );
  indices.push_back( 3 );

  indices.push_back( 4 );
  indices.push_back( 5 );
  indices.push_back( 6 );

  indices.push_back( 4 );
  indices.push_back( 6 );
  indices.push_back( 7 );

  indices.push_back( 8 );
  indices.push_back( 9 );
  indices.push_back( 10 );

  indices.push_back( 8 );
  indices.push_back( 10 );
  indices.push_back( 11 );

  indices.push_back( 12 );
  indices.push_back( 13 );
  indices.push_back( 14 );

  indices.push_back( 12 );
  indices.push_back( 14 );
  indices.push_back( 15 );

  indices.push_back( 16 );
  indices.push_back( 17 );
  indices.push_back( 18 );

  indices.push_back( 16 );
  indices.push_back( 18 );
  indices.push_back( 19 );

  indices.push_back( 20 );
  indices.push_back( 21 );
  indices.push_back( 22 );

  indices.push_back( 20 );
  indices.push_back( 22 );
  indices.push_back( 23 );
  ComputeTangentsBitangents( vertices, indices );
  XV_CORE_INFO( "Primitive Cube Loaded" );
  return Mesh( vertices, indices );
}

Mesh CreateSkyBox()
{
  XV_CORE_INFO( "Primitive Skybox Loaded" );
  VertexBuffer vertices;
  vertices.AddSpace( 24 );
  vertices.data()[0].position = Vector3( -0.5f, -0.5f, -0.5f );
  vertices.data()[0].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[0].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[1].position = Vector3( 0.5f, -0.5f, -0.5f );
  vertices.data()[1].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[1].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[2].position = Vector3( 0.5f, 0.5f, -0.5f );
  vertices.data()[2].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[2].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[3].position = Vector3( -0.5f, 0.5f, -0.5f );
  vertices.data()[3].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[3].normal = Vector3( 0.0f, 0.0f, -.5f );

  vertices.data()[4].position = Vector3( -0.5f, -0.5f, 0.5f );
  vertices.data()[4].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[4].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[5].position = Vector3( 0.5f, -0.5f, 0.5f );
  vertices.data()[5].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[5].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[6].position = Vector3( 0.5f, 0.5f, 0.5f );
  vertices.data()[6].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[6].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[7].position = Vector3( -0.5f, 0.5f, 0.5f );
  vertices.data()[7].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[7].normal = Vector3( 0.0f, 0.0f, .5f );

  vertices.data()[8].position = Vector3( -0.5f, 0.5f, 0.5f );
  vertices.data()[8].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[8].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[9].position = Vector3( -0.5f, 0.5f, -0.5f );
  vertices.data()[9].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[9].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[10].position = Vector3( -0.5f, -0.5f, -0.5f );
  vertices.data()[10].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[10].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[11].position = Vector3( -0.5f, -0.5f, 0.5f );
  vertices.data()[11].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[11].normal = Vector3( -.5f, 0.0f, 0.0f );

  vertices.data()[12].position = Vector3( 0.5f, 0.5f, 0.5f );
  vertices.data()[12].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[12].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[13].position = Vector3( 0.5f, 0.5f, -0.5f );
  vertices.data()[13].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[13].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[14].position = Vector3( 0.5f, -0.5f, -0.5f );
  vertices.data()[14].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[14].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[15].position = Vector3( 0.5f, -0.5f, 0.5f );
  vertices.data()[15].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[15].normal = Vector3( .5f, 0.0f, 0.0f );

  vertices.data()[16].position = Vector3( -0.5f, -0.5f, -0.5f );
  vertices.data()[16].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[16].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[17].position = Vector3( 0.5f, -0.5f, -0.5f );
  vertices.data()[17].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[17].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[18].position = Vector3( 0.5f, -0.5f, 0.5f );
  vertices.data()[18].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[18].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[19].position = Vector3( -0.5f, -0.5f, 0.5f );
  vertices.data()[19].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[19].normal = Vector3( 0.0f, -.5f, 0.0f );

  vertices.data()[20].position = Vector3( -0.5f, 0.5f, -0.5f );
  vertices.data()[20].texture_coord = Vector2( 0.0f, 1.0f );
  vertices.data()[20].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[21].position = Vector3( 0.5f, 0.5f, -0.5f );
  vertices.data()[21].texture_coord = Vector2( 1.0f, 1.0f );
  vertices.data()[21].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[22].position = Vector3( 0.5f, 0.5f, 0.5f );
  vertices.data()[22].texture_coord = Vector2( 1.0f, 0.0f );
  vertices.data()[22].normal = Vector3( 0.0f, .5f, 0.0f );

  vertices.data()[23].position = Vector3( -0.5f, 0.5f, 0.5f );
  vertices.data()[23].texture_coord = Vector2( 0.0f, 0.0f );
  vertices.data()[23].normal = Vector3( 0.0f, .5f, 0.0f );

  IndexBuffer indices;
  indices.push_back( 0 );
  indices.push_back( 1 );
  indices.push_back( 2 );

  indices.push_back( 2 );
  indices.push_back( 3 );
  indices.push_back( 0 );

  indices.push_back( 4 );
  indices.push_back( 5 );
  indices.push_back( 6 );

  indices.push_back( 6 );
  indices.push_back( 7 );
  indices.push_back( 4 );

  indices.push_back( 8 );
  indices.push_back( 9 );
  indices.push_back( 10 );

  indices.push_back( 10 );
  indices.push_back( 11 );
  indices.push_back( 8 );

  indices.push_back( 12 );
  indices.push_back( 13 );
  indices.push_back( 14 );

  indices.push_back( 14 );
  indices.push_back( 15 );
  indices.push_back( 12 );

  indices.push_back( 16 );
  indices.push_back( 17 );
  indices.push_back( 18 );

  indices.push_back( 18 );
  indices.push_back( 19 );
  indices.push_back( 16 );

  indices.push_back( 20 );
  indices.push_back( 21 );
  indices.push_back( 22 );

  indices.push_back( 22 );
  indices.push_back( 23 );
  indices.push_back( 20 );
  XV_CORE_INFO( "Primitive SkyBox Loaded" );
  return Mesh( vertices, indices );

  /*VertexBuffer sbVertices;
  sbVertices.data()[0].position = Vector3(-1.0f, 1.0f, -1.0f);
  sbVertices.data()[1].position = Vector3(-1.0f, -1.0f, -1.0f);
  sbVertices.data()[2].position = Vector3(1.0f, -1.0f, -1.0f);
  sbVertices.data()[3].position = Vector3(1.0f, 1.0f, -1.0f);

  sbVertices.data()[4].position = Vector3(-1.0f, -1.0f, 1.0f);
  sbVertices.data()[5].position = Vector3(-1.0f, -1.0f, -1.0f);
  sbVertices.data()[6].position = Vector3(-1.0f, 1.0f, -1.0f);
  sbVertices.data()[7].position = Vector3(-1.0f, 1.0f, 1.0f);

  sbVertices.data()[8].position = Vector3(1.0f, -1.0f, -1.0f);
  sbVertices.data()[9].position = Vector3(1.0f, -1.0f, 1.0f);
  sbVertices.data()[10].position = Vector3(1.0f, 1.0f, 1.0f);
  sbVertices.data()[11].position = Vector3(1.0f, 1.0f, -1.0f);

  sbVertices.data()[12].position = Vector3(-1.0f, -1.0f, 1.0f);
  sbVertices.data()[13].position = Vector3(-1.0f, 1.0f, 1.0f);
  sbVertices.data()[14].position = Vector3(1.0f, 1.0f, 1.0f);
  sbVertices.data()[15].position = Vector3(1.0f, -1.0f, 1.0f);

  sbVertices.data()[16].position = Vector3(-1.0f, 1.0f, -1.0f);
  sbVertices.data()[17].position = Vector3(1.0f, 1.0f, -1.0f);
  sbVertices.data()[18].position = Vector3(1.0f, 1.0f, 1.0f);
  sbVertices.data()[19].position = Vector3(-1.0f, 1.0f, 1.0f);

  sbVertices.data()[20].position = Vector3(-1.0f, -1.0f, -1.0f);
  sbVertices.data()[21].position = Vector3(-1.0f, -1.0f, 1.0f);
  sbVertices.data()[22].position = Vector3(1.0f, -1.0f, -1.0f);
  sbVertices.data()[23].position = Vector3(-1.0f, -1.0f, 1.0f);*/

  //float skyboxVertices[] = {
  //	// positions
  //	-1.0f,  1.0f, -1.0f,//0
  //	-1.0f, -1.0f, -1.0f,//1
  //	 1.0f, -1.0f, -1.0f,//2
  //	 1.0f, -1.0f, -1.0f,//2
  //	 1.0f,  1.0f, -1.0f,//3
  //	-1.0f,  1.0f, -1.0f,//0

  //	-1.0f, -1.0f,  1.0f,//4
  //	-1.0f, -1.0f, -1.0f,//5
  //	-1.0f,  1.0f, -1.0f,//6
  //	-1.0f,  1.0f, -1.0f,//6
  //	-1.0f,  1.0f,  1.0f,//7
  //	-1.0f, -1.0f,  1.0f,//4

  //	 1.0f, -1.0f, -1.0f,//8
  //	 1.0f, -1.0f,  1.0f,//9
  //	 1.0f,  1.0f,  1.0f,//10
  //	 1.0f,  1.0f,  1.0f,//10
  //	 1.0f,  1.0f, -1.0f,//11
  //	 1.0f, -1.0f, -1.0f,//8

  //	-1.0f, -1.0f,  1.0f,//12
  //	-1.0f,  1.0f,  1.0f,//13
  //	 1.0f,  1.0f,  1.0f,//14
  //	 1.0f,  1.0f,  1.0f,//14
  //	 1.0f, -1.0f,  1.0f,//15
  //	-1.0f, -1.0f,  1.0f,//12

  //	-1.0f,  1.0f, -1.0f,//16
  //	 1.0f,  1.0f, -1.0f,//17
  //	 1.0f,  1.0f,  1.0f,//18
  //	 1.0f,  1.0f,  1.0f,//18
  //	-1.0f,  1.0f,  1.0f,//19
  //	-1.0f,  1.0f, -1.0f,//16

  //	-1.0f, -1.0f, -1.0f,//20
  //	-1.0f, -1.0f,  1.0f,//21
  //	 1.0f, -1.0f, -1.0f,//22
  //	 1.0f, -1.0f, -1.0f,//22
  //	-1.0f, -1.0f,  1.0f,//23
  //	 1.0f, -1.0f,  1.0f//20
  //};
}

Mesh QUAD()
{
  std::array<Vertex, 6> vertex;
  vertex[0].position = Vec3f( -1.f, -1.f, 0.0f );
  vertex[1].position = Vec3f( 1.f, -1.f, 0.0f );
  vertex[2].position = Vec3f( 1.f, 1.f, 0.0f );
  vertex[3].position = Vec3f( -1.f, 1.f, 0.0f );

  vertex[0].texture_coord = Math::Vector2( 0.0f, 0.0f );
  vertex[1].texture_coord = Math::Vector2( 1.0f, 0.0f );
  vertex[2].texture_coord = Math::Vector2( 1.0f, 1.0f );
  vertex[3].texture_coord = Math::Vector2( 0.0f, 1.0f );

  GLuint vbo_hdl = 0;
  std::array<GLushort, 6> idx_vtx;
  //setting up rectangle topology
  idx_vtx[0] = 0; idx_vtx[1] = 1; idx_vtx[2] = 2;
  idx_vtx[3] = 2; idx_vtx[4] = 3; idx_vtx[5] = 0;

  GLsizeiptr buffer_size = sizeof( Vertex ) * vertex.size();
  glCreateBuffers( 1, &vbo_hdl );
  //allows storage to the server side (gpu)
  glNamedBufferStorage( vbo_hdl, buffer_size, nullptr, GL_DYNAMIC_STORAGE_BIT );
  glNamedBufferSubData( vbo_hdl, 0, buffer_size, vertex.data() );

  //creating a vertex array object
  glCreateVertexArrays( 1, &vbo_hdl );
  //creating a vertex array buffer
  glVertexArrayVertexBuffer( vbo_hdl, 0, vbo_hdl, 0, sizeof( Vertex ) );

  //binding position coordinates into the buffer
  glEnableVertexArrayAttrib( vbo_hdl, 0 );
  glVertexArrayAttribFormat( vbo_hdl, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, Vertex::position ) );
  glVertexArrayAttribBinding( vbo_hdl, 0, 0 );

  //binding texture coordinates into the buffer
  glEnableVertexArrayAttrib( vbo_hdl, 1 );
  glVertexArrayAttribFormat( vbo_hdl, 1, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, Vertex::texture_coord ) );
  glVertexArrayAttribBinding( vbo_hdl, 1, 0 );

  GLuint ebo_hdl; glCreateBuffers( 1, &ebo_hdl );
  glNamedBufferStorage( ebo_hdl, sizeof( GLushort ) * idx_vtx.size(), reinterpret_cast<GLvoid *>( idx_vtx.data() ), GL_DYNAMIC_STORAGE_BIT );
  glVertexArrayElementBuffer( vbo_hdl, ebo_hdl );
  glBindVertexArray( 0 );

  //Setting up information and addresses for mdl
  Mesh mdl;
  mdl.vaoid = vbo_hdl;
  mdl.primitive_type = GL_TRIANGLES; //type of rendered object
  mdl.draw_cnt = static_cast<GLuint>( idx_vtx.size() ); //number of vertices
  mdl.primitive_cnt = static_cast<GLuint>( vertex.size() ); //number of vertices
  return mdl; //returns the rendered information
}

Mesh CreateSquare()
{
  XV_CORE_INFO( "Loading 2D Square" );
  //std::array<Vertex, 6> vertex;
  VertexBuffer vertices;
  vertices.AddSpace( 4 );
  vertices[0].position = Vector3( -0.5f, -0.5f, 0.0f );
  vertices[1].position = Vector3( 0.5f, -0.5f, 0.0f );
  vertices[2].position = Vector3( 0.5f, 0.5f, 0.0f );
  vertices[3].position = Vector3( -0.5f, 0.5f, 0.0f );

  vertices[0].texture_coord = Math::Vector2( 0.0f, 0.0f );
  vertices[1].texture_coord = Math::Vector2( 1.0f, 0.0f );
  vertices[2].texture_coord = Math::Vector2( 1.0f, 1.0f );
  vertices[3].texture_coord = Math::Vector2( 0.0f, 1.0f );

  GLuint vbo_hdl = 0;
  std::array<GLushort, 6> idx_vtx;
  //setting up rectangle topology
  idx_vtx[0] = 0; idx_vtx[1] = 1; idx_vtx[2] = 2;
  idx_vtx[3] = 2; idx_vtx[4] = 3; idx_vtx[5] = 0;
  IndexBuffer indices;
  indices.push_back( 0 );
  indices.push_back( 1 );
  indices.push_back( 2 );
  indices.push_back( 2 );
  indices.push_back( 3 );
  indices.push_back( 0 );
  return Mesh( vertices, indices );

  //GLsizeiptr buffer_size = sizeof(Vertex) * vertex.size();
  //glCreateBuffers(1, &vbo_hdl);
  ////allows storage to the server side (gpu)
  //glNamedBufferStorage(vbo_hdl, buffer_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
  //glNamedBufferSubData(vbo_hdl, 0, buffer_size, vertex.data());

  ////creating a vertex array object
  //glCreateVertexArrays(1, &vbo_hdl);
  ////creating a vertex array buffer
  //glVertexArrayVertexBuffer(vbo_hdl, 0, vbo_hdl, 0, sizeof(Vertex));

  ////binding position coordinates into the buffer
  //glEnableVertexArrayAttrib(vbo_hdl, 0);
  //glVertexArrayAttribFormat(vbo_hdl, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Vertex::position));
  //glVertexArrayAttribBinding(vbo_hdl, 0, 0);

  ////binding texture coordinates into the buffer
  //glEnableVertexArrayAttrib(vbo_hdl, 1);
  //glVertexArrayAttribFormat(vbo_hdl, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Vertex::texture_coord));
  //glVertexArrayAttribBinding(vbo_hdl, 1, 0);

  //GLuint ebo_hdl; glCreateBuffers(1, &ebo_hdl);
  //glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_vtx.size(), reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
  //glVertexArrayElementBuffer(vbo_hdl, ebo_hdl);
  //glBindVertexArray(0);

  ////Setting up information and addresses for mdl
  //Mesh mdl;
  //mdl.vaoid = vbo_hdl;
  //mdl.primitive_type = GL_TRIANGLES; //type of rendered object
  //mdl.draw_cnt = static_cast<GLuint>(idx_vtx.size()); //number of vertices
  //mdl.primitive_cnt = static_cast<GLuint>(vertex.size()); //number of vertices
  //return mdl; //returns the rendered information
}

void ComputeTangentsBitangents( VertexBuffer &vertices, const IndexBuffer &indices )
{
  int numVertices = vertices.size();
  int numIndices = indices.size();

  int *AverageCounter = new int[numVertices];

  std::memset( AverageCounter, 0, numVertices * sizeof( int ) );

  for ( int index = 0; index < numIndices; index += 3 )
  {
    int p0 = indices[index + 0];
    int p1 = indices[index + 1];
    int p2 = indices[index + 2];

    Vertex &A = vertices[p0];
    Vertex &B = vertices[p1];
    Vertex &C = vertices[p2];

    // Get our two vectors across the triangles
    Vec3f P = B.position - A.position;
    Vec3f Q = C.position - A.position;

    // Get our texture components of those vectors
    Vec2f Puv = B.texture_coord - A.texture_coord;
    Vec2f Quv = C.texture_coord - A.texture_coord;

    float texCoord = Puv.x * Quv.y - Quv.x * Puv.y;

    // This would handle the case of a small (possibly zero)
    // reciprocal, just skip this vertex, Tan and Bin aren't incremented
    // and the counter isn't incremented.
    if ( texCoord < 0.00001f && texCoord > -0.00001f )
      continue;

    float reciprocal = 1.0f / texCoord;

    Vec3f Tangent( ( P * Quv.y - Q * Puv.y ) * reciprocal );
    Vec3f Bitangent( ( Q * Puv.x - P * Quv.x ) * reciprocal );

    A.tan += Tangent;
    B.tan += Tangent;
    C.tan += Tangent;

    A.bit += Bitangent;
    B.bit += Bitangent;
    C.bit += Bitangent;

    ++AverageCounter[p0];
    ++AverageCounter[p1];
    ++AverageCounter[p2];
  }

  // For each vertex, divide by the number of triangles shared by it
  for ( int vert = 0; vert < numVertices; ++vert )
  {
    if ( AverageCounter[vert] )
    {
      vertices[vert].tan /= static_cast<float>( AverageCounter[vert] );
      vertices[vert].bit /= static_cast<float>( AverageCounter[vert] );
    }
  }

  delete[] AverageCounter;
}
}