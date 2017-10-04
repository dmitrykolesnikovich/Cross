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
#include "ScrollScreen.h"
#include "Graphics2D.h"
#include "Camera2D.h"

void ScrollScreen::Start(){
	Screen::Start();
	width = Screen::GetWidth();
	height = Screen::GetHeight();
}

void ScrollScreen::Stop(){
	Screen::Stop();
}

float ScrollScreen::GetWidth(){
	return width;
}

float ScrollScreen::GetHeight(){
	return height;
}

void ScrollScreen::SetWidth(float width){
	this->width = width;
	SizeChanged(width, height);
}

void ScrollScreen::SetHeight(float height){
	this->height = height;
	SizeChanged(width, height);
}

void ScrollScreen::ResetScreenSize(){
	width = Screen::GetWidth();
	height = Screen::GetHeight();
}

void ScrollScreen::ActionDown(Input::Action action){
	touch = action.pos;
}

void ScrollScreen::ActionMove(Input::Action action){
	Vector2D diff = action.pos - touch;
	diff /= 2.f;
	diff.y = -diff.y;
	touch = action.pos;

	Vector2D camPos = Vector2D(camera2D->GetPosition().x, camera2D->GetPosition().y);
	camPos += diff;
	float camW = camera2D->GetViewWidth();
	float camH = camera2D->GetViewHeight();

	if(camPos.x < 0) {
		camPos.x = 0;
	}
	if(camPos.y < 0) {
		camPos.y = 0;
	}
	if(camPos.x > GetWidth() - camW){
		camPos.x = GetWidth() - camW;
	}
	if(camPos.y > GetHeight() - camH){
		camPos.y = GetHeight() - camH;
	}

	camera2D->SetPosition(camPos);
}

void ScrollScreen::ActionUp(Input::Action action){
	ActionMove(action);
}