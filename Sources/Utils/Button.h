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
#include "Events\Event.h"

namespace cross {

class Button{
public:
	DECLARE_EVENT(void) Clicked;

	Button(Rect area, string text);
	Button(Vector2D location, string text);
	Button(string text);
	Button(Rect area);
	Button(Vector2D location);
	~Button();

	virtual void Update();

	void SetLocation(Vector2D location);
	void SetText(string text);
	void SetImages(Sprite* up, Sprite* down);
	void SetImages(Sprite* up);
	void SetSounds(Audio* push, Audio* pull);
	void SetActive(bool active);
	void Scale(float coef);
	bool IsPressed() const;
	float GetWidth() const;
	float GetHeight() const;
	Sprite* GetUpImage() const;
	Sprite* GetDownImage() const;
	Rect GetRect() const;
	Vector2D GetCenter() const;
protected:

	Button();
	//mandatory parameters
	Vector2D location;
	bool located;
	bool is_pressed;
	bool active;
	Rect area;
	float def_width;
	float def_height;
	//optional parameters
	Sprite* up_image;
	Sprite* down_image;
	Audio* push_sound;
	Audio* pull_sound;
	Font* font;
	string label_text;
	Vector2D text_size;

	FastDelegate1<Vector2D, void> action_down_delegate;
	FastDelegate1<Vector2D, void> action_up_delegate;

	void Locate(Vector2D loc, float width, float heiht);
	void Locate(Rect rect);
	bool OnLocation(float x, float y);
	bool OnLocation(Vector2D p);
	void FitText(string text);
	//connections
	void ActionDownHandler(Vector2D pos);
	void ActionUpHandler(Vector2D pos);
};
    
}
