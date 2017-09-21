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
#include "SpecularScene.h"
#include "Light.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"

void SpecularScene::Start(){
	CameraControlsScene::Start();
	//lights
	light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	light->SetName("Light");
	AddEntity(light);

	shader = gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Specular Multiplier", "uSpecularMultiplier", 4.f);
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();

	diffuse = gfx2D->LoadTexture("gfx3D/Camaro/Diffuse.png");
	specular = gfx2D->LoadTexture("gfx3D/Camaro/Specular.png");

	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", diffuse);
	material->SetPropertyValue("Specular Map", specular);
	material->SetPropertyValue("Shininess", 64.f);
	Entity* model = GetModel("gfx3D/Camaro/Camaro.fbx")->GetHierarchy();
	gfx3D->AdjustMaterial(model, material);
	AddEntity(model);
}

void SpecularScene::Stop(){
	delete material;
	delete specular;
	delete diffuse;
	delete shader;
	CameraControlsScene::Stop();
}

void SpecularScene::Update(float sec){
	CameraControlsScene::Update(sec);
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}