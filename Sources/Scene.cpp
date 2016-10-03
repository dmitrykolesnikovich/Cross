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
#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "Launcher.h"
#include "Utils/Light.h"
#include "Graphics3D.h"
#include "GraphicsGL.h"
#include "Material.h"
#include "Model.h"

using namespace cross;

Scene::Scene() :
	Screen(),
	ambient_color(Color(0.1f, 0.1f, 0.1f)),
	point_light(NULL),
	spot_light(NULL),
	light_material(NULL)
{ }

void Scene::Start(){
	Screen::Start();
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);

	window_resize_handle = MakeDelegate(this, &Scene::WindowResizeHandle);
	game->WindowResized += window_resize_handle;
	//light drawing stuff
	light_shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	light_shader->Compile();
	light_material = new Material(light_shader);
	point_light = gfx3D->LoadModel("Engine/gfx3D/Sphere.obj");
	point_light->SetScale(0.2f);
	point_light->SetMaterial(light_material);
	spot_light = gfx3D->LoadModel("Engine/gfx3D/Cone.obj");
	spot_light->SetScale(0.2f);
	spot_light->SetMaterial(light_material);
}

void Scene::Update(float sec){
	camera->Update(sec);
}

void Scene::Stop(){
	delete camera;
	game->WindowResized -= window_resize_handle;
	for(Light* light : lights){
		delete light;
	}
	lights.clear();
	delete light_material;
	delete point_light;
	delete spot_light;
	delete light_shader;
	Screen::Stop();
}

void Scene::SetCameraViewDistance(float distance){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, distance);
	camera->SetProjectionMatrix(projection);
}

void Scene::DrawLights(){
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:
			point_light->SetPosition(light->GetPosition());
			point_light->GetMaterial()->SetPropertyValue("Color", (void*)light->GetColor().GetData());
			point_light->Draw();
		break;
		case Light::Type::SPOT:
			spot_light->SetPosition(light->GetPosition());
			spot_light->SetDirection(light->GetDirection());
			spot_light->GetMaterial()->SetPropertyValue("Color", (void*)light->GetColor().GetData());
			spot_light->Draw();
		break;
		default:
			break;
		}
	}
}

void Scene::AddLight(Light* light){
	lights.push_back(light);
}

CRArray<Light*>& Scene::GetLights(){
	return lights;
}

Camera* Scene::GetCamera(){
	return camera;
}

Color Scene::GetAmbientColor() const{
	return ambient_color;
}

void Scene::SetAmbientColor(const Color& color){
	this->ambient_color = color;
}

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera->SetProjectionMatrix(projection);
}