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
#pragma once
#include "Graphics3D/CameraControlsScreen.h"
#include "Utils/Skybox.h"

class PBRScene : public CameraControlsScreen {
public:
	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

private:
	Skybox* skybox;
	Cubemap* embient;
	Light* light;

	Shader* shader;
	Texture* albedo;
	Texture* metalness;
	Texture* roughness;
	Texture* normals;
	Material* material;
	Model* model;
};