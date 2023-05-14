/*****************************************************************
*\file         TimerEngine.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "TimerEngine.h"
#include "Xivi/ECS/XVECS.h"

namespace Xivi
{
  // Initalize data
void TimerEngine::Initialise() noexcept
{
  m_stepcounter = m_counter = m_fps = 0;
  m_timescale = 1.0f;
  m_scaleddt = m_fixedscaledtime = m_fixedtime = m_scaledtime = m_frametracker = m_accumulatedtime = m_elapsedtime = m_deltatime = 0.0f;
  m_prevtime = HighResClock::now();
}

// Update loop
void TimerEngine::Update() noexcept
{
  // Reset counter
  m_stepcounter = 0;
  // Increase counter by one
  ++m_counter;

  TimePt cur_time = HighResClock::now();
  Nanosec delta = std::chrono::duration_cast<Nanosec>( cur_time - m_prevtime );
  m_deltatime = ( static_cast<float>( delta.count() ) * 1e-9f );
  m_deltatime = Math::Min( m_deltatime, s_fixeddt );
  m_prevtime = cur_time;
  m_elapsedtime += m_deltatime;
  m_accumulatedtime += m_deltatime;
  m_scaleddt = m_deltatime * m_timescale;
  m_scaledtime += m_scaleddt;

  // Every 1 second
  if ( m_elapsedtime - m_frametracker >= 1.0f )
  {
    // Save the FPS
    m_fps = m_counter;
    // Reset the counter
    m_counter = 0;
    // Update frame tracker
    m_frametracker = m_elapsedtime;
  }

  // FPS control
  while ( m_accumulatedtime >= s_fixeddt )
  {
    m_accumulatedtime -= s_fixeddt;
    ++m_stepcounter;
    m_fixedtime += s_fixeddt;
    m_fixedscaledtime += s_fixeddt * m_timescale;
  }
}

// Get the step count
size_t TimerEngine::GetStepCount() const noexcept
{
  return m_stepcounter;
}

// Get the fps
size_t TimerEngine::GetFPS() const noexcept
{
  return m_fps;
}

// Get the delta time
f32 TimerEngine::GetDeltaTime() const noexcept
{
  return m_deltatime;
}

// Get the scaled delta time
f32 TimerEngine::GetScaledDeltaTime() const noexcept
{
  return m_scaleddt;
}

// Get the elapsed time
f32 TimerEngine::GetElapsedTime() const noexcept
{
  return m_elapsedtime;
}

// Get fixed time
f32 TimerEngine::GetFixedTime() const noexcept
{
  return m_fixedtime;
}

// Get scaled fixed time
f32 TimerEngine::GetScaledFixedTime() const noexcept
{
  return m_fixedscaledtime;
}

// Get time scale
f32 TimerEngine::GetTimeScale() const noexcept
{
  return m_timescale;
}

// Set time scale
void TimerEngine::SetTimeScale( const f32 scale ) noexcept
{
  m_timescale = scale;
}

void TimerEngine::StartFrameRate() noexcept
{
  #ifdef XV_EDITOR
  m_cur_time = GetElapsedTime();
  m_start_time = HighResClock::now();
  #endif
}

void TimerEngine::EndFrameRate( const ECS::System::ID system_id, const std::string name ) noexcept
{
  #ifdef XV_EDITOR
  auto cur_time = HighResClock::now();
  Nanosec time_spent = std::chrono::duration_cast<Nanosec>( cur_time - m_start_time );
  auto time_diff_in_sec = ( static_cast<float>( time_spent.count() ) * 1e-9f );
  m_total_time += time_diff_in_sec;
  m_system_fps[system_id] = { m_system_fps[system_id].first + time_diff_in_sec, name };
  #endif
}

void TimerEngine::Reset() noexcept
{
  #ifdef XV_EDITOR
      // Reset system
  if ( m_cur_time - m_prev_time >= 1.0f )
  {
    m_prev_time = m_cur_time;
    for ( auto &[id, data] : m_system_fps )
      data.first /= m_total_time;
    m_total_time = 1.0f;
  }
  #endif
}
}