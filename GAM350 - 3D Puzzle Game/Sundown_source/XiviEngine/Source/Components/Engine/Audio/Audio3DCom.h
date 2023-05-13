/*****************************************************************
*\file         Audio3DCom.h
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#ifndef AUDIOCOM3D_H
#define AUDIOCOM3D_H
#include <unordered_map>
#include <string>
#include <vector>
#include <rttr/registration_friend.h>

class Audio3DComponent
{
private:
  //use map key for action
  //2nd string is for audio location
  //Key can be changed to ENUM when player/AI state is up.
  //std::unordered_map<std::string, std::string> _Audio3DComp;
public:
  //use map key for action
  //2nd string is for audio location
  //Key can be changed to ENUM when player/AI state is up.
  std::unordered_map<std::string, std::string> _Audio3DComp;
  void AddAudio3DComp( std::string action, std::string filename );
  void DeleteAudio3DComp( std::string action );
  void EditAudio3DComp( std::string action, std::string filename );
  std::string GetAudio3DLocation( std::string action );
  bool ExistAudio3DComp( std::string );
  std::unordered_map < std::string, std::string>GetAudio3DCont();
  //void Play3DAudio(std::string);=

  constexpr static auto info = Xivi::ECS::Component::Type::Data { .m_name = "Audio3D" };

  ~Audio3DComponent()
  {
    _Audio3DComp.clear();
  }

  RTTR_REGISTRATION_FRIEND
};

namespace Xivi::Components::Audio3D::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Audio3DComponent>( std::string( Audio3DComponent::info.m_name ) )
  ( rttr::metadata( "GUID", ECS::Component::info<Audio3DComponent>.m_id ) )
  .property( "Maps", &Audio3DComponent::_Audio3DComp )( rttr::metadata( "NoEditor", true ) );
}
}

#endif