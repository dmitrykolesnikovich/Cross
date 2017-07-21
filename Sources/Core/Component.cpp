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
#include "Component.h"
#include "Entity.h"

using namespace cross;

Component::Component(Type type) :
	type(type)
{ }

Component* Component::Clone() const{
	throw CrossException("Unimplemented Component::Clone");
}

Component::Type Component::GetType() const{
	return type;
}

Entity* Component::GetEntity(){
	return entity;
}

Component* Component::GetComponent(Component::Type type){
	return entity->GetComponent(type);
}

Transformable* Component::GetTransform(){
	return entity;
}

Vector3D Component::GetPosition() const{
	return entity->GetPosition();
}

bool Component::IsEnabled(){
	return enabled;
}

void Component::Enable(bool e){
	this->enabled = e;
}