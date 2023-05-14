/*****************************************************************
*\file         CutSceneList.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include <Resource/Manager.h>
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Manager/GameState.h"
namespace Xivi::Graphics
{
struct CutScene
{
  //using CutScenes = std::vector<std::string>;
  std::string m_bgm;
  std::vector<std::string> scenes;
  std::vector<std::string> audio;
  std::vector<float> mInterval;
  std::vector<float> mCurrentTime;
  int index = 0;
};
class CutSceneFunc
{
  using CutSceneList = std::unordered_map<GSM::GS_STATES, CutScene>;
  CutSceneList list;
  GSM::GS_STATES mType;
  //float mCurrentTime = 0.f;
  //float mInterval = 0.f;
  bool audioPlaying = false;
public:
  bool isActive = false;
  void AddScene( GSM::GS_STATES  type, std::string image, float timer, std::string audio = "" );
  void AddBGM( GSM::GS_STATES  type, std::string bgm );
  void ActivateScene( GSM::GS_STATES  type, float interval );
  void PrevScene( bool skip, float dt );
  void NextScene( bool skip, float dt );
  void NextSceneAudio( bool skip, float dt );
  void GetCutScene( std::vector<unsigned int> &images, float dt );
  bool IsThereScene( GSM::GS_STATES state );
  void SkipScene();
  int GetIndex();
};
}