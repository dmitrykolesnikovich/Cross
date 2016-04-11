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
#include "Camera2DScreen.h"
#include "Graphics2D.h"
#include "Sprite.h"
#include "Utils/Misc.h"
#include "Input.h"
#include "Launcher.h"
#include "Camera2D.h"
#include "Game.h"
#include "Font.h"

void Camera2DScreen::Start(){
	grid = gfx2D->LoadImage("1000grid.jpg");
	awesomefase = gfx2D->LoadImage("AwesomeFace.png");
	for(int i = 0; i < 10; ++i){
		Sprite* sprite = new Sprite(*awesomefase);
		sprite->SetScale(0.1f);
		sprite->SetPosition(Vector2D(Random(100.f, 900.f), Random(100.f, 900.f)));
		sprites.push_back(sprite);
		velocities.push_back(Vector3D(Random(-5.f, 5.f), Random(-5.f, 5.f), 0.f));
	}
	cam_speed = 10.f;
	view_width = (float)launcher->GetTargetWidth();
	cam_positon = Vector2D(0.f, 0.f);
	camera = new Camera2D();
	gfx2D->SetCamera(camera);
	tip_font = new Font("Engine/Fonts/VeraMono.ttf", 35, Color(150, 217, 234));

	input->MouseWheelUp += MakeDelegate(this, &Camera2DScreen::WheelUpHandler);
	input->MouseWheelDown += MakeDelegate(this, &Camera2DScreen::WheelDownHandler);
}

void Camera2DScreen::Stop(){
	delete camera;
	delete tip_font;
	for(Sprite* sprite : sprites){
		delete sprite;
	}
	gfx2D->ReleaseSprite(awesomefase);
	gfx2D->ReleaseSprite(grid);
	input->MouseWheelUp.Clear();
	input->MouseWheelDown.Clear();
}

void Camera2DScreen::Update(float sec){
	gfx2D->DrawSprite(Vector2D(500.f, 500.f), grid);
	Color fontColor = tip_font->GetColor();
	static float tipTimeOut = 2.f;
	tipTimeOut -= sec;
	if(tipTimeOut < 0){
		fontColor.A -= 0.01f;
	}
	tip_font->SetColor(fontColor);
	gfx2D->DrawText(Vector2D(50.f, launcher->GetTargetHeight() / 2.f), "Use WSDA for scroling", tip_font);
	gfx2D->DrawText(Vector2D(50.f, launcher->GetTargetHeight() / 2.f - 50.f), "Mouse wheel for zooming", tip_font);
	for(unsigned int i = 0; i < sprites.size(); ++i){
		Sprite* sprite = sprites[i];
		float spriteWidth = sprite->GetWidth() / 2;
		float spriteHeight = sprite->GetHeight() / 2;
		Vector3D position = sprite->GetPosition();
		position += velocities[i] * sec * 100;
		sprite->SetPosition(position);

		if((position.x - spriteWidth) <= 0 || (position.x + spriteWidth) >= 1000.f) {
			velocities[i].x *= -1;
		}
		if((position.y - spriteHeight) <= 0 || (position.y + spriteHeight) >= 1000.f) {
			velocities[i].y *= -1;
		}

		gfx2D->DrawSprite(sprite);
	}

	if(input->IsPressed(Key::D)){
		if(cam_positon.x + view_width < 1000.f){
			cam_positon.x += cam_speed;
		}else{
			cam_positon.x = 1000.f - view_width;
		}
	}
	if(input->IsPressed(Key::A)){
		if(cam_positon.x > 0){
			cam_positon.x -= cam_speed;
		}else{
			cam_positon.x = 0;
		}
	}
	if(input->IsPressed(Key::W)){
		float view_height = view_width / launcher->GetAspectRatio();
		if(cam_positon.y + view_height < 1000.f){
			cam_positon.y += cam_speed;
		}else{
			cam_positon.y = 1000.f - view_height;
		}
	}
	if(input->IsPressed(Key::S)){
		if(cam_positon.y > 0){
			cam_positon.y -= cam_speed;
		}else{
			cam_positon.y = 0;
		}
	}
	camera->SetPosition(cam_positon);

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}

void Camera2DScreen::WheelUpHandler(){
	float height = view_width / launcher->GetAspectRatio();
	if(height < 1000.f){
		view_width *= 1.05f;
	}else{
		view_width = 1000.f * launcher->GetAspectRatio();
	}

	if(cam_positon.x + view_width < 1000.f) {
		cam_positon.x += cam_speed;
	} else {
		cam_positon.x = 1000.f - view_width;
	}
	float view_height = view_width / launcher->GetAspectRatio();
	if(cam_positon.y + view_height < 1000.f) {
		cam_positon.y += cam_speed;
	} else {
		cam_positon.y = 1000.f - view_height;
	}

	camera->SetViewWidth(view_width);

}

void Camera2DScreen::WheelDownHandler(){
	view_width *= 0.95f;
	camera->SetViewWidth(view_width);
}