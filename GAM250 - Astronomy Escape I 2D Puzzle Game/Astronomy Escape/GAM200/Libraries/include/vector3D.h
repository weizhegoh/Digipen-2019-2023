/*****************************************************************
*\file		 vector3D.h
*\brief		 Contains declaration for vector 3D math library

*\author(s)   Goh Wei Zhe	38 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#ifndef VECTOR3D_H
#define VECTOR3D_H

namespace mathEngine
{
	class Vector3D
	{
	private:

	public:
		float x, y, z;
		float& a = x;
		float& b = y;
		float& c = z;

		//Constructors
		Vector3D();										
		Vector3D(float uX, float uY, float uZ);				
		Vector3D(const Vector3D& rhs);					

		Vector3D& operator=(const Vector3D& rhs);	

		//Addition
		Vector3D operator+(const Vector3D& rhs) const;		
		Vector3D operator+(const float rhs) const;		
		Vector3D& operator+=(const Vector3D& rhs);			 

		//Subtraction
		Vector3D operator-(const Vector3D& rhs) const;		 
		Vector3D operator-(const float rhs) const;		 
		Vector3D& operator-=(const Vector3D& rhs);			 

		//Scalar multiplication
		Vector3D operator*(const float rhs) const;			 
		Vector3D& operator*=(const float rhs);				 
		
		//Scalar division
		Vector3D operator/(const float rhs) const;			 
		Vector3D& operator/=(const float rhs);				 

		//Unary operator
		Vector3D operator -() const;

		//Dot product
		float operator*(const Vector3D& rhs) const;			
		float dot(const Vector3D& rhs)const;
		float dot(const Vector3D& lhs, const Vector3D& rhs);

		//Cross product
		Vector3D cross(const Vector3D& rhs) const;
		Vector3D& operator %=(const Vector3D& rhs);
		Vector3D operator %(const Vector3D& rhs) const;

		//Magnitude or length
		float magnitude();

		//distance
		float distance(const Vector3D& lhs, const Vector3D& rhs);

		//Unit vector
		void normalize();

		//Print test case
		void print();

		~Vector3D();
	};
}

#endif // !VECTOR3D_H