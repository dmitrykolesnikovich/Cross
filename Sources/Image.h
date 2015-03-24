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

#include "RectX.h"
#include "Graphics.h"

class Image{
//User module
public:
	/* Return virtual image width */
    float GetWidth();
	/* Return virtual image height */
    float GetHeight();
//Framework module. You don't need call any of this methods or modify variable
public:
    float angle;
    
	Image(GLuint id, int texWidth, int texHeight, RectX region);
	void Scale(float factor);
	GLuint GetTextureID();
	float* GetVertices();
	static const short indices[6];
	~Image();
private:

	GLuint textureID;
	float vertices[16];
	RectX region;
	float u1, v1;
	float u2, v2;
};