/*****************************************************************
*\file         CutSceneList.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "CutSceneList.h"
#include "Xivi/Graphics/Model/Model.h"
#include "Xivi/Core/Declaration.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include <Resource/Manager.h>
#include "Systems/Audio/Audio.h"
namespace Xivi::Graphics
{
void CutSceneFunc::AddScene( GSM::GS_STATES type, std::string image, float timer, std::string audio )
{
  CutSceneList::iterator it = list.find( type );
  if ( it == list.end() )
  {
    CutScene temp;
    temp.index = 0;
    temp.scenes.push_back( image );
    temp.audio.push_back( audio );
    temp.mCurrentTime.push_back( timer );
    temp.mInterval.push_back( timer );
    list.insert( { type, temp } );
  }
  else
  {
    it->second.scenes.push_back( image );
    it->second.audio.push_back( audio );
    it->second.mCurrentTime.push_back( timer );
    it->second.mInterval.push_back( timer );
  }
}

void CutSceneFunc::ActivateScene( GSM::GS_STATES type, float interval )
{
  mType = type;
  isActive = true;
  //mCurrentTime = mInterval = interval;
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
  {
    isActive = false;
    return;
  }
  it->second.index = 0;
}

void CutSceneFunc::NextSceneAudio( bool skip, float dt )
{
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
  {
    isActive = false;
    return;
  }
  const auto &scene = it->second.scenes;
  auto &index = it->second.index;
  if ( skip )
  {
    index++;
    audioPlaying = false;
    Audio::AudioManager::Instance().StopAllSFX();
    auto &mCurrentTime = it->second.mCurrentTime[index];
    auto &mInterval = it->second.mInterval[index];
    mCurrentTime = mInterval;
    if ( index >= scene.size() )
    {
      isActive = false;
      index = 0;
      return;
    }
    Audio::AudioManager::Instance().StopAllSFX();
  }
}

void CutSceneFunc::NextScene( bool skip, float dt )
{
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
  {
    isActive = false;
    return;
  }
  const auto &scene = it->second.scenes;
  auto &index = it->second.index;
  auto &mCurrentTime = it->second.mCurrentTime[index];
  auto &mInterval = it->second.mInterval[index];

  const auto &audio = it->second.audio;
  const auto &audioText = audio[index];

  if ( !audioText.empty() )
  {
    bool isPlaying = Audio::AudioManager::Instance().getChlnSFX( audioText );
    if ( !isPlaying )
    {
      mCurrentTime -= dt;
    }
  }
  else
  {
    mCurrentTime -= dt;
  }

  if ( skip || mCurrentTime <= 0.f )
  {
    index++;
    audioPlaying = false;
    Audio::AudioManager::Instance().StopAllSFX();
    mCurrentTime = mInterval;
    if ( index >= scene.size() )
    {
      const auto &audioText = it->second.m_bgm;
      isActive = false;
      index = 0;
      if ( !audioText.empty() )
      {
        auto &audio_mgr = Audio::AudioManager::Instance();
        if ( !audio_mgr.IfLoopPlaying( audioText ) )
        {
          audio_mgr.StopAllLoop();
          audio_mgr.playLoop( audioText, false );
        }
      }
      return;
    }

    auto nextaudioText = audio[index];
    if ( !nextaudioText.empty() )
      Audio::AudioManager::Instance().playSFX( nextaudioText, false );
  }
}
void CutSceneFunc::PrevScene( bool skip, float dt )
{
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
  {
    isActive = false;
    return;
  }

  if ( skip )
  {
    const auto &scene = it->second.scenes;
    auto &index = it->second.index;
    auto &mCurrentTime = it->second.mCurrentTime[index];
    auto &mInterval = it->second.mInterval[index];
    index--;
    audioPlaying = false;
    mCurrentTime = mInterval;
    if ( index <= 0 )
    {
      index = 0;
      return;
    }
    Audio::AudioManager::Instance().StopAllSFX();
    const auto &audio = it->second.audio;
    auto audioText = audio[index];
    if ( !audioText.empty() )
      Audio::AudioManager::Instance().playSFX( audioText, false );
  }
}

void CutSceneFunc::GetCutScene( std::vector<unsigned int> &images, float dt )
{
  //find scene type
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
  {
    isActive = false;
    return;
  }
  //find scene from scenes list
  const auto &scene = it->second.scenes;
  const auto &audio = it->second.audio;
  auto &index = it->second.index;
  ////check index
  if ( index >= scene.size() )
  {
    isActive = false;
    index = 0;
    return;
  }
  auto &rm = Service::Locator::Get<RM>();
  Resource::Handler<Graphics::Texture> image = rm.Get<Graphics::Texture>( scene[index] );
  images.push_back( image->pTex );
  return;
}
bool CutSceneFunc::IsThereScene( GSM::GS_STATES state )
{
  CutSceneList::iterator it = list.find( state );
  return !( it == list.end() );
}
void CutSceneFunc::AddBGM( GSM::GS_STATES  type, std::string bgm )
{
  CutSceneList::iterator it = list.find( type );
  if ( it == list.end() )
    return;
  else
    it->second.m_bgm = bgm;
}

void CutSceneFunc::SkipScene()
{
  isActive = false;
  Audio::AudioManager::Instance().StopAllSFX();
}

int CutSceneFunc::GetIndex()
{
  CutSceneList::iterator it = list.find( mType );
  if ( it == list.end() )
    return 0;
  return it->second.index;
}
}