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
#include "Launcher.h"
#include "Mesh.h"
#include "Material.h"

using namespace cross;

Model::Model(Model& obj) : 
	Transformable(obj),
	meshes(obj.meshes),
	original(false),
	format(obj.format)
{ }

Model::Model(const string& name) :
	name(name),
	original(true),
	format(UNKNOW)
{ 
	filepath = launcher->PathFromFile(name);
	string extension = launcher->ExtensionFromFile(name);
	if(extension == "fbx" || extension == "FBX"){
		format = FBX;
	}else{
		format = UNKNOW;
	}
}

Model::~Model(){
	if(original){
		for(Mesh* mesh : meshes){
			delete mesh;
		}
	}
}

void Model::Draw(){
	for(Mesh* mesh : meshes){
		mesh->Draw(GetModelMatrix());
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

U32 Model::GetPolyCount(){
	U32 polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPrimitivesCount();
	}
	return polyCount;
}

void Model::FaceCulling(bool enabled){
	for(Mesh* mesh : meshes){
		mesh->FaceCulling(enabled);
	}
}

void Model::SetStencil(Mesh::StencilBehaviour behaviour){
	for(Mesh* mesh : meshes){
		mesh->SetStencil(behaviour);
	}
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