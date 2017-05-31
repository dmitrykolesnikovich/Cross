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

#include "Screen.h"
#include "Utils/Animation.h"
#include "Input.h"

using namespace cross;

class AnimationScreen : public Screen{
public:
	void Start();
	void Stop();
    void Update(float sec);
private:
	Texture* texture;
	Animation* spider_run_anim;
	Sprite* spider_body;
	Sprite* spider_head;
	Sprite* background;
	float run_time;
	float thinking_time;
	float head_angle;
	bool turn_left;
	float deltaY;
};
