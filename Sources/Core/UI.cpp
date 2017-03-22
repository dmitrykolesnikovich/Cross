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
#include "UI.h"

using namespace cross;

UI::UI():
	visible(true),
	enable(true)
{ }

bool UI::OnLocation(Vector2D p) const {
	return PointInRect(p, GetRect());
}

bool UI::IsVisible(){
	return visible;
}

void UI::SetVisible(bool yes){
	visible = yes;
}

bool UI::IsEnabled(){
	return enable;
}

void UI::SetEnable(bool enable){
	this->enable = enable;
}