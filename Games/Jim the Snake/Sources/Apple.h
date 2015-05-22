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
#include "SnakyGame.h"
#include "Screen.h"
#include "Eatable.h"

enum class AppleState {
	FRESH,
	ROT,
	DEAD,
	EMPTY
};

class Apple : public Eatable{
public:
	static void Init(Game* game);
	Apple();
	void Draw();
	void SetPosition(PointX pos);
	PointX GetPosition();
	float GetRadius();
	void Update(float sec);
	float GetLifeTime();
	AppleState GetState();
	void SetLifeTime(float lifeTime);
private:
	static Graphics* graphics;
	static Game* game;
	static Image* fresh_img;
	static Image* rot_img;
	static Image* dead_img;

	static const float radius;

	PointX pos;
	float angle;
	float life_time;
	AppleState state;
	Image* current_img;
};