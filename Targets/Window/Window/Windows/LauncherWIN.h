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

#define TARGET_WIDTH 550
#define TARGET_HEIGHT 900;

#include <Windows.h>
#include <stdio.h>
#include "Launcher.h"

class LauncherWIN : public Launcher{
public:
	int GetTargetWidth();
	int GetTargetHeight();
	void LogIt(const char* str);
private:
	char str_buffer[256];
};