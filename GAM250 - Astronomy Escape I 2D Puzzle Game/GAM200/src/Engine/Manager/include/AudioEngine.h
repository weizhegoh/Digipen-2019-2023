/*****************************************************************
*\file		 AudioEngine.h
*\brief		 Contains declaration for audio engine

*\author(s)   Goh Wei Zhe	40 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <EnginePch.h>
#include <FakeEngine.h>

struct Audio
{
	int channelCount;

	Audio();
	void Update();
	~Audio();

	FMOD::System* Syst;

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap soundMap;
	ChannelMap channelMap;
};

class AudioEngine
{
public:
	float volume;
	bool mute;
	bool overidemute;

	AudioEngine();
	std::string TypeToString(SoundType sound);
	void LoadSound(const std::string& audioTrack, FMOD_MODE loop);
	void PlaySound(const std::string& audioTrack, FMOD_MODE loop, float vol);
	void UnloadSound(const std::string& audioTrack);
	void UnloadAll();

	void setVolume(float vol);
	void setMute(bool value);
	void toggleMute();
	void setOverrideMute(bool value);

	void CheckInput();
	int ErrorCheck(FMOD_RESULT result);

	void Init();
	void Update();
	void Exit();

	~AudioEngine() = default;
};

#endif