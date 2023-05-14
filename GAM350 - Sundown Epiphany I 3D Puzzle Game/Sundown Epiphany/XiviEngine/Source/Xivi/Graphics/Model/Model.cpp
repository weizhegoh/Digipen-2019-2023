/*****************************************************************
*\file         Model.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "Model.h"
#include "../Reader/ModelReader.h"
#include "../Reader/MeshCreate.h"

namespace Xivi::Graphics
{
StaticModel::StaticModel( const std::string type )
{
  unitScales = Math::Vector3( 10.f );
  if ( std::string::npos != type.find( "cube.BinaryObj" ) )
    Xivi::Graphics::CreatePrimative( Model::StaticType::CUBE, mesh );
  else if ( std::string::npos != type.find( "plane.BinaryObj" ) )
    Xivi::Graphics::CreatePrimative( Model::StaticType::PLANE, mesh );
  else if ( std::string::npos != type.find( "skybox.BinaryObj" ) )
    Xivi::Graphics::CreatePrimative( Model::StaticType::SKYBOX, mesh );
  else if ( std::string::npos != type.find( "square.BinaryObj" ) )
    Xivi::Graphics::CreatePrimative( Model::StaticType::SQUARE, mesh );
  else
  {
    std::size_t found = type.find_last_of( '.' );
    std::string_view view = type;
    if ( view.substr( found + 1, type.length() ) == "BinaryObj" )
      Xivi::Graphics::LoadBinaryObj( type, mesh, unitScales );
    else if ( view.substr( found + 1, type.length() ) == "BinaryStatic" )
      Xivi::Graphics::LoadBinaryStatic( type, mesh, unitScales, hasTanBi );
    else
      Xivi::Graphics::CreatePrimative( Model::StaticType::CUBE, mesh );
  }
}

StaticModel::~StaticModel()
{
  for ( auto &x : mesh )
  {
    Gfx::Renderer::DeleteVAO( x.vaoid );
    Gfx::Renderer::DeleteVAO( x.vaoid_normal );
  }
}

void StaticModel::Draw()
{
  for ( const auto &x : mesh )
  {
    x.Draw();
  }
}

AnimationModel::AnimationModel( const std::string type )
{
  XV_CORE_ASSERT_MSG( is_fileExist( type ), "Unable to find File" );
  model = AnimatedModel( type );
  animation = Animation( type, &model );
  //animatior.Setup(&animation);
}

AnimationModel::~AnimationModel()
{
  for ( auto &x : model.meshes )
  {
    Gfx::Renderer::DeleteVAO( x.vaoid );
    Gfx::Renderer::DeleteVAO( x.vaoid_normal );
  }
}
}