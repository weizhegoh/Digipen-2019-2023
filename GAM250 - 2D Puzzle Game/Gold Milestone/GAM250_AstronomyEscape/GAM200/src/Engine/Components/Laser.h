/*****************************************************************
*\file		 Laser.h
*\brief		 Laser Component 

*\author(s)  Lee Liang Ping		9 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Laser
{
	LaserState laserState{ LaserState::LASER_IDLE };
	bool charging{false};
	float lifetime;
	bool lifetimeCounter;
	Entity charging_id;
};