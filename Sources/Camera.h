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

namespace cross{

class Camera{
public:
	Camera();

	void SetPosition(const Vector2D& pos);
	void SetPosition(const Vector3D& pos);
	void SetDirection(const Vector3D& dir);
	Vector3D GetPosition() const;
	Vector3D GetDirection() const;
//Framework module. You don't need call any of this methods or modify variable
public:
	const Matrix& GetViewMatrix();
	const Matrix& GetProjectionMatrix() const;
protected:
	Matrix view;
	Matrix projection;
	Vector3D position;
	Vector3D direction;
	//Vector3D up;
	bool recalc_view;

	void RecalView();
};

};