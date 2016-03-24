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
#include "Utils/Button.h"

using namespace cross;

class Menu{
public:
	Menu();
	~Menu();

	void Update(float sec);
	void Active(bool active);
	void LocateButtons();
	void AddButton(Button* but);
	int Count();
	void Clear();
private:
	CRArray<Button*> buttons;
	float button_def_width;
	float button_def_height;
	float button_width;
	float button_height;
	float offset;

	FastDelegate2<int, int, void> window_resized_delegate;

	void WindowResizedHandle(int width, int height);
};