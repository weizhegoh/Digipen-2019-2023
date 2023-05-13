/*****************************************************************
*\file		 Input.h
*\brief		 This file contains definition of Inputs function 
			 press and triggered

*\author(s)   Lee Liang Ping	74 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>

struct InputBoolean
{
	//Gameplay
	bool space_key = false;
	bool win_state = false;
	bool win_audio = false;
	bool activate_hack = false;
	bool god_mode = false;
	bool speed_hack = false;
	bool boulder_collied_hack = false;
	//Debug
	bool activate_imgui = false;
	bool line_key = false;
	bool hotspot_key = false;
	bool print_map = false;
	bool rotate_button = false;
	bool scale_button = false;
	bool activateMouseDraw = false;
	//Main Menu
	bool enterMain = true;
	bool enterLevelSelect = false;
	bool enterOption = false;
	bool enterControls = false;
	bool enterCredits = false;
	bool enterQuit = false;
	//Pause
	bool pause_screen = false;
	bool active_pause_screen = false;
	bool change_state_pause = true;
	bool win_screen = false;
	bool next_page = false;
	Entity hoverEntity = 0;
	//Defeat
	bool defeat_state = false;
	//Logo
	LogoState logostate = LogoState::DIGIPENLOGO;
	bool logoskip = false;
	PauseState pause_state = PauseState::STATE_DEFAULT;
	//Sound
	bool hoverPlayed = false;
	std::pair <ObjectType, int> mouseHover{ObjectType::DEFAULT, 0};


	bool resetPositionALL = false;

};

struct Input
{
private:

	std::list<int>	keyTriggered;
	std::list<int>	keyPressed;

	//std::list<BooleanType>	booleanTriggered;
	std::list<BooleanType>	booleanPressed;
	
	InputBoolean inputBool;

	mathEngine::Vector3D mousepos;
	mathEngine::Vector3D mouseposWorld;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Input Helper Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template<typename T>
	void AddKey(const T& key, std::list<T>& vector);
	template<typename T>
	void RemoveKey(const T& key, std::list<T>& vector);
	template<typename T>
	bool CheckKey(const T& key, std::list<T>& vector);
public:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Input Setup Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AddKeyTriggered(const int& key);
	void RemoveKeyTriggered(const int& key);
	void ResetKeyTriggered();
	void AddKeyPressed(const int& key);
	void RemoveKeyPressed(const int& key);
	void ResetKeyPressed();
	bool CheckTriggered(const int& key);
	bool CheckPress(const int& key);

	bool CheckTriggered();
	bool CheckPress();

	void ActivateBoolean(const BooleanType& key);
	void DeactivateBoolean(const BooleanType& key);
	bool CheckBooleanPress(const BooleanType& key);

	void Reset();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Input Holder Functions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	mathEngine::Vector3D  GetMousePos() const;
	mathEngine::Vector3D  GetMousePosWorld() const;
	void SetMousePos(const double& x, const double& y);
	InputBoolean& GetInputBooleen();
	bool ToggleBoolean(bool& boolean);
};