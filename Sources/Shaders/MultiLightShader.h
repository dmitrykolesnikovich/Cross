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
#include "Shaders/Shader.h"

namespace cross{

class MultiLightShader : public Shader{
public:
	static const U32 MaxPointLights = 16;
	static const U32 MaxDirectionalLights = 4;
	static const U32 MaxSpotLights = 8;

	MultiLightShader();
	MultiLightShader(const string& vert, const string& frag);

	virtual void Compile();
	virtual void Compile(const Array<Light*>& lights);

	virtual bool UseLights();
	virtual void TransferLightData(const Array<Light*>& lights);

private:
	LightUniforms uPointLights[MaxPointLights];
	LightUniforms uDirectionalLights[MaxDirectionalLights];
	LightUniforms uSpotLights[MaxSpotLights];
};

}