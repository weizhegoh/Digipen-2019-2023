/*****************************************************************
*\file		 File main.cpp
*\brief		 Main file of the repo

*\author(s)   Lee Liang Ping 87 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <AudioEngine.h>
#include "../GAM200/Graphics/WindowManager.h"
#include "../GAM200/src/GameState/GameState.h"
#include <glslshader.h>
#include <EntityRegister.h>

//Uncomment the below line to enable console Infomation in ReleaseMode
//#define CONSOLE_RELEASE

#ifndef  _DEBUG
#ifndef  CONSOLE_RELEASE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
// Engines
std::unique_ptr<FakeEngine> gFakeEngine;
std::unique_ptr<AudioEngine> gAudioEngine;
float g_dt = 0.f;
double g_fps = 0.f;

void EnableMemoryLeakChecking(int breakAlloc = -1)
{
	//Set the leak checking flag
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	//If a valid break alloc provided set the breakAlloc
	if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
}

int main()
{
	// Enable run-time memory check for debug builds.
	EnableMemoryLeakChecking();
#pragma warning( push )
#pragma warning( disable : 4127 )

	// Engine Initialize
	gFakeEngine = std::make_unique<FakeEngine>();
	gFakeEngine->Init();
	gAudioEngine = std::make_unique<AudioEngine>();
	gAudioEngine->Init();

	//Register Base Manager ptr
	gFakeEngine->RegisterGraphicManager<Window>();
	gFakeEngine->RegisterGraphicManager<FPS>();
	gFakeEngine->RegisterGraphicManager<Graphics::Gfx::RenderFunctions>();
	gFakeEngine->RegisterGraphicManager<Graphics::Storage>();
	gFakeEngine->RegisterGraphicManager<Input>();
	gFakeEngine->RegisterGraphicManager<Camera2d>();
	gFakeEngine->RegisterGraphicManager<Graphics::FreeType>();


	gFakeEngine->RegisterGraphicManager<StageChanger>();
	gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::MAIN_MENU);

	//Register Game ECS
	gFakeEngine->RegisterECSGameManager<ECSManager>();
	gFakeEngine->RegisterECSGameManager<GameSystemsList>();
	EntityRegister::RegisterECSGameComponents();
	EntityRegister::RegisterECSGameSystem();

	//Register Particle ECS
	gFakeEngine->RegisterECSParticlesManager<ECSManager>();
	gFakeEngine->RegisterECSParticlesManager<ParticleSystemsList>();
	EntityRegister::RegisterECSParticlesComponents();
	EntityRegister::RegisterECSParticlesSystem();

	gFakeEngine->RegisterECSLightManager<ECSManager>();
	gFakeEngine->RegisterECSLightManager<LightSystemsList>();
	EntityRegister::RegisterECSLightComponents();
	EntityRegister::RegisterECSLightSystem();

	//Create Window
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	if (!window.Init(1920, 1080, "Puzzle Name")) {
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	window.SetWindowFullScreen(true);
	window.hideMouseCursor(true);


	//Register Shader
	gFakeEngine->RegisterShader(ShaderOption::SHAPE,
		"Assets/shaders/vertexShader.vert", "Assets/shaders/fragmentShader.frag");
	gFakeEngine->RegisterShader(ShaderOption::FULL_TEXTURE,
		"Assets/shaders/TvertexShader.vert", "Assets/shaders/TfragmentShader.frag");
	gFakeEngine->RegisterShader(ShaderOption::ANIMATION_TEXTURE,
		"Assets/shaders/Animation_vertexShader.vert", "Assets/shaders/Animation_fragmentShader.frag");
	gFakeEngine->RegisterShader(ShaderOption::TEXT,
		"Assets/shaders/Text_vertexShader.vert", "Assets/shaders/Text_fragmentShader.frag");
	gFakeEngine->RegisterShader(ShaderOption::TEXTURE_LIGHT,
		"Assets/shaders/PointLight_VertexShader.vert", "Assets/shaders/PointLight_FragShader.frag");

	//Register Text Font
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::OpenSansRegular, "Assets/Serialization/font/OpenSans-Regular.ttf");
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::OpenSans_Bold, "Assets/Serialization/font/OpenSans-Bold.ttf");
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::OpenSans_Italic, "Assets/Serialization/font/OpenSans-Italic.ttf");
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::OpenSans_BoldItalic, "Assets/Serialization/font/OpenSans-BoldItalic.ttf");
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::OCR_A_EXTENDED, "Assets/Serialization/font/OCRAEXT.ttf");
	gFakeEngine->RegisterFreeTypeFont(FreeType_Font::AgencyFBBold, "Assets/Serialization/font/agencyfb-bold.ttf");

	// System Initialize
	EntityRegister::InitECSGameSystem();
	EntityRegister::InitECSParticlesSystem();
	EntityRegister::InitECSLightSystem();

	//GSM
	GameStatesInitialize();
	GSM_Initialize(GS_MAINMENU);
	GSM_Loop();
	//Clean Up Memory
	window.cleanup();
	gAudioEngine->Exit();
	gFakeEngine->Exit();
#pragma warning( pop )

}