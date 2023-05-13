/*****************************************************************
*\file		 GlobalVar.cpp
*\brief		 This file contains Map Data functions from Screen and
			 Binary

*\author(s)   Lee Liang Ping	139 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <EnginePch.h>
#include <GlobalVar.h>

/******************************************************************************/
/*!
 \fn void GlobalVar::SetMapData(Data _data, int width, int height)

 \brief Set Map Data by copy

 \param _data
		width
		height

 \return none.
*/
/******************************************************************************/
void GlobalVar::SetMapData(Data _data, int width, int height)
{
	data = _data;
	binaryMapWidth = width;
	binaryMapHeight = height;
}

/******************************************************************************/
/*!
 \fn ObjectType GlobalVar::GetCellValue(glm::vec3 position)

 \brief Over load function for glm:vec3 Get Cell Value from the MapData

 \param position

 \return ObjectType of the current Cell.
*/
/******************************************************************************/
ObjectType GlobalVar::GetCellValue(glm::vec3 position)
{
	return GetCellValue((int)position.x, (int)position.y);
}

/******************************************************************************/
/*!
 \fn ObjectType GlobalVar::GetCellValue(glm::vec3 position)

 \brief Get Cell Value from the MapData

 \param X
		Y

 \return ObjectType of the current Cell.
*/
/******************************************************************************/
ObjectType GlobalVar::GetCellValue(int X, int Y)
{
	//ARTLESS_ASSERT(X < binaryMapWidth&& Y < binaryMapHeight&& X >= 0 && Y >= 0, " Error access data out of bound  ");	
	if (X < binaryMapWidth && Y < binaryMapHeight && X >= 0 && Y >= 0)
	{
		if (data._mapdata[Y][X] == ObjectType::KEY_OBJECTIVE || data._mapdata[Y][X] == ObjectType::COLLECTIBLES || data._mapdata[Y][X] == ObjectType::TUTORIAL_FLOOR)
		{
			return ObjectType::FLOOR;
		}
		return data._mapdata[Y][X]; //return the value of the spot
	}
	else
		return ObjectType::OUTOFBOUND; //return FLOOR
}

/******************************************************************************/
/*!
 \fn void GlobalVar::SetCellValue(mathEngine::Vector3D position, ObjectType setObject)

 \brief Overload Function for mathEngine::Vector3D Set Cell Value to the MapData

 \param position
		setObject

 \return none.
*/
/******************************************************************************/
void GlobalVar::SetCellValue(mathEngine::Vector3D position, ObjectType setObject)
{
	SetCellValue((int)position.x, (int)position.y, setObject);
}

/******************************************************************************/
/*!
 \fn void GlobalVar::SetCellValue(mathEngine::Vector3D position, ObjectType setObject)

 \brief  Set Cell Value to the MapData

 \param X
		Y
		setObject

 \return none.
*/
/******************************************************************************/
void GlobalVar::SetCellValue(int X, int Y, ObjectType setObject)
{
	//ARTLESS_ASSERT(X < binaryMapWidth&& Y < binaryMapHeight&& X >= 0 && Y >= 0, " Error access data out of bound  ");
	if (X < binaryMapWidth && Y < binaryMapHeight && X >= 0 && Y >= 0)
	{
		data._mapdata[Y][X] = setObject;
	}
	else
		std::cout << " Error access data out of bound " << std::endl;
}

/******************************************************************************/
/*!
 \fn void GlobalVar::SetAndConvertCellValue(int X, int Y, ObjectType setObject)

 \brief  Set and Convert Screen To Binary to map data

 \param X
		Y
		setObject

 \return none.
*/
/******************************************************************************/
void GlobalVar::SetAndConvertCellValue(int X, int Y, ObjectType setObject)
{
	int x = X, y = Y;
	ConvertScreenToBinary(x, y);
	SetCellValue(x, y, setObject);
}

/******************************************************************************/
/*!
 \fn void GlobalVar::SetAndConvertCellValue(int X, int Y, ObjectType setObject)

 \brief  Overload Function for mathEngine::Vector3D Set and Convert Screen To Binary to map data

 \param position
		setObject

 \return none.
*/
/******************************************************************************/
void GlobalVar::SetAndConvertCellValue(mathEngine::Vector3D position, ObjectType setObject)
{
	int x = (int)position.x, y = (int)position.y;
	SetAndConvertCellValue(x, y, setObject);
}

/******************************************************************************/
/*!
 \fn ObjectType GlobalVar::GetAndConvertCellValue(int X, int Y)

 \brief  Get Converted value of Screen To Binary from the map data

 \param X
		Y

 \return ObjectType of the map data.
*/
/******************************************************************************/
ObjectType GlobalVar::GetAndConvertCellValue(int X, int Y)
{
	int x = X, y = Y;
	ConvertScreenToBinary(x, y);
	return GetCellValue(x, y);
}

/******************************************************************************/
/*!
 \fn ObjectType GlobalVar::GetAndConvertCellValue(mathEngine::Vector3D position)

 \brief Overload Function for Get Converted value of Screen To Binary from the map data

 \param position

 \return ObjectType of the map data.
*/
/******************************************************************************/
ObjectType GlobalVar::GetAndConvertCellValue(mathEngine::Vector3D position)
{
	int x = (int)position.x, y = (int)position.y;
	return GetAndConvertCellValue(x, y);
}

/******************************************************************************/
/*!
 \fn void GlobalVar::ConvertScreenToBinary(T& X, T& Y)

 \brief Convert Value from Screen to Binary

 \param X
		Y

 \return none.
*/
/******************************************************************************/
template<typename T>
void GlobalVar::ConvertScreenToBinary(T& X, T& Y)
{
	X = (T)(abs(X / objectSize + (binaryMapWidth / 2.f)));
	Y = (T)(abs(Y / objectSize - (binaryMapHeight / 2.f)));
}

/******************************************************************************/
/*!
 \fn void GlobalVar::ConvertScreenToBinary(T& X, T& Y)

 \brief Overload Function for Convert Value from Screen to Binary

 \param X
		Y

 \return none.
*/
/******************************************************************************/
void GlobalVar::ConvertScreenToBinary(mathEngine::Vector3D& position)
{
	ConvertScreenToBinary(position.x, position.y);
}

/******************************************************************************/
/*!
 \fn mathEngine::Vector3D GetConvertScreenToBinary(mathEngine::Vector3D position)

 \brief Return Function for Convert Value from Screen to Binary

 \param position

 \return none.
*/
/******************************************************************************/
mathEngine::Vector3D GlobalVar::GetConvertScreenToBinary(const mathEngine::Vector3D position)
{
	mathEngine::Vector3D temp = position;
	ConvertScreenToBinary(temp);
	return mathEngine::Vector3D(temp);
}


/******************************************************************************/
/*!
 \fn void GlobalVar::ConvertScreenToBinary(T& X, T& Y)

 \brief Convert Value from Binary to Screen

 \param X
		Y

 \return none.
*/
/******************************************************************************/
template<typename T>
void GlobalVar::ConvertBinaryToScreen(T& X, T& Y)
{
	X = (-objectSize * (binaryMapWidth / 2.f)) + (objectSize * X);
	Y = (objectSize * (binaryMapHeight / 2.f)) - (objectSize * Y);
}

/******************************************************************************/
/*!
 \fn void GlobalVar::ConvertScreenToBinary(T& X, T& Y)

 \brief Overload Function for Convert Value from Binary to Screen

 \param position

 \return none.
*/
/******************************************************************************/
void GlobalVar::ConvertBinaryToScreen(mathEngine::Vector3D& position)
{
	ConvertBinaryToScreen(position.x, position.y);
}

/******************************************************************************/
/*!
 \fn mathEngine::Vector3D GetConvertBinaryToScreen(mathEngine::Vector3D position);

 \brief Return Function for Convert Value from Screen to Binary

 \param position

 \return none.
*/
/******************************************************************************/
mathEngine::Vector3D GlobalVar::GetConvertBinaryToScreen(const mathEngine::Vector3D position)
{
	mathEngine::Vector3D temp = position;
	ConvertBinaryToScreen(temp);
	return mathEngine::Vector3D(temp);
}

/******************************************************************************/
/*!
 \fn void GlobalVar::printMapData()

 \brief For Debug Purpose Print MapData

 \return none.
*/
/******************************************************************************/
void GlobalVar::printMapData()
{
	std::cout
		<< "=========[Map Data]============\n Width: "
		<< binaryMapWidth << " Height: " << binaryMapHeight
		<< std::endl;
	for (auto& x : data._mapdata)
	{
		for (auto& y : x)
			std::cout << (int)y << " ";
		std::cout << std::endl;
	}
	std::cout << "=========[Map Data]============" << std::endl;

	for (auto& x : data._dirdata)
	{
		for (auto& y : x)
			std::cout << (int)y << " ";
		std::cout << std::endl;
	}

	std::cout << "=========[Dir Data]============" << std::endl;
}

/******************************************************************************/
/*!
 \fn void GlobalVar::Reset()

 \brief Reset the variables in GlobalVar

 \return none.
*/
/******************************************************************************/
void GlobalVar::Reset()
{
	max_objective = binaryMapWidth = binaryMapHeight = NULL;
	objectSize = NULL;
	mainCharID = NULL;
	GAME_SIZE = { 0, 0 };
	data._mapdata.clear();
	data._texdata.clear();
	sound.clear();
	entrance_door_opened = true;
	exit_door_opened = false;
	level_door_opened = false;
	door.clear();
}

void GlobalVar::SetZeroData()
{
	for (auto& x : data._mapdata)
	{
		for (auto& y : x)
			y = ObjectType::FLOOR;
	}
}

void GlobalVar::ResetMapData()
{
	std::copy(starting_data._mapdata.begin(), starting_data._mapdata.end(), std::back_inserter(data._mapdata));
}