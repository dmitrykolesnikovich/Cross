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
#include "LightCastersScreen.h"
#include "Shaders/Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Utils/Misc.h"

void LightCastersScreen::Start(){
	CameraControlScreen::Start();
	gfx3D->GetCamera()->SetPosition(Vector3D(0.f, 0.f, -60.f));
	model = gfx3D->LoadModel(Shader::Type::LIGHT_CASTERS, "gfx3D/Cube.obj");
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model->SetDiffuseTexture(diffuseTexture);
	model->SetSpecularTexture(specularTexture);

	for(int i = 0; i < 10; ++i){
		Model* clone = model->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void LightCastersScreen::Stop(){
	CameraControlScreen::Stop();
}

void LightCastersScreen::Update(float sec){
	for(Model* obj : objects){
		obj->Draw();
	}

	CameraControlScreen::Update(sec);
}