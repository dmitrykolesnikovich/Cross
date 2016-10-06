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
#include "Cross.h"
#include "Scene.h"
#include "Camera.h"
#include "Input.h"
#include "Utils/Button.h"
#include "Utils/ToggleButton.h"
#include "Utils/FreeCameraScene.h"

using namespace cross;

class SceneView : public FreeCameraScene{
public:
	SceneView();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);
private:
	Shader* shader;
	Material* material;
	Model* cube;

	void InitializeCube();
};