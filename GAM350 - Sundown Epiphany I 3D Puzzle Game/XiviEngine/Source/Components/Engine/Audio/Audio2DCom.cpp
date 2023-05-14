/*****************************************************************
*\file         Audio2DCom.cpp
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "Audio2DCom.h"
#include "../../../Systems/Audio/Audio.h"

void Audio2DComponent::AddAudio2DComp( std::string action, std::string filename )
{
  if ( !ExistAudio2DComp( action ) )
    _Audio2DComp.insert( std::make_pair( action, filename ) );
}

void Audio2DComponent::DeleteAudio2DComp( std::string action )
{
  std::unordered_map<std::string, std::string>::iterator it;

  it = _Audio2DComp.find( action );
  _Audio2DComp.erase( it );
  std::cout << "Sound deleted" << std::endl;
}

void Audio2DComponent::EditAudio2DComp( std::string action, std::string filename )
{
  DeleteAudio2DComp( action );
  AddAudio2DComp( action, filename );
}

std::string Audio2DComponent::GetAudio2DLocation( std::string action )
{
  auto search = _Audio2DComp.find( action );

  if ( search != _Audio2DComp.end() )
  {
    return search->second.c_str();
  }

  else
    return "Not Found";
}

bool Audio2DComponent::ExistAudio2DComp( std::string key )
{
  std::unordered_map<std::string, std::string>::iterator it;

  it = _Audio2DComp.find( key );
  if ( it != _Audio2DComp.end() )
    return true; // found

  return false; // not found
}

std::unordered_map < std::string, std::string> Audio2DComponent::GetAudio2DCont()
{
  return _Audio2DComp;
}

void Audio2DComponent::Play2DSFX( std::string action )
{
  Xivi::Audio::AudioManager::Instance().playSFX( GetAudio2DLocation( action ), false );
}

void Audio2DComponent::Play2DLoop( std::string action )
{
  Xivi::Audio::AudioManager::Instance().playLoop( GetAudio2DLocation( action ), false );
}

void Audio2DComponent::Stop2DLoop( std::string action )
{
  std::unordered_map<std::string, std::string>::iterator it;
  std::string filename;

  it = _Audio2DComp.find( action );

  if ( it != _Audio2DComp.end() )
    filename = it->second.c_str(); // found

  Xivi::Audio::AudioManager::Instance().StopSpecificLoop( filename );
}

//void Audio2DComponent::LowPass(std::string action, int level)
//{
//    FMOD::Channel* _channel = Audio::AudioManager::GetInstance().getChlnSFX(GetAudio2DLocation(action));
//
//    switch (level)
//   {
//    case 0:
//        FMOD::DSP * _nofilter;
//        Audio::AudioManager::GetInstance().getSystem()->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_nofilter);
//        _nofilter->setParameterInt(FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_DISABLED);
//        _channel->addDSP(0, _nofilter);
//        break;
//     case 1:
//         FMOD::DSP * _filter1;
//         Audio::AudioManager::GetInstance().getSystem()->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_filter1);
//         _filter1->setParameterInt(FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_12DB);
//         _channel->addDSP(0, _filter1);
//         break;
//     case 2:
//         FMOD::DSP * _filter2;
//         Audio::AudioManager::GetInstance().getSystem()->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_filter2);
//         _filter2->setParameterInt(FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB);
//         _channel->addDSP(0, _filter2);
//         break;
//     case 3:
//         FMOD::DSP * _filter3;
//         Audio::AudioManager::GetInstance().getSystem()->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_filter3);
//         _filter3->setParameterInt(FMOD_DSP_MULTIBAND_EQ_A_FILTER, FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_48DB);
//         _channel->addDSP(0, _filter3);
//         break;
//     default:
//         _channel->setMode(FMOD_DSP_MULTIBAND_EQ_FILTER_DISABLED);
//   }
//}