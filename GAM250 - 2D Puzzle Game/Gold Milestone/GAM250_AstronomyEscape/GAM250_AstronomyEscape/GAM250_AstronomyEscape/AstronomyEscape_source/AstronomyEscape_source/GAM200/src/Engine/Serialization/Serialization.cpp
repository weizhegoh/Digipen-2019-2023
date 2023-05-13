/*****************************************************************
*\file		 Serialization.cpp
*\brief		 Serialization functions definitions.

*\author(s)   Kenric Tan Wei Liang	770 lines x 90% Code contribution
*\author(s)   Yip Ling Kit Justin	18  lines x 1% Code contribution
			  Goh Wei Zhe			79  lines x 9% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "Serialization.h"
#include <FakeEngine.h>
#include <chrono>
extern std::unique_ptr<FakeEngine> gFakeEngine;

std::map<std::string, ObjectType> objMap =
{
	{"FLOOR", ObjectType::FLOOR},
	{"MAIN_CHARACTER", ObjectType::MAIN_CHARACTER},
	{"MONSTER", ObjectType::MONSTER},
	{"MONSTER_PATROL", ObjectType::MONSTER_PATROL},
	{"MONSTER_GUIDE", ObjectType::MONSTER_GUIDE},
	{"WALL", ObjectType::WALL},
	{"WALL_HORIZONTAL", ObjectType::WALL_HORIZONTAL},
	{"WALL_VERTICLE", ObjectType::WALL_VERTICLE},
	{"PUSHABLE_BARREL", ObjectType::PUSHABLE_BARREL},
	{"ENTRANCE_DOOR", ObjectType::ENTRANCE_DOOR},
	{"EXIT_DOOR", ObjectType::EXIT_DOOR},
	{"LEVEL_DOOR", ObjectType::LEVEL_DOOR},
	{"BACKGROUND", ObjectType::BACKGROUND},
	{"KEY_OBJECTIVE", ObjectType::KEY_OBJECTIVE},
	{"COLLECTIBLES", ObjectType::COLLECTIBLES},
	{"LEVER_DOOR", ObjectType::LEVER_DOOR},
	{"LEVER_SWITCH", ObjectType::LEVER_SWITCH},
	{"TELEPORTER", ObjectType::TELEPORTER},
	{"TUTORIAL", ObjectType::TUTORIAL},
	{"TUTORIAL_FLOOR", ObjectType::TUTORIAL_FLOOR},
	{"TUTORIAL_BOX", ObjectType::TUTORIAL_BOX},
	{"TUTORIAL_INSTRUCT", ObjectType::TUTORIAL_INSTRUCT},
	//Main Menu
	{"MAIN_MENU_BG", ObjectType::MAIN_MENU_BG},
	{"PLAY", ObjectType::PLAY},
	{"CONTROL", ObjectType::CONTROL},
	{"OPTION", ObjectType::OPTION},
	{"CREDIT", ObjectType::CREDIT},
	{"QUIT", ObjectType::QUIT},
	//Level Select Menu
	{"LEVELSELECT_MENU_BG", ObjectType::LEVELSELECT_MENU_BG},
	{"LEVEL1", ObjectType::LEVEL1},
	{"LEVEL2", ObjectType::LEVEL2},
	{"LEVEL3", ObjectType::LEVEL3},
	{"LEVEL4", ObjectType::LEVEL4},
	{"LEVEL5", ObjectType::LEVEL5},
	{"LEVEL_BACK", ObjectType::LEVEL_BACK},
	//Option Menu
	{"OPTION_MENU_BG", ObjectType::OPTION_MENU_BG},
	{"OPTION_1920x1080", ObjectType::OPTION_1920x1080},
	{"OPTION_800x600", ObjectType::OPTION_800x600},
	{"FULLSCREEN", ObjectType::FULLSCREEN},
	{"BGM", ObjectType::BGM},
	{"OPTION_BACK", ObjectType::OPTION_BACK},
	//How to play Menu
	{"CONTROL_MENU_BG_1", ObjectType::CONTROL_MENU_BG_1},
	{"CONTROL_MENU_BG_2", ObjectType::CONTROL_MENU_BG_2},
	{"CONTROL_MENU_BG_3", ObjectType::CONTROL_MENU_BG_3},

	{"CONTROL_NEXT", ObjectType::CONTROL_NEXT},
	{"CONTROL_BACK", ObjectType::CONTROL_BACK},
	//Credit Menu
	{"CREDIT_MENU_BG_1", ObjectType::CREDIT_MENU_BG_1},
	{"CREDIT_MENU_BG_2", ObjectType::CREDIT_MENU_BG_2},
	{"CREDIT_MENU_BG_3", ObjectType::CREDIT_MENU_BG_3},
	{"CREDIT_MENU_BG_4", ObjectType::CREDIT_MENU_BG_4},
	{"CREDIT_MENU_BG_5", ObjectType::CREDIT_MENU_BG_5},
	{"CREDIT_MENU_BG_6", ObjectType::CREDIT_MENU_BG_6},
	{"CREDIT_NEXT", ObjectType::CREDIT_NEXT},
	{"CREDIT_BACK", ObjectType::CREDIT_BACK},
	//Quit Menu
	{"QUIT_MENU_BG", ObjectType::QUIT_MENU_BG},
	{"QUIT_YES", ObjectType::QUIT_YES},
	{"QUIT_NO", ObjectType::QUIT_NO},
	//Pause Menu
	{"PAUSE_MENU_BG", ObjectType::PAUSE_MENU_BG},
	{"PAUSE_RESUME", ObjectType::PAUSE_RESUME},
	{"PAUSE_QUIT", ObjectType::PAUSE_QUIT},
	{"PAUSE_MAINMENU", ObjectType::PAUSE_MAINMENU},
	{"PAUSE_HOWTOPLAY", ObjectType::PAUSE_HOWTOPLAY},
	{"PAUSE_HOWTOPLAY_BG", ObjectType::PAUSE_HOWTOPLAY_BG},
	{"PAUSE_HOWTOPLAY_BG3", ObjectType::PAUSE_HOWTOPLAY_BG3},
	{"PAUSE_HOWTOPLAY_BG2", ObjectType::PAUSE_HOWTOPLAY_BG2},

	{"PAUSE_OPTIONS", ObjectType::PAUSE_OPTIONS},
	//Defeat Screen
	{"DEFEAT_BG", ObjectType::DEFEAT_BG},
	{"DEFEAT_RESTART", ObjectType::DEFEAT_RESTART},
	{"DEFEAT_MAINMENU", ObjectType::DEFEAT_MAINMENU},
	//Victory Screen
	{"VICTORY_BG", ObjectType::VICTORY_BG},
	{"VICTORY_CONTINUE", ObjectType::VICTORY_CONTINUE},
	{"VICTORY_RESTART", ObjectType::VICTORY_RESTART},
	{"VICTORY_MAINMENU", ObjectType::VICTORY_MAINMENU},
	//UI
	{"UI_HP_ICON", ObjectType::UI_HP_ICON},
	{"UI_OBJECTIVE_ICON", ObjectType::UI_OBJECTIVE_ICON},
	{"UI_HP_COUNTER", ObjectType::UI_HP_COUNTER},
	//Resolution Buttons
	{"RES_1920_1080", ObjectType::RES_1920_1080},
	{"RES_800_600", ObjectType::RES_800_600},
	//
	{"RED_DOT", ObjectType::RED_DOT},
	{"TURRET", ObjectType::TURRET},
	{"CIRCLE", ObjectType::CIRCLE},
	{"SQUARE", ObjectType::SQUARE},

	{"LOADING_SCREENS", ObjectType::LOADING_SCREENS},
	{"LOADING_LOGO", ObjectType::LOADING_LOGO},
	{"POPUP", ObjectType::POPUP},

	{ "MOUSE_CURSOR", ObjectType::MOUSE_CURSOR },

	{"DEFAULT", ObjectType::DEFAULT}
};

std::map<std::string, TextureType> texMap =
{
	{"MAIN_CHARACTER",TextureType::MAIN_CHARACTER},
	{"MONSTER",TextureType::MONSTER},
	{"MONSTER_PATROL",TextureType::MONSTER_PATROL},
	{"MONSTER_GUIDE", TextureType::MONSTER_GUIDE},
	{"WALL",TextureType::WALL},
	{"WALL2",TextureType::WALL2},
	{"WALL3",TextureType::WALL3},
	{"WALL_MOSS_1",TextureType::WALL_MOSS_1},
	{"WALL_MOSS_2",TextureType::WALL_MOSS_2},
	{"WALL_MOSS_3",TextureType::WALL_MOSS_3},

	{"WALL_HORIZONTAL",TextureType::WALL_HORIZONTAL},
	{"WALL_VERTICLE",TextureType::WALL_VERTICLE},
	{"PUSHABLE_BARREL",TextureType::PUSHABLE_BARREL},
	{"COLLECTIBLES", TextureType::COLLECTIBLES},

	{"LEVER_DOOR",TextureType::LEVER_DOOR},
	//
	{"TELEPORTER", TextureType::TELEPORTER},
	{"TELEPORTER_LEFT", TextureType::TELEPORTER_LEFT},
	{"TELEPORTER_UP", TextureType::TELEPORTER_UP},
	{"TELEPORTER_DOWN", TextureType::TELEPORTER_DOWN},
	{"TELEPORTER_RIGHT", TextureType::TELEPORTER_RIGHT},

	//
	{"LEVER_SWITCH_OFF_UP" , TextureType::LEVER_SWITCH_OFF_UP},
	{"LEVER_SWITCH_OFF_DOWN" , TextureType::LEVER_SWITCH_OFF_DOWN},
	{"LEVER_SWITCH_OFF_LEFT" , TextureType::LEVER_SWITCH_OFF_LEFT},
	{"LEVER_SWITCH_OFF_RIGHT" , TextureType::LEVER_SWITCH_OFF_RIGHT},
	{"LEVER_SWITCH_ON_UP" , TextureType::LEVER_SWITCH_ON_UP},
	{"LEVER_SWITCH_ON_DOWN" , TextureType::LEVER_SWITCH_ON_DOWN},
	{"LEVER_SWITCH_ON_LEFT" , TextureType::LEVER_SWITCH_ON_LEFT},
	{"LEVER_SWITCH_ON_RIGHT" , TextureType::LEVER_SWITCH_ON_RIGHT},

	//
	{"ENTRANCE_DOOR",TextureType::ENTRANCE_DOOR},
	{"EXIT_DOOR",TextureType::EXIT_DOOR},
	{"LEVEL_DOOR",TextureType::LEVEL_DOOR},

	{"BACKGROUND",TextureType::BACKGROUND},
	{"KEY_OBJECTIVE",TextureType::KEY_OBJECTIVE},
	//Main Menu
	{"MAIN_MENU_BG", TextureType::MAIN_MENU_BG},
	{"PLAY", TextureType::PLAY},
	{"PLAY_HOVER", TextureType::PLAY_HOVER},
	{"CONTROL", TextureType::CONTROL},
	{"HOW_TO_PLAY_HOVER", TextureType::HOW_TO_PLAY_HOVER},
	{"OPTION", TextureType::OPTION},
	{"OPTION_HOVER", TextureType::OPTION_HOVER},
	{"CREDIT", TextureType::CREDIT},
	{"CREDIT_HOVER", TextureType::CREDIT_HOVER},
	{"QUIT", TextureType::QUIT},
	{"QUIT_HOVER", TextureType::QUIT_HOVER},
	//Level Select Menu
	{"LEVELSELECT_MENU_BG", TextureType::LEVELSELECT_MENU_BG},
	{"LEVEL1", TextureType::LEVEL1},
	{"LEVEL1_HOVER", TextureType::LEVEL1_HOVER},
	{"LEVEL2", TextureType::LEVEL2},
	{"LEVEL2_HOVER", TextureType::LEVEL2_HOVER},
	{"LEVEL3", TextureType::LEVEL3},
	{"LEVEL3_HOVER", TextureType::LEVEL3_HOVER},
	{"LEVEL4", TextureType::LEVEL4},
	{"LEVEL4_HOVER", TextureType::LEVEL4_HOVER},
	{"LEVEL5", TextureType::LEVEL5},
	{"LEVEL5_HOVER", TextureType::LEVEL5_HOVER},
	{"LEVEL_BACK", TextureType::LEVEL_BACK},
	{"OPTION_MENU_BG", TextureType::OPTION_MENU_BG},
	//Option Menu
	{"OPTION_1920x1080", TextureType::OPTION_1920x1080},
	{"OPTION_1920x1080_HOVER", TextureType::OPTION_1920x1080_HOVER},
	{"OPTION_800x600", TextureType::OPTION_800x600},
	{"OPTION_800x600_HOVER", TextureType::OPTION_800x600_HOVER},
	{"TOGGLE_ON", TextureType::TOGGLE_ON},
	{"TOGGLE_OFF", TextureType::TOGGLE_OFF},
	{"OPTION_BACK", TextureType::OPTION_BACK},
	//How to play Menu
	{"CONTROL_MENU_BG_1", TextureType::CONTROL_MENU_BG_1},
	{"CONTROL_MENU_BG_2", TextureType::CONTROL_MENU_BG_2},
	{"CONTROL_MENU_BG_3", TextureType::CONTROL_MENU_BG_3},
	{"CONTROL_NEXT", TextureType::CONTROL_NEXT},
	{"CONTROL_BACK", TextureType::CONTROL_BACK},
	//Credit Menu
	{"CREDIT_MENU_BG_1", TextureType::CREDIT_MENU_BG_1},
	{"CREDIT_MENU_BG_2", TextureType::CREDIT_MENU_BG_2},
	{"CREDIT_MENU_BG_3", TextureType::CREDIT_MENU_BG_3},
	{"CREDIT_MENU_BG_4", TextureType::CREDIT_MENU_BG_4},
	{"CREDIT_MENU_BG_5", TextureType::CREDIT_MENU_BG_5},
	{"CREDIT_MENU_BG_6", TextureType::CREDIT_MENU_BG_6},
	{"CREDIT_NEXT", TextureType::CREDIT_NEXT},
	{"CREDIT_BACK", TextureType::CREDIT_BACK},
	//Quit Menu
	{"QUIT_MENU_BG", TextureType::QUIT_MENU_BG},
	{"QUIT_YES", TextureType::QUIT_YES},
	{"QUIT_YES_HOVER", TextureType::QUIT_YES_HOVER},
	{"QUIT_NO", TextureType::QUIT_NO},
	{"QUIT_NO_HOVER", TextureType::QUIT_NO_HOVER},
	//Defeat Screen
	{"DEFEAT_BG", TextureType::DEFEAT_BG},
	{"DEFEAT_RESTART", TextureType::DEFEAT_RESTART},
	{"DEFEAT_RESTART_HOVER", TextureType::DEFEAT_RESTART_HOVER},
	{"DEFEAT_MAINMENU", TextureType::DEFEAT_MAINMENU},
	{"DEFEAT_MAINMENU_HOVER", TextureType::DEFEAT_MAINMENU_HOVER},
	//Victory Screen
	{"VICTORY_BG", TextureType::VICTORY_BG},
	{"VICTORY_CONTINUE", TextureType::VICTORY_CONTINUE},
	{"VICTORY_CONTINUE_HOVER", TextureType::VICTORY_CONTINUE_HOVER},
	{"VICTORY_RESTART", TextureType::VICTORY_RESTART},
	{"VICTORY_RESTART_HOVER", TextureType::VICTORY_RESTART_HOVER},
	{"VICTORY_MAINMENU", TextureType::VICTORY_MAINMENU},
	{"VICTORY_MAINMENU_HOVER", TextureType::VICTORY_MAINMENU_HOVER},
	//UI
	{"UI_HP_ICON", TextureType::UI_HP_ICON},
	{"UI_OBJECTIVE_ICON", TextureType::UI_OBJECTIVE_ICON},
	//UI COUNTER
	{ "UI_HP_COUNTER_0", TextureType::UI_HP_COUNTER_0 },
	{ "UI_HP_COUNTER_1", TextureType::UI_HP_COUNTER_1 },
	{ "UI_HP_COUNTER_2", TextureType::UI_HP_COUNTER_2 },
	{ "UI_HP_COUNTER_3", TextureType::UI_HP_COUNTER_3 },
	{ "UI_HP_COUNTER_4", TextureType::UI_HP_COUNTER_4 },
	{ "UI_HP_COUNTER_5", TextureType::UI_HP_COUNTER_5 },
	{ "UI_HP_COUNTER_6", TextureType::UI_HP_COUNTER_6 },
	{ "UI_HP_COUNTER_7", TextureType::UI_HP_COUNTER_7 },
	{ "UI_HP_COUNTER_8", TextureType::UI_HP_COUNTER_8 },
	{ "UI_HP_COUNTER_9", TextureType::UI_HP_COUNTER_9 },
	//Back & Next Button
	{"BACK_BUTTON_HOVER", TextureType::BACK_BUTTON_HOVER},
	{"NEXT_BUTTON_HOVER", TextureType::NEXT_BUTTON_HOVER},
	//Pause Menu
	{"PAUSE_MENU_BG", TextureType::PAUSE_MENU_BG},
	{"PAUSE_RESUME", TextureType::PAUSE_RESUME},
	{"PAUSE_RESUME_HOVER", TextureType::PAUSE_RESUME_HOVER},
	{"PAUSE_QUIT", TextureType::PAUSE_QUIT},
	{"PAUSE_QUIT_HOVER", TextureType::PAUSE_QUIT_HOVER},
	{"PAUSE_MAINMENU", TextureType::PAUSE_MAINMENU},
	{"PAUSE_MAINMENU_HOVER", TextureType::PAUSE_MAINMENU_HOVER},
	{"PAUSE_HOWTOPLAY", TextureType::PAUSE_HOWTOPLAY},
	{"PAUSE_HOWTOPLAY_HOVER", TextureType::PAUSE_HOWTOPLAY_HOVER},
	{"PAUSE_HOWTOPLAY_BG", TextureType::PAUSE_HOWTOPLAY_BG},
	{"PAUSE_HOWTOPLAY_BG3", TextureType::PAUSE_HOWTOPLAY_BG3},
	{"PAUSE_HOWTOPLAY_BG2", TextureType::PAUSE_HOWTOPLAY_BG2},
	{"PAUSE_OPTIONS", TextureType::PAUSE_OPTIONS},
	{"PAUSE_OPTIONS_HOVER", TextureType::PAUSE_OPTIONS_HOVER},
	//Resolution Buttons
	{"RES_1920_1080", TextureType::RES_1920_1080},
	{"RES_800_600", TextureType::RES_800_600},
	{"RES_1920_1080_HOVER", TextureType::RES_1920_1080_HOVER},
	{"RES_800_600_HOVER", TextureType::RES_800_600_HOVER},
	//LoadingScreens
	{"LOADINGBG1", TextureType::LOADINGBG1},
	{"LOADINGBG2", TextureType::LOADINGBG2},
	{"LOADINGLOGO1", TextureType::LOADINGLOGO1},
	{"LOADINGLOGO2", TextureType::LOADINGLOGO2},
	{"LOADINGLOGO3", TextureType::LOADINGLOGO3},
	//Cinematic
	{ "Cinematic_1", TextureType::Cinematic_1 },
	{ "Cinematic_2", TextureType::Cinematic_2 },
	{ "Cinematic_3", TextureType::Cinematic_3 },
	{ "Cinematic_4", TextureType::Cinematic_4 },
	{ "Cinematic_5", TextureType::Cinematic_5 },
	{ "Cinematic_6", TextureType::Cinematic_6 },
	{ "Cinematic_7", TextureType::Cinematic_7 },
	{ "Cinematic_8", TextureType::Cinematic_8 },
	{ "Cinematic_9", TextureType::Cinematic_9 },
	{ "Cinematic_10", TextureType::Cinematic_10 },
	{ "Cinematic_11", TextureType::Cinematic_11 },
	{ "Cinematic_12", TextureType::Cinematic_12 },
	//
	{"BACKGROUND2", TextureType::BACKGROUND2}, 
	{ "POPUP_1", TextureType::POPUP_1, },
	{ "POPUP_2", TextureType::POPUP_2, },
	{ "POPUP_3", TextureType::POPUP_3, },
	{ "POPUP_4", TextureType::POPUP_4, },
	{ "POPUP_5", TextureType::POPUP_5, },
	{ "MOUSE_CURSOR", TextureType::MOUSE_CURSOR },
	//
	{"RED_DOT", TextureType::RED_DOT},
	{"TURRET", TextureType::TURRET},
	{"DEFAULT",TextureType::DEFAULT},

	//
	{"TUTORIAL", TextureType::TUTORIAL},
	{"TUTORIAL_FLOOR", TextureType::TUTORIAL_FLOOR},
	{"TUTORIAL_TEXT_1", TextureType::TUTORIAL_TEXT_1 },
	{"TUTORIAL_TEXT_2", TextureType::TUTORIAL_TEXT_2 },
	{"TUTORIAL_TEXT_3", TextureType::TUTORIAL_TEXT_3 },
	{"TUTORIAL_TEXT_4", TextureType::TUTORIAL_TEXT_4 },
	{"TUTORIAL_TEXT_5", TextureType::TUTORIAL_TEXT_5 },
	{"TUTORIAL_TEXT_6", TextureType::TUTORIAL_TEXT_6 },
	{"TUTORIAL_TEXT_7", TextureType::TUTORIAL_TEXT_7 },
	{"TUTORIAL_TEXT_8", TextureType::TUTORIAL_TEXT_8 },
	{"TUTORIAL_TEXT_9", TextureType::TUTORIAL_TEXT_9 },
	{"TUTORIAL_BOX", TextureType::TUTORIAL_BOX },
	{"TUTORIAL_INSTRUCT", TextureType::TUTORIAL_INSTRUCT },
	{"TUTORIAL_INSTRUCT1", TextureType::TUTORIAL_INSTRUCT1 },
	{"TUTORIAL_INSTRUCT2", TextureType::TUTORIAL_INSTRUCT2 },
	{"TUTORIAL_INSTRUCT3", TextureType::TUTORIAL_INSTRUCT3 }
};

std::map<std::string, ObjectShapes> shapeMap =
{
	{"SQUARE", ObjectShapes::SQUARE},
	{"CIRCLE", ObjectShapes::CIRCLE},
	{"TRIANGLE", ObjectShapes::TRIANGLE}
};

std::map<std::string, ShaderOption> shaderMap =
{
	{"SHAPE", ShaderOption::SHAPE},
	{"FULL_TEXTURE", ShaderOption::FULL_TEXTURE},
	{"ANIMATION_TEXTURE", ShaderOption::ANIMATION_TEXTURE},
	{"TEXTURE_LIGHT", ShaderOption::TEXTURE_LIGHT}
};

std::map<std::string, unsigned int>flagMap =
{
	{"FLAG_ACTIVE", FLAG_ACTIVE},
	{"FLAG_VISIBLE", FLAG_VISIBLE}
};

std::map<std::string, TURN_TYPE> turnMap =
{
	{"TURN_LEFT", TURN_TYPE::TURN_LEFT},
	{"TURN_RIGHT", TURN_TYPE::TURN_RIGHT},
	{"TURN_BACK", TURN_TYPE::TURN_BACK},
	{"TURN_NULL", TURN_TYPE::TURN_NULL}
};

std::map<std::string, FreeRenderType> font_frtMap =
{
	{"MODEL_WORLD", FreeRenderType::MODEL_WORLD},
	{"SCREEN", FreeRenderType::SCREEN}
};

SoundType intToEnum_sound(int sound_id)
{
	return static_cast<SoundType>(sound_id);
}

template<typename TP>
time_t to_time_t(TP tp)
{
	auto systemclock_tp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp - TP::clock::now() + std::chrono::system_clock::now());
	return std::chrono::system_clock::to_time_t(systemclock_tp);
}

void Serialization::load_sound_FromFile()
{

	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	std::string Loadedpath = "Audio/";
	std::vector<std::string>  soundTest;
	std::vector< std::pair<time_t, std::string> > sort_time;
	std::vector<std::string> wrongFormat;

	for (auto& entry : std::filesystem::directory_iterator(Loadedpath))
	{
		auto time = to_time_t(entry.last_write_time());
		std::pair<time_t, std::string> store(time, entry.path().generic_string());

		if (entry.path().extension().string() != ".wav")
		{
			wrongFormat.push_back(entry.path().generic_string() + ",");
		}
		sort_time.push_back(store);
	}

	//check sound format
	if (!wrongFormat.empty())
	{
		std::string name;
		for (const auto& x : wrongFormat)
		{
			name += (x + " ");
		}
		ARTLESS_ASSERT((false), " These are the files with the wrong format " + name + " Please change it to a WAV file!");
	}

	//sort sound file by date
	std::sort(sort_time.begin(), sort_time.end(),
		[](std::pair<time_t, std::string> a, std::pair<time_t, std::string> b) {

			std::string lhs = a.second;
			std::string rhs = b.second;

			std::for_each(std::begin(lhs), std::end(lhs), [](char& c) {
				c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
				});

			std::for_each(std::begin(rhs), std::end(rhs), [](char& c) {
				c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
				});

			return a.first < b.first || (a.first == b.first && (lhs < rhs));
		}
	);

	int i = 0;
	for (auto& x : sort_time)
	{
		globalVar.sound.insert(std::pair<SoundType, std::string>(intToEnum_sound(i), x.second));

		//std::cout << i  <<  x.second << std::endl;
		i++;
	}

}

void Serialization::load_object_FromFile()
{
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	std::string Loadedpath = "Assets/Serialization/Objects/";
	for (auto& entry : std::filesystem::directory_iterator(Loadedpath))
	{
		stageChanger.allobjects.push_back(Serialization::load_objects(entry.path().generic_string()));
		//std::cout << entry.path().generic_string() << std::endl;
	}

}

Object Serialization::load_objects(std::string filename)
{
	Object tmp_object;
	std::ifstream ifs{ filename, std::ios::in };
	if (!ifs)
	{
		std::cout << "Error: Unable to open obj file"
			<< filename << "\n";
		exit(EXIT_FAILURE);
	}

	std::string line;
	std::string name;
	while (!ifs.eof())
	{
		getline(ifs, line);

		if (line.substr(0, 2) == "n ")
		{
			name = (line.substr(2));
			tmp_object._name.first = true;
			tmp_object._name.second = name;
		}
		else if (line.substr(0, 2) == "e ")
		{
			std::string stream = (line.substr(2));
			auto found = objMap.find(stream);
			if (found != objMap.end())
			{
				tmp_object._object.first = true;
				tmp_object._object.second = { found->second };
			}
		}
		else if (line.substr(0, 3) == "so ")
		{
			std::istringstream stream(line.substr(3));

			std::string shaderOption;
			stream >> shaderOption;

			auto found = shaderMap.find(shaderOption);
			if (found != shaderMap.end())
			{
				tmp_object._render.first.first = true;
				tmp_object._render.second.shaderOption = { found->second };
			}
		}
		else if (line.substr(0, 3) == "rs ")
		{
			std::istringstream stream(line.substr(3));

			std::string shapes;
			stream >> shapes;

			auto found = shapeMap.find(shapes);
			if (found != shapeMap.end())
			{
				tmp_object._render.first.second = true;

				if (!gFakeEngine->GetShapes(found->second))
					gFakeEngine->PushShapeAssets(found->second);
				tmp_object._render.second.shape = { gFakeEngine->GetShapes(found->second) };
			}
		}
		else if (line.substr(0, 3) == "rp ")
		{
			std::istringstream numbers(line.substr(3));
			tmp_object._renderPos.first = true;
			numbers
				>> tmp_object._renderPos.second.offset.x
				>> tmp_object._renderPos.second.offset.y;


		}
		else if (line.substr(0, 3) == "rt ")
		{
			std::istringstream stream(line.substr(3));
			std::string texName;
			std::string texStr;

			stream >> texName;
			stream >> texStr;
			auto found = texMap.find(texName);
			if (found != texMap.end())
			{
				tmp_object._texture.first = true;
				if (!gFakeEngine->GetTexture(found->second))
					gFakeEngine->PushTextureAssets(found->second, texStr.c_str());
				tmp_object._texture.second.push_back({ gFakeEngine->GetTexture(found->second) });
			}

		}
		else if (line.substr(0, 3) == "ra ")
		{
			std::istringstream stream(line.substr(3));
			std::string aniName;
			std::string aniStr;

			stream >> aniName;
			stream >> aniStr;

			auto found = texMap.find(aniName);
			if (found != texMap.end())
			{
				tmp_object._animation.first = true;
				gFakeEngine->PushObjectAnimations(found->second, aniStr.c_str());
				tmp_object._animation.second = { gFakeEngine->GetObjectAnimations(found->second) };
			}
		}
		else if (line.substr(0, 2) == "t ")
		{
			tmp_object._transform.first = true;
			std::string freeRenderType;
			std::istringstream numbers(line.substr(2));
			numbers
				>> tmp_object._transform.second.scale.x
				>> tmp_object._transform.second.scale.y
				>> tmp_object._transform.second.scale.z;
			numbers
				>> tmp_object._transform.second.angle;

			numbers >> freeRenderType;
			if (!freeRenderType.empty())
			{
				auto found = font_frtMap.find(freeRenderType);
				if (found != font_frtMap.end())
				{
					tmp_object._transform.first = true;
					tmp_object._transform.second.renderType = found->second;
				}
			}
		}
		else if (line.substr(0, 2) == "p ")
		{
			tmp_object._position.first = true;
			std::istringstream numbers(line.substr(2));
			numbers
				>> tmp_object._position.second.pos.x
				>> tmp_object._position.second.pos.y
				>> tmp_object._position.second.pos.z;
			tmp_object._position.second.starting_pos = tmp_object._position.second.pos;
		}
		else if (line.substr(0, 3) == "rb ")
		{
			std::istringstream numbers(line.substr(3));
			tmp_object._rigidbody.first = true;
			numbers
				>> tmp_object._rigidbody.second.velocity.x
				>> tmp_object._rigidbody.second.velocity.y
				>> tmp_object._rigidbody.second.velocity.z;

			numbers
				>> tmp_object._rigidbody.second.acceleration.x
				>> tmp_object._rigidbody.second.acceleration.y
				>> tmp_object._rigidbody.second.acceleration.z;

		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream numbers(line.substr(2));
			tmp_object._force.first = true;
			numbers
				>> tmp_object._force.second.mResultantForce.x
				>> tmp_object._force.second.mResultantForce.y
				>> tmp_object._force.second.mResultantForce.z;

			numbers
				>> tmp_object._force.second.lifeTime;

			numbers
				>> tmp_object._force.second.invMass;
		}
		else if (line.substr(0, 4) == "col ")
		{
			tmp_object._colour.first = true;
			std::istringstream numbers(line.substr(4));
			numbers
				>> tmp_object._colour.second.colour.x
				>> tmp_object._colour.second.colour.y
				>> tmp_object._colour.second.colour.z;
		}
		else if (line.substr(0, 5) == "aabb ")
		{
			tmp_object._aabb.first = true;
			std::istringstream numbers(line.substr(5));
			numbers
				>> tmp_object._aabb.second.size.x
				>> tmp_object._aabb.second.size.y;
		}
		else if (line.substr(0, 5) == "state")
		{
			tmp_object._state.first = true;
		}
		else if (line.substr(0, 5) == "flag ")
		{
			std::istringstream stream(line.substr(5));
			std::string flag_1;
			std::string flag_2;
			stream >> flag_1 >> flag_2;

			auto found = flagMap.find(flag_1);
			if (found != flagMap.end())
			{
				tmp_object._flag.second.activeFlag = found->second;

				auto found_2 = flagMap.find(flag_2);
				if (found_2 != flagMap.end())
				{
					tmp_object._flag.second.activeFlag |= found_2->second;
				}
			}
			else
				tmp_object._flag.second.activeFlag = FLAG_INACTIVE;
			tmp_object._flag.first = true;

		}
		else if (line.substr(0, 5) == "cflag")
		{
			tmp_object._cflag.first = true;
		}
		else if (line.substr(0, 9) == "inventory")
		{
			tmp_object._inventory.first = true;
		}
		else if (line.substr(0, 2) == "ai")
		{
			tmp_object._ai.first = true;

			std::istringstream stream(line.substr(3));
			std::string collision_turn;
			std::string open_turn;

			stream >> collision_turn >> open_turn >> tmp_object._ai.second.turn_delay_max >> tmp_object._ai.second.turn_delay;

			auto found = turnMap.find(collision_turn);
			if (found != turnMap.end())
				tmp_object._ai.second.turn_type = found->second;

			auto found_2 = turnMap.find(open_turn);
			if (found_2 != turnMap.end())
				tmp_object._ai.second.corner_turn = found_2->second;
		}
		else if (line.substr(0, 5) == "light")
		{
			tmp_object._light.first = true;
			if (line.length() > 5)
			{
				std::istringstream numbers(line.substr(6));
				numbers
					>> tmp_object._light.second.highlight.r
					>> tmp_object._light.second.highlight.g
					>> tmp_object._light.second.highlight.b
					>> tmp_object._light.second.highlight.a;
			}
		}
		else if (line.substr(0, 7) == "f_light")
		{
			tmp_object._lightFlag.first = true;
			if (line.length() > 7)
			{
				std::istringstream numbers(line.substr(8));
				{
					int v1, v2, v3, v4, v5;
					numbers >> v1 >> v2 >> v3 >> v4 >> v5;

					tmp_object._lightFlag.second.Read(v1, v2, v3, v4 , v5);

				}
			}
		}
		else if (line.substr(0, 5) == "laser")
		{
			tmp_object._laser.first = true;
		}
		else if (line.substr(0, 11) == "loadingtype")
		{
			tmp_object._loadingType.first = true;
		}
		else if (line.substr(0, 7) == "leverid")
		{
			tmp_object._lever.first = true;
		}
		else if (line.substr(0, 10) == "teleporter")
		{
			tmp_object._teleporter.first = true;
		}
		else if (line.substr(0, 3) == "ui ")
		{
			tmp_object._ui.first = true;
			std::istringstream numbers(line.substr(3));
			numbers >> tmp_object._ui.second.popup;
		}
		else if (line.substr(0, 6) == "keyObj")
		{
			tmp_object._keyObjective.first = true;
		}
		else if (line.substr(0, 8) == "lifetime")
		{
			tmp_object._lifetime.first = true;
			//tmp_object._lifetime.second.Random(5.f, 10.f);
		}
		else if (line.substr(0, 5) == "mouse")
		{
			tmp_object._mouse.first = true;
		//tmp_object._lifetime.second.Random(5.f, 10.f);
		}	
		else if (line.substr(0, 2) == "# ")
		{
			continue;
		}
	}
	return tmp_object;
}

void Serialization::testObjData(std::vector<Object> _objects)
{
	for (const auto& x : _objects)
	{
		std::cout << "************************************************************" << std::endl;

		std::cout << "name : " << x._name.second << std::endl;

		std::cout << "obj type" << static_cast<int>(x._object.second.objType) << std::endl;

		for (const auto& y : x._texture.second)
		{
			std::cout << &y.texture->pTex << std::endl;


			if (y.texture == nullptr)
				std::cout << "render::texture not added" << std::endl;

		}
		if (x._render.second.shape == nullptr)
			std::cout << "render::shape not added" << std::endl;
		std::cout << "Scale -> " << " x: " << x._transform.second.scale.x << " y: " << x._transform.second.scale.y << " z: " << x._transform.second.scale.z << std::endl;

		std::cout << "Angle -> " << x._transform.second.angle << std::endl;

		std::cout << "transform render type -> " << EnumToString::Get(x._transform.second.renderType) << std::endl;

		std::cout << "Position -> " << " x: " << x._position.second.pos.x << " y: " << x._position.second.pos.y << " z: " << x._position.second.pos.z << std::endl;

		std::cout << "Velocity -> " << " x: " << x._rigidbody.second.velocity.x << " y: " << x._rigidbody.second.velocity.y << " z: " << x._rigidbody.second.velocity.z << std::endl;

		std::cout << "Acceleration -> " << " x: " << x._rigidbody.second.acceleration.x << " y: " << x._rigidbody.second.acceleration.y << " z: " << x._rigidbody.second.acceleration.z << std::endl;

		std::cout << "Colour -> " << " x: " << x._colour.second.colour.x << " y: " << x._colour.second.colour.y << " z: " << x._colour.second.colour.z << std::endl;
		std::cout << "************************************************************" << std::endl;
		std::cout << std::endl;


	}
}




void Serialization::LoadBoard(std::string filename)
{
	Data _data;
	//variables to store
	int stacks = 0;
	int slices = 0;
	float tileSize = 0.0f;
	int max_objective = 0;
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	std::ifstream ifs{ filename, std::ios::in };
	if (!ifs)
	{
		std::cout << "Error: Unable to open obj file"
			<< filename << "\n";
		exit(EXIT_FAILURE);
	}

	std::string line;
	while (!ifs.eof())
	{
		getline(ifs, line);

		if (line == " ")
		{
			continue;
		}
		else if (line.substr(0, 8) == "Stacks: ")
		{
			std::istringstream numbers(line.substr(8));
			numbers >> stacks;
		}
		else if (line.substr(0, 8) == "Slices: ")
		{
			std::istringstream numbers(line.substr(8));
			numbers >> slices;
		}
		else if (line.substr(0, 10) == "TileSize: ")
		{
			std::istringstream numbers(line.substr(10));
			numbers >> tileSize;
		}
		else if (line.substr(0, 2) == "# ")
		{
			continue;
		}
		else if (line.substr(0, 8) == "MapStart")
		{
			ARTLESS_ASSERT((stacks != 0 && slices != 0), "Width and Height is not defined first");
			//defining map size
			_data._mapdata.resize(stacks, std::vector<ObjectType>(slices));
			_data._texdata.resize(stacks, std::vector<int>(slices));
			_data._dirdata.resize(stacks, std::vector<ObjectDirection>(slices));
			_data._leverID.resize(stacks, std::vector<int>(slices));
			_data._teleporterID.resize(stacks, std::vector<int>(slices));

			getline(ifs, line);

			int _slices = 0;
			int _stacks = 0;

			while (line.substr(0, 3) != "End")
			{
				if (_slices == slices)
				{
					getline(ifs, line);
					_slices = 0;
					_stacks++;
				}
				if (_stacks == stacks)
					continue;
				std::istringstream numbers(line);
				int objNumber = 0, texNumber = 0, dirNumber = 0, leverId = 0, teleporterID = 0;
				while (numbers >> objNumber)
				{
					auto found = serializationVar.objData.find(objNumber);
					if (found != serializationVar.objData.end())
					{
						if (found->second.first == ObjectType::KEY_OBJECTIVE)
						{
							max_objective++;
						}
						//object ID
						_data._mapdata[_stacks][_slices] = found->second.first;
					}

					if (numbers.peek() == '/')
					{
						//texture ID
						numbers.ignore(1, '/');
						numbers >> texNumber;
						_data._texdata[_stacks][_slices] = texNumber;
						if (numbers.peek() == '/')
						{
							//direction ID
							numbers.ignore(1, '/');
							numbers >> dirNumber;
							auto found_dir = serializationVar.dirData.find(dirNumber);
							if (found_dir != serializationVar.dirData.end())
							{
								_data._dirdata[_stacks][_slices] = found_dir->second.first;
							}
							if (numbers.peek() == '/' && (_data._mapdata[_stacks][_slices] == ObjectType::LEVER_DOOR
								|| _data._mapdata[_stacks][_slices] == ObjectType::LEVER_SWITCH))
							{
								//direction ID
								numbers.ignore(1, '/');
								numbers >> leverId;
								_data._leverID[_stacks][_slices] = leverId;

								//if (numbers.peek() == '/')
								//{
								//	//direction ID
								//	numbers.ignore(1, '/');

								//}
							}
							else
								_data._leverID[_stacks][_slices] = -1;

							if (numbers.peek() == '/' && (_data._mapdata[_stacks][_slices] == ObjectType::TELEPORTER))
							{
								numbers.ignore(1, '/');
								numbers >> teleporterID;
								_data._teleporterID[_stacks][_slices] = teleporterID;
								serializationVar.teleporterCount++;
							}
							else
								_data._teleporterID[_stacks][_slices] = -1;
						}
					}
					_slices++;

				}
			}
		}
	}

	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	globalVar.SetMapData(_data, slices, stacks);
	globalVar.objectSize = tileSize;
	//globalVar.GAME_SIZE = (glm::vec2{ (slices / 2.f) * globalVar.objectSize, (stacks / 2.f) * globalVar.objectSize }) * (16.f/9.f);
	globalVar.GAME_SIZE = glm::vec2{ 1920 , 1080 };
	globalVar.max_objective = max_objective;
	Camera2d& camera = gFakeEngine->GetGraphicManager<Camera2d>();
	camera.cam_window = globalVar.GAME_SIZE * 0.75f;
	//globalVar.SetStartingData();

	std::cout << globalVar.max_objective << std::endl;
	std::cout << " finish loading board" << std::endl;
}

void Serialization::creation_of_gameplay_stuff(std::string filename, const std::vector<Object> allobjects)
{
	std::ifstream ifs{ filename, std::ios::in };
	if (!ifs)
	{
		std::cout << "Error: Unable to open obj file"
			<< filename << "\n";
		exit(EXIT_FAILURE);
	}

	std::string name;
	Position obj_pos;
	Transform transform;
	Colour colour;
	MainMenuType mainMenuType;
	UI ui;
	colour.colour = { 1.F, 1.F, 1.F };

	bool transform_changes = false;
	bool texture_changes = false;
	bool color_changes = false;
	bool ui_changes = false;
	unsigned __int64 textureID = 1;
	std::string line;
	while (!ifs.eof())
	{
		getline(ifs, line);

		if (line.substr(0, 2) == "n ")
		{
			name = (line.substr(2));
			std::cout << name << std::endl;
			//if (name == "MOUSE_CURSOR")
 		//		std::cout << "fuidshfidsh";
		}
		else if (line.substr(0, 2) == "t ")
		{
			transform_changes = true;
			std::istringstream numbers(line.substr(2));
			numbers
				>> transform.scale.x
				>> transform.scale.y
				>> transform.scale.z;
			numbers
				>> transform.angle;

			std::string freeRenderType;
			numbers >> freeRenderType;
			if (!freeRenderType.empty())
			{
				auto found = font_frtMap.find(freeRenderType);
				if (found != font_frtMap.end())
				{
					transform.renderType = found->second;
				}
			}

		}
		else if (line.substr(0, 4) == "col ")
		{
			color_changes = true;
			std::istringstream numbers(line.substr(4));
			numbers
				>> colour.colour.x
				>> colour.colour.y
				>> colour.colour.z;
		}
		else if (line.substr(0, 3) == "gs ")
		{
			GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
			std::istringstream numbers(line.substr(2));
			numbers
				>> globalVar.GAME_SIZE.x >> globalVar.GAME_SIZE.y;
			Camera2d& camera = gFakeEngine->GetGraphicManager<Camera2d>();
			camera.cam_window = globalVar.GAME_SIZE;
		}
		else if (line.substr(0, 4) == "tex ")
		{
			texture_changes = true;
			std::istringstream numbers(line.substr(4));
			numbers >> textureID;
		}
		else if (line.substr(0, 2) == "p ")
		{
			std::istringstream numbers(line.substr(2));
			numbers
				>> obj_pos.pos.x
				>> obj_pos.pos.y
				>> obj_pos.pos.z;
			obj_pos.starting_pos = obj_pos.pos;
		}
		else if (line.substr(0, 3) == "ui ")
		{
			ui_changes = true;
			std::istringstream numbers(line.substr(2));
			numbers >> ui.popup;
		}
		else if (line.substr(0, 7) == "mmTpye ")
		{
			std::istringstream numbers(line.substr(2));
			numbers
				>> mainMenuType.type;
		}
		else if (line.substr(0, 3) == "end")
		{
			auto& ecsGameEngine = gFakeEngine->GetECSGameManager<ECSManager>();
			Entity entity = ecsGameEngine.CreateEntity();
			std::cout << entity << std::endl;
			std::string tmpname = name;
			for (const auto& x : allobjects)
			{
				if (name == x._name.second)
				{
					//adding object
					if (x._object.first)
						ecsGameEngine.AddComponent(entity, x._object.second);
					//adding shape and shader option
					if (x._render.first.first && x._render.first.second)
						ecsGameEngine.AddComponent(entity, x._render.second);
					//adding texture

					if (x._animation.first && x._renderPos.first)
					{
						ecsGameEngine.AddComponent(entity, x._animation.second);
						ecsGameEngine.AddComponent(entity, x._renderPos.second);
					}
					if (x._texture.first)
					{
						if (texture_changes)
						{
							ecsGameEngine.AddComponent(entity, x._texture.second[textureID - 1]);
						}
						else
							ecsGameEngine.AddComponent(entity, x._texture.second[0]);
					}

					//adding transform
					if (x._transform.first)
					{
						if (transform_changes)
							ecsGameEngine.AddComponent(entity, transform);
						else
							ecsGameEngine.AddComponent(entity, x._transform.second);
					}
					//adding position
					if (x._position.first)
						ecsGameEngine.AddComponent(entity, obj_pos);
					//adding rigidbody
					if (x._rigidbody.first)
						ecsGameEngine.AddComponent(entity, x._rigidbody.second);
					//adding forces
					if (x._force.first)
						ecsGameEngine.AddComponent(entity, x._force.second);
					//adding colour
					if (x._colour.first)
					{
						if (color_changes)
							ecsGameEngine.AddComponent(entity, colour);
						else
							ecsGameEngine.AddComponent(entity, x._colour.second);
					}
					//adding aabb
					if (x._aabb.first)
						ecsGameEngine.AddComponent(entity, x._aabb.second);
					//adding state
					if (x._state.first)
						ecsGameEngine.AddComponent(entity, x._state.second);
					//adding flag
					if (x._flag.first)
						ecsGameEngine.AddComponent(entity, x._flag.second);
					//adding collision flag
					if (x._flag.first)
						ecsGameEngine.AddComponent(entity, x._cflag.second);
					//adding inventory
					if (x._inventory.first)
						ecsGameEngine.AddComponent(entity, x._inventory.second);
					//adding ai
					if (x._ai.first)
						ecsGameEngine.AddComponent(entity, x._ai.second);
					//adding light
					if (x._light.first)
						ecsGameEngine.AddComponent(entity, x._light.second);
					if (x._lightFlag.first)
						ecsGameEngine.AddComponent(entity, x._lightFlag.second);
					//adding laser
					if (x._laser.first)
						ecsGameEngine.AddComponent(entity, x._laser.second);
					if (x._loadingType.first)
						ecsGameEngine.AddComponent(entity, x._loadingType.second);
					if (x._ui.first)
					{
						if (ui_changes)
							ecsGameEngine.AddComponent(entity, ui);
						else
							ecsGameEngine.AddComponent(entity, x._ui.second);
					}
					if (x._lifetime.first)
						ecsGameEngine.AddComponent(entity, x._lifetime.second);
					if (x._mouse.first)
						ecsGameEngine.AddComponent(entity, x._mouse.second);
				}
			}
			ui_changes = false;
			transform_changes = false;
			texture_changes = false;
			color_changes = false;
		}
		else if (line.substr(0, 2) == "# ")
		{
			continue;
		}
	}
	std::cout << " finish creation " << std::endl;
}

void Serialization::AssignBoard(const Data _data, const std::vector<Object> allobjects)
{
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	std::vector< std::vector<ObjectType> >::const_iterator stack;
	std::vector<ObjectType>::const_iterator slices;
	//float startPos_stack, startPos_slice = 0;
	float objectOffset = globalVar.objectSize / 2.f;
	for (int i = 0; i < _data._mapdata.size(); ++i)
	{
		for (int j = 0; j < _data._mapdata[0].size(); ++j)
		{
			Position obj_pos;
			obj_pos.pos = {
			   (-globalVar.objectSize * (_data._mapdata[0].size() / 2.f) + objectOffset) + (globalVar.objectSize * j),
			   (globalVar.objectSize * (_data._mapdata.size() / 2.f) - objectOffset) - (globalVar.objectSize * i),
				0.0f
			};
			obj_pos.starting_pos = obj_pos.pos;
			auto& ecsGameEngine = gFakeEngine->GetECSGameManager<ECSManager>();
			Entity entity = ecsGameEngine.CreateEntity();

			for (const auto& x : allobjects)
			{
				if (_data._mapdata[i][j] == x._object.second.objType)
				{
					//Light randLight = Light{ glm::vec4(x._light.second.highlight.x) };

					if (x._name.second == "MAIN_CHARACTER")
					{
						globalVar.data._mapdata[i][j] = ObjectType::FLOOR;
						globalVar.mainCharID = entity;
					}
					else if (x._name.second == "MONSTER" || x._name.second == "MONSTER_PATROL" || x._name.second == "MONSTER_GUIDE")
						globalVar.data._mapdata[i][j] = ObjectType::FLOOR;
					else if(x._name.second == "LEVEL_DOOR")
						globalVar.level_doorID = entity;
					else if (x._name.second == "MOUSE_CURSOR")
						globalVar.mouseID = entity;
						
					//else if (x._name.second == "WALL")
					//{
					//	randLight.highlight.w = mathEngine::RandNumberBetween(.2f, 1.f);
					//}



					//adding object
					if (x._object.first)
						ecsGameEngine.AddComponent(entity, x._object.second);
					//adding shape and shader option
					if (x._render.first.first && x._render.first.second)
						ecsGameEngine.AddComponent(entity, x._render.second);
					//adding texture
					if (x._animation.first && x._renderPos.first)
					{
						ecsGameEngine.AddComponent(entity, x._animation.second);
						ecsGameEngine.AddComponent(entity, x._renderPos.second);
					}
					if (x._texture.first)
					{
						if (x._name.second == "TUTORIAL_FLOOR")
						{
							serializationVar.tutorialMap.insert({ entity , x._texture.second[_data._texdata[i][j]] });
							//serializationVar.tutorialMap[entity] = _data._texdata[i][j];
							ecsGameEngine.AddComponent(entity, x._texture.second[0]);
						}
						else
							ecsGameEngine.AddComponent(entity, x._texture.second[_data._texdata[i][j]]);
					}

					//adding transform
					if (x._transform.first)
						ecsGameEngine.AddComponent(entity, x._transform.second);
					//adding position
					if (x._position.first)
						ecsGameEngine.AddComponent(entity, obj_pos);
					//adding rigidbody
					if (x._rigidbody.first)
						ecsGameEngine.AddComponent(entity, x._rigidbody.second);
					//adding forces
					if (x._force.first)
						ecsGameEngine.AddComponent(entity, x._force.second);
					//adding colour
					if (x._colour.first)
						ecsGameEngine.AddComponent(entity, x._colour.second);
					//adding aabb
					if (x._aabb.first)
						ecsGameEngine.AddComponent(entity, x._aabb.second);
					//adding state
					if (x._state.first)
						ecsGameEngine.AddComponent(entity, State{ x._state.second.objectState,_data._dirdata[i][j] });
					//adding flag
					if (x._flag.first)
						ecsGameEngine.AddComponent(entity, x._flag.second);
					//adding collision flag
					if (x._flag.first)
						ecsGameEngine.AddComponent(entity, x._cflag.second);
					//adding inventory
					if (x._inventory.first)
						ecsGameEngine.AddComponent(entity, x._inventory.second);
					//adding ai
					if (x._ai.first)
						ecsGameEngine.AddComponent(entity, x._ai.second);
					//adding light
					if (x._light.first)
						ecsGameEngine.AddComponent(entity, x._light.second);
					//adding lightflag
					if (x._lightFlag.first)
						ecsGameEngine.AddComponent(entity, x._lightFlag.second);

					//adding laser
					if (x._laser.first)
						ecsGameEngine.AddComponent(entity, x._laser.second);
					if (x._loadingType.first)
						ecsGameEngine.AddComponent(entity, x._loadingType.second);
					if (x._lever.first)
						ecsGameEngine.AddComponent(entity, Lever{ globalVar.data._leverID[i][j] , false });
					if (x._teleporter.first)
						ecsGameEngine.AddComponent(entity, Teleporter{ globalVar.data._teleporterID[i][j] });
					if(x._keyObjective.first)
						ecsGameEngine.AddComponent(entity, x._keyObjective.second);
					if (x._lifetime.first)
					{
						ecsGameEngine.AddComponent(entity, LifeTime(mathEngine::RandNumberBetween(1.f, 2.f)));
					}

				}
			}
		}
	}
	std::cout << " finish assigning " << std::endl;
}