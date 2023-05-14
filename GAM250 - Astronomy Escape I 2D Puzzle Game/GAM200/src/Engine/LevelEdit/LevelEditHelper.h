/*****************************************************************
*\file		 LevelEditHelper.h
*\brief		 Header file declarations for HotSpotCollisionSystem.

*\author(s)   Kenric Tan Wei Liang	22 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <FakeEngine.h>
#include <LevelEditSystem.h> 
#include "../Serialization/Serialization.h"

namespace LevelEditHelper
{
	void calculateGamePos(mathEngine::Vector3D& pos, int i, int j);
	
	void setBoardData();
	
	void createEntity(ObjectType objType, int textureID, ObjectDirection objDirection, Position obj_pos, int leverID, int teleporterID, Entity& entity_ID);

	void getCurrentDirectory();

	void getFileName();

	void LoadData();

	//creating a new board (new text file)
	void CreateBoard();

	//loading an exsisting board
	void LoadBoard(std::string filename);

	//saving the edited board
	void SaveBoard(SaveBoard_MODE MODE, Data _data);

	//Generating the board
	void GenerateBoard();

	//Clear the edited board
	void ClearBoard();
	
	/*to be placed in update*/
	//get position of mouse
	void GetPos();
	//check current input
	void CheckInput();

	void ResetValues();

}