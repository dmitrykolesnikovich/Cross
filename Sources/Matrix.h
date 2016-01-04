/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#pragma once
#include "Cross.h"
#include "Vector4D.h"

#undef near
#undef far

namespace cross{

class Matrix{
public:
	static Matrix CreateZero();
	static Matrix CreateIdentity();
	static Matrix CreateTranslation(Vector3D &vec);
	static Matrix CreateOrthogonalProjection(float left, float right, float bottom, float top, float near, float far);

	float m[4][4];

	Matrix() { };

	float* GetData();
	void SetTranslation(Vector2D trans);
	void SetTranslation(Vector3D trans);
	void SetRotationZ(float angle);

	Matrix operator + (float s) const;
	void operator += (float s);
	Matrix operator - (float s) const;
	void operator -= (float s);
	Matrix operator * (float s) const;
	void operator *= (float s);
	Matrix operator / (float s) const;
	void operator /= (float s);
	Vector4D operator * (const Vector4D& vec) const;
	Matrix operator * (const Matrix &m2) const;
	void operator *= (const Matrix &m2);
};

};