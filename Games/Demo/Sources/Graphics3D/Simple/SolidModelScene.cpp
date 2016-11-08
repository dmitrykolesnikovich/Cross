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
#include "SolidModelScene.h"
#include "Graphics3D.h"
#include "GraphicsGL.h"
#include "Material.h"
#include "Model.h"

void SolidModelScene::Start(){
	CameraControlsScreen::Start();
	shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	shader->Compile();
	material = new Material(shader);
	material->SetPropertyValue("Color", (void*)&Color::Green);
	cube = gfx3D->LoadModel("Engine/gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void SolidModelScene::Stop(){
	delete cube;
	delete material;
	delete shader;
	CameraControlsScreen::Stop();
}

void SolidModelScene::Update(float sec){
	cube->Draw();
	CameraControlsScreen::Update(sec);
}