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
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Mesh.h"

using namespace cross;

Scene::Scene() :
	Screen(),
	ambient_color(Color(0.1f, 0.1f, 0.1f)),
	light_drawing(false),
	point_light_mesh(nullptr),
	spot_light_mesh(nullptr)
{ }

void Scene::Start(){
	Screen::Start();
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);

	window_resize_handle = MakeDelegate(this, &Scene::WindowResizeHandle);
	game->WindowResized += window_resize_handle;
}

void Scene::Update(float sec){
	if(light_drawing){
		for(Light* light : lights){
			switch(light->GetType())
			{
			case Light::Type::POINT:
			{
				point_light_mesh->SetPosition(light->GetPosition());
				point_light_mesh->GetMaterial()->SetDiffuseColor(light->GetColor());
				point_light_mesh->Draw();
			}break;
			case Light::Type::SPOT:
			{
				spot_light_mesh->SetPosition(light->GetPosition());
				spot_light_mesh->SetDirection(light->GetDirection());
				spot_light_mesh->GetMaterial()->SetDiffuseColor(light->GetColor());
				spot_light_mesh->Draw();
			}break;
			default:
				break;
			}
		}
	}
}

void Scene::Stop(){
	delete camera;
	game->WindowResized -= window_resize_handle;
	for(Light* light : lights){
		delete light;
	}
	lights.clear();
	delete light_material;
	delete point_light_mesh;
	delete spot_light_mesh;
	Screen::Stop();
}

void Scene::DrawLights(bool enabled){
	if(enabled && !light_drawing){
		light_material = new Material(gfxGL->GetShader(DefaultShader::SIMPLE));
		point_light_mesh = gfx3D->LoadMesh("Engine/gfx3D/Sphere.obj");
		point_light_mesh->SetScale(0.2f);
		point_light_mesh->SetMaterial(light_material);
		spot_light_mesh = gfx3D->LoadMesh("Engine/gfx3D/Cone.obj");
		spot_light_mesh->SetScale(0.2f);
		spot_light_mesh->SetMaterial(light_material);
	}
	if(!enabled){
		delete light_material;
		delete point_light_mesh;
		delete spot_light_mesh;
	}
	light_drawing = enabled;
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

void Scene::WindowResizeHandle(int width, int height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera->SetProjectionMatrix(projection);
}