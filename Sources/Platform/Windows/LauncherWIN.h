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
// 16:9
//#define TARGET_WIDTH 540		
//#define TARGET_HEIGHT 960
//iPhone 5
//#define TARGET_WIDTH 320		
//#define TARGET_HEIGHT 568	
// 4:3 (htc wildfire, iPhone 4)
#define TARGET_WIDTH 320 
#define TARGET_HEIGHT 480

#include <Windows.h>
#include "Launcher.h"
#include "Audio.h"

namespace cross{

class LauncherWIN : public Launcher{
public:
	LauncherWIN(HWND wnd);
	~LauncherWIN();
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	void LogIt(string msg);
	void Sleep(float milis);
	void ShowMessage(string msg);
private:
	HWND wnd;
};

}