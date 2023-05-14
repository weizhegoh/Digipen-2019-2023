/*****************************************************************
*\file         AudioPanel.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "AudioPanel.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include <IconsFontAwesome5.h>

namespace Xivi::Editor
{
AudioPanel::AudioPanel( std::string name,
                        Ptr<ECS::World::Instance> world,
                        Ptr<Config> configs,
                        Ptr<EntityContext> entity_context,
                        Ptr<FileContext> file_context,
                        Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{}

AudioPanel::~AudioPanel()
{
  m_enabled = false;
}

void AudioPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    ImVec2 size = ImGui::GetContentRegionAvail();
    static size_t c_id { 0 };
    static std::string c_sound {};
    static std::string c_selection {};
    ImGuiHelper::ChildWindow( "Sound Configuration", true, [&]()
    {
      ImGui::Text( "Audio Files" );
      ImGui::Separator();
      for ( auto &ref : m_sounds )
      {
        if ( c_selection != ref )
          ImGui::GetStateStorage()->SetInt( ImGui::GetID( ref.c_str() ), 0 );

        ImGuiHelper::Group( [&]
        {
          if ( ImGui::CollapsingHeader( ref.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap ) )
          {
            c_selection = ref;
            ImGui::Dummy( ImVec2( 0.0f, 10.0f ) );

            if ( ImGui::Button( ( ICON_FA_PLAY_CIRCLE "##" + ref ).c_str(), { 24.f, 22.f } ) )
            {
              //if ( AudioEngine::IsChannelPlaying( c_id ) )
              //  AudioEngine::StopChannel( c_id );
              //c_id = AudioEngine::PlayAudio( ref );
              //c_sound = ref;
            }
            ImGui::SameLine();
            if ( ImGui::Button( ( ICON_FA_STOP_CIRCLE "##" + ref ).c_str(), { 24.f, 22.f } ) )
            {
              //AudioEngine::StopChannel( c_id );
              c_sound.clear();
            }
            ImGui::SameLine();
            //unsigned e_pos = AudioEngine::GetSoundLength( ref );
            //unsigned e_pos_min = e_pos / 60000;
            //unsigned e_pos_sec = ( e_pos % 60000 ) / 1000;
            //auto t_size = ImGui::CalcTextSize( "00:00 / 00:00" );

            //if ( AudioEngine::IsChannelPlaying( c_id ) && c_sound == ref )
            //{
            //  unsigned c_pos = AudioEngine::GetChannelPlaybackPosition( c_id );
            //  unsigned c_pos_min = c_pos / 60000;
            //  unsigned c_pos_sec = ( c_pos % 60000 ) / 1000;
            //  float frac = static_cast<float>( c_pos ) / static_cast<float>( e_pos );
            //  ImGui::ProgressBar( frac, { ImGui::GetContentRegionAvail().x - t_size.x, 0 } );
            //  ImGui::SameLine();
            //  ImGui::Text( "%d:%02d / %d:%02d", c_pos_min, c_pos_sec, e_pos_min, e_pos_sec );
            //}
            //else
            //{
            //  ImGui::ProgressBar( 0, { ImGui::GetContentRegionAvail().x - t_size.x, 0 } );
            //  ImGui::SameLine();
            //  ImGui::Text( "%d:%02d / %d:%02d", 0, 0, e_pos_min, e_pos_sec );
            //}
          }
        } );

        ImGui::SameLine();
        auto region = ImGui::GetContentRegionAvail();
        ImGui::SetCursorPosX( ImGui::GetCursorPosX() + region.x - 20.f );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY() + 1.f );
        ImGui::PushID( ( "Close" + ref ).c_str() );
        if ( ImGui::Button( ICON_FA_TIMES, { 24.0f, 22.0f } ) )
        {
          m_sounds.erase( std::find_if( m_sounds.begin(),
                          m_sounds.end(),
                          [&]( const std::string &lhs )
          {
            if ( lhs == ref )
            {
              //AudioEngine::StopChannel(c_id);
              c_sound.clear();
              return true;
            }
            return false;
          } ) );
          ImGui::PopID();
          break;
        }
        ImGui::PopID();
      }
    } );
    if ( ImGui::BeginDragDropTarget() )
    {
      if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "CreateFromAssetAudio" ); payload )
      {
        std::string file = *static_cast<std::string *>( payload->Data );
        std::filesystem::path p { file };
        if ( p.extension() == ".wav" ||
             p.extension() == ".ogg" ||
             p.extension() == ".mp3" )
          m_sounds.insert( { file } );
      }
      ImGui::EndDragDropTarget();
    }
  } );
}
}

#endif