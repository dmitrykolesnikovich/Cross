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
#include "Screen.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Launcher.h"

using namespace cross;

float Screen::GetWidth(){
	return gfx2D->GetCamera()->GetViewWidth();
}

float Screen::GetHeight(){
	return gfx2D->GetCamera()->GetViewHeight();
}

float Screen::GetScaleFactor(){
	return (float)launcher->GetTargetWidth() / GetWidth();
}

void Screen::SetBackground(const Color& c){
	glClearColor(c.R, c.G, c.B, 1.f);
}