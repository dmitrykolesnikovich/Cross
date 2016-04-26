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
#include "DiffuseSpecularMapScreen.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Model.h"
#include "Game.h"

void DiffuseSpecularMapScreen::Start() {
	CameraControlScreen::Start();

	light_caster_mesh = gfx3D->LoadMesh("gfx3D/Cube.obj");
	light = new LightCaster(light_caster_mesh, Vector3D(0.5f), Vector3D(1.f), Vector3D(1.f));
	//light->SetPosition(Vector3D(13.f, -3.f, -5.f));
	light->SetPosition(Vector3D(0.f, 0.f, 0.f));
	light->SetScale(0.2f);
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model = gfx3D->LoadModel("gfx3D/Cube.obj", diffuseTexture, specularTexture);
	model->SetPosition(Vector3D(-20.f, 0.f, 0.f));
}

void DiffuseSpecularMapScreen::Stop() {
	CameraControlScreen::Stop();
	delete model;
	delete light;
}

void DiffuseSpecularMapScreen::Update(float sec) {
	light->Draw();
	gfx3D->DrawModelLightCaster(model, light);

	//model->SetRotateY(game->GetRunTime() * 15.f);
	model->SetRotate(Vector3D(0.f, 1.f, 0.f), game->GetRunTime() * 15.f);

	CameraControlScreen::Update(sec);
}