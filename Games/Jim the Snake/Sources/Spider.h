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
#include "Eatable.h"
#include "Game.h"
#include "Screen.h"
#include "Animation.h"
#include "Apple.h"

enum class SpiderState{
	RUNNING,
	THINKING,
	ROTATE,
	HIDING
};

class Spider{
public:
	static void Init(Game* game);
	Spider();
	void Start();
	void Draw();
	void Update(float sec, list<Apple*> &apples);
	float GetRadius();
private:
	static Game* game;
	static Graphics* graphics;
	static Animation* anim;
	static Image* head;
	static Image* body;
	static Audio* run_snd;

	static const float radius;
	static const float speedV;
	static const float speedW;

	SpiderState state;
	PointX pos;
	PointX end_point;
	float angle;
	float head_angle;
	float thinking_time;
	bool eaten;
	//float run_time;
	//float think_time;
	bool OnScreen();
	void ScanForApples(list<Apple*> &apples);
	void SetNearestBorder();
	void EatApple(list<Apple*> &apples);
	Apple* target_apple;
};