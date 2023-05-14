/*****************************************************************
*\file         Audio3DCom.cpp
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "Audio3DCom.h"
#include "../../../Systems/Audio/Audio.h"

void Audio3DComponent::AddAudio3DComp( std::string action, std::string filename )
{
  if ( !ExistAudio3DComp( action ) )
    _Audio3DComp.insert( std::make_pair( action, filename ) );
}

void Audio3DComponent::DeleteAudio3DComp( std::string action )
{
  std::unordered_map<std::string, std::string>::iterator it;

  it = _Audio3DComp.find( action );
  _Audio3DComp.erase( it );
}

void Audio3DComponent::EditAudio3DComp( std::string action, std::string filename )
{
  DeleteAudio3DComp( action );
  AddAudio3DComp( action, filename );
}

std::string Audio3DComponent::GetAudio3DLocation( std::string action )
{
  auto search = _Audio3DComp.find( action );

  if ( search != _Audio3DComp.end() )
  {
    return search->second.c_str();
  }

  else
    return "Not Found";
}

bool Audio3DComponent::ExistAudio3DComp( std::string key )
{
  std::unordered_map<std::string, std::string>::iterator it;

  it = _Audio3DComp.find( key );
  if ( it != _Audio3DComp.end() )
    return true; // found

  return false; // not found
}

std::unordered_map < std::string, std::string> Audio3DComponent::GetAudio3DCont()
{
  return _Audio3DComp;
}

//void Audio3DComponent::Play3DAudio(std::string action)
//{
//
//}