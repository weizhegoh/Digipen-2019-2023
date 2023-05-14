/*****************************************************************
*\file		 GlobalVar.h
*\brief		 This file contains Map Data functions from Screen and
			 Binary

*\author(s)   Lee Liang Ping	50 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Object.h>

/******************************************************************************/
/*!
 \class GlobalVar;

 \brief Map Data functions that allows to convert Screen and Binary position vise vesa

 \param mainCharID
		binaryMapWidth
		binaryMapHeight
		objectSize
		GAME_SIZE
		max_objective
		data
		allobjects
		entrance_door_opened
		exit_door_opened
		level_door_opened
		door
*/
/******************************************************************************/
struct GlobalVar
{
	Entity mainCharID;
	Entity level_doorID;
	Entity mouseID;
	Entity selected_EntityID;
	int binaryMapWidth;
	int binaryMapHeight;
	float objectSize;
	glm::vec2 GAME_SIZE;
	int max_objective;
	Data data;
	int score = 0; 
	double scoreTime = 0.0;
	
	std::map<SoundType, std::string> sound;
	//door usuage
	bool entrance_door_opened = true;
	bool exit_door_opened = false;
	bool level_door_opened = false;
	std::vector<Entity> door;
	float win_time = 0;
	float winscreen_time = 0;
	float logo_time = 0;

	void SetMapData(Data _data, int width, int height);

	ObjectType GetCellValue(glm::vec3 position);
	ObjectType GetCellValue(int X, int Y);
	
	void SetCellValue(mathEngine::Vector3D position, ObjectType setObject);
	void SetCellValue(int X, int Y, ObjectType setObject);
	
	void SetAndConvertCellValue(int X, int Y, ObjectType setObject);
	void SetAndConvertCellValue(mathEngine::Vector3D position, ObjectType setObject);
	
	ObjectType GetAndConvertCellValue(int X, int Y);
	ObjectType GetAndConvertCellValue(mathEngine::Vector3D position);
	
	template<typename T>
	void ConvertScreenToBinary(T& X, T& Y);
	void ConvertScreenToBinary(mathEngine::Vector3D& position);
	mathEngine::Vector3D GetConvertScreenToBinary(const mathEngine::Vector3D position);
	
	template<typename T>
	void ConvertBinaryToScreen(T& X, T& Y);
	void ConvertBinaryToScreen(mathEngine::Vector3D& position);
	mathEngine::Vector3D GetConvertBinaryToScreen(const mathEngine::Vector3D position);
	
	void printMapData();
	void Reset();

	void SetZeroData();
	void ResetMapData();
private:
	Data starting_data;
};