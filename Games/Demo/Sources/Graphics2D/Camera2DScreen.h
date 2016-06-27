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
#include "Screen.h"
#include "Camera.h"

using namespace cross;

class Camera2DScreen : public Screen{
public:
	void Start();
	void Stop();
	void Update(float sec);
private:
	Sprite* awesomefase;
	CRArray<Sprite*> sprites;
	CRArray<Vector3D> velocities;
	float cam_speed;
	float view_width;
	float tip_timeout;
	Vector2D cam_positon;
	Camera2D* camera;
	Font* tip_font;

	void WheelUpHandler();
	void WheelDownHandler();
};