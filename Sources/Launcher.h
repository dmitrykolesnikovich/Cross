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

#include "Game.h"
#include "Music.h"
#include "Sound.h"

/* Class witch contains platform specific code */
class Launcher{
public:
	/* Returns phisycal screen width in pixels */
	virtual int GetTargetWidth() = 0;
	/* Returns phisycal screen height in pixels */
	virtual int GetTargetHeight() = 0;
	/* Log out message */
	virtual void LogIt(string msg) = 0;
	/* return path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* return path to the application data folder */
	virtual string DataPath() = 0;
	/* return object of Sound. Input file must be short and wav formated */
	virtual Sound* CreateSound(string filename, bool loop) = 0;
	/* return object of Music. Input file needs to be mp3 */
	virtual Music* CreateMusic(string filename, bool loop) = 0;
//Internal data. You don't need call any of this methods or modify variable
public:
	virtual ~Launcher() { }
private:
};
