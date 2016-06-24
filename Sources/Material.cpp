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
#include "Material.h"

using namespace cross;

Material::Material(Shader* shader) :
	shader(shader),
	diffuse_color(Color(0.f, 0.f, 0.f)),
	shininess(0.f),
	diffuse_texture(nullptr)
{ }

void Material::SetDiffuseColor(const Color& color){
	diffuse_color = color;
}

void Material::SetDiffuseTexture(Texture* diffuse){
	diffuse_texture = diffuse;
}

void Material::SetShininess(float snininess){
	this->shininess = shininess;
}