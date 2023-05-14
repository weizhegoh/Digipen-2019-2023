/*****************************************************************
*\file         TimerEngine.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <chrono>
#include <Types/Base.h>
#include <Utils/Singleton.h>

namespace Xivi
{
  // The fps control
static constexpr f32 s_fixeddt = 1.0f / 60.f;

// Forward declare
template <typename Tag = void>
struct GUID;

namespace ECS::System
{
using ID = GUID<struct SystemID>;
}

// Class for time
struct TimerEngine
{
  // Initialize system
  void Initialise() noexcept;

  // Update
  void Update() noexcept;

  // Get the step count
  size_t GetStepCount() const noexcept;

  // Get fps
  size_t GetFPS() const noexcept;

  // Get delta time
  f32 GetDeltaTime() const noexcept;

  // Get scaled delta time
  f32 GetScaledDeltaTime() const noexcept;

  // Get elapsed time
  f32 GetElapsedTime() const noexcept;

  // Get fixed time
  f32 GetFixedTime() const noexcept;

  // Get scaled fixed time
  f32 GetScaledFixedTime() const noexcept;

  //Get time scale
  f32 GetTimeScale() const noexcept;

  // Set time scale
  void SetTimeScale( const f32 scale ) noexcept;

  void StartFrameRate() noexcept;

  void EndFrameRate( const ECS::System::ID system_id, const std::string name ) noexcept;

  void Reset() noexcept;

  #ifdef XV_EDITOR
      // For performance tracking
  std::map<ECS::System::ID, std::pair<float, std::string>> m_system_fps;
  float m_total_time = 0.0f;
  #endif
private:
  using Nanosec = std::chrono::nanoseconds;
  using HighResClock = std::chrono::high_resolution_clock;
  using TimePt = std::chrono::time_point<HighResClock>;

  #ifdef XV_EDITOR
      // For performance tracking
  float m_cur_time = 0.0f;
  float m_prev_time = 0.0f;
  TimePt m_start_time = HighResClock::now();
  #endif

      // The fps
  size_t m_fps = 0;
  // Counter to count the fps
  size_t m_counter = 0;
  // The previous time
  TimePt m_prevtime = HighResClock::now();
  // Track 1 sec
  f32 m_frametracker = 0.0f;
  // Step counter for fixed delta time
  size_t m_stepcounter = 0;
  // The time accumulated
  f32 m_accumulatedtime = 0.0f;
  // Elaspe time
  f32 m_elapsedtime = 0.0f;
  // Delta time
  f32 m_deltatime = 0.0f;
  // Time scale
  f32 m_timescale = 1.0f;
  // Delta time with scale
  f32 m_scaleddt = 0.0f;
  // Time calcualted with scale
  f32 m_scaledtime = 0.0f;
  // Fixed time
  f32 m_fixedtime = 0.0f;
  // Fixed scaled time
  f32 m_fixedscaledtime = 0.0f;
};
}
