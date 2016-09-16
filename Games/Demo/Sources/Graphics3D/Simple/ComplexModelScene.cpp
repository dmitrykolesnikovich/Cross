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
#include "ComplexModelScene.h"
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Model.h"

void ComplexModelScene::Start(){
	CCScene::Start();
	SetOrbitDistance(60.f);
	camaro = gfx3D->LoadModel("gfx3D/Camaro/camaro.fbx");
	camaro->SetRotateX(-90.f);
}

void ComplexModelScene::Stop(){
	delete camaro;
	CCScene::Stop();
}

void ComplexModelScene::Update(float sec){
	camaro->Draw();
	CCScene::Update(sec);
}