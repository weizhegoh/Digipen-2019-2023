/*****************************************************************
*\file		 EntityTypes.h
*\brief		 EntityTypes Component

*\author(s)   Lee Liang Ping	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <GameTypes.h>

struct EntityType
{
    ObjectType objType{ ObjectType::DEFAULT };

    void Print() { std::cout << (int)objType << std::endl; }
    int GetInt() { return (int)objType; }
};


//template<typename T>
//struct EntityType
//{
//	T objType;
//};

//
//enum class ET_MainCharacter {};
//enum class ET_Particle {};
//enum class ET_LoadingScreen {};
//enum class ET_PushableBarrel {};
//
//enum class ET_Laser 
//{ 
//	TURRET,
//	LASER_PROJECTILE 
//};
//
//enum class ET_Monster
//{
//	MONSTER,
//	MONSTER_PATROL,
//	MONSTER_GUIDE,
//};
//
//enum class ET_StaticObjects
//{
//	FLOOR,
//	WALL,
//	WALL_VERTICLE,
//	WALL_HORIZONTAL,
//};
//
//enum class ET_UIObjects
//{
//	MAIN_MENU_BG,
//	PLAY,
//	OPTION,
//	CONTROL,
//	CREDIT,
//	QUIT,
//	//Level Select Menu
//	LEVELSELECT_MENU_BG,
//	LEVEL1,
//	LEVEL2,
//	LEVEL3,
//	TUTORIAL,
//	LEVEL_BACK,
//	//Option Menu
//	OPTION_MENU_BG,
//	OPTION_1920x1080,
//	OPTION_800x600,
//	FULLSCREEN,
//	BGM,
//	OPTION_BACK,
//	//How to play Menu
//	CONTROL_MENU_BG_1,
//	CONTROL_MENU_BG_2,
//	CONTROL_NEXT,
//	CONTROL_BACK,
//	//Credit Menu
//	CREDIT_MENU_BG_1,
//	CREDIT_MENU_BG_2,
//	CREDIT_MENU_BG_3,
//	CREDIT_MENU_BG_4,
//	CREDIT_MENU_BG_5,
//	CREDIT_MENU_BG_6,
//	CREDIT_NEXT,
//	CREDIT_BACK,
//	//Quit Menu
//	QUIT_MENU_BG,
//	QUIT_YES,
//	QUIT_NO,
//	//Pause Menu
//	PAUSE_MENU_BG,
//	PAUSE_RESUME,
//	PAUSE_QUIT,
//	PAUSE_MAINMENU,
//	PAUSE_OPTIONS,
//	PAUSE_HOWTOPLAY,
//	PAUSE_HOWTOPLAY_BG,
//	//Resolution Buttons
//	RES_1920_1080,
//	RES_800_600,
//};
