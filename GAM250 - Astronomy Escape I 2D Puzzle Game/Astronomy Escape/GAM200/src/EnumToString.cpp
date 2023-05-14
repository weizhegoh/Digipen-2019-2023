/*****************************************************************
*\file		 EnumToString.cpp
*\brief		 Overload function for enum to string

*\author(s)   Lee Liang Ping	102 lines x 50% Code contribution
*\author(s)   Lee Jun Jie		102 lines x 50% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <EnumToString.h> 

std::string EnumToString::Get(const ObjectState objState)
{
	switch (objState)
	{
	case ObjectState::IDLE:
		return "Idle";
	case ObjectState::MOVING:
		return "Moving";
	case ObjectState::PUSHING_BARREL:
		return "Pushing Barrel";
	case ObjectState::CHARGING:
		return "CHARGING";
	case ObjectState::DEATH:
		return "Death";
	case ObjectState::MONSTER_MOVING:
		return "Monster Moving";
	case ObjectState::MONSTER_IDLE:
		return "Monster idle";
	case ObjectState::MONSTER_GUIDE_MOVING:
		return "Monster Guide Moving";
	case ObjectState::MONSTER_GUIDE_IDLE:
		return "Monster Guide Idle";
	case ObjectState::MONSTER_PATROL_MOVING:
		return "Monster Patrol Moving";
	case ObjectState::MONSTER_PATROL_IDLE:
		return "Monster Patrol Moving";
	case ObjectState::DOOR_CLOSE:
		return "DOOR_CLOSE";
	case ObjectState::DOOR_OPEN:
		return "DOOR_OPEN";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const ObjectDirection objDir)
{
	switch (objDir)
	{
	case ObjectDirection::NO_DIR:
		return "NO_DIR";
	case ObjectDirection::LEFT:
		return "LEFT";
	case ObjectDirection::RIGHT:
		return "RIGHT";
	case ObjectDirection::UP:
		return "UP";
	case ObjectDirection::DOWN:
		return "DOWN";
	case ObjectDirection::UPLEFT:
		return "UPLEFT";
	case ObjectDirection::UPRIGHT:
		return "UPRIGHT";
	case ObjectDirection::DOWNLEFT:
		return "DOWNLEFT";
	case ObjectDirection::DOWNRIGHT:
		return "DOWNRIGHT";
	default:
		return "Other";
	}
}

 std::string EnumToString::Get(const ZoomOption zoomOption)
{
	switch (zoomOption)
	{
	case ZoomOption::NORMAL:
		return "NORMAL";
	case ZoomOption::MAP:
		return "MAP";
	case ZoomOption::MAP_FULLVIEW:
		return "MAP_FULLVIEW";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const CollisionType shaderOption)
{
	switch (shaderOption)
	{
	case CollisionType::CHARGING_LASER_PROJECTILE:
		return "CHARGING_LASER_PROJECTILE";
	case CollisionType::OBJECT_TO_LASER_PROJECTILE:
		return "OBJECT_TO_LASER_PROJECTILE";
	case CollisionType::ALL_COLLISIONTYPE:
		return "ALL_COLLISIONTYPE";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const TimerMap shaderOption)
{
	switch (shaderOption)
	{
	case TimerMap::PARTICLE_PULSING_INTERVAL:
		return "PARTICLE_PULSING_INTERVAL";
	case TimerMap::PARTICLE_WALKING_INTERVAL:
		return "PARTICLE_WALKING_INTERVAL";
	case TimerMap::IDLE_TIME:
		return "IDLE_TIME";
	case TimerMap::PUSHING_BARREL_INPUT:
		return "PUSHING_BARREL_INPUT";
	case TimerMap::LASER_IDLE:
		return "LASER_IDLE";
	case TimerMap::LASER_CHARGE:
		return "LASER_CHARGE";
	case TimerMap::LASER_FIRE:
		return "LASER_FIRE";
	case TimerMap::LASER_IMAGE:
		return "LASER_IMAGE";
	case TimerMap::MOVEMENT_LOCK:
		return "MOVEMENT_LOCK";
	case TimerMap::LASER_FIRE_TO_IDLE:
		return "LASER_FIRE_TO_IDLE";
	case TimerMap::LASER_PROJECTILE_INCREASE:
		return "LASER_PROJECTILE_INCREASE";
	case TimerMap::TIMER_NIL:
		return "TIMER_NIL";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const EntitySystemOption shaderOption)
{
	switch (shaderOption)
	{
	case EntitySystemOption::GAME_ENTITY:
		return "GAME_ENTITY";
	case EntitySystemOption::PARTICLE_ENTITY:
		return "PARTICLE_ENTITY";
	default:
		return "Other";
	}
}


std::string EnumToString::Get(const FreeRenderType shaderOption)
{
	switch (shaderOption)
	{
	case FreeRenderType::MODEL_WORLD:
		return "MODEL_WORLD";
	case FreeRenderType::SCREEN:
		return "SCREEN";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const StagesEnum state)
{
	switch (state)
	{
	case StagesEnum::ACTUAL_STAGE:
		return "ACTUAL_STAGE";
	case StagesEnum::LEVEL2:
		return "LEVEL2";
	case StagesEnum::LEVEL3:
		return "LEVEL3";
	case StagesEnum::LEVEL4:
		return "LEVEL4";
	case StagesEnum::LEVEL5:
		return "LEVEL5";
	case StagesEnum::TUTORIAL_1:
		return "TUTORIAL_1";	
	case StagesEnum::TUTORIAL_2:
			return "TUTORIAL_2";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const ParticleAction action)
{
	switch (action)
	{
	case ParticleAction::START:
		return "START";
	case ParticleAction::END:
		return "END";
	case ParticleAction::PULSE:
		return "PULSE";
	case ParticleAction::TOWARDS_LEVEL_DOOR:
		return "TOWARDS_LEVEL_DOOR";
	default:
		return "Other";
	}
}

std::string EnumToString::Get(const ParticleTypesEnum type)
{
	switch (type)
	{
	case ParticleTypesEnum::PARTICLE_PULSE_INWARDS:
		return "PARTICLE_PULSE_INWARDS";
	case ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS:
		return "PARTICLE_PULSE_OUTWARDS";
	case ParticleTypesEnum::PARTICLE_PICKUP:
		return "PARTICLE_PICKUP";
	case ParticleTypesEnum::PARTICLE_SMOKE:
		return "PARTICLE_SMOKE";
	case ParticleTypesEnum::PARTICLE_LASER_CHARGING:
		return "PARTICLE_LASER_CHARGING";
	case ParticleTypesEnum::PARTICLE_NIL:
		return "PARTICLE_NIL";
	default:
		return "Other";
	}
}