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
#include "SkyboxScene.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Model.h"

void SkyboxScene::Start(){
	CameraControlsScreen::Start();
	
	skybox = new Skybox("gfx3D/Skybox/right.png",
						"gfx3D/Skybox/left.png",
						"gfx3D/Skybox/up.png",
						"gfx3D/Skybox/down.png",
						"gfx3D/Skybox/back.png",
						"gfx3D/Skybox/front.png");

	shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	shader->Compile();
	material = new Material(shader);
	texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png");
	material->SetPropertyValue("Texture", texture);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetMaterial(material);
}

void SkyboxScene::Stop(){

	CameraControlsScreen::Stop();
}

void SkyboxScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	cube->Draw();
	skybox->Draw();
}