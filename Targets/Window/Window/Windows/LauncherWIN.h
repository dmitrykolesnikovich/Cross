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

#define TARGET_WIDTH 500
#define TARGET_HEIGHT 900;

#include <Windows.h>
#include "Launcher.h"
#include "MusicWIN.h"
#include "AudioWIN.h"

class LauncherWIN : public Launcher{
public:
	LauncherWIN(HWND wnd);
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	void LogIt(string msg);
	Sound* CreateSound(string filename, bool loop);
	Music* CreateMusic(string filename, bool loop);
	void ShowMessage(string msg);
private:
	HWND wnd;
	char str_buffer[256];
};