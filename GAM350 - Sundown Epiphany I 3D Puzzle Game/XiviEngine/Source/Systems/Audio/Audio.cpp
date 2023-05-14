/*****************************************************************
*\file         Audio.cpp
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <pch.h>
#include "Audio.h"
#include <Resource/Manager.h>
#include "Xivi/Core/Utils/Math.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"

#pragma warning(disable: 4505)

namespace Xivi::Audio
{
int MAX_CHANNEL = 128;

AudioManager::AudioManager() :_system( NULL ), _channel( NULL ), _sound( NULL )
{}

int AudioManager::Initialize()
{
  FMOD_RESULT result = System_Create( &_system );
  if ( result != FMOD_OK )
  {
    return result;
  }
  else
  {
      //Added in sound groups to be used in game.
    _system->init( MAX_CHANNEL, FMOD_INIT_NORMAL, NULL );
    _system->createSoundGroup( NULL, &_GroupSFX );
    _system->createSoundGroup( NULL, &_GroupLoop );
    _system->createSoundGroup( NULL, &_3DSFXGroup );
    _system->createSoundGroup( NULL, &_3DLoopGroup );
    FMOD_RESULT result1 = _system->set3DNumListeners( 2 );
    if ( result1 != FMOD_OK )
    {
      XV_WARN( "2 LISTENER SET" );
    }
    SetWorld( Xivi::Service::Locator::Get<Xivi::ECS::World::Instance>() );
  }

  LoadAllAudio( "Assets/Audio" );

  _GroupSFX->setVolume( maxSFXvol );
  _3DSFXGroup->setVolume( maxSFXvol );
  _3DLoopGroup->setVolume( maxSFXvol );
  _GroupLoop->setVolume( maxBGMvol );
  return 0;
}

AudioManager::~AudioManager()
{
  if ( _sound )
  {
    _GroupSFX->release();
    _GroupLoop->release();
    _3DSFXGroup->release();
    _3DLoopGroup->release();
    _sound->release();
  }
  if ( _system )
  {
    _system->close();
    _system->release();
  }
}

int AudioManager::LoadFromPath( std::string Filepath )
{
  FMOD_RESULT result = _system->createSound( Filepath.c_str(), FMOD_LOOP_NORMAL, 0, &_sound );
  return result;
}

void AudioManager::LoadAllAudio( std::string filepath )
{
  std::string pathStr = std::filesystem::current_path().parent_path().string() + "/Assets/Audio/";
  FMOD_MODE eMode = FMOD_DEFAULT;
  std::cout << pathStr << std::endl;

  if ( std::filesystem::is_directory( pathStr + "SFX" ) ) //2dsfx
  {
    for ( auto &S : std::filesystem::directory_iterator( pathStr + "SFX" ) )
    {
      const std::string Sdir = S.path().string();
      _system->createSound( ( S.path() ).string().c_str(), FMOD_LOOP_OFF, 0, &_sound );
      _SFXCont.insert( std::make_pair( S.path().filename().string(), _sound ) );
      _sound->setSoundGroup( _GroupSFX );
    }
  }

  if ( std::filesystem::is_directory( pathStr + "LOOP" ) ) //2dloop
  {
    for ( auto &SF : std::filesystem::directory_iterator( pathStr + "LOOP" ) )
    {
      const std::string SFdir = SF.path().string();
      _system->createSound( ( SF.path() ).string().c_str(), FMOD_LOOP_NORMAL, 0, &_sound );
      _LoopCont.insert( std::make_pair( SF.path().filename().string(), _sound ) );
      _sound->setSoundGroup( _GroupLoop );
    }
  }

  if ( std::filesystem::is_directory( pathStr + "3DSFX" ) )
  {
    eMode |= FMOD_LOOP_OFF;
    eMode |= FMOD_3D;

    for ( auto &S : std::filesystem::directory_iterator( pathStr + "3DSFX" ) )
    {
      const std::string Sdir = S.path().string();
      _system->createSound( ( S.path() ).string().c_str(), eMode, 0, &_sound );
      _3DSFXCont.insert( std::make_pair( S.path().filename().string(), _sound ) );
      _sound->setSoundGroup( _3DSFXGroup );
    }
  }

  if ( std::filesystem::is_directory( pathStr + "3DLOOP" ) )
  {
      //Change to loop off, loop on is just for testing purposes
    eMode |= FMOD_LOOP_NORMAL;
    eMode |= FMOD_3D;

    for ( auto &S : std::filesystem::directory_iterator( pathStr + "3DLOOP" ) )
    {
      const std::string Sdir = S.path().string();
      _system->createSound( ( S.path() ).string().c_str(), eMode, 0, &_sound );
      _3DLoopCont.insert( std::make_pair( S.path().filename().string(), _sound ) );
      _sound->setSoundGroup( _3DLoopGroup );
    }
  }
}

int AudioManager::LoadFromMemory( char *MemPtr, unsigned int MemSize )
{
  FMOD_CREATESOUNDEXINFO sndinfo = { 0 };
  sndinfo.cbsize = sizeof( sndinfo );
  sndinfo.length = MemSize;
  FMOD_RESULT result = _system->createSound( MemPtr, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &sndinfo, &_sound );
  return result;
}

void AudioManager::PlayAudio()
{
  _system->playSound( _sound, 0, false, &_channel );
}

void AudioManager::playSFX( std::string filename, bool pause )
{
  auto searchSFX = _SFXCont.find( filename );
  if ( searchSFX != _SFXCont.end() )
  {
    FMOD_RESULT result = _system->playSound( searchSFX->second, 0, pause, &_channel );

    if ( result == FMOD_OK )
      _NowPlayingCont.insert( std::make_pair( searchSFX->first.c_str(), _channel ) );

    if ( result != FMOD_OK )
    {
      std::cout << "Cannot play" << std::endl;
    }
  }

  else
  {
    std::cout << filename << std::endl;
    std::cout << "Sound to play not attached or not found.\n";
  }
}

void AudioManager::playLoop( std::string filename, bool pause )
{
  auto searchLSFX = _LoopCont.find( filename );
  auto searchPlaying = _NowPlayingCont.find( filename );

  if ( searchPlaying != _NowPlayingCont.end() )
  {
    return;
  }

  if ( searchLSFX != _LoopCont.end() )
  {
    FMOD_RESULT result = _system->playSound( searchLSFX->second, 0, pause, &_channel );

    if ( result == FMOD_OK )
      _NowPlayingCont.insert( std::make_pair( searchLSFX->first.c_str(), _channel ) );

    if ( result != FMOD_OK )
    {
      std::cout << "Cannot play" << std::endl;
    }
  }

  else
  {
    std::cout << filename << std::endl;
    std::cout << "Not found";
  }
}

void AudioManager::play3DSFX( Xivi::Entity ent, std::string action )
{
  auto search3DSFX = _3DSFXCont.find( m_world->GetComponent<Audio3DComponent>( ent ).GetAudio3DLocation( action ) );

  if ( search3DSFX != _3DSFXCont.end() )
  {
    FMOD::Channel *_3Dchannel = nullptr;
    FMOD_RESULT result = _system->playSound( search3DSFX->second, 0, 0, &_3Dchannel );

    if ( result == FMOD_OK )
      _NowPlaying3DCont.insert( std::make_pair( ent, _3Dchannel ) );

    if ( result != FMOD_OK )
    {
      std::cout << "Cannot play" << std::endl;
    }
  }

  else
  {
    std::cout << "Sound to play not attached or not found.\n";
  }

  const auto &entitypos = m_world->GetComponent<Xivi::Position>( ent ).m_position;
  FMOD_VECTOR pos { entitypos.getX(), entitypos.getY(), entitypos.getZ() };
  _channel->set3DAttributes( &pos, nullptr );
}

void AudioManager::play3DLoop( Xivi::Position *pos, std::string filename )
{
  auto search3DLoop = _3DLoopCont.find( filename );
  auto searchPlaying = _NowPlayingCont.find( filename );

  if ( searchPlaying != _NowPlayingCont.end() )
  {
    return;
  }

  if ( search3DLoop != _3DLoopCont.end() )
  {
    FMOD::Channel *_3Dchannel = nullptr;
    FMOD_RESULT result = _system->playSound( search3DLoop->second, 0, 0, &_3Dchannel );

    if ( result == FMOD_OK )
      _NowPlaying3DLoop.emplace( std::make_pair( pos,
                                 std::unordered_map <std::string, FMOD::Channel *>(
                                 { std::make_pair( filename, _3Dchannel ) } ) ) );

    if ( result != FMOD_OK )
    {
      std::cout << "Cannot play" << std::endl;
    }
  }

  else
  {
    std::cout << "Sound to play not attached or not found.\n";
  }

  FMOD_VECTOR pos1 { pos->m_position.getX(), pos->m_position.getY(), pos->m_position.getZ() };
  _channel->set3DAttributes( &pos1, nullptr );
}

void AudioManager::PauseAudio()
{
  bool isPaused;
  _channel->getPaused( &isPaused );

  if ( !isPaused )
    _channel->setPaused( true );
}

void AudioManager::UnpauseAudio()
{
  bool isPaused;
  _channel->getPaused( &isPaused );

  if ( isPaused )
    _channel->setPaused( false );
}

void AudioManager::StopAudio()
{
  _channel->stop();
}

void AudioManager::StopSpecificLoop( std::string filename )
{
  if ( !_NowPlayingCont.size() )
  {
    return;
  }

  auto search = _NowPlayingCont.find( filename );

  if ( search != _NowPlayingCont.end() && search->first == filename )
  {
    if ( !_NowPlayingCont.empty() )
    {
      search->second->stop();
      _NowPlayingCont.erase( filename );
    }
  }
}

void AudioManager::StopSpecific3DLoop( Xivi::Position *pos, std::string filename )
{
  if ( !_NowPlaying3DLoop.size() )
  {
    return;
  }

  auto search = _NowPlaying3DLoop.find( pos );

  if ( search != _NowPlaying3DLoop.end() && search->first == pos )
  {
    if ( !_NowPlaying3DLoop.empty() )
    {
      search->second.find( filename )->second->stop();
      _NowPlaying3DLoop.erase( search->first );
    }
  }
}

void AudioManager::setSFXVol( float vol )
{
  _GroupSFX->setVolume( vol );
}

void AudioManager::setLoopVol( float vol )
{
  _GroupLoop->setVolume( vol );
}

void AudioManager::set3DSFXVol( float vol )
{
  _3DSFXGroup->setVolume( vol );
}

void AudioManager::set3DLoopVol( float vol )
{
  _3DLoopGroup->setVolume( vol );
}

void AudioManager::setMasterVol( float vol )
{
  set3DSFXVol( vol );
  setLoopVol( vol );
  setSFXVol( vol );
  set3DLoopVol( vol );
}

void AudioManager::set3DListener( const glm::vec3 &vPos, float vol )
{
  glm::vec3 _listener;
  _listener.x = vPos.x;
  _listener.y = vPos.y;
  _listener.z = vPos.z;

  UNREFERENCED_PARAMETER( vol );
}

float AudioManager::getSFXVol()
{
  float currVol = 0.0f;
  _GroupSFX->getVolume( &currVol );
  return currVol;
}

float AudioManager::getLoopVol()
{
  float currVol = 0.0f;
  _GroupLoop->getVolume( &currVol );
  return currVol;
}

float AudioManager::get3DSFXVol()
{
  float currVol = 0.0f;
  _3DSFXGroup->getVolume( &currVol );
  return currVol;
}

float AudioManager::get3DLoopVol()
{
  float currVol = 0.0f;
  _3DLoopGroup->getVolume( &currVol );
  return currVol;
}

void AudioManager::increaseSFXVol()
{
  float _currvolume = 0.0f;
  _GroupSFX->getVolume( &_currvolume );

  if ( _currvolume == maxSFXvol )
    _currvolume = maxSFXvol;

  else
    _GroupSFX->setVolume( _currvolume + ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::increaseLoopVol()
{
  float _currvolume = 0.0f;
  _GroupLoop->getVolume( &_currvolume );

  if ( _currvolume == maxBGMvol )
    _currvolume = maxBGMvol;

  else
    _GroupLoop->setVolume( _currvolume + ( ( maxBGMvol / 10 ) ) );
}

void AudioManager::increase3DSFXVol()
{
  float _currvolume = 0.0f;
  _3DSFXGroup->getVolume( &_currvolume );

  if ( _currvolume == maxSFXvol )
    _currvolume = maxSFXvol;

  else
    _3DSFXGroup->setVolume( _currvolume + ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::increase3DLoopVol()
{
  float _currvolume = 0.0f;
  _3DLoopGroup->getVolume( &_currvolume );

  if ( _currvolume == maxSFXvol )
    _currvolume = maxSFXvol;

  else
    _3DLoopGroup->setVolume( _currvolume + ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::increaseMasterVol()
{
  increaseSFXVol();
  increaseLoopVol();
  increase3DSFXVol();
  increase3DLoopVol();

  gui_vol++;
}

void AudioManager::decreaseSFXVol()
{
  float _currvolume = 0.0f;
  _GroupSFX->getVolume( &_currvolume );

  if ( _currvolume == 0.0f )
    _currvolume = 0.0f;

  else
    _GroupSFX->setVolume( _currvolume - ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::decreaseLoopVol()
{
  float _currvolume = 0.0f;
  _GroupLoop->getVolume( &_currvolume );

  if ( _currvolume == 0.0f )
    _currvolume = 0.0f;

  else
    _GroupLoop->setVolume( _currvolume - ( ( maxBGMvol / 10 ) ) );
}

void AudioManager::decrease3DSFXVol()
{
  float _currvolume = 0.0f;
  _3DSFXGroup->getVolume( &_currvolume );

  if ( _currvolume == 0.0f )
    _currvolume = 0.0f;

  else
    _3DSFXGroup->setVolume( _currvolume - ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::decrease3DLoopVol()
{
  float _currvolume = 0.0f;
  _3DLoopGroup->getVolume( &_currvolume );

  if ( _currvolume == 0.0f )
    _currvolume = 0.0f;

  else
    _3DLoopGroup->setVolume( _currvolume - ( ( maxSFXvol ) / 10 ) );
}

void AudioManager::decreaseMasterVol()
{
  decreaseSFXVol();
  decreaseLoopVol();
  decrease3DSFXVol();
  decrease3DLoopVol();

  gui_vol--;
}

void AudioManager::StopAllAudio()
{
  for ( int iter = 0; iter < MAX_CHANNEL; iter++ )
  {
    FMOD::Channel *ref = nullptr;
    FMOD_RESULT _result = _system->getChannel( iter, &ref );
    if ( _result == FMOD_OK && ref )
    {
      ref->stop();
    }
  }

  _NowPlayingCont.clear();
  _NowPlaying3DCont.clear();
}

void AudioManager::PauseAllAudio()
{
  for ( int iter = 0; iter < MAX_CHANNEL; iter++ )
  {
    FMOD::Channel *ref = nullptr;
    FMOD_RESULT _result = _system->getChannel( iter, &ref );
    if ( _result == FMOD_OK && ref )
    {
      ref->setPaused( 1 );
    }
  }
}

void AudioManager::UnpauseAllAudio()
{
  for ( int iter = 0; iter < MAX_CHANNEL; iter++ )
  {
    FMOD::Channel *ref = nullptr;
    FMOD_RESULT _result = _system->getChannel( iter, &ref );
    if ( _result == FMOD_OK && ref )
    {
      ref->setPaused( 0 );
    }
  }
}

void AudioManager::AddSFXpath( std::string filepath )
{
  _system->createSound( filepath.c_str(), FMOD_LOOP_OFF, 0, &_sound );
  _SFXCont.insert( std::make_pair( filepath, _sound ) );
  _sound->setSoundGroup( _GroupSFX );
}

void AudioManager::AddLOOPpath( std::string filepath )
{
  _system->createSound( filepath.c_str(), FMOD_LOOP_NORMAL, 0, &_sound );
  _LoopCont.insert( std::make_pair( filepath, _sound ) );
  _sound->setSoundGroup( _GroupLoop );
}

void AudioManager::Add3DSFXpath( std::string filepath )
{
  FMOD_MODE eMode = FMOD_DEFAULT;
  eMode |= FMOD_3D;
  eMode |= FMOD_LOOP_OFF;

  _system->createSound( filepath.c_str(), eMode, 0, &_sound );
  _3DSFXCont.insert( std::make_pair( filepath, _sound ) );
  _sound->setSoundGroup( _3DSFXGroup );
}

void AudioManager::Add3DLOOPpath( std::string filepath )
{
  FMOD_MODE eMode = FMOD_DEFAULT;
  eMode |= FMOD_3D;
  eMode |= FMOD_LOOP_NORMAL;

  _system->createSound( filepath.c_str(), eMode, 0, &_sound );
  _3DLoopCont.insert( std::make_pair( filepath, _sound ) );
  _sound->setSoundGroup( _3DLoopGroup );
}

void AudioManager::StopAllSFX()
{
  _GroupSFX->stop();
  _3DSFXGroup->stop();
  _3DLoopGroup->stop();
}

void AudioManager::StopAllLoop()
{
  _GroupLoop->stop();
}

void AudioManager::AudioUpdate()
{
    //const FMOD_VECTOR pos1{ 0.0f };
    //const FMOD_VECTOR pos2{ 0.0f };

    //These 2 can remove if dont need
  const FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
  const FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

  if ( &Xivi::Service::Locator::Get<Xivi::Graphics::M_FrameBuffer>() != nullptr )
  {
    auto &m_framebuffer = Xivi::Service::Locator::Get<Xivi::Graphics::M_FrameBuffer>();
    auto &cameraLeft = m_framebuffer.CameraGame( 0, 0 );
    auto &cameraRight = m_framebuffer.CameraGame( 0, 1 );
    auto &cameraEye1 = cameraLeft.CameraEye();
    auto &cameraEye2 = cameraRight.CameraEye();
    const FMOD_VECTOR pos1 { cameraEye1.getX(), cameraEye1.getY(), cameraEye1.getZ() };
    const FMOD_VECTOR pos2 { cameraEye2.getX(), cameraEye2.getY(), cameraEye2.getZ() };

    //Check and test if applicable. If not remove it (due to 2 listeners)
    //float dopplerscale = 1.0f;
    float distfactor = 2.0f;
    float rolloffscale = 0.5f;
    _system->set3DSettings( 0.f, distfactor, rolloffscale );

    //Remove forward and up to nullptr if variable is removed.
    //_system->set3DListenerAttributes(0, &pos1, 0, &forward, &up);
    //_system->set3DListenerAttributes(1, &pos2, 0, &forward, &up);

    _system->set3DListenerAttributes( 0, &pos1, 0, 0, 0 );
    _system->set3DListenerAttributes( 1, &pos2, 0, 0, 0 );
  }

  static std::vector<std::unordered_map<Xivi::Entity, FMOD::Channel *>::iterator> to_erase;
  //It is a must to update every loop
  if ( _system )
  {
    if ( !_NowPlaying3DCont.empty() )
    {
      for ( std::unordered_map<Xivi::Entity, FMOD::Channel *>::iterator iter = _NowPlaying3DCont.begin(); iter != _NowPlaying3DCont.end(); iter++ )
      {
        bool isplaying = false;
        iter->second->isPlaying( &isplaying );
        if ( isplaying == false )
        {
            // _NowPlaying3DCont.erase(iter);
          to_erase.push_back( iter );
        }

        else
        {
          const auto &entitypos = m_world->GetComponent<Xivi::Position>( iter->first ).GetGlm();
          FMOD_VECTOR epos { entitypos.x, entitypos.y, entitypos.z };
          iter->second->set3DAttributes( &epos, nullptr );
        }
      }
    }

    static std::vector<std::unordered_map<Xivi::Position *, std::unordered_map<std::string, FMOD::Channel *>>::iterator> to_erase3;

    if ( !_NowPlaying3DLoop.empty() )
    {
      for ( std::unordered_map<Xivi::Position *, std::unordered_map<std::string, FMOD::Channel *>>::iterator iter = _NowPlaying3DLoop.begin(); iter != _NowPlaying3DLoop.end(); iter++ )
      {
        float posx = iter->first->m_position.getX();
        float posy = iter->first->m_position.getY();
        float posz = iter->first->m_position.getZ();
        //`XV_CORE_WARN(posx);
        FMOD_VECTOR epos { posx, posy, posz };
        XV_CORE_WARN( epos.x );
        bool isplaying = false;
        for ( std::unordered_map<std::string, FMOD::Channel *> ::iterator initer = iter->second.begin(); initer != iter->second.end(); initer++ )
        {
          initer->second->isPlaying( &isplaying );

          if ( isplaying == false )
          {
              // _NowPlaying3DCont.erase(iter);
            to_erase3.push_back( iter );
          }

          else
            initer->second->set3DAttributes( &epos, nullptr );
        }
      }
    }
    to_erase.clear();

    static std::vector<std::unordered_map<std::string, FMOD::Channel *>::iterator> to_erase2;
    if ( !_NowPlayingCont.empty() )
    {
      for ( std::unordered_map<std::string, FMOD::Channel *>::iterator iter = _NowPlayingCont.begin(); iter != _NowPlayingCont.end(); iter++ )
      {
        bool isplaying = false;
        iter->second->isPlaying( &isplaying );
        if ( isplaying == false )
        {
            //_NowPlayingCont.erase(iter);
          to_erase2.push_back( iter );
        }
      }
    }

    for ( auto &iter : to_erase )
    {
      _NowPlaying3DCont.erase( iter );
    }
    to_erase.clear();

    for ( auto &iter : to_erase2 )
    {
      _NowPlayingCont.erase( iter );
    }
    to_erase2.clear();

    for ( auto &iter : to_erase3 )
    {
      _NowPlaying3DLoop.erase( iter );
    }
    to_erase3.clear();

    //_system->set3DListenerAttributes(0, &pos, 0, &forward, &up);
    _system->update();
  }
}

void AudioManager::clearCont()
{
  _SFXCont.clear();
  _LoopCont.clear();
  _3DSFXCont.clear();
  _3DLoopCont.clear();
  _NowPlayingCont.clear();
  _NowPlaying3DCont.clear();
}

void AudioManager::UpdateAudioAssets()
{
  if ( _sound )
  {
    _GroupSFX->release();
    _GroupLoop->release();
    _3DSFXGroup->release();
    _3DLoopGroup->release();
  }

  LoadAllAudio( "Assets/Audio" );
}

bool AudioManager::SFXExist( std::string filename )
{
  for ( auto itr = _SFXCont.find( filename ); itr != _SFXCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return true;
  }
  return false;
}

bool AudioManager::LoopExist( std::string filename )
{
  for ( auto itr = _LoopCont.find( filename ); itr != _LoopCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return true;
  }
  return false;
}

bool AudioManager::SFX3DExist( std::string filename )
{
  for ( auto itr = _3DSFXCont.find( filename ); itr != _3DSFXCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return true;
  }
  return false;
}

bool AudioManager::Loop3DExist( std::string filename )
{
  for ( auto itr = _3DLoopCont.find( filename ); itr != _3DLoopCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return true;
  }
  return false;
}

bool AudioManager::IfLoopPlaying( std::string filename )
{
  if ( !_NowPlayingCont.size() )
    return false;

  auto search = _NowPlayingCont.find( filename );

  if ( search != _NowPlayingCont.end() && search->first == filename )
    return true;

  return false;
}

FMOD_VECTOR AudioManager::vec3DtoFMODVEC( const glm::vec3 &vector3D )
{
  FMOD_VECTOR FVEC;
  FVEC.x = vector3D.x;
  FVEC.y = vector3D.y;
  FVEC.z = vector3D.z;

  return FVEC;
}

std::unordered_map<std::string, FMOD::Sound *> AudioManager::GetSFXCont()
{
  return _SFXCont;
}

std::unordered_map<std::string, FMOD::Sound *> AudioManager::GetLoopCont()
{
  return _LoopCont;
}

std::unordered_map<std::string, FMOD::Sound *> AudioManager::Get3DSFXCont()
{
  return _3DSFXCont;
}

std::unordered_map<std::string, FMOD::Sound *> AudioManager::Get3DLoopCont()
{
  return _3DLoopCont;
}

FMOD::Channel *AudioManager::getChlnSFX( std::string filename )
{
  for ( auto itr = _NowPlayingCont.find( filename ); itr != _NowPlayingCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return itr->second;
  }
  return nullptr;
}

FMOD::Channel *AudioManager::getChln3DSFX( Xivi::Entity e )
{
  for ( auto itr = _NowPlaying3DCont.find( e ); itr != _NowPlaying3DCont.end(); itr++ )
  {
    if ( e == itr->first )
      return itr->second;
  }
  return nullptr;
}

FMOD::Channel *AudioManager::getPlayingLoopChannel( std::string filename )
{
  for ( auto itr = _NowPlayingCont.find( filename ); itr != _NowPlayingCont.end(); itr++ )
  {
    if ( filename == itr->first )
      return itr->second;
  }
  return nullptr;
}

FMOD::System *AudioManager::getSystem()
{
  return _system;
}

void AudioManager::SetWorld( Xivi::ECS::World::Instance &world )
{
  if ( std::addressof( *m_world ) != std::addressof( world ) )
  {
    StopAllAudio();
    m_world = &world;
  }

  else
    return;
}

void AudioManager::CrossFade( std::string _LoopIn,
                              std::string _LoopOut,
                              int const _millisec )
{
    //channel_in is the pasive channel which will play soon, it will fade in
    //channel_out is the active channel which is currently playing, it will fade away
  FMOD::Channel *_channelOut = getPlayingLoopChannel( _LoopOut );
  playLoop( _LoopIn, 0 );
  FMOD::Channel *_channelIn = getPlayingLoopChannel( _LoopIn );

  float channelVolumeIn = 0.0f;
  float channelVolumeOut = 1.0f;
  _channelIn->setVolume( 0.0f );

  while ( channelVolumeIn <= 1.0f && channelVolumeOut >= 0.0f )
  {
      //1-(x^2.5) crossfade calculation with 0.15 base value
      //Increment-decrement value is approximately 0.0087 per decided millisecs
    channelVolumeIn += pow( 0.15f, 2.5f );
    channelVolumeOut -= pow( 0.15f, 2.5f );
    _channelIn->setVolume( channelVolumeIn );
    _channelOut->setVolume( channelVolumeOut );
    std::this_thread::sleep_for( std::chrono::milliseconds( _millisec ) );

    if ( channelVolumeOut <= 0.01f && channelVolumeIn >= 0.99f )
    {
      _channelOut->setVolume( 0.0f ); //For precision
      _channelIn->setVolume( 1.0f );
      _channelOut->stop();
      break;
    }
    _channelIn->getVolume( &channelVolumeIn );
    _channelOut->getVolume( &channelVolumeOut );
  }
}

float *AudioManager::getSFXGroupVol()
{
  float *vol = nullptr;
  _GroupSFX->getVolume( vol );
  return vol;
}

float *AudioManager::getLoopGroupVol()
{
  float *vol = nullptr;
  _GroupLoop->getVolume( vol );
  return vol;
}

float *AudioManager::get3DSFXGroupVol()
{
  float *vol = nullptr;
  _3DSFXGroup->getVolume( vol );
  return vol;
}

float *AudioManager::get3DLoopGroupVol()
{
  float *vol = nullptr;
  _3DLoopGroup->getVolume( vol );
  return vol;
}

void AudioManager::setSFXGroupVol( float vol )
{
  _GroupSFX->setVolume( vol );
}

void AudioManager::setLoopGroupVol( float vol )
{
  _GroupLoop->setVolume( vol );
}

void AudioManager::set3DSFXGroupVol( float vol )
{
  _3DSFXGroup->setVolume( vol );
}

void AudioManager::set3DLoopGroupVol( float vol )
{
  _3DLoopGroup->setVolume( vol );
}

void AudioManager::setMuteSFX( bool b )
{
  if ( b )
  {
    _GroupSFX->getVolume( &initialSFXvol );
    _GroupSFX->setVolume( 0.f );
    _3DSFXGroup->setVolume( 0.f );
    _3DLoopGroup->setVolume( 0.f );
  }

  else
  {
    _GroupSFX->setVolume( initialSFXvol );
    _3DSFXGroup->setVolume( initialSFXvol );
    _3DLoopGroup->setVolume( initialSFXvol );
  }
}

void AudioManager::setMuteBGM( bool b )
{
  if ( b )
  {
    _GroupLoop->getVolume( &initialBGMvol );
    _GroupLoop->setVolume( 0.f );
  }

  else
  {
    _GroupLoop->setVolume( initialBGMvol );
  }
}

i32 AudioManager::GetGUIvol()
{
  return gui_vol;
}

void Init()
{
  AudioManager::Instance().Initialize();
}

void Update()
{
  AudioManager::Instance().AudioUpdate();
}

void Quit()
{
  AudioManager::Instance().clearCont();
}
}