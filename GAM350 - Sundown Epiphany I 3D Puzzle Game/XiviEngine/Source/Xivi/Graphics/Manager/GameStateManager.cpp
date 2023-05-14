/*****************************************************************
*\file         GameStateManager.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "GameStateManager.h"
#include "../Sundown/Source/HelperFunctions/GameGUIHelperFunction.h"
#include "Xivi/ECS/World.h"
#include "Xivi/GUI/GUIManager.h"

#include "Systems/Audio/Audio.h"
namespace Xivi::GSM
{
void GSManager::InitManager( Ptr<ECS::World::Instance> w, Ptr<RM> r, GS_STATES state )
{
  mGS_current = mGS_next = mGS_previous = state;
  world = w;
  resource_manager = r;
  InitState();
}

bool GSManager::isQuit()
{
  return mGS_current == GS_QUIT;
}
bool GSManager::isSameState()
{
  return ( mGS_current == mGS_next );
}
bool GSManager::isRestart()
{
  return mGS_current == GS_RESTART;
}
void GSManager::ChangeState( GS_STATES state )
{
  mGS_next = state;
}

void GSManager::UpdateState()
{
  mGS_previous = mGS_current;
  mGS_current = mGS_next;
}
void GSManager::SetCurrentNext_toPrev()
{
  mGS_current = mGS_previous;
  mGS_next = mGS_previous;
}
GS_STATES GSManager::GetPreviousState()
{
  return static_cast<GS_STATES>( mGS_previous );
}
GS_STATES GSManager::GetNextStage( GS_STATES state )
{
  //return static_cast<GS_STATES>(mGS_previous);
  auto list = stateList.find( state );
  ++list;
  return list->first;
}
void GSManager::NextStage()
{
  //auto list = stateList.find(static_cast<GS_STATES>(mGS_current));
  //++list;
  //auto gs = list->first;
  ////if (gs == GAMEOVER2 ||
  ////	gs == GAMEOVER ||
  ////	gs == SETTING ||
  ////	gs == WIN ||
  ////	gs == WIN2)
  ////	return;
  ////if (gs == MAINMENU_NOLOGO ||
  ////	gs == MAINMENU)
  ////	return;
  //if (gs == END)
  //	gs = MAINMENU_NOLOGO;
  //ChangeState(gs);
}
bool GSManager::isMainMenu()
{
  return ( mGS_current == GSM::MAINMENU || mGS_current == GSM::MAINMENU_NOLOGO );
}
void GSManager::InitState()
{
  stateList.insert( { MAINMENU,         "LOOP_MAINMENU.ogg" } );
  stateList.insert( { MAINMENU_NOLOGO,  "LOOP_MAINMENU.ogg" } );
  stateList.insert( { TUTORIAL_1,		 "DIALOGUE_BGM.ogg" } );
  stateList.insert( { LEVEL1_STAGE1,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { LEVEL1_STAGE2,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { LEVEL1_STAGE3,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { LEVEL1_STAGE4,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { TUTORIAL_2,		 "LOOP_LEVEL.ogg" } );
  stateList.insert( { LEVEL2_STAGE1,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { LEVEL2_STAGE2,    "LOOP_LEVEL.ogg" } );
  stateList.insert( { END,              "" } );
  stateList.insert( { SETTING,          "LOOP_MAINMENU.ogg" } );
  stateList.insert( { GAMEOVER,         "SFX_GAMEOVER.ogg" } );
  stateList.insert( { GAMEOVER2,        "SFX_GAMEOVER.ogg" } );
  stateList.insert( { WIN,              "SFX_PUZZLESOLVED.ogg" } );
  stateList.insert( { WIN2,             "SFX_PUZZLESOLVED.ogg" } );

  stateLoadFile.insert( { MAINMENU,         "L1MainMenu.xv" } );
  stateLoadFile.insert( { MAINMENU_NOLOGO,  "L1MainMenu.xv" } );
  stateLoadFile.insert( { TUTORIAL_1,       "GAM350_Design_Level_1_Stage_1_Tutorial.xv" } );
  stateLoadFile.insert( { LEVEL1_STAGE1,    "GAM350_Design_Level_1_Stage_1.xv" } );
  stateLoadFile.insert( { LEVEL1_STAGE2,    "GAM350_Design_Level_1_Stage_2.xv" } );
  stateLoadFile.insert( { LEVEL1_STAGE3,    "GAM350_Design_Level_1_Stage_3.xv" } );
  stateLoadFile.insert( { LEVEL1_STAGE4,    "GAM350_Design_Level_1_Stage_4.xv" } );
  stateLoadFile.insert( { TUTORIAL_2,       "GAM350_Design_Level_1_Stage_2_Tutorial.xv" } );
  stateLoadFile.insert( { LEVEL2_STAGE1,    "GAM350_Design_Level_2_Stage_1.xv" } );
  stateLoadFile.insert( { LEVEL2_STAGE2,    "GAM350_Design_Level_2_Stage_2.xv" } );
  //stateLoadFile.insert({ END_CS,           "GAM350_Design_Level_1_Stage_1_Tutorial.xv" });
  stateLoadFile.insert( { END,			     "GAM350_Design_Summary.xv" } );
  stateLoadFile.insert( { SETTING,          "GAM350_Design_Setting.xv" } );
  stateLoadFile.insert( { GAMEOVER,         "GameOver.xv" } );
  stateLoadFile.insert( { GAMEOVER2,        "GameOver_John.xv" } );
  stateLoadFile.insert( { WIN,              "WinScreen.xv" } );
  stateLoadFile.insert( { WIN2,             "WinScreen_John.xv" } );

  auto &cs = cutSceneFunc;

  cs.AddScene( MAINMENU, "Assets/Textures/DigiPenLogo.dds", 5.f );
  cs.AddScene( MAINMENU, "Assets/Textures/FMOD_logo.dds", 5.f );
  cs.AddScene( MAINMENU, "Assets/Textures/Team_Logo.dds", 5.f );

  cs.AddScene( MAINMENU_NOLOGO, "Assets/Textures/LoadingScreen.dds", 1.f );

  cs.AddScene( TUTORIAL_1, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_1.dds", 5.f, "DIALOGUE_1.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_2.dds", 5.f, "DIALOGUE_2.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_3.dds", 0.5f, "DIALOGUE_3.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_4.dds", 0.5f, "DIALOGUE_4.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_5.dds", 0.2f, "DIALOGUE_SURPRISED.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_6.dds", 0.5f, "DIALOGUE_6.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_7.dds", 0.2f, "DIALOGUE_7.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_8.dds", 0.5f, "DIALOGUE_DIMENSIONSUCC.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_9.dds", 1.5f );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_10.dds", 0.5f, "DIALOGUE_10.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_11.dds", 0.5f, "DIALOGUE_11.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_12.dds", 0.5f, "DIALOGUE_12.ogg" );
  cs.AddScene( TUTORIAL_1, "Assets/Textures/CutScene_13.dds", 5.f, "DIALOGUE_13.ogg" );
  cs.AddBGM( TUTORIAL_1, "LOOP_LEVEL.ogg" );

  cs.AddScene( TUTORIAL_2, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( LEVEL1_STAGE1, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( LEVEL1_STAGE2, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( LEVEL1_STAGE3, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( LEVEL1_STAGE4, "Assets/Textures/LoadingScreen.dds", 1.f );

  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/TRCutScene_1.dds", 0.5f, "DIALOGUE_14.ogg" );
  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/TRCutScene_2.dds", 0.5f, "DIALOGUE_15.ogg" );
  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/TRCutScene_3.dds", 0.5f, "DIALOGUE_16.ogg" );
  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/TRCutScene_4.dds", 0.5f, "DIALOGUE_17.ogg" );
  cs.AddScene( LEVEL2_STAGE1, "Assets/Textures/TRCutScene_5.dds", 5.f, "DIALOGUE_18.ogg" );

  cs.AddScene( LEVEL2_STAGE2, "Assets/Textures/LoadingScreen.dds", 1.f );

  cs.AddScene( END, "Assets/Textures/LoadingScreen.dds", 1.f );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_1.dds", 1.f );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_2.dds", 0.5f, "DIALOGUE_DIMENSIONSUCC.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_3.dds", 0.5f, "DIALOGUE_19.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_4.dds", 0.5f, "DIALOGUE_20.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_5.dds", 0.5f, "DIALOGUE_21.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_6.dds", 0.5f, "DIALOGUE_22.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_7.dds", 0.5f, "DIALOGUE_SURPRISED.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_8.dds", 0.5f, "DIALOGUE_23.ogg" );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_9.dds", 0.8f );
  cs.AddScene( END, "Assets/Textures/FinalCutScene_10.dds", 2.f, "OHSHHH.ogg" );
}

void GSManager::InitScene()
{
  auto &cs = cutSceneFunc;
  if ( cs.IsThereScene( static_cast<GS_STATES>( mGS_current ) ) && once && !retry )
    cs.ActivateScene( static_cast<GS_STATES>( mGS_current ), 100.f );
  if ( retry )
    retry = false;
  auto file = GetCurrentStateAudioFileName();
  auto &audio_mgr = Audio::AudioManager::Instance();
  // Change the bgm only if playing different bgm
  if ( !audio_mgr.IfLoopPlaying( file ) )
  {
    audio_mgr.StopAllLoop();
    if ( !file.empty() )
    {
      if ( mGS_current == WIN || mGS_current == WIN2 || mGS_current == GAMEOVER || mGS_current == GAMEOVER2 )
        audio_mgr.playSFX( file, false );
      else
        audio_mgr.playLoop( file, false );
    }
  }
  ECS::Query::Instance cutsceneui;
  cutsceneui.AllOf<CutSceneUI>();
  world->ForEach( cutsceneui, [&]( Entity &entity )
  {
    Helper::RecursivePushGUI( *world, entity );
  } );
  if ( mGS_current != WIN && mGS_current != WIN2 && mGS_current != GAMEOVER && mGS_current != GAMEOVER2 )
    audio_mgr.StopAllSFX();
  Helper::UpdateGUI( world, resource_manager, static_cast<GS_STATES>( mGS_current ), collectables_collected );
  Helper::RefreshPages( world );
}

void GSManager::UpdateListByIndex( u32 index ) noexcept
{
  list_collectable_collected[index] = collectables_collected;
  list_move_left[index] = move_left;
  if ( list_collectable_highscore[index] < collectables_collected )
  {
    list_collectable_highscore[index] = collectables_collected;
    list_move_highscore[index] = move_left;
  }
  else if ( list_collectable_highscore[index] == collectables_collected && list_move_highscore[index] < move_left )
    list_move_highscore[index] = move_left;
}

void GSManager::UpdateList()
{
  if ( static_cast<GS_STATES>( mGS_next ) != WIN && static_cast<GS_STATES>( mGS_next ) != WIN2 )
    return;

  switch ( static_cast<GS_STATES>( mGS_current ) )
  {
    case LEVEL1_STAGE1:
    UpdateListByIndex( 0 );
    break;
    case LEVEL1_STAGE2:
    UpdateListByIndex( 1 );
    break;
    case LEVEL1_STAGE3:
    UpdateListByIndex( 2 );
    break;
    case LEVEL1_STAGE4:
    UpdateListByIndex( 3 );
    break;
    case LEVEL2_STAGE1:
    UpdateListByIndex( 4 );
    break;
    case LEVEL2_STAGE2:
    UpdateListByIndex( 5 );
    break;
    case TUTORIAL_1:
    UpdateListByIndex( 6 );
    break;
    case TUTORIAL_2:
    UpdateListByIndex( 7 );
    break;
    default:
    break;
  }
}

std::string GSManager::GetSelectedStateFileName( GS_STATES state )
{
  return stateLoadFile.find( state )->second;
}
std::string GSManager::GetCurrentStateFileName()
{
  return stateLoadFile.find( static_cast<GS_STATES>( mGS_current ) )->second;
}
std::string GSManager::GetCurrentStateAudioFileName()
{
  return stateList.find( static_cast<GS_STATES>( mGS_current ) )->second;
}
}