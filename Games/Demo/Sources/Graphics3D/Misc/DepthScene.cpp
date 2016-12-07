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
#include "DepthScene.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

#include <math.h>

void DepthScene::Start(){
	CameraControlsScreen::Start();

	SetAmbientColor(Color(0.1f));

	shader = new Shader("gfx3D/shaders/depth_test.vert", "gfx3D/shaders/depth_test.frag");
	shader->Compile();

	car_mat = new Material(shader);
	camaro = gfx3D->LoadModel("gfx3D/Camaro/Camaro.fbx");
	camaro->SetMaterial(car_mat);
	
	road_mat = new Material(shader);
	road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(250.f);
	road->FaceCulling(false);
	road->SetMaterial(road_mat);
}

void DepthScene::Stop(){
	delete road;
	delete camaro;
	delete car_mat;
	delete road_mat;
	delete shader;
	CameraControlsScreen::Stop();
}

void DepthScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	camaro->Draw();
	road->Draw();
}