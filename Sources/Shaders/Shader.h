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
#include "GraphicsGL.h"

namespace cross{

class Shader{
public:
	class Property{
	public:
		enum Type{
			SAMPLER,
			MAT4,
			MAT3,
			VEC4,
			VEC3,
			VEC2,
			FLOAT,
			INT
		};

		Property(string name, Type type, string glName):
			Property(name, type, glName, NULL)
		{ }

		Property(string name, Type type, string glName, void* value):
			name(name),
			type(type),
			glName(glName),
			glId(-1),
			value(value)
		{ }

		string name;
		Type type;
		string glName;
		GLint glId;
		void* value;
	};

	class LightUniforms{
	public:
		GLint position;
		GLuint direction;
		GLint color;

		GLint intensity;
	
		GLuint cut_off;
		GLuint outer_cut_off;
	};

	bool compiled;
	//general attributes
	GLint aPosition;
	GLint aTexCoords;
	GLint aNormal;
	
	//general uniforms
	GLint uMVP;
	GLint uModelMatrix;
	GLint uNormalMatrix;
	GLint uCameraPosition;
	GLint uAmbientLight;
	GLint uColor;
	//custom uniforms
	CRDictionary<string, Property*> properties;

	Shader(const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

	virtual void Compile();

	virtual bool UseLights();
	virtual void TransferLightData(const CRArray<Light*>& lights);

	bool IsCompiled();

	void AddMakro(const string& makro);
	void AddMakro(const string& makro, int value);
	void AddProperty(Property* prop);
	void AddProperty(const string& name, Property::Type type, const string& glName);
	void AddProperty(const string& name, Property::Type type, const string& glName, void* deffaltValue);
	Property* GetProperty(const string& name);

	GLuint GetProgram();

protected:
	GLuint program;
	CRArray<string>	macrosies;

private:
	GLuint vertex_shader;
	GLuint fragment_shader;
	File* vertex_file;
	File* fragment_file;
	U32 makro_len;

	GLuint CompileShader(GLuint type, File* file);
	void CompileProgram();
};

}