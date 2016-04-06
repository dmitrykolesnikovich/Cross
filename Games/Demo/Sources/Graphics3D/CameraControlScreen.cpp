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
#include "CameraControlScreen.h"
#include "Launcher.h"
#include "Input.h"
#include "Utils/Misc.h"
#include "Graphics2D.h"
#include "Sprite.h"

#include <math.h>

CameraControlScreen::CameraControlScreen() :
	liner_speed(1.f),
	angular_speed(45.f),
	orbit_speed(5.f),
	yaw(0.f),
	pitch(0.f),
	eye_btn(nullptr),
	handle_action(false)
{ }

void CameraControlScreen::Start() {
	orbit_distance = 1.f;
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);
	camera->SetPosition(Vector3D(0.f, 0.f, -1.f));

	action_down_delegate = MakeDelegate(this, &CameraControlScreen::ActionDownHandle);
	action_move_delegate = MakeDelegate(this, &CameraControlScreen::ActionMoveHandle);
	action_up_delegate = MakeDelegate(this, &CameraControlScreen::ActionUpHandle);
	input->ActionDown += action_down_delegate;
	input->ActionMove += action_move_delegate;
	input->ActionUp += action_up_delegate;

	arrow_released = gfx2D->LoadImage("ArrowUp.png");
	arrow_pressed = gfx2D->LoadImage("ArrowDown.png");

	//button
	float btnWidth = 300.f;
	up_btn = new Button(Vector2D(btnWidth, btnWidth));
	Sprite* cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(90.f);
	Sprite* clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(90.f);
	up_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(up_btn);
	
	left_btn = new Button(Vector2D(btnWidth/3.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(180.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(180.f);
	left_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(left_btn);

	down_btn = new Button(Vector2D(btnWidth, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(-90.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(-90.f);
	down_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(down_btn);

	right_btn = new Button(Vector2D(btnWidth + (btnWidth / 3.f) * 2.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	clonePressed = arrow_pressed->Clone();
	right_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(right_btn);

	Sprite* eye = gfx2D->LoadImage("EyeBtn.png");
	Sprite* eyePressed = gfx2D->LoadImage("EyeBtnPressed.png");
	eye_btn = new ToggleButton(eye, eyePressed);
	eye_btn->Clicked += MakeDelegate(this, &CameraControlScreen::OnEyeClick);
	eye_btn->SetLocation(Vector2D(GetWidth() - eye_btn->GetWidth()/2.f, GetHeight() - eye_btn->GetHeight()/2.f));
	gui.push_back(eye_btn);
}

void CameraControlScreen::Stop(){
	delete camera;
	input->ActionDown -= action_down_delegate;
	input->ActionMove -= action_move_delegate;
	input->ActionUp -= action_up_delegate;

	delete arrow_released;
	delete arrow_pressed;
	for(Button* btn : gui){
		delete btn;
	}
	gui.clear();
}

void CameraControlScreen::Update(float sec){
	bool eulerAngles = false;
	if(input->IsPressed(Key::LEFT)) {
		yaw -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::RIGHT)) {
		yaw += angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::UP)) {
		pitch -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::DOWN)) {
		pitch += angular_speed * sec;
		eulerAngles = true;
	}
	if(eulerAngles) {
		RecalcAngles();
	}

	if(eye_btn->GetState()){	//free camera
		if(input->IsPressed(Key::W) || up_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::S) || down_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::A) || left_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::D) || right_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::SHIFT)) {
			camera->SetPosition(camera->GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
		if(input->IsPressed(Key::CONTROL)) {
			camera->SetPosition(camera->GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
	}else{	//look at camera
		if(input->IsPressed(Key::A) || left_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * sec);
		}
		if(input->IsPressed(Key::D) || right_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * sec);
		}
		if(input->IsPressed(Key::W)) {
			camera->SetPosition(camera->GetPosition() + camera->GetUpVector() * orbit_speed * sec);
		}
		if(input->IsPressed(Key::S)) {
			camera->SetPosition(camera->GetPosition() - camera->GetUpVector() * orbit_speed * sec);
		}
		if(up_btn->IsPressed()){
			orbit_distance -= liner_speed * sec;
		}
		if(down_btn->IsPressed()){
			orbit_distance += liner_speed * sec;
		}
		camera->LookAt(Vector3D(0.f, 0.f, 0.f));
		float objectDistance = camera->GetPosition().Length();
		float diff = objectDistance - orbit_distance;
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
	}
	//gui
	for(Button* btn : gui){
		btn->Update();
	}
}

Camera* CameraControlScreen::GetCamera(){
	return camera;
}

bool CameraControlScreen::OnGuiArea(Vector2D pos){
	for(Button* btn : gui){
		if(btn->OnLocation(pos)){
			return true;
		}
	}
	return false;
}

void CameraControlScreen::RecalcAngles(){
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	float cosPitch = cos(pitch / 180.f * PI);
	float cosYaw = cos(yaw / 180.f * PI);
	float sinPitch = sin(pitch / 180.f * PI);
	float sinYaw = sin(yaw / 180.f * PI);

	Vector3D direction;
	direction.z = cosPitch * cosYaw;
	direction.y = sinPitch;
	direction.x = cosPitch * sinYaw;

	camera->SetDirection(direction);
}

void CameraControlScreen::OnEyeClick(){
	//orbit
	orbit_distance = camera->GetPosition().Length();
	//free
	Vector3D direction = camera->GetDirection();
	float sinP = direction.y;
	float cosP = sqrt(1.f - pow(direction.y, 2.f));
	float sinY = direction.x / cosP;
	pitch = asin(sinP) * 180.f / PI;
	yaw = asin(sinY) * 180.f / PI;
	RecalcAngles();
}

void CameraControlScreen::ActionDownHandle(Vector2D position){
	if(!OnGuiArea(position)){
		handle_action = true;
		touch_position = position;
	}
}

void CameraControlScreen::ActionMoveHandle(Vector2D position){
	if(handle_action){
		if(eye_btn->GetState()){	//free camera
			Vector2D deltaPosition = touch_position - position;
			touch_position = position;
			yaw += deltaPosition.x / 10;
			pitch += deltaPosition.y / 10;
			RecalcAngles();
		}else{						//look at camera
			Vector2D deltaPosition = touch_position - position;
			touch_position = position;
			Vector3D horizontal = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f));
			Vector3D vertical = camera->GetUpVector();
			camera->SetPosition(camera->GetPosition() + horizontal * deltaPosition.x * 0.004f);
			camera->SetPosition(camera->GetPosition() + vertical * deltaPosition.y * 0.004f);

			camera->LookAt(Vector3D(0.f, 0.f, 0.f));
			float objectDistance = camera->GetPosition().Length();
			float diff = objectDistance - orbit_distance;
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
		}
	}
}

void CameraControlScreen::ActionUpHandle(Vector2D position){
	handle_action = false;
}