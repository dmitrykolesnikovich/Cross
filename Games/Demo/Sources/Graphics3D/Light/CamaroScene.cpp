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
#include "CamaroScene.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

#include <math.h>

void CamaroScene::Start(){
	CCScene::Start();
	DrawLights(true);
	SetAmbientColor(Color(0.05f, 0.05f, 0.05f));
	SetCameraViewDistance(500.f);
	SetBackground(Color::Black);
	SetOrbitDistance(60.f);

	shader = new MultiLightShader("gfx3D/Camaro/camaro.vert", "gfx3D/Camaro/camaro.frag");
	Material* carMaterial = new Material(shader);
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/Camaro/diffuse.jpg");
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/Camaro/specular.jpg");
	Texture* shininessTexture = gfx2D->LoadTexture("gfx3D/Camaro/shininess.jpg");
	carMaterial->SetDiffuseTexture(diffuseTexture);
	carMaterial->SetSpecularTexture(specularTexture);
	carMaterial->SetShininessTexture(shininessTexture);
	camaro = gfx3D->LoadModel("gfx3D/Camaro/Camaro.fbx");
	camaro->SetMaterial(carMaterial);
	
	road = gfx3D->LoadModel("gfx3D/Road/road.3DS");
	Shader* roadShader = gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	Material* roadMaterial = new Material(roadShader);
	Texture* roadDiffuse = gfx2D->LoadTexture("gfx3D/Road/diffuse.jpg");
	roadDiffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	Texture* roadSpecular = gfx2D->LoadTexture("gfx3D/Road/specular.jpg");
	roadSpecular->SetTilingMode(Texture::TilingMode::REPEAT);
	roadMaterial->SetDiffuseTexture(roadDiffuse);
	roadMaterial->SetSpecularTexture(roadSpecular);
	road->SetMaterial(roadMaterial);
	//road->SetShader(gfxGL->GetShader(DefaultShader::MULTI_LIGHT));
	//road->GetMaterial(0)->GetDiffuseTexture()->SetTilingMode(Texture::TilingMode::REPEAT);
	road->SetRotateX(-90.f);
	//lights
	light = new Light(Light::Type::POINT);
	AddLight(light);
}

void CamaroScene::Stop(){
	delete road;
	delete camaro;
	delete shader;
	CCScene::Stop();
}

void CamaroScene::Update(float sec){
	camaro->Draw();
	road->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*23.f, 12.f, sin(game->GetRunTime() / 2.f)*23.f));
	CCScene::Update(sec);
}