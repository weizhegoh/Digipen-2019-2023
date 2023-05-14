/*****************************************************************
*\file		 LevelEditVar.h
*\brief		 Header file declarations for LevelEditVar.

*\author(s)  Kenric Tan Wei Liang	55 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Object.h>

struct LevelEditVar
{

	enum class CreationMode
	{
		NEW,
		LOAD
	};

	int stack;
	int slice;
	int tilesize;

	std::string filename;
	std::vector<Object> allobjects;
	Data data;

	//check map data whether is it added
	std::vector<std::vector<bool>> checkMapData;
	std::vector<std::vector<Entity>> entityData;
	std::vector<std::vector<int>> boardData;
	std::vector<std::vector<int>> boardData_dir;


	// input checks 
	//imgui generation button
	bool generationTrue = false;
	//imgui generation button to allow user to generate once
	bool generationCheck = true;
	//check whether user is adding
	bool addObject_Check = false;
	//check whether user is deleting
	bool deleteObject_Check = false;
	//check save button click
	bool button_click = false;
	//check whether main character is added
	bool isMainCharacterAdded = false;


	//check whether user is loading
	bool load_board = false;
	//check whether user is creating new
	bool new_board = false;

	//check wehther load_board or new_board is changed
	bool check_load_new_changed = false;
	CreationMode curr_createMode;
	CreationMode prev_createMode;

	//store object
	std::vector<std::string> str_vectorObj;
	int selectedObject = 0;
	std::string selectedObjectName;
	int prev_selectedObject = 9999;

	//selected object type data
	ObjectType selectedObjectType;

	//selected object struct data
	Object selectedObject_data;

	//store texture
	std::vector<std::string> str_vectorTex;
	int selectedTexture;


	//store direction
	std::vector <std::string> str_vectorDir;
	int selectedDir;

	std::vector<std::string> currentDir;
	int selectedFileName;


	//store leverid
	std::vector <std::string> str_vectorLeverID;
	int selectedLeverID;
	unsigned int leverSwitchCount = 1;
	unsigned int leverDoorCount = 1;

	//store textureid
	std::vector <std::string> str_vectorTeleporterID;
	int selectedTeleporterID;
	unsigned int teleporterCount = 1;

	int mousepos_x;
	int mousepos_y;
	
	//output message in imgui
	std::string message;
};