/*****************************************************************
*\file		 matrix3x3.h
*\brief		 Contains declaration for matrix 3x3 math library

*\author(s)   Goh Wei Zhe	48 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "vector3D.h"

namespace mathEngine
{
	class Matrix3
	{
	private:

	public: 
		float m[9] = { 0.0 };

		//Constructors
		Matrix3();		
		Matrix3(const float* pArr);
		Matrix3(float m0, float m3, float m6,
				float m1, float m4, float m7,
				float m2, float m5, float m8);

		Matrix3(const Matrix3& rhs);
		Matrix3& operator= (const Matrix3& rhs);

		//Addition
		Matrix3 operator+(const Matrix3& rhs) const;	
		Matrix3 operator+=(const Matrix3& rhs);

		//Subtraction
		Matrix3 operator-(const Matrix3& rhs) const;
		Matrix3 operator-=(const Matrix3& rhs);

		//Multiplication
		Matrix3 operator*(const Matrix3& rhs);
		Matrix3 operator*=(const Matrix3& rhs);

		//Scalar Multiplication
		Matrix3 operator*(const float s) const;
		Matrix3 operator*=(const float s);	

		//Identity Matrix
		void setIdentity();		
		void Identity(Matrix3& rhs);

		//Translation
		void Translate(float x, float y);
		void Translate(Matrix3& rhs, float x, float y);
		
		//Scale
		void Scale(float x, float y);
		void Scale(Matrix3& pResult, float x, float y);


		//Rotation in radian
		void RotRad(float angle);
		void RotRad(Matrix3& rhs, float angle);

		//Rotation in degreee
		void RotDeg(float angle);
		void RotDeg(Matrix3& pResult, float angle);

		//Transpose Matrix 
		void setTranspose(const Matrix3& rhs);
		Matrix3 getTranspose() const;

		void Transpose(Matrix3& pResult, const Matrix3& pMtx);
 
		//Inverse Matrix 
		void setInverse(const Matrix3& rhs);
		Matrix3 getInverse() const;
		
		void Inverse(Matrix3* pResult, float* determinant, const Matrix3& pMtx);
		
		//Vector Transformation
		Vector3D operator*(const Vector3D& v) const;
		Vector3D transformVector(const Vector3D& v) const;

		//print test case
		void print();

		//destructors 
		~Matrix3();
	};
}

#endif // !MATRIX3X3_H