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
#include "JimTheSnake.h"
#include "Animation.h"
#include "Apple.h"

#include <list>

class Spider : public Eatable{
public:
	static void Init();
	static void Release();

	Spider();
	~Spider();

	float GetRadius();
	int Eat();
	bool Eaten();
	void Draw();

	void Update(float sec);
	vector<Collisioner*>& GetRadars();
private:
	enum SpiderState{
		RUNNING,
		THINKING,
		ROTATING,
		HIDING,
		DEAD
	};
	class NearRadar : public Collisioner{
	public:
		NearRadar(Point pos, Spider* spider):Collisioner(pos), spider(spider){};
		float GetRadius() { return 1.f; };
		void CollisionOccurred(Collisioner* obj){
			spider->NearRadarCollision();
		}
		Spider* spider;
	};
	class FarRadar : public Collisioner{
	public:
		FarRadar(Point pos, Spider* spider):Collisioner(pos), spider(spider){};
		float GetRadius() { return 1.f; };
		void CollisionOccurred(Collisioner* obj){
			spider->FarRadarCollision();
		}
		Spider* spider;
	};

	static const float speedW;

	static Animation* st_anim;
	static Image* head;
	static Image* body;

	SpiderState state;
	Animation* anim;
	Audio* run_snd;
	Point destanation;
	vector<Collisioner*> radars;
	Apple* target_apple;
	bool temporary_run;
	float angle;
	float head_angle;
	float thinking_time;
	float speedV;
	bool hungry;

	void NearRadarCollision();
	void FarRadarCollision();

	void ScanForApples(list<Apple*> &apples);
	void SetNearestBorder();
	void EatApple(list<Apple*> &apples);
};
