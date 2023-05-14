/*****************************************************************
*\file		 InputSystem.cpp
*\brief		 InputSystem functions definitions.

*\author(s)   Lee Liang Ping	55 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <InputSystem.h>
#include <FakeEngine.h>
#include <AudioEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
extern float g_dt;
extern unsigned _gameStateCurrent;

void InputSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<InputSystem>(signature);
}

void InputSystem::Update()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	//Window Focus
	input.GetInputBooleen().active_pause_screen = !window.IsFocus();
	gAudioEngine->setOverrideMute(!window.IsFocus());

	if (input.CheckTriggered(GLFW_KEY_LEFT_ALT))
	{
		if (input.CheckPress(GLFW_KEY_H))
			input.ToggleBoolean(input.GetInputBooleen().activate_hack);

		//if (input.CheckPress(GLFW_KEY_L))
		//	_gameStateNext = GS_STATES::GS_LEVELEDIT;

		//if (input.CheckPress(GLFW_KEY_R))
		//	_gameStateNext = GS_STATES::GS_RESTART;

		//if (input.CheckPress(GLFW_KEY_N))
		//{
		//	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
		//	stageChanger.NextState();
		//	stageChanger.Print();
		//}
	}

	if (input.CheckPress(GLFW_KEY_ESCAPE))
		input.ToggleBoolean(input.GetInputBooleen().pause_screen);

	if (input.GetInputBooleen().activateMouseDraw)
		if (input.CheckTriggered(GLFW_MOUSE_BUTTON_LEFT))
			gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
				input.GetMousePosWorld(), 1.f, ObjectDirection::CIRCLE,
				ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
				std::make_pair<float, float>(0, 0),
				std::make_pair<float, float>(0.f, 0.f),
				std::make_pair<float, float>(20, 20),
				std::make_pair<float, float>(2, 2),
				std::vector<ParticleAction>(),
				{ 1.f, 1.f, 0.f }, { 0.7f,0.f,0.f,0.9f }, 1.f, ShaderOption::FULL_TEXTURE,
				ObjectShapes::SQUARE, TextureType::GLOWINGBALL, NULL, mathEngine::RandNumberBetween(1, 1), false, PhysicFlag{ P_NORMAL }));

	//if (input.CheckPress(GLFW_KEY_1))
	//	gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, 1.f);

	//if (input.CheckPress(GLFW_KEY_P))
	//	input.ToggleBoolean(input.GetInputBooleen().line_key);

}