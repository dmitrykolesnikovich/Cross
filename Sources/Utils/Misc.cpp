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
	
#include "Misc.h"
#include "PointX.h"
#include "RectX.h"
#include <math.h>

bool PointInRect(PointX p, RectX rect){
	return  p.x > rect.x &&
			p.x < rect.x + rect.width &&
			p.y > rect.y &&
			p.y < rect.y + rect.height;
}

bool CircleOnCollision(PointX p1, float r1, PointX p2, float r2) {
	//gfx.DrawCircle(p1, r1);		//debug
	//gfx.DrawCircle(p2, r2);		//debug
	double distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	if(distance < (r1 + r2))
		return true;
	else return false;
}

float Lerp(float v0, float v1, float t) {
	return (1-t)*v0 + t*v1;
}