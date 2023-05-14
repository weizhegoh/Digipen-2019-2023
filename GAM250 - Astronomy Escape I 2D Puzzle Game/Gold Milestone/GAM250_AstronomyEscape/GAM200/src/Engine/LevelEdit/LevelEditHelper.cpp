/*****************************************************************
*\file		 LevelEditHelper.cpp
*\brief		 LevelEditHelper functions definitions.

*\author(s)   Kenric Tan Wei Liang	452 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "LevelEditHelper.h"

extern std::unique_ptr<FakeEngine> gFakeEngine;


void LevelEditHelper::calculateGamePos(mathEngine::Vector3D& pos, int i, int j)
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	float objectOffset = leveleditVar.tilesize / 2.f;

	pos.x = (-leveleditVar.tilesize * (leveleditVar.slice / 2.f) + objectOffset) + (leveleditVar.tilesize * j);
	pos.y = (leveleditVar.tilesize * (leveleditVar.stack / 2.f) - objectOffset) - (leveleditVar.tilesize * i);
	pos.z = 0.0f;
}

void LevelEditHelper::setBoardData()
{
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();

	for (int i = 0; i < leveleditVar.stack; i++)
	{
		for (int j = 0; j < leveleditVar.slice; j++)
		{

			for (auto& s : serializationVar.objData)
			{
				if (leveleditVar.data._mapdata[i][j] == s.second.first)
				{
					leveleditVar.boardData[i][j] = s.first;
				}
			}

			for (auto& s : serializationVar.dirData)
			{
				if (leveleditVar.data._dirdata[i][j] == s.second.first)
				{
					leveleditVar.boardData_dir[i][j] = s.first;
				}
			}
		}
	}
}


void LevelEditHelper::createEntity(ObjectType objType, int textureID, ObjectDirection objDirection, Position obj_pos, int leverID, int teleporterID, Entity& entity_ID)
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	auto& ecsGameEngine = gFakeEngine->GetECSGameManager<ECSManager>();

	Entity entity = ecsGameEngine.CreateEntity();
	entity_ID = entity;
	for (const auto& x : leveleditVar.allobjects)
	{
		//if (x._object.second.objType == ObjectType::SQUARE)
		if (x._object.second.objType == objType)
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
				ecsGameEngine.AddComponent(entity, x._texture.second[textureID]);

			//adding transform
			if (x._transform.first)
				ecsGameEngine.AddComponent(entity, x._transform.second);
			//adding position
			if (x._position.first)
				ecsGameEngine.AddComponent(entity, obj_pos);
			//adding rigidbody
			if (x._rigidbody.first)
				ecsGameEngine.AddComponent(entity, x._rigidbody.second);
			//adding colour
			if (x._colour.first)
				ecsGameEngine.AddComponent(entity, x._colour.second);
			//adding aabb
			if (x._aabb.first)
				ecsGameEngine.AddComponent(entity, x._aabb.second);
			//adding state
			if (x._state.first)
				ecsGameEngine.AddComponent(entity, State{ x._state.second.objectState, objDirection });
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
			{
				Light light = x._light.second;
				light.highlight.w = 1.f;
				ecsGameEngine.AddComponent(entity, light);

			}
			if (x._lever.first)
				ecsGameEngine.AddComponent(entity, Lever{ leverID , false });
			if (x._teleporter.first)
				ecsGameEngine.AddComponent(entity, Teleporter{ teleporterID});
			if (x._lightFlag.first)
				ecsGameEngine.AddComponent(entity, LightFlag());
			if(x._keyObjective.first)
				ecsGameEngine.AddComponent(entity, KeyObjective());

		}
	}
}

void LevelEditHelper::getCurrentDirectory()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	std::string Loadedpath = "Assets/Serialization/Board";
	for (auto& entry : std::filesystem::directory_iterator(Loadedpath))
	{
		leveleditVar.currentDir.push_back(entry.path().generic_string());
	}
}


void LevelEditHelper::getFileName()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();

	LevelEditHelper::getCurrentDirectory();

	std::stringstream ss;
	ss << leveleditVar.currentDir.size() + 1;
	std::string s;
	ss >> s;
	leveleditVar.filename = ("Assets/Serialization/Board/board" + s + ".txt");
}

void LevelEditHelper::LoadData()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	for (const auto x : serializationVar.objData)
	{
		if (x.second.first == ObjectType::FLOOR)
			continue;
		leveleditVar.str_vectorObj.push_back(x.second.second);
	}

	for (const auto x : serializationVar.dirData)
	{
		leveleditVar.str_vectorDir.push_back(x.second.second);
	}

	

	LevelEditHelper::getFileName();

	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	globalVar.GAME_SIZE = glm::vec2{ 1920.f, 1080.f };
	//globalVar.GAME_SIZE = glm::vec2{ (leveleditVar.stack / 2) * globalVar.objectSize, (leveleditVar.stack / 2) * globalVar.objectSize };
	Camera2d& camera = gFakeEngine->GetGraphicManager<Camera2d>();
	camera.cam_window = glm::vec2{ 1920.f, 1080.f };
}

//creating a new board (new text file)
void LevelEditHelper::CreateBoard()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	leveleditVar.data._mapdata.resize(leveleditVar.stack, std::vector<ObjectType>(leveleditVar.slice));
	leveleditVar.data._texdata.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));
	leveleditVar.data._dirdata.resize(leveleditVar.stack, std::vector<ObjectDirection>(leveleditVar.slice));
	leveleditVar.data._leverID.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));
	leveleditVar.data._teleporterID.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));

	leveleditVar.boardData.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));
	leveleditVar.boardData_dir.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));

	for (int i = 0; i < leveleditVar.stack; i++)
	{
		for (int j = 0; j < leveleditVar.slice; j++)
		{
			if (i == 0 || j == 0 || i == leveleditVar.stack - 1 || j == leveleditVar.slice - 1)
				leveleditVar.data._mapdata[i][j] = ObjectType::WALL;
			else
				leveleditVar.data._mapdata[i][j] = ObjectType::SQUARE;
			leveleditVar.data._texdata[i][j] = 0;
			leveleditVar.data._dirdata[i][j] = ObjectDirection::NO_DIR;
		}
	}

	GlobalVar & globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	globalVar.GAME_SIZE = glm::vec2{ 1920.f, 1080.f };
	globalVar.binaryMapHeight = leveleditVar.stack;
	globalVar.binaryMapWidth = leveleditVar.slice;
	globalVar.objectSize = (float)leveleditVar.tilesize;
	Camera2d& camera = gFakeEngine->GetGraphicManager<Camera2d>();
	camera.cam_window = globalVar.GAME_SIZE;
}

//loading an exsisting board
void LevelEditHelper::LoadBoard(std::string filename)
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
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
			numbers >> leveleditVar.stack;
		}
		else if (line.substr(0, 8) == "Slices: ")
		{
			std::istringstream numbers(line.substr(8));
			numbers >> leveleditVar.slice;
		}
		else if (line.substr(0, 10) == "TileSize: ")
		{
			std::istringstream numbers(line.substr(10));
			numbers >> leveleditVar.tilesize;
		}
		else if (line.substr(0, 2) == "# ")
		{
			continue;
		}
		else if (line.substr(0, 8) == "MapStart")
		{
			//ARTLESS_ASSERT((stacks != 0 && slices != 0), "Width and Height is not defined first");
			//defining map size

			leveleditVar.data._mapdata.resize(leveleditVar.stack, std::vector<ObjectType>(leveleditVar.slice));
			leveleditVar.data._texdata.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));
			leveleditVar.data._dirdata.resize(leveleditVar.stack, std::vector<ObjectDirection>(leveleditVar.slice));
			leveleditVar.data._leverID.resize(leveleditVar.stack, std::vector<int>(leveleditVar.slice));

			LevelEditHelper::CreateBoard();
			LevelEditHelper::GenerateBoard();

			getline(ifs, line);

			int _slices = 0;
			int _stacks = 0;

			while (line.substr(0, 3) != "End")
			{
				if (_slices == leveleditVar.slice)
				{
					getline(ifs, line);
					_slices = 0;
					_stacks++;
				}
				if (_stacks == leveleditVar.stack)
					continue;
				std::istringstream numbers(line);
				int objNumber = 0, texNumber = 0, dirNumber=0 , leverId=0, teleporterID;
				while (numbers >> objNumber)
				{
					auto found = serializationVar.objData.find(objNumber);
					if (found != serializationVar.objData.end())
					{
						if (found->second.first == ObjectType::LEVER_SWITCH)
						{
							leveleditVar.leverSwitchCount++;
						}
						else if (found->second.first == ObjectType::LEVER_DOOR)
						{
							leveleditVar.leverDoorCount++;
						}
						else if (found->second.first == ObjectType::TELEPORTER)
						{
							leveleditVar.teleporterCount++;
						}
						//object ID
						leveleditVar.data._mapdata[_stacks][_slices] = found->second.first;
					}

					if (numbers.peek() == '/')
					{
						//texture ID
						numbers.ignore(1, '/');
						numbers >> texNumber;
						leveleditVar.data._texdata[_stacks][_slices] = texNumber;
						if (numbers.peek() == '/')
						{
							//direction ID
							numbers.ignore(1, '/');
							numbers >> dirNumber;
							auto found_dir = serializationVar.dirData.find(dirNumber);
							if (found_dir != serializationVar.dirData.end())
							{
								leveleditVar.data._dirdata[_stacks][_slices] = found_dir->second.first;
							}
							if (numbers.peek() == '/' && (leveleditVar.data._mapdata[_stacks][_slices] == ObjectType::LEVER_DOOR
								|| leveleditVar.data._mapdata[_stacks][_slices] == ObjectType::LEVER_SWITCH))
							{
								//direction ID
								numbers.ignore(1, '/');
								numbers >> leverId;
								leveleditVar.data._leverID[_stacks][_slices] = leverId;

								//if (numbers.peek() == '/')
								//{
								//	//direction ID
								//	numbers.ignore(1, '/');

								//}
							}
							else
								leveleditVar.data._leverID[_stacks][_slices] = -1;


							if (numbers.peek() == '/' && (leveleditVar.data._mapdata[_stacks][_slices] == ObjectType::TELEPORTER))
							{
								numbers.ignore(1, '/');
								numbers >> teleporterID;
								leveleditVar.data._teleporterID[_stacks][_slices] = teleporterID;
							}
							else
								leveleditVar.data._teleporterID[_stacks][_slices] = -1;
						}
					}
					_slices++;

				}
			}
		}
	}
}

//saving the edited board
void LevelEditHelper::SaveBoard(SaveBoard_MODE MODE, Data _data)
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	std::ofstream myfile(leveleditVar.filename);
	if (myfile.is_open())
	{

		myfile << "Stacks: " << leveleditVar.stack << "\n";
		myfile << "Slices: " << leveleditVar.slice << "\n";
		myfile << "TileSize: " << leveleditVar.tilesize << "\n";

		myfile << " \n";
		myfile << "MapStart\n";
		for (int i = 0; i < leveleditVar.stack; i++)
		{
			for (int j = 0; j < leveleditVar.slice; j++)
			{
				if (i == 0 || j == 0 || i == leveleditVar.stack - 1 || j == leveleditVar.slice - 1)
				{
					myfile << 2 << "/" << 0 << "/" << 0 << " ";
				}
				else
				{
					if (MODE == SaveBoard_MODE::SAVE_ONLY_FLOORS)
						myfile << 0 << "/" << 0 << " ";
					else if (MODE == SaveBoard_MODE::SAVE_MAP_DATA)
					{
						myfile << leveleditVar.boardData[i][j] << "/" << leveleditVar.data._texdata[i][j] << "/" << leveleditVar.boardData_dir[i][j];
						ObjectType check = leveleditVar.data._mapdata[i][j];
							//leveleditVar.data._mapdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x]
						if (check == ObjectType::LEVER_DOOR || check == ObjectType::LEVER_SWITCH)
							myfile << "/" << leveleditVar.data._leverID[i][j];
						if (check == ObjectType::TELEPORTER)
							myfile << "/" << leveleditVar.data._teleporterID[i][j];
						myfile << " " ;
					}
				}
			}
			myfile << "\n";
		}
		myfile << "End\n";
		myfile.close();
	}
}

//Generating the board
void LevelEditHelper::GenerateBoard()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();

	leveleditVar.checkMapData.resize(leveleditVar.stack, std::vector<bool>(leveleditVar.slice));
	leveleditVar.entityData.resize(leveleditVar.stack, std::vector<Entity>(leveleditVar.slice));
	for (int i = 0; i < leveleditVar.stack; ++i)
	{
		for (int j = 0; j < leveleditVar.slice; ++j)
		{

			std::cout << (int)leveleditVar.data._mapdata[i][j] << std::endl;

			Position obj_pos;
			Entity entityID;
			calculateGamePos(obj_pos.pos, i, j);
			createEntity(leveleditVar.data._mapdata[i][j], leveleditVar.data._texdata[i][j], leveleditVar.data._dirdata[i][j],obj_pos, leveleditVar.selectedLeverID, leveleditVar.selectedTeleporterID, entityID);
			if (leveleditVar.data._mapdata[i][j] == ObjectType::SQUARE || leveleditVar.data._mapdata[i][j] == ObjectType::FLOOR)
			{
				leveleditVar.data._mapdata[i][j] = ObjectType::FLOOR;
			}
			else
			{
				if (leveleditVar.data._mapdata[i][j] == ObjectType::MAIN_CHARACTER)
				{
					leveleditVar.isMainCharacterAdded = true;
				}
				leveleditVar.checkMapData[i][j] = true;
				leveleditVar.entityData[i][j] = entityID;
			}

		}
	}
}

//Clear the edited board
void LevelEditHelper::ClearBoard()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();

	for (int i = 0; i < leveleditVar.stack; i++)
	{
		for (int j = 0; j < leveleditVar.slice; j++)
		{
			if (!(i == 0 || j == 0 || i == leveleditVar.stack - 1 || j == leveleditVar.slice - 1))
			{
				if (leveleditVar.checkMapData[i][j])
				{
					mGameECSmanager.DestroyEntity(leveleditVar.entityData[i][j]);
					leveleditVar.data._mapdata[i][j] = ObjectType::FLOOR;
					leveleditVar.data._dirdata[i][j] = ObjectDirection::NO_DIR;
					leveleditVar.data._texdata[i][j] = 0;
					leveleditVar.data._leverID[i][j] = 0;
					leveleditVar.data._teleporterID[i][j] = 0;
				}
			}
			leveleditVar.checkMapData[i][j] = false;
		}
	}
	leveleditVar.leverDoorCount = 1;
	leveleditVar.leverSwitchCount = 1;
	leveleditVar.teleporterCount = 1;
	leveleditVar.str_vectorTeleporterID.clear();
	leveleditVar.str_vectorLeverID.clear();

	leveleditVar.isMainCharacterAdded = false;
}

/*to be placed in update*/
//get position of mouse
void LevelEditHelper::GetPos()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

	mathEngine::Vector3D tmp_pos = input.GetMousePosWorld();

	globalVar.ConvertScreenToBinary(tmp_pos);
	leveleditVar.mousepos_x = (int)tmp_pos.x;
	leveleditVar.mousepos_y = (int)tmp_pos.y;
}

//check current input
void LevelEditHelper::CheckInput()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	input.GetInputBooleen().line_key = true;

	if (input.CheckTriggered(GLFW_KEY_Z))
		camera2d.zoomIN_input = true;
	else
		camera2d.zoomIN_input = false;

	if (input.CheckTriggered(GLFW_KEY_X))
		camera2d.zoomOUT_input = true;
	else
		camera2d.zoomOUT_input = false;

	if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
		leveleditVar.button_click = true;
	else
		leveleditVar.button_click = false;
	if (input.CheckTriggered(GLFW_KEY_C))
		_gameStateNext = GS_STATES::GS_STAGE1;
}

void LevelEditHelper::ResetValues()
{
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	leveleditVar.stack = 0;
	leveleditVar.slice = 0;
	leveleditVar.tilesize = 0;

	leveleditVar.filename.clear();

	//Data data;
	leveleditVar.data._mapdata.clear();
	leveleditVar.data._texdata.clear();

	leveleditVar.checkMapData.clear();
	leveleditVar.entityData.clear();
	leveleditVar.boardData.clear();

	//// input checks 
	leveleditVar.generationTrue = false;
	////imgui generation button to allow user to generate once
	leveleditVar.generationCheck = true;
	////check whether user is adding
	leveleditVar.addObject_Check = false;

	////check whether user is deleting
	leveleditVar.deleteObject_Check = false;
	////check save button click
	leveleditVar.button_click = false;
	////check whether main character is added
	leveleditVar.isMainCharacterAdded = false;
	////check whether user is loading
	leveleditVar.load_board = false;
	////check whether user is creating new
	leveleditVar.new_board = false;

	////store object
	leveleditVar.selectedObject = 0;
	leveleditVar.prev_selectedObject = 9999;

	////store texture
	leveleditVar.str_vectorTex.clear();
	leveleditVar.selectedTexture  = 0;

	leveleditVar.currentDir.clear();
	leveleditVar.selectedFileName = 0;

	leveleditVar.data._dirdata.clear();





	leveleditVar.leverDoorCount = 1;
	leveleditVar.leverSwitchCount = 1;

	leveleditVar.teleporterCount = 1;
	leveleditVar.selectedTeleporterID = 0;
	leveleditVar.selectedLeverID = 0;

	leveleditVar.str_vectorTeleporterID.clear();
	leveleditVar.str_vectorLeverID.clear();


	////output message in imgui
	leveleditVar.message.clear();


	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	mGameECSmanager.FreeEntityList();
}