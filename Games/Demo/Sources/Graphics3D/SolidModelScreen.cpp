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
#include "SolidModelScreen.h"
#include "Material.h"
#include "Mesh.h"

void SolidModelScreen::Start(){
	CCScene::Start();
	cubeShader = new Shader("gfx3D/shaders/simple.vert", "gfx3D/shaders/simple.frag");
	cubeMaterial = new Material(cubeShader);
	cube = gfx3D->LoadMesh("gfx3D/Cube.obj");
	cube->SetMaterial(cubeMaterial);
}

void SolidModelScreen::Stop(){
	CCScene::Stop();
	delete cube;
	delete cubeMaterial;
	delete cubeShader;
}

void SolidModelScreen::Update(float sec){
	cube->Draw();

	CCScene::Update(sec);
}