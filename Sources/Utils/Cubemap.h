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
#include "GraphicsGL.h"

namespace cross{

/*	Special type of texture which represents 360 observible texutre. 
	Created by 6 faces(top, right, back etc.) */
class Cubemap {
public:
	Cubemap(const string& right,
			const string& left,
			const string& top,
			const string& bottom,
			const string& back,
			const string& front);
	~Cubemap();

private:
	CROSS_FRIENDLY;

	GLuint textureID = 0;

	GLuint GetTextureID() const;
};

};