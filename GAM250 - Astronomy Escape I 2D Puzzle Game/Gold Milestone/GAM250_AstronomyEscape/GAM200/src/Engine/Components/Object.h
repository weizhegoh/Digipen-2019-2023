/*****************************************************************
*\file		 Object.h
*\brief		 Header file declarations for Object.

*\author(s)   Kenric Tan Wei Liang	33 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <ComponentsHeader.h>

struct Object
{
	std::pair<bool, std::string> _name;
	std::pair<bool, EntityType> _object;
	std::pair<std::pair<bool,bool>, Render> _render;
	std::pair<bool, std::vector<RenderTex>> _texture;
	std::pair<bool, RenderPos> _renderPos;
	std::pair<bool, Animation> _animation;
	std::pair<bool, Transform> _transform;
	std::pair<bool, Position> _position;
	std::pair<bool, Rigidbody> _rigidbody;
	std::pair<bool, Colour> _colour;
	std::pair<bool, AABB> _aabb;
	std::pair<bool, State> _state;
	std::pair<bool, Flags> _flag;
	std::pair<bool, CollisionFlag> _cflag;
	std::pair<bool, Inventory> _inventory;
	std::pair<bool, AI> _ai;
	std::pair<bool, UI> _ui;
	std::pair<bool, Light> _light;
	std::pair<bool, Laser> _laser;
	std::pair<bool, Force> _force;
	std::pair<bool, LoadingType> _loadingType;
	std::pair<bool, LightFlag> _lightFlag;
	std::pair<bool, Lever> _lever;
	std::pair<bool, Teleporter> _teleporter;
	std::pair<bool, KeyObjective> _keyObjective;
	std::pair<bool, LifeTime> _lifetime;
	std::pair<bool, Mouse> _mouse;
};