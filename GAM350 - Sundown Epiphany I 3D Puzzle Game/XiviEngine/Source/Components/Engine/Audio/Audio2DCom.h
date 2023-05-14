/*****************************************************************
*\file         Audio2DCom.h
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#ifndef AUDIOCOM_H
#define AUDIOCOM_H
#include <unordered_map>
#include <string>
#include <vector>
#include <rttr/registration_friend.h>

class Audio2DComponent
{
private:
  //use map key for action
  //2nd string is for audio location
  //Key can be changed to ENUM when player/AI state is up.
  //std::unordered_map<std::string, std::string> _Audio2DComp;
public:
  //use map key for action
  //2nd string is for audio location
  //Key can be changed to ENUM when player/AI state is up.
  std::unordered_map<std::string, std::string> _Audio2DComp;
  void AddAudio2DComp( std::string action, std::string filename );
  void DeleteAudio2DComp( std::string action );
  void EditAudio2DComp( std::string action, std::string filename );
  std::string GetAudio2DLocation( std::string action );
  bool ExistAudio2DComp( std::string );
  std::unordered_map<std::string, std::string>GetAudio2DCont();
  void Play2DSFX( std::string );
  void Play2DLoop( std::string );
  void Stop2DLoop( std::string );
  //FMOD::Sound* getSFXsoundC(std::string);
  //void LowPass(std::string, int level);
  //FMOD::Sound* GetLoopSoundC(std::string);

  constexpr static auto info = Xivi::ECS::Component::Type::Data { .m_name = "Audio2D" };

  ~Audio2DComponent()
  {
    _Audio2DComp.clear();
  }

  RTTR_REGISTRATION_FRIEND
};

namespace Xivi::Components::Audio2D::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Audio2DComponent>( std::string( Audio2DComponent::info.m_name ) )
  ( rttr::metadata( "GUID", ECS::Component::info<Audio2DComponent>.m_id ) )
  .property( "Maps", &Audio2DComponent::_Audio2DComp )( rttr::metadata( "NoEditor", true ) );
}
}
#endif