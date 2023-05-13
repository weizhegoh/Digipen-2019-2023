/*****************************************************************
*\file		 PauseMenu.cpp
*\brief		 PauseMenu functions definitions.

*\author(s)   Justin Yip Ling Kit	287 lines x 75% Code contribution
			  Goh Wei Zhe			100 lines x 25% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <PauseMenu.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;


void PauseMenu::Init() {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<PauseMenu>(signature);
}

void PauseMenu::Update() {

	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();

	static int page = PAUSE_HOW_TO_PLAY_1;
	static bool r1920_1080 = true;

	if (input.CheckTriggered(GLFW_KEY_ESCAPE))
		input.GetInputBooleen().pause_state = PauseState::STATE_DEFAULT;

	for (auto const& entity : mEntities) 
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
			
		if (objecttype.objType == ObjectType::LOADING_SCREENS)
			continue;

		if (objecttype.objType == ObjectType::PAUSE_MENU_BG ||
			objecttype.objType == ObjectType::PAUSE_RESUME ||
			objecttype.objType == ObjectType::PAUSE_HOWTOPLAY ||
			objecttype.objType == ObjectType::PAUSE_MAINMENU ||
			objecttype.objType == ObjectType::PAUSE_QUIT ||
			objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG ||
			objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG3 ||
			objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG2 ||
			objecttype.objType == ObjectType::PAUSE_OPTIONS ||
			objecttype.objType == ObjectType::QUIT_MENU_BG ||
			objecttype.objType == ObjectType::QUIT_YES ||
			objecttype.objType == ObjectType::QUIT_NO ||
			objecttype.objType == ObjectType::OPTION_MENU_BG ||
			objecttype.objType == ObjectType::CONTROL_NEXT ||
			objecttype.objType == ObjectType::OPTION_BACK ||
			objecttype.objType == ObjectType::FULLSCREEN ||
			objecttype.objType == ObjectType::BGM ||
			objecttype.objType == ObjectType::RES_1920_1080 ||
			objecttype.objType == ObjectType::RES_800_600) 
		{

			if (!input.GetInputBooleen().pause_screen)
			{
				flag.activeFlag = FLAG_INACTIVE;
			}
			else if (input.GetInputBooleen().pause_screen && !input.GetInputBooleen().win_state)
			{ //Game is paused
				//check pause state

				switch (input.GetInputBooleen().pause_state) 
				{
					//===============================================================
					//						How to play screen
					//===============================================================
					case PauseState::STATE_HOWTOPLAY:

					if (page == PAUSE_HOW_TO_PLAY_1)
					{
						if (objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG ||
							objecttype.objType == ObjectType::CONTROL_NEXT ||
							objecttype.objType == ObjectType::OPTION_BACK)
						{
							flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
						}
						else
							flag.activeFlag = FLAG_INACTIVE;
					}
					else if (page == PAUSE_HOW_TO_PLAY_2)
					{
						if (objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG3 ||
							objecttype.objType == ObjectType::CONTROL_NEXT ||
							objecttype.objType == ObjectType::OPTION_BACK)
						{
							flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
						}
						else
							flag.activeFlag = FLAG_INACTIVE;
					}
					else if (page == PAUSE_HOW_TO_PLAY_3)
					{
						if (objecttype.objType == ObjectType::PAUSE_HOWTOPLAY_BG2 ||
							objecttype.objType == ObjectType::OPTION_BACK)
						{
							flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
						}
						else
							flag.activeFlag = FLAG_INACTIVE;
					}

					if (objecttype.objType == ObjectType::CONTROL_NEXT ||
					objecttype.objType == ObjectType::OPTION_BACK)
					{
						if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos + mathEngine::Vector3D(camera2d.pos.x, camera2d.pos.y, camera2d.pos.z), transform.scale.x, transform.scale.y))
						{
							Hover(entity);

							if (flag.activeFlag && input.CheckPress(GLFW_MOUSE_BUTTON_1))
							{
								if (objecttype.objType == ObjectType::CONTROL_NEXT)
								{
									++page;
									SetChangeTexture(false, entity);
									input.GetInputBooleen().next_page = true;
								}
								if (objecttype.objType == ObjectType::OPTION_BACK)
								{
									page = 0;
									input.GetInputBooleen().pause_state = PauseState::STATE_DEFAULT;
								}
								gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);
							}
						}
		
					break;

					//===============================================================
					//						Options screen
					//===============================================================
					case PauseState::STATE_OPTIONS:
						
					if (objecttype.objType == ObjectType::OPTION_MENU_BG ||
					objecttype.objType == ObjectType::OPTION_BACK ||
					objecttype.objType == ObjectType::FULLSCREEN ||
					objecttype.objType == ObjectType::BGM ||
					objecttype.objType == ObjectType::RES_1920_1080)// ||objecttype.objType == ObjectType::RES_800_600
					{
						flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
					}
					else
						flag.activeFlag = FLAG_INACTIVE;

					for (auto const& x : stageChanger.allobjects)
					{
						if (objecttype.objType == ObjectType::FULLSCREEN)
						{
							if (window.IsFullScreen() == false && x._object.second.objType == objecttype.objType)
							{
								mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;

							}
						}
						if (objecttype.objType == ObjectType::BGM)
						{
							if (gAudioEngine->mute && x._object.second.objType == objecttype.objType)
							{
								mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
							}
						}
					}

					//check if it collides with the screen
					if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos + mathEngine::Vector3D(camera2d.pos.x, camera2d.pos.y, camera2d.pos.z), transform.scale.x, transform.scale.y))
					{
						if (//objecttype.objType == ObjectType::RES_1920_1080 ||
						//	objecttype.objType == ObjectType::RES_800_600 ||
							objecttype.objType == ObjectType::OPTION_BACK)
						{
							Hover(entity);
						}

						if (flag.activeFlag && input.CheckPress(GLFW_MOUSE_BUTTON_1))
						{
							EntityType& objecttype_hoverEntity = mGameECSmanager.GetComponent<EntityType>(input.GetInputBooleen().hoverEntity);
							
							switch (objecttype.objType) 
							{
								case ObjectType::FULLSCREEN:

									if (window.IsFullScreen())
									{
										window.SetWindowFullScreen(false);
										for (auto const& x : stageChanger.allobjects)
										{
											if (x._object.second.objType == objecttype.objType)
											{
												mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
											}
										}
									}
									else
									{
										window.SetWindowFullScreen(true);
										for (auto const& x : stageChanger.allobjects)
										{
											if (x._object.second.objType == objecttype.objType)
											{
												mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[0].texture;
											}
										}
									}

									break;

								case ObjectType::BGM:
									gAudioEngine->mute = !gAudioEngine->mute;

									if (gAudioEngine->mute)
									{
										gAudioEngine->setMute(false);
										for (auto const& x : stageChanger.allobjects)
										{
											if (x._object.second.objType == objecttype.objType)
											{
												mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
											}
										}
									}
									else
									{
										gAudioEngine->setMute(true);
										for (auto const& x : stageChanger.allobjects)
										{
											if (x._object.second.objType == objecttype.objType)
											{
												mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[0].texture;
											}
										}
									}
									break;

								case ObjectType::RES_1920_1080:

									for (auto const& entity_1 : mEntities)
									{
										EntityType& objecttype1 = mGameECSmanager.GetComponent<EntityType>(entity_1);

										if (objecttype1.objType == ObjectType::RES_1920_1080)
										{
											if (r1920_1080)
											{
												window.SetWindowResolution(800, 600);
												SetChangeTexture(false, entity_1);
												//Pause_SetChangeTexture(entity);
												r1920_1080 = false;
											}
											else
											{
												window.SetWindowResolution(1920, 1080);
												SetChangeTexture(true, entity_1);
												//Pause_SetChangeTexture(entity);
												r1920_1080 = true;
											}
										}
									}
									break;
									/*
																	case ObjectType::RES_800_600:
																		window.SetWindowResolution(800, 600);
																		break;
																		*/
								case ObjectType::OPTION_BACK:
									for (auto const& x : stageChanger.allobjects)
									{
										if (x._object.second.objType == objecttype_hoverEntity.objType)
										{
											mGameECSmanager.GetComponent<RenderTex>(input.GetInputBooleen().hoverEntity).texture = x._texture.second[0].texture;
											//mGameECSmanager.GetComponent<RenderTex>(hover_Entity).texture = x._texture.second[0].texture;
										}
									}
									input.GetInputBooleen().hoverEntity = 0;
									input.GetInputBooleen().pause_state = PauseState::STATE_DEFAULT;

									break;
								default:
									break;
							}
							gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);
						}
						
					break;
					//===============================================================
					//						Quit Confirmation screen
					//===============================================================
					case PauseState::STATE_QUIT: //How to play screen
						
						if (objecttype.objType == ObjectType::QUIT_MENU_BG ||
						objecttype.objType == ObjectType::QUIT_YES ||
						objecttype.objType == ObjectType::QUIT_NO
						) {
						flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;

						//check if it collides with the screen
						if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos + mathEngine::Vector3D(camera2d.pos.x, camera2d.pos.y, camera2d.pos.z), transform.scale.x, transform.scale.y)) {
							if (objecttype.objType == ObjectType::QUIT_YES ||
								objecttype.objType == ObjectType::QUIT_NO)
							{
								Hover(entity);
							}
							if (flag.activeFlag && input.CheckPress(GLFW_MOUSE_BUTTON_1)) {
								switch (objecttype.objType) {
								case ObjectType::QUIT_YES:
									_gameStateNext = GS_QUIT;
									break;

								case ObjectType::QUIT_NO:
									input.GetInputBooleen().pause_state = PauseState::STATE_DEFAULT;
									break;

								default:
									break;
								}
								gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);
							}
						}
					}
					else
						flag.activeFlag = FLAG_INACTIVE;
					break;
					//===============================================================
					//						Main Pause Menu screen
					//===============================================================
				default:
					if (objecttype.objType == ObjectType::PAUSE_MENU_BG ||
						objecttype.objType == ObjectType::PAUSE_RESUME ||
						objecttype.objType == ObjectType::PAUSE_OPTIONS ||
						objecttype.objType == ObjectType::PAUSE_HOWTOPLAY ||
						objecttype.objType == ObjectType::PAUSE_MAINMENU ||
						objecttype.objType == ObjectType::PAUSE_QUIT
						) { //Activate if the object is one of the buttons in the default menu
						flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;

						//check if it collides with the buttons
						if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y)) {
							if (objecttype.objType == ObjectType::PAUSE_RESUME ||
								objecttype.objType == ObjectType::PAUSE_OPTIONS ||
								objecttype.objType == ObjectType::PAUSE_HOWTOPLAY ||
								objecttype.objType == ObjectType::PAUSE_MAINMENU ||
								objecttype.objType == ObjectType::PAUSE_QUIT)
							{
								Hover(entity);
							}
							//Add Animation to button
							//Check when the left mouse is clicked
							if (flag.activeFlag && input.CheckPress(GLFW_MOUSE_BUTTON_1)) {
								switch (objecttype.objType)
								{
								case ObjectType::PAUSE_RESUME:
									input.GetInputBooleen().pause_screen = false;
									break;
								case ObjectType::PAUSE_OPTIONS:
									input.GetInputBooleen().pause_state = PauseState::STATE_OPTIONS;
									break;
								case ObjectType::PAUSE_HOWTOPLAY:
									input.GetInputBooleen().pause_state = PauseState::STATE_HOWTOPLAY;
									break;
								case ObjectType::PAUSE_MAINMENU:
									input.GetInputBooleen().hoverEntity = 0;
									_gameStateNext = GS_MAINMENU;
									break;
								case ObjectType::PAUSE_QUIT:
									input.GetInputBooleen().pause_state = PauseState::STATE_QUIT;
									input.RemoveKeyPressed(GLFW_MOUSE_BUTTON_1);
									break;
								default:
									break;
								}
								gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);
							}
						}

					}
					else
						flag.activeFlag = FLAG_INACTIVE;
					break;
					}
				}
			}
		}
	}
	}
}

void PauseMenu::Pause_SetChangeTexture(Entity entity)
{
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	StageChanger& stageChanger = gFakeEngine->GetGameManager<StageChanger>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
	for (auto const& x : stageChanger.allobjects)
	{
		if (x._object.second.objType == objecttype.objType)
		{
			if (mGameECSmanager.GetComponent<RenderTex>(entity).texture == x._texture.second[0].texture)
				//based on the texture in the object file
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
			else
				//change texture to second
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[0].texture;
		}

	}
}

void PauseMenu::Hover(Entity entity)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	if (input.GetInputBooleen().hoverEntity != entity)
	{
		//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
		StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
		EntityType& objecttype_1 = mGameECSmanager.GetComponent<EntityType>(entity);

		for (auto const& x : stageChanger.allobjects)
		{
			if (x._object.second.objType == objecttype_1.objType)
			{
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
			}
			if (input.GetInputBooleen().hoverEntity)
			{
				EntityType& objecttype_2 = mGameECSmanager.GetComponent<EntityType>(input.GetInputBooleen().hoverEntity);

				if (x._object.second.objType == objecttype_2.objType)
				{
					mGameECSmanager.GetComponent<RenderTex>(input.GetInputBooleen().hoverEntity).texture = x._texture.second[0].texture;
				}
			}
		}
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	input.GetInputBooleen().hoverEntity = entity;
}

void PauseMenu::SetChangeTexture(bool check, Entity entity)
{
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
	for (auto const& x : stageChanger.allobjects)
	{
		if (x._object.second.objType == objecttype.objType)
		{
			if (check)
				//based on the texture in the object file
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[0].texture;
			else
				//change texture to second
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
		}

	}
}