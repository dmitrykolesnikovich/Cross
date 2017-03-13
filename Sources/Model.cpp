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
#include "Model.h"
#include "System.h"
#include "Mesh.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Entity.h"

using namespace cross;

Model::Model(const string& name) :
	Component(Component::Type::MODEL),
	name(name),
	original(true),
	face_culling(true),
	alpha_blending(false),
	stencil(Graphics3D::StencilBehaviour::IGNORED),
	format(UNKNOW)
{ 
	filepath = system->PathFromFile(name);
	string extension = system->ExtensionFromFile(name);
	if(extension == "fbx" || extension == "FBX"){
		format = FBX;
	}else{
		format = UNKNOW;
	}
}

Model::Model(Model& obj) :
	Component(Component::Type::MODEL),
	Transformable(obj),
	original(false),
	format(obj.format),
	face_culling(obj.face_culling),
	alpha_blending(obj.alpha_blending),
	stencil(obj.stencil)
{ 
	for(Mesh* mesh : obj.meshes){
		meshes.push_back(mesh->Clone());
	}
}

Model::~Model(){
	for(Mesh* mesh : meshes){
		delete mesh;
	}
}

void Model::Update(float sec){
	Draw();
}

void Model::Draw(){
	for(Mesh* mesh : meshes){
		if(!Initialized()){
			gfx3D->DrawMesh(mesh, GetModelMatrix(), face_culling, alpha_blending, stencil);
		}else{
			gfx3D->DrawMesh(mesh, GetModelMatrix() * GetEntity()->GetModelMatrix(), face_culling, alpha_blending, stencil);
		}
	}
}

string Model::GetName(){
	return name;
}

string Model::GetFilePath(){
	return filepath;
}

Model::Format Model::GetFormat(){
	return format;
}

void Model::Initialize(){
	for(Mesh* mesh : meshes){
		mesh->Initialize();
	}
}

void Model::FaceCulling(bool enabled){
	face_culling = enabled;
}

void Model::AlphaBlending(bool enabled){
	alpha_blending = enabled;
}

void Model::SetStencil(Graphics3D::StencilBehaviour behaviour){
	stencil = behaviour;
}

void Model::AddMesh(Mesh* mesh){
	meshes.push_back(mesh);
}

Mesh* Model::GetMesh(const string& name){
	for(Mesh* mesh : meshes){
		if(mesh->GetName() == name){
			return mesh;
		}
	}
	throw CrossException("Model does not contain current mesh");
}

void Model::SetMaterial(Material* mat){
	material = mat;
	for(Mesh* mesh : meshes){
		mesh->SetMaterial(mat);
	}
}

Material* Model::GetMaterial(){
	return material;
}

Model* Model::Clone(){
	return new Model(*this);
}