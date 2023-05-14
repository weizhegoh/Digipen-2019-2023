/*****************************************************************
*\file         Audio.h
*\author(s)    Kevin Neo Jian Sheng

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#ifndef AUDIO_H
#define AUDIO_H

#pragma warning(disable: 4505)
#include <string>
//#include <fmod.hpp>
#include <fmod_errors.h>
#include <unordered_map>
#include "AudioPackage.h"
#include "Components/Position.h"

namespace Xivi::Audio
{
class AudioManager : public Singleton<AudioManager>
{
  FMOD::System *_system;
  FMOD::Channel *_channel;
  FMOD::Sound *_sound;
  FMOD::SoundGroup *_GroupSFX = 0;
  FMOD::SoundGroup *_GroupLoop = 0;
  FMOD::SoundGroup *_3DSFXGroup = 0;
  FMOD::SoundGroup *_3DLoopGroup = 0;

  std::unordered_map<std::string, FMOD::Sound *> _SFXCont;
  std::unordered_map<std::string, FMOD::Sound *> _LoopCont;
  std::unordered_map<std::string, FMOD::Sound *> _3DSFXCont;
  std::unordered_map<std::string, FMOD::Sound *> _3DLoopCont;

  std::unordered_map<std::string, FMOD::Channel *> _NowPlayingCont;
  std::unordered_map<Xivi::Entity, FMOD::Channel *> _NowPlaying3DCont;
  std::unordered_map<Xivi::Position *, std::unordered_map<std::string, FMOD::Channel *>> _NowPlaying3DLoop;

  friend class Singleton<AudioManager>;
  AudioManager();
  ~AudioManager();

  float initialBGMvol;
  float initialSFXvol;

  float maxBGMvol = 0.1f;
  float maxSFXvol = 0.3f;

  i32 gui_vol = 10;

public:
  Xivi::ECS::World::Instance *m_world = nullptr;

  int Initialize();
  int LoadFromPath( std::string Filepath );
  int LoadFromMemory( char *MemPtr, unsigned int memSize );

  void PlayAudio();

  void playSFX( std::string, bool pause );
  void playLoop( std::string, bool pause );
  void play3DSFX( Xivi::Entity, std::string );

  void play3DLoop( Xivi::Position *, std::string );

  void CrossFade( std::string _LoopIn, std::string _LoopOut, int const _millisec );

  void AddSFXpath( std::string filepath );
  void AddLOOPpath( std::string filepath );
  void Add3DSFXpath( std::string filepath );
  void Add3DLOOPpath( std::string filepath );

  bool SFXExist( std::string filename );
  bool LoopExist( std::string filename );
  bool SFX3DExist( std::string filename );
  bool Loop3DExist( std::string filename );

  void PauseAudio();
  void UnpauseAudio();
  void StopAudio();

  void StopSpecificLoop( std::string filename );
  void StopSpecific3DLoop( Xivi::Position *, std::string filename );

  float getSFXVol();
  float getLoopVol();
  float get3DSFXVol();
  float get3DLoopVol();

  void setSFXVol( float vol );
  void setLoopVol( float vol );
  void set3DSFXVol( float vol );
  void set3DLoopVol( float vol );
  void setMasterVol( float vol );

  void set3DListener( const glm::vec3 &vPos = glm::vec3 { 0, 0, 0 }, float fVolumedB = 0.0f );

  void increaseSFXVol();
  void increaseLoopVol();
  void increase3DSFXVol();
  void increase3DLoopVol();
  void increaseMasterVol();

  void decreaseSFXVol();
  void decreaseLoopVol();
  void decrease3DSFXVol();
  void decrease3DLoopVol();
  void decreaseMasterVol();

  void StopAllSFX();
  void StopAllLoop();

  void AudioUpdate();
  void LoadAllAudio( std::string filepath );
  void StopAllAudio();
  void PauseAllAudio();
  void UnpauseAllAudio();
  void clearCont();

  bool IfLoopPlaying( std::string filepath );

  void UpdateAudioAssets();
  FMOD_VECTOR vec3DtoFMODVEC( const glm::vec3 &vector3D );

  std::unordered_map<std::string, FMOD::Sound *> GetSFXCont();
  std::unordered_map<std::string, FMOD::Sound *> GetLoopCont();
  std::unordered_map<std::string, FMOD::Sound *> Get3DSFXCont();
  std::unordered_map<std::string, FMOD::Sound *> Get3DLoopCont();

  FMOD::Channel *getChlnSFX( std::string );
  FMOD::Channel *getChln3DSFX( Xivi::Entity );
  FMOD::Channel *getPlayingLoopChannel( std::string );
  FMOD::System *getSystem();

  float *getSFXGroupVol();
  float *getLoopGroupVol();
  float *get3DSFXGroupVol();
  float *get3DLoopGroupVol();

  void setSFXGroupVol( float vol );
  void setLoopGroupVol( float vol );
  void set3DSFXGroupVol( float vol );
  void set3DLoopGroupVol( float vol );

  void setMuteSFX( bool );
  void setMuteBGM( bool );

  void SetWorld( Xivi::ECS::World::Instance &world );

  i32 GetGUIvol();
};

void Init();
void Update();
void Quit();
}

#endif