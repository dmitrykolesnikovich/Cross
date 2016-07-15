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

#include "Libs/Events/Event.h"

namespace cross{

/*	Core game class. Designed for contains function and fields shared between multiple Screens.
	Every game must implement this class for set start Screen. */
class Game{
public:
	/* You need to override this method to get engine know from which screen it must start */
	virtual Screen* GetStartScreen() = 0;
	/* Occurs when game window resize */
	DECLARE_EVENT(void, int, int) WindowResized;

	Game();
	virtual ~Game();
	/* Cause when game is about to start */
	virtual void Start() { };
	/* Cause when game is about to stop */
	virtual void Stop() { };
	/* Update game every frame. Must be call from overided virsion either */
	virtual void Update();
	/* Cause when game needs to be paused. For example input call or window lost focus */
	virtual void Suspend();
	/* Cause when game gained focus */
	virtual void Resume();
	/* Returns time since game start */
	float GetRunTime();
	/* New Screen will be updated and deleted due to game live cycle. Previous screen data will be deleted */
	void SetScreen(Screen* screen);
	/* New Scene will be updated and deleted due to game live cycle. Previous screen data will be deleted */
	void SetScene(Scene* scene);
	/* Returns active game Screen */
	Screen* GetCurrentScreen();
	/* Returns active game Scene*/
	Scene* GetCurrentScene();
	/* Exit from application */
    void Exit();
//Internal data. You don't need call any of this methods or modify variable
private:
	Screen* current_screen;
	bool on_scene;
	unsigned long render_time;
	unsigned long run_time;
};
    
}
