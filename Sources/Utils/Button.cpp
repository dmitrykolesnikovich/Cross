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
	
#include "Button.h"

using namespace cross;

Button::Button(Game* game, Image* up, Image* down){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = Point();
	this->up = up;
	this->down = down;
	this->press_loc = NULL;
	this->callback_registered = false;
	this->have_area = false;
	this->area.width = up->GetWidth();
	this->area.height = up->GetHeight();
}

Button::Button(Game* game, Point location, Image* up, Image* down){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = location;
	this->up = up;
	this->down = down;
	this->press_loc = NULL;
	this->callback_registered = false;
	this->have_area = false;
	InitRect(location, up->GetWidth(), up->GetHeight());
}

Button::Button(Game* game, Point location, float width, float height){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = Point();
	this->area = area;
	this->up = NULL;
	this->down = NULL;
	this->press_loc = NULL;
	this->callback_registered = false;
	InitRect(location, width, height);
	/*
	area.x = location.x - width / 2;
	area.y = location.y - height / 2;
	area.width = width;
	area.height = height;
	have_area = true;*/
}

Button::Button(Game* game, Rect area){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = Point();
	this->area = area;
	this->up = NULL;
	this->down = NULL;
	this->press_loc = NULL;
	this->callback_registered = false;
	this->have_area = true;
}

void Button::SetLocation(Point location){
	this->location = location;
	InitRect(location, area.width, area.height);
}

void Button::Update(){
	//first press
	if(press_loc == NULL && input->HaveInput()){
		press_loc = new Point(input->GetInput().x, input->GetInput().y);
		if(OnLocation(input->GetInput().x, input->GetInput().y)){
			is_pressed = true;
		}
	}
	//callback
	if(!input->HaveInput() && press_loc != NULL){
		if(OnLocation(input->GetInput().x, input->GetInput().y)){
			if(callback_registered){
				delete press_loc;
				press_loc = NULL;
				if(down != NULL){
					graphics->DrawImage(location, down);
				}
				callback();
				return;
			}
		}
	}
	if(!input->HaveInput()){
		delete press_loc;
		press_loc = NULL;
		is_pressed = false;
	}
	if(input->HaveInput() && press_loc != NULL && OnLocation(press_loc->x, press_loc->y)){
		if(down != NULL){
			graphics->DrawImage(location, down);
		}
	}else{
		if(up != NULL){
			graphics->DrawImage(location, up);
		}
	}
}

float Button::GetWidth(){
	return up->GetWidth();
}

float Button::GetHeight(){
	return up->GetHeight();
}

Rect Button::GetRect(){
	if(!have_area){
		throw string("This button have not area");
	}/*
	rect.x = location.x - up->GetWidth() / 2;
	rect.y = location.y - up->GetHeight() / 2;
	rect.width = up->GetWidth();
	rect.height = up->GetHeight();
	return rect;*/
	return area;
}

Point Button::GetCenter(){
	return location;
}

bool Button::OnLocation(float x, float y){/*
		return  x > (location.x - up->GetWidth() / 2) &&
				x < (location.x + up->GetWidth() / 2) &&
				y > (location.y - up->GetHeight() / 2) &&
				y < (location.y + up->GetHeight() / 2);*/
	return	x > area.x &&
			x < (area.x + area.width) &&
			y > area.y &&
			y < (area.y + area.height);
}

void Button::InitRect(Point loc, float width, float heiht){
	area.x = loc.x - width / 2.f;
	area.y = loc.y - heiht / 2.f;
	area.width = width;
	area.height = heiht;
	have_area = true;
}

void Button::RegisterCallback(function<void()> callback){
	callback_registered = true;
	this->callback = callback;
}

bool Button::IsPressed(){
	return is_pressed;
}

Button::~Button(){
	graphics->ReleaseImage(down);
	graphics->ReleaseImage(up);
}
