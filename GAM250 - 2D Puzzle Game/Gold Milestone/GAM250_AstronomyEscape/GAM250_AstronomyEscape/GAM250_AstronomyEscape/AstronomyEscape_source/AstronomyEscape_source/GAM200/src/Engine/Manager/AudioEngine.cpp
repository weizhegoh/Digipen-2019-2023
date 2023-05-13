/*****************************************************************
*\file		 AudioEngine.cpp
*\brief		 Contains definition for audio engine

*\author(s)  Goh Wei Zhe	179 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "AudioEngine.h"

Audio* audio;

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

Audio::Audio() :Syst{ nullptr }, channelCount{ 0 }
{
    gAudioEngine->ErrorCheck(FMOD::System_Create(&Syst));
    gAudioEngine->ErrorCheck(Syst->init(32, FMOD_INIT_NORMAL, NULL));
}

void Audio::Update()
{
    std::vector<ChannelMap::iterator> stoppedChannel;

    //if channel map is empty, return 
    if (channelMap.empty())
        return;
    else
    {
        for (auto it = channelMap.begin(), itEnd = channelMap.end(); it != itEnd; ++it)
        {
            bool playing = false;
            it->second->isPlaying(&playing);

            //if audio track not play, push back into vector stoppedChannel
            if (!playing)
            {
                stoppedChannel.push_back(it);
            }
        }
    }

    //erase audio track in channel map
    for (auto& it : stoppedChannel)
    {
        channelMap.erase(it);
    }

    //update audio engine
    gAudioEngine->ErrorCheck(Syst->update());
}

//destructor
Audio::~Audio()
{
    gAudioEngine->ErrorCheck(Syst->release());
}

//default constructor
AudioEngine::AudioEngine() :volume{ 1.0f }, mute{ false } {}

std::string AudioEngine::TypeToString(SoundType sound)
{
    GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
    auto found = globalVar.sound.find(sound);
    return found->second;
}


void AudioEngine::LoadSound(const std::string& audioTrack, FMOD_MODE loop)
{
    //find audio track in sound map
    auto it = audio->soundMap.find(audioTrack);

    //if audio map exist in sound map, return
    if (it != audio->soundMap.end())
    {
        std::cout << "Audio Track Exist" << std::endl;
        return;
    }

    FMOD::Sound* sound = nullptr;

    //else load and create audio
    AudioEngine::ErrorCheck(audio->Syst->createSound(audioTrack.c_str(), loop, nullptr, &sound));

    if (sound)
        audio->soundMap[audioTrack] = sound;
}

void AudioEngine::PlaySound(const std::string& audioTrack, FMOD_MODE loop, float vol)
{
    int channelID = audio->channelCount++;

    //if sound map is empty, load audio track
    if (audio->soundMap.empty())
    {
        //std::cout << "Sound Map is empty" << std::endl;
        LoadSound(audioTrack, loop);
    }
    else
    {
        auto it = audio->soundMap.find(audioTrack);

        //if audio track not found in sound map, load audio track
        if (it == audio->soundMap.end())
            LoadSound(audioTrack, loop);
    }

    auto it = audio->soundMap.find(audioTrack);

    FMOD::Channel* channel = nullptr;

    //play audio
    AudioEngine::ErrorCheck(audio->Syst->playSound(it->second, nullptr, true, &channel));


    if (channel)
    {
        AudioEngine::ErrorCheck(channel->setVolume(vol));
        AudioEngine::ErrorCheck(channel->setPaused(false));
        audio->channelMap[channelID] = channel;
    }
}

void AudioEngine::UnloadSound(const std::string& audioTrack)
{
    //find audio track
    auto it = audio->soundMap.find(audioTrack);

    //if audio track not found in sound map, return
    if (it == audio->soundMap.end())
        return;

    //else, release and erase audio track from sound map
    AudioEngine::ErrorCheck(it->second->release());
    audio->soundMap.erase(it);
}

void AudioEngine::UnloadAll()
{
    for (auto x : audio->soundMap)
    {
        AudioEngine::ErrorCheck(x.second->release());
    }
    audio->soundMap.clear();
}

int AudioEngine::ErrorCheck(FMOD_RESULT result)
{
    //fmod error check, prints error message if any fault
    if (result != FMOD_OK)
    {
        std::cout << "FMOD ERROR " << result << std::endl;
        return 1;
    }
    return 0;
}

void AudioEngine::CheckInput()
{
    Input& input = gFakeEngine->GetGraphicManager<Input>();
    if (_gameStateCurrent == GS_STATES::GS_MAINMENU)
    {
        if (input.CheckPress(GLFW_KEY_DOWN) || input.CheckPress(GLFW_KEY_UP))
            gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
    }

    //Press 1 to mute or unmute
    if (input.CheckPress(GLFW_KEY_1))
    {
        toggleMute();
        gAudioEngine->setMute(gAudioEngine->overidemute || gAudioEngine->mute);
    }

    //Press 2 to decrease volume
    if (input.CheckTriggered(GLFW_KEY_2))
    {
        if (gAudioEngine->volume >= 0.0f)
        {
            gAudioEngine->volume -= 0.01f;
            setVolume(gAudioEngine->volume);
        }
    }

    //Press 3 to increase volume
    if (input.CheckTriggered(GLFW_KEY_3))
    {
        if (gAudioEngine->volume < 1.0f)
        {
            gAudioEngine->volume += 0.01f;
            setVolume(gAudioEngine->volume);
        }
    }
}

void AudioEngine::Init()
{
    audio = new Audio();
}

void AudioEngine::Update()
{
    CheckInput();
    audio->Update();
}

//set mute function
void AudioEngine::setMute(bool value)
{
    if (!audio->channelMap.empty())
    {
        for (auto& it : audio->channelMap)
        {
            it.second->setMute(value);
        }
    }
}

//set audio volume function
void AudioEngine::setVolume(float vol)
{
    if (!audio->channelMap.empty())
    {
        for (auto& it : audio->channelMap)
        {
            it.second->setVolume(vol);
        }
    }

    //std::cout << "Curent Volume is " << gAudioEngine->volume << std::endl;
}

void AudioEngine::setOverrideMute(bool value)
{
    gAudioEngine->overidemute = value;
    gAudioEngine->setMute(gAudioEngine->overidemute || gAudioEngine->mute);
}

//toggle mute or unmute function
void AudioEngine::toggleMute()
{
    gAudioEngine->mute = !gAudioEngine->mute;
}

//Unload audio engine
void AudioEngine::Exit()
{
    audio->channelMap.clear();
    audio->soundMap.clear();

    delete audio;
}