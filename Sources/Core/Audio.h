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

namespace FMOD{
	class System;
}

namespace cross {

/*	Class responsible for loading sound and music */
class Audio {
public:
	Audio();
	~Audio();

	Sound* LoadSound(const string& path, bool loop, bool isStream);
	void Suspend();
	void Resume();

private:
	CROSS_FRIENDLY

	FMOD::System* fmod_system = NULL;
};

}
