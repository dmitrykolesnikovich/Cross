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
#include "Game.h"
#include "System.h"
#include "Input.h"
#include "Config.h"
#include "Scene.h"
#include "System/Debugger.h"
#include "Graphics2D.h"

using namespace cross;

Game*		cross::game		= NULL;
System*		cross::system	= NULL;
Audio*		cross::audio	= NULL;
GraphicsGL* cross::gfxGL	= NULL;
Graphics2D* cross::gfx2D	= NULL;
Graphics3D* cross::gfx3D	= NULL;
Input*		cross::input	= NULL;
Config*		cross::config	= NULL;

Game::Game() :
	current_screen(NULL),
	next_screen(NULL),
	run_time(0),
	timestamp(0)
{
	system->LogIt("Game::Game()");
	input = new Input();
	config = new Config();
}

Game::~Game(){
	system->LogIt("Game::~Game");
	delete current_screen;
	delete config;
	delete input;
}

void Game::Start(){
	gfxGL->Start();
}

void Game::Stop(){
	gfxGL->Stop();
}

void Game::SetScreen(Screen* screen){
	next_screen = screen;
	if(!current_screen){	//in this case we need momentaly load new screen
		LoadNextScreen();
	}
}

Screen* Game::GetCurrentScreen(){
	return current_screen;
}

Scene* Game::GetCurrentScene(){
	if(current_screen->IsScene()){
		return (Scene*)current_screen;
	}else{
		throw CrossException("Current game state does not have 3D scene");
	}
}

void Game::Suspend(){
	if(current_screen != nullptr) {
		current_screen->Suspend();
	}
}

void Game::Resume(){
	timestamp = system->GetTime();
	if(current_screen != nullptr) {
		current_screen->Resume();
	}
}

float Game::GetRunTime(){
	return (float)(run_time / 1000000.f);
}

void Game::EngineUpdate(){
	U64 now = system->GetTime();
	U64 updateTime = now - timestamp;
	float secTime = (float)(updateTime / 1000000.);
	timestamp = now;
	run_time += updateTime;

	if(next_screen){
		LoadNextScreen();
	}

	input->Update();
	gfxGL->PreProcessFrame();
	gfx2D->Update(secTime);
	game->GetCurrentScreen()->Update(secTime);
	game->GetCurrentScreen()->LateUpdate(secTime);
	game->Update(secTime);
	gfxGL->PostProcessFrame();

	Debugger::Instance()->Update((float)updateTime);
	U64 cpuTime = system->GetTime() - timestamp;
	Debugger::Instance()->SetCPUTime((float)cpuTime);
	/*
	float milis = cpuTime / 1000.f;
	if(milis < 5){
		system->Sleep(5 - milis);
	}*/
	//system->Sleep(160.f);
}

void Game::Exit(){
#ifdef WIN
	exit(0);
#endif // WIN
}

void Game::LoadNextScreen(){
	system->LogIt("Game::LoadNextScreen()");
	Debugger::Instance()->SetTimeCheck();

	if(current_screen){
		current_screen->Stop();
		delete current_screen;
	}
	current_screen = next_screen;
	next_screen = NULL;
	current_screen->Start();
	timestamp = system->GetTime();
	float loadTime = Debugger::Instance()->GetTimeCheck();
	system->LogIt("Screen(no name) loaded in %0.1fms", loadTime);
	TRIGGER_EVENT(ScreenChanged, current_screen);
}