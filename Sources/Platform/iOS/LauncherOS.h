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
#include "Launcher.h"

using namespace cross;

class LauncherOS : public Launcher{
public:
    LauncherOS();
    ~LauncherOS();
    
    U32 GetTargetWidth();
    U32 GetTargetHeight();
    string AssetsPath();
    string DataPath();
    void Log(const char* msg);
    U64 GetTime();
    Commercial* GetCommercial();
    
private:
    int width;
    int height;
    Commercial* commercial;
};
