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
#include "Camera.h"

#include "Libs/Events/Event.h"

namespace cross{

/*	This class designed for managing 2D space. Allow you set view distance what 
	This allow you scale 2D sceene and keep abstract from physical screen ppi. 
	In additional you you can scroll across your 2D sceene by SetPosition() function */
class Camera2D : public Camera{
public:
	Camera2D();
	~Camera2D();
	/* Set up camera view distance */
	void SetViewWidth(float width);
	/* Returns width view distance */
	float GetViewWidth();
	/* Returns height view distance */
	float GetViewHeight();

private:
	float view_width;
	float view_height;

	FastDelegate2<S32, S32, void> window_resized_delegate;

	void WindowResizedHandle(S32 width, S32 height);
	void WindowRisezedHandleCust(S32 width, S32 height);
};

}