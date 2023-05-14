/*****************************************************************
*\file		 matrix3x3.cpp
*\brief		 Contains definition for matrix 3x3 math library

*\author(s)   Goh Wei Zhe	329 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "matrix3x3.h"

#define PI	 3.14159265358f
#define EPSILON		0.0001f

namespace mathEngine
{
	//Constructors
	Matrix3::Matrix3()
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = { 0.0f };
		}

		this->m[0] = this->m[4] = this->m[8] = 1.0f;
	};

	Matrix3::Matrix3(const float* mArr)
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = mArr[i];
		}
	}

	Matrix3::Matrix3(float m0, float m3, float m6,
					 float m1, float m4, float m7,
					 float m2, float m5, float m8)
	{
		m[0] = m0;
		m[3] = m3;
		m[6] = m6;

		m[1] = m1;
		m[4] = m4;
		m[7] = m7;

		m[2] = m2;
		m[5] = m5;
		m[8] = m8;	
	}

	Matrix3::Matrix3(const Matrix3& rhs)
	{		 
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = rhs.m[i];
		}
	}

	Matrix3& Matrix3::operator=(const Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = rhs.m[i];
		}

		return *this;
	}

	//Addition
	Matrix3 Matrix3::operator+(const Matrix3& rhs) const
	{
		Matrix3 n;

		n.m[0] = this->m[0] + rhs.m[0];
		n.m[3] = this->m[3] + rhs.m[3];
		n.m[6] = this->m[6] + rhs.m[6];

		n.m[1] = this->m[1] + rhs.m[1];
		n.m[4] = this->m[4] + rhs.m[4];
		n.m[7] = this->m[7] + rhs.m[7];

		n.m[2] = this->m[2] + rhs.m[2];
		n.m[5] = this->m[5] + rhs.m[5];
		n.m[8] = this->m[8] + rhs.m[8];

		return n;
	}

	Matrix3 Matrix3::operator+=(const Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] += rhs.m[i];
		}

		return *this;
	}

	//Subtraction
	Matrix3 Matrix3::operator-(const Matrix3& rhs) const
	{
		Matrix3 n;

		n.m[0] = this->m[0] - rhs.m[0];
		n.m[3] = this->m[3] - rhs.m[3];
		n.m[6] = this->m[6] - rhs.m[6];

		n.m[1] = this->m[1] - rhs.m[1];
		n.m[4] = this->m[4] - rhs.m[4];
		n.m[7] = this->m[7] - rhs.m[7];

		n.m[2] = this->m[2] - rhs.m[2];
		n.m[5] = this->m[5] - rhs.m[5];
		n.m[8] = this->m[8] - rhs.m[8];

		return n;
	}

	Matrix3 Matrix3::operator-=(const Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] -= rhs.m[i];
		}

		return *this;
	}

	//Multiplication
	Matrix3 Matrix3::operator*(const Matrix3& rhs)
	{
		return Matrix3(this->m[0] * rhs.m[0] + this->m[3] * rhs.m[1] + this->m[6] * rhs.m[2],
			this->m[0] * rhs.m[3] + this->m[3] * rhs.m[4] + this->m[6] * rhs.m[5],
			this->m[0] * rhs.m[6] + this->m[3] * rhs.m[7] + this->m[6] * rhs.m[8],

			this->m[1] * rhs.m[0] + this->m[4] * rhs.m[1] + this->m[7] * rhs.m[2],
			this->m[1] * rhs.m[3] + this->m[4] * rhs.m[4] + this->m[7] * rhs.m[5],
			this->m[1] * rhs.m[6] + this->m[4] * rhs.m[7] + this->m[7] * rhs.m[8],

			this->m[2] * rhs.m[0] + this->m[5] * rhs.m[1] + this->m[8] * rhs.m[2],
			this->m[2] * rhs.m[3] + this->m[5] * rhs.m[4] + this->m[8] * rhs.m[5],
			this->m[2] * rhs.m[6] + this->m[5] * rhs.m[7] + this->m[8] * rhs.m[8]);
	}

	Matrix3 Matrix3::operator*=(const Matrix3& rhs)
	{
		float n[9];

		n[0] = (this->m[0] * rhs.m[0]) + (this->m[3] * rhs.m[1]) + (this->m[6] * rhs.m[2]);
		n[3] = (this->m[0] * rhs.m[3]) + (this->m[3] * rhs.m[4]) + (this->m[6] * rhs.m[5]);
		n[6] = (this->m[0] * rhs.m[6]) + (this->m[3] * rhs.m[7]) + (this->m[6] * rhs.m[8]);

		n[1] = (this->m[1] * rhs.m[0]) + (this->m[4] * rhs.m[1]) + (this->m[7] * rhs.m[2]);
		n[4] = (this->m[1] * rhs.m[3]) + (this->m[4] * rhs.m[4]) + (this->m[7] * rhs.m[5]);
		n[7] = (this->m[1] * rhs.m[6]) + (this->m[4] * rhs.m[7]) + (this->m[7] * rhs.m[8]);

		n[2] = (this->m[2] * rhs.m[0]) + (this->m[5] * rhs.m[1]) + (this->m[8] * rhs.m[2]);
		n[5] = (this->m[2] * rhs.m[3]) + (this->m[5] * rhs.m[4]) + (this->m[8] * rhs.m[5]);
		n[8] = (this->m[2] * rhs.m[6]) + (this->m[5] * rhs.m[7]) + (this->m[8] * rhs.m[8]);


		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = n[i];
		}

		return *this;
	}

	//Scalar Muliplication
	Matrix3 Matrix3::operator*(const float s) const
	{
		Matrix3 n;

		for (int i = 0; i < 9; ++i)
		{
			n.m[i] = m[i] * s;
		}

		return n;
	}

	Matrix3 Matrix3::operator*=(const float s)
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] *= s;
		}

		return *this;
	}

	//Identity Matrix
	void Matrix3::setIdentity()
	{
		for (int i = 0; i < 9; ++i)
		{
			this->m[i] = 0.0f;
		}

		this->m[0] = this->m[4] = this->m[8] = 1.0f;
	}

	void Matrix3::Identity(Matrix3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (i % 4 == 0)
			{
				rhs.m[i] = 1.0f;
			}
			else
			{
				rhs.m[i] = 0.0f;
			}
		}
	}

	//Translation
	void Matrix3::Translate(float x, float y)
	{
		this->setIdentity();

		this->m[6] = x;
		this->m[7] = y;
	}

	void Matrix3::Translate(Matrix3& pResult, float x, float y)
	{
		Identity(pResult);

		pResult.m[6] = x;
		pResult.m[7] = y;
	}
	
	//Scale
	void Matrix3::Scale(float x, float y)
	{
		this->setIdentity();

		this->m[0] *= x;
		this->m[4] *= y;
	}

	void Matrix3::Scale(Matrix3& pResult, float x, float y)
	{
		Identity(pResult);

		pResult.m[0] *= x;
		pResult.m[4] *= y;
	}

	//Rotation in radian
	void Matrix3::RotRad(float angle)
	{
		this->setIdentity();

		this->m[0] = cosf(angle);
		this->m[3] = -sinf(angle);
		this->m[1] = sinf(angle);
		this->m[4] = cosf(angle);
	}

	void Matrix3::RotRad(Matrix3& pResult, float angle)
	{
		Identity(pResult);

		pResult.m[0] = cosf(angle);
		pResult.m[3] = -sinf(angle);
		pResult.m[1] = sinf(angle);
		pResult.m[4] = cosf(angle);
	}

	//Rotation in degree
	void Matrix3::RotDeg(float angle)
	{
		float angleInRadian = angle * PI / 180;

		this->setIdentity();

		this->m[0] = cosf(angleInRadian);
		this->m[3] = -sinf(angleInRadian);
		this->m[1] = sinf(angleInRadian);
		this->m[4] = cosf(angleInRadian);
	}

	void Matrix3::RotDeg(Matrix3& pResult, float angle)
	{
		float angleInRadian = angle * PI / 180;

		Identity(pResult);

		pResult.m[0] = cosf(angleInRadian);
		pResult.m[3] = -sinf(angleInRadian);
		pResult.m[1] = sinf(angleInRadian);
		pResult.m[4] = cosf(angleInRadian);
	}

	//Transpose
	void Matrix3::setTranspose(const Matrix3& rhs)
	{
		this->m[0] = rhs.m[0];
		this->m[3] = rhs.m[1];
		this->m[6] = rhs.m[2];

		this->m[1] = rhs.m[3];
		this->m[4] = rhs.m[4];
		this->m[7] = rhs.m[5];

		this->m[2] = rhs.m[6];
		this->m[5] = rhs.m[7];
		this->m[8] = rhs.m[8];
	}

	Matrix3 Matrix3::getTranspose() const
	{
		Matrix3 result;
		result.setTranspose(*this);
		return result;
	}

	void Matrix3::Transpose(Matrix3& pResult, const Matrix3& pMtx)
	{
		pResult.m[0] = pMtx.m[0];
		pResult.m[3] = pMtx.m[1];
		pResult.m[6] = pMtx.m[2];

		pResult.m[1] = pMtx.m[3];
		pResult.m[4] = pMtx.m[4];
		pResult.m[7] = pMtx.m[5];

		pResult.m[2] = pMtx.m[6];
		pResult.m[5] = pMtx.m[7];
		pResult.m[8] = pMtx.m[8];
	}

	//Inverse Matrix
	void Matrix3::setInverse(const Matrix3& rhs)
	{
		float det = ((rhs.m[0] * rhs.m[4] * rhs.m[8])
					+ (rhs.m[3] * rhs.m[7] * rhs.m[2])
					+ (rhs.m[6] * rhs.m[1] * rhs.m[5])
					- (rhs.m[6] * rhs.m[4] * rhs.m[2])
					- (rhs.m[0] * rhs.m[7] * rhs.m[5])
					- (rhs.m[3] * rhs.m[1] * rhs.m[8]));

		//if (det <= EPSILON) return;

		float inverseDet = 1.0f / det;

		m[0] = ((rhs.m[4] * rhs.m[8]) - (rhs.m[5] * rhs.m[7])) * inverseDet;					//cofactor matrix
		m[3] = -((rhs.m[3] * rhs.m[8]) - (rhs.m[5] * rhs.m[6])) * inverseDet;
		m[6] = ((rhs.m[3] * rhs.m[7]) - (rhs.m[4] * rhs.m[6])) * inverseDet;

		m[1] = -((rhs.m[1] * rhs.m[8]) - (rhs.m[2] * rhs.m[7])) * inverseDet;
		m[4] = ((rhs.m[0] * rhs.m[8]) - (rhs.m[2] * rhs.m[6])) * inverseDet;
		m[7] = -((rhs.m[0] * rhs.m[7]) - (rhs.m[1] * rhs.m[6])) * inverseDet;

		m[2] = ((rhs.m[1] * rhs.m[5]) - (rhs.m[2] * rhs.m[4])) * inverseDet;
		m[5] = -((rhs.m[0] * rhs.m[5]) - (rhs.m[2] * rhs.m[3])) * inverseDet;
		m[8] = ((rhs.m[0] * rhs.m[4]) - (rhs.m[1] * rhs.m[3])) * inverseDet;

		for (int i = 0; i < 9; ++i)
		{
			if (m[i] == -0)
				m[i] = 0;
		}
 	}

	Matrix3 Matrix3::getInverse() const
	{
		Matrix3 result;
		result.setInverse(*this);
		return result;
	}

	void Matrix3::Inverse(Matrix3* pResult, float* determinant, const Matrix3& pMtx)
	{
		*determinant = (pMtx.m[0] * ((pMtx.m[4] * pMtx.m[8]) - (pMtx.m[7] * pMtx.m[5]))) +
			(-(pMtx.m[1] * ((pMtx.m[3] * pMtx.m[8]) - (pMtx.m[5] * pMtx.m[6])))) +
			(pMtx.m[2] * ((pMtx.m[3] * pMtx.m[7]) - (pMtx.m[4] * pMtx.m[6])));

		if (determinant)
		{
			Matrix3 adjugateMatrix;
			Matrix3 transposeMatrix;

			adjugateMatrix.m[0] = pMtx.m[4] * pMtx.m[8] - pMtx.m[5] * pMtx.m[7];
			adjugateMatrix.m[1] = pMtx.m[3] * pMtx.m[8] - pMtx.m[5] * pMtx.m[6];
			adjugateMatrix.m[2] = pMtx.m[3] * pMtx.m[7] - pMtx.m[4] * pMtx.m[6];

			adjugateMatrix.m[3] = pMtx.m[1] * pMtx.m[8] - pMtx.m[2] * pMtx.m[7];
			adjugateMatrix.m[4] = pMtx.m[0] * pMtx.m[8] - pMtx.m[2] * pMtx.m[6];
			adjugateMatrix.m[5] = pMtx.m[0] * pMtx.m[7] - pMtx.m[1] * pMtx.m[6];

			adjugateMatrix.m[6] = pMtx.m[1] * pMtx.m[5] - pMtx.m[2] * pMtx.m[4];
			adjugateMatrix.m[7] = pMtx.m[0] * pMtx.m[5] - pMtx.m[2] * pMtx.m[3];
			adjugateMatrix.m[8] = pMtx.m[0] * pMtx.m[4] - pMtx.m[1] * pMtx.m[3];

			Transpose(transposeMatrix, adjugateMatrix);

			for (int i = 0; i < 9; ++i)
			{
				pResult->m[i] = adjugateMatrix.m[i] / *determinant;
			}
		}
		else
		{
			pResult = NULL;
		}
	}

	//Vector Transformation
	Vector3D Matrix3::operator*(const Vector3D& v) const
	{
		return Vector3D(m[0] * v.x + m[3] * v.y + m[6] * v.z,
						m[1] * v.x + m[4] * v.y + m[7] * v.z,
						m[2] * v.x + m[5] * v.y + m[8] * v.z);
	}
	
	Vector3D Matrix3::transformVector(const Vector3D& v) const
	{
		return (*this) * v;
	}

	//Print test case
	void Matrix3::print()
	{
		std::cout << "[" << m[0] << "," << m[3] << "," << m[6] << "," << std::endl;
		std::cout << m[1] << "," << m[4] << "," << m[7] << "," << std::endl;
		std::cout << m[2] << "," << m[5] << "," << m[8] << "]" << std::endl;

		std::cout << std::endl;
	}

	Matrix3::~Matrix3() {};
}