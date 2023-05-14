/*****************************************************************
*\file		 vector3D.cpp
*\brief		 Contains definition for vector 3D math library

*\author(s)   Goh Wei Zhe	136 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "vector3D.h"

namespace mathEngine
{
	//Constructors
	Vector3D::Vector3D() :x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
	Vector3D::Vector3D(float X, float Y, float Z) : x{ X }, y{ Y }, z{ Z }{}

	Vector3D::Vector3D(const Vector3D& rhs) : x{ rhs.x }, y{ rhs.y }, z{ rhs.z }{}

	Vector3D& Vector3D::operator=(const Vector3D& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;

		return *this;
	};

	//Addition
	Vector3D Vector3D::operator+(const Vector3D& rhs) const
	{
		return Vector3D(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
	}

	Vector3D Vector3D::operator+(const float rhs) const
	{
		return Vector3D(this->x + rhs, this->y + rhs, this->z + rhs);
	}

	Vector3D& Vector3D::operator+=(const Vector3D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;

		return *this;
	}

	//Subtraction
	Vector3D Vector3D::operator-(const Vector3D& rhs) const
	{
		return Vector3D(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}
	
	Vector3D Vector3D::operator-(const float rhs) const
	{
		return Vector3D(this->x - rhs, this->y - rhs, this->z - rhs);
	}

	Vector3D& Vector3D::operator-=(const Vector3D& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;

		return *this;
	}

	//Scalar multiplication
	Vector3D Vector3D::operator*(const float rhs) const
	{
		return Vector3D(this->x * rhs, this->y * rhs, this->z * rhs);
	}

	Vector3D& Vector3D::operator*=(const float rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;

		return *this;
	}

	//Scalar division
	Vector3D Vector3D::operator/(const float rhs) const
	{
		return Vector3D(this->x / rhs, this->y / rhs, this->z / rhs);
	}

	Vector3D& Vector3D::operator/=(const float rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;

		return *this;
	}

	//Unary operator
	Vector3D Vector3D::operator -() const
	{
		Vector3D v2;

		v2.x = -(this->x);
		v2.y = -(this->y);

		return v2;
	}

	//Dot product
	float Vector3D::operator*(const Vector3D& rhs) const
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}

	float Vector3D::dot(const Vector3D& rhs)const
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}

	float Vector3D::dot(const Vector3D& lhs, const Vector3D& rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z *lhs.z);
	}

	//Cross product
	Vector3D Vector3D::cross(const Vector3D& rhs) const
	{
		return Vector3D(this->y * rhs.z - this->z * rhs.y,
						this->z * rhs.x - this->x * rhs.z,
						this->x * rhs.y - this->y * rhs.x);
	}

	Vector3D& Vector3D::operator %=(const Vector3D& rhs)
	{
		float _x, _y, _z;

		_x = this->y * rhs.z - this->z * rhs.y;
		_y = this->z * rhs.x - this->x * rhs.z;
		_z = this->x * rhs.y - this->y * rhs.x;

		this->x = _x;
		this->y = _y;
		this->z = _z;

		return *this;
	}

	Vector3D Vector3D::operator %(const Vector3D& rhs) const
	{
		return Vector3D(this->y * rhs.z - this->z * rhs.y,
						this->z * rhs.x - this->x * rhs.z,
						this->x * rhs.y - this->y * rhs.x);
	}

	//Magnitude or length
	float Vector3D::magnitude()
	{
		float magnitude = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		return magnitude;
	}

	//distance
	float Vector3D::distance(const Vector3D& lhs, const Vector3D& rhs)
	{
		float distance = (float)sqrt((rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y) + (rhs.z - lhs.z) * (rhs.z - lhs.z));
		return distance;
	}

	//Unit vector
	void Vector3D::normalize()
	{
		float magnitude = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		
		if (magnitude > 0.0f)
		{
			float oneOverMag = 1.0f / magnitude;
			
			this->x *= oneOverMag;
			this->y *= oneOverMag;
			this->z *= oneOverMag;
		}
	}

	//Print test case
	void Vector3D::print()
	{
		std::cout << "[" << x << "," << y << "," << z << "]" << std::endl;
	
		std::cout << std::endl;
	}
	Vector3D:: ~Vector3D() {}
}