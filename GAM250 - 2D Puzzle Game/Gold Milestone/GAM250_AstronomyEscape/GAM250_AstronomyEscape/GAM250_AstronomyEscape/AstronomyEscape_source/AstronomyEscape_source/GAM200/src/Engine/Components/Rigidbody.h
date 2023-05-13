/*****************************************************************
*\file		 Rigidbody.h
*\brief		 Rigidbody Component 

*\author(s)   Lee Jun Jie	34 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Rigidbody
{
	mathEngine::Vector3D velocity{ 0.0f,0.0f,0.0f };
	mathEngine::Vector3D acceleration{ 0.0f,0.0f,0.0f };
	float invMass{ 1.f };

	void Integrate(mathEngine::Vector3D force, float dt)
	{
		// Determine the acceleration
		acceleration = force * invMass;
		
		// Integrate the velocity
		velocity.x += acceleration.x * dt;
		velocity.y += acceleration.y * dt;

		// Apply friction
		velocity *= VELOCITY_CAP;

		// Update position
		//position.x += velocity.x * dt;
		//position.y += velocity.y * dt;

		// Reset the acceleration to (0, 0)
		acceleration = { 0.f, 0.f, 0.f };
	}

	void Epsilon()
	{
		if (velocity.x < 0.02f && velocity.x > -0.02f)
			velocity.x = 0.0f;
		if (velocity.y < 0.02f && velocity.y > -0.02f)
			velocity.y = 0.0f;
	}

	void Print()
	{
		std::cout << "------------velocity-------------" << std::endl;
		std::cout << "x: "<< velocity.x << std::endl;
		std::cout << "y: "<< velocity.y << std::endl;
		std::cout << "z: "<< velocity.z << std::endl;
		std::cout << "------------acceleration---------" << std::endl;
		std::cout << "x: " << acceleration.x << std::endl;
		std::cout << "y: " << acceleration.y << std::endl;
		std::cout << "z: " << acceleration.z << std::endl;
		std::cout << "-----------------End-------------" << std::endl;
	}


};