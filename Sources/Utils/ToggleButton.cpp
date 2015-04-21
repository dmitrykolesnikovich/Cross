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
	
#include "ToggleButton.h"

ToggleButton::ToggleButton(Game* game, PointX location, Image* on, Image* off)
	:Button(game, location, on, off){
	this->on = on;
	this->off = off;
}

void ToggleButton::Update(){
	if(state)
		graphics->DrawImage(location, on);
	else
		graphics->DrawImage(location, off);

	//first press
	if(input->HaveInput() && press_loc == NULL){
		press_loc = new PointX(input->GetInput());
	}
	//call listener
	if(input->HaveInput() == false && press_loc != NULL){
		if(OnLocation(input->GetInput().x, input->GetInput().y)){
			SetState(!GetState());
			if(callback_registered){
				delete press_loc;
				press_loc = NULL;
				callback();
				return;
			}else{
				launcher->LogIt("Callback not registered");
			}
		}
	}
	if(!input->HaveInput()){
		delete press_loc;
		press_loc = NULL;
	}
}

bool ToggleButton::GetState(){
	return state;
}

void ToggleButton::SetState(bool state){
	this->state = state;
}