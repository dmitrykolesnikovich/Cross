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
#include "System.h"
#include "Config.h"
#include "Light.h"
#include "Entity.h"
#include "Material.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Shaders/MultiLightShader.h"

#include "Libs/TinyXML/tinyxml.h"

using namespace cross;

Scene::Scene() :
	Screen(),
	ambient_color(Color(0.1f, 0.1f, 0.1f))
{ }

void Scene::Start(){
	Screen::Start();
	is_scene = true;
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera = new Camera(projection);

	resize_del = sys->WindowResized.Connect(this, &Scene::WindowResizeHandle);
}

void Scene::Update(float sec){
	Screen::Update(sec);
	camera->Update(sec);
	for(Entity* e : objects){
		e->Update(sec);
	}
}

void Scene::Stop(){
	delete camera;
	sys->WindowResized.Disconnect(resize_del);
	for(Entity* e : objects){
		delete e;
	}
	objects.clear();
	Screen::Stop();
}

void Scene::Load(const string& file, bool absolute){
	const int loaderVersion = 11;
	string path = "";
	if(!absolute){
		path = sys->AssetsPath() + file;
	}else{
		path = file;
	}

	TiXmlDocument doc(path.c_str());
	doc.LoadFile();

	TiXmlHandle xmlDoc(&doc);
	TiXmlElement* scene;

	scene = xmlDoc.FirstChildElement("Scene").Element();
	if(scene){
		name = scene->Attribute("name");
		int curVersion = MAXINT;
		scene->Attribute("version", &curVersion);
		if(curVersion <= loaderVersion){
			//general lighting information
			int pointLightCount = 0;
			int spotLightCount = 0;
			int directionalLightCount = 0;
			TiXmlElement* lightXML = scene->FirstChildElement("Light");
			if(lightXML){
				TiXmlElement* pointXML = lightXML->FirstChildElement("Point");
				if(pointXML){
					pointXML->Attribute("count", &pointLightCount);
				}
				TiXmlElement* spotXML = lightXML->FirstChildElement("Spot");
				if(spotXML) {
					spotXML->Attribute("count", &spotLightCount);
				}
				TiXmlElement* directionalXML = lightXML->FirstChildElement("Directional");
				if(directionalXML) {
					directionalXML->Attribute("count", &directionalLightCount);
				}
			}
			//shaders loading
			TiXmlElement* shadersXML = scene->FirstChildElement("Shaders");
			if(shadersXML){
				TiXmlElement* shaderXML = shadersXML->FirstChildElement("Shader");
				while(shaderXML){
					MultiLightShader* shader = NULL;
					int id = -1;
					shaderXML->Attribute("id", &id);
					const char* useLightsStr = shaderXML->Attribute("multiLight");
					bool multiLight = strcmp(useLightsStr, "true") == 0;
					if(multiLight){
						shader = new MultiLightShader();
					}else{
						throw CrossException("Do not implement yet");
					}
					TiXmlElement* macrosies = shaderXML->FirstChildElement("Macrosies");
					if(macrosies){
						TiXmlElement* macro = macrosies->FirstChildElement("Macro");
						while(macro){
							const char* text = macro->GetText();
							shader->AddMacro(text);
							macro = macro->NextSiblingElement("Macro");
						}
					}
					TiXmlElement* properties = shaderXML->FirstChildElement("Properties");
					if(properties){
						TiXmlElement* property = properties->FirstChildElement("Property");
						while(property){
							const char* name = property->Attribute("name");
							const char* glName = property->Attribute("glName");
							const char* defVal = property->Attribute("default");
							if(defVal){
								float def = atof(defVal);
								shader->AddProperty(name, glName, def);
							}else{
								shader->AddProperty(name, glName);
							}
							property = property->NextSiblingElement("Property");
						}
					}

					shader->Compile(pointLightCount, spotLightCount, directionalLightCount);
					shaders[id] = shader;
					shaderXML = shaderXML->NextSiblingElement("Shader");
				}
			}
			//textures loading
			TiXmlElement* texturesXML = scene->FirstChildElement("Textures");
			if(texturesXML){
				TiXmlElement* textureXML = texturesXML->FirstChildElement("Texture");
				while(textureXML){
					int id = -1;
					textureXML->Attribute("id", &id);
					const char* file = textureXML->Attribute("file");
					Texture* texture = gfx2D->LoadTexture(file);
					textures[id] = texture;
					textureXML = textureXML->NextSiblingElement("Texture");
				}
			}
			//materials loading
			TiXmlElement* materialsXML = scene->FirstChildElement("Materials");
			if(materialsXML){
				TiXmlElement* materialXML = materialsXML->FirstChildElement("Material");
				while(materialXML){
					int id = -1;
					int shaderID = -1;
					materialXML->Attribute("id", &id);
					materialXML->Attribute("shader", &shaderID);
					Material* material = new Material(shaders[shaderID]);
					TiXmlElement* property = materialXML->FirstChildElement("Property");
					while(property){
						const char* type = property->Attribute("type");
						const char* name = property->Attribute("name");

						if(strcmp(type, "Texture") == 0){
							int textureID = -1;
							property->Attribute("value", &textureID);
							material->SetPropertyValue(name, textures[textureID]);
						}else if(strcmp(type, "Float") == 0){
							double val = -1.f;
							property->Attribute("value", &val);
							material->SetPropertyValue(name, (float)val);
						}else{
							throw CrossException("Unknown material property type");
						}

						property = property->NextSiblingElement("Property");
					}

					materials[id] = material;
					materialXML = materialXML->NextSiblingElement("Material");
				}
			}
			//objects loading
			TiXmlElement* objectsXML = scene->FirstChildElement("Objects");
			if(objectsXML){
				TiXmlElement* objectXML = objectsXML->FirstChildElement("Object");
				while(objectXML){
					const char* name = objectXML->Attribute("name");
					const char* file = objectXML->Attribute("file");
					Entity* entity = NULL;
					if(file){
						int materialID = -1;
						objectXML->Attribute("material", &materialID);

						entity = gfx3D->LoadModel(file);
						gfx3D->AdjustMaterial(entity, materials[materialID]);
					}else{
						entity = new Entity();
					}

					TiXmlElement* posXML = objectXML->FirstChildElement("Position");
					if(posXML){
						double x, y, z;
						posXML->Attribute("x", &x);
						posXML->Attribute("y", &y);
						posXML->Attribute("z", &z);
						Vector3D pos(x, y, z);
						entity->SetPosition(pos);
					}

					TiXmlElement* componentsXML = objectXML->FirstChildElement("Components");
					if(componentsXML){
						TiXmlElement* componentXML = componentsXML->FirstChildElement("Component");
						while(componentXML){
							const char* name = componentXML->Attribute("name");
							if(strcmp(name, "Light") == 0){
								const char* type = componentXML->Attribute("type");
								if(strcmp(type, "Point") == 0){
									Light* light = new Light(Light::Type::POINT);
									entity->AddComponent(light);
								}else{
									throw CrossException("Unknown light type");
								}
							}else{
								throw CrossException("Unknown component");
							}
							componentXML = componentXML->NextSiblingElement();
						}
					}
					AddEntity(entity);
					objectXML = objectXML->NextSiblingElement("Object");
				}
			}
		}else{
			throw CrossException("Version missmatch");
		}
	}else{
		throw CrossException("Can not load scene. Wrong file format");
	}
}

void Scene::Save(const string& file){

}

void Scene::SetCameraViewDistance(float distance){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, distance);
	camera->SetProjectionMatrix(projection);
}

Entity* Scene::GetEntity(const string& name){
	for(Entity* e : objects){
		if(e->GetName() == name){
			return e;
		}
		Entity* child = e->FindChild(name);
		if(child){
			return child;
		}
	}
	throw CrossException("Can not find entity %s", name.c_str());
}

void Scene::AddEntity(Entity* entity){
	entity->Initialize();
	objects.push_back(entity);
}

Entity* Scene::RemoveEntity(const string& name){
	for(auto it = objects.begin(); it != objects.end(); it++){
		Entity* e = (*it);
		if(e->GetName() == name){
			e->Remove();
			objects.erase(it);
			return e;
		}else{
			Entity* child = e->RemoveChild(name);
			if(child){
				return child;
			}
		}
	}
	return NULL;
}

List<Entity*>& Scene::GetObjects(){
	return objects;
}

List<Light*>& Scene::GetLights(){
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
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}