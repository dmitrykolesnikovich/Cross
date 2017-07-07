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

/*	This class needed to link gpu shader input data to engine cpu core. 
	Needed to create any kind of material */
class Shader{
public:
	class Property{
	public:
		Property(const string& name, const string& glName);
		Property(const Property& obj);
		~Property();

		void SetValue(U32 v);
		void SetValue(float v);
		void SetValue(const Color& v);
		void SetValue(const Vector3D& v);
		void SetValue(const Vector4D& v);
		void SetValue(Matrix& v);
		void SetValue(Texture* texture);
		void SetValue(Cubemap* cubemap);

		Property* Clone() const;

		GLuint GetID() const;

	private:
		CROSS_FRIENDLY

		enum Type{
			SAMPLER,
			MAT4,
			VEC4,
			VEC3,
			VEC2,
			FLOAT,
			INT,
			CUBEMAP,
			UNKNOWN,
		};

		void RealocateIfNeeded(U32 newSIze);

		string name		= string();
		Type type		= UNKNOWN;
		string glName	= string();
		GLint glId		= -1;
		U32 size		= 0;
		void* value		= NULL;
		bool original	= true;
	};

	Shader(const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

	virtual void Compile();

	virtual bool UseLights();
	virtual void TransferLightData(const List<Light*>& lights);

	bool IsCompiled() const;

	void AddVersion(const string& version);
	void AddMacro(const string& makro);
	void AddMacro(const string& makro, int value);
	void AddProperty(const string& name, const string& glName);
	void AddProperty(const string& glName, float defValue);
	void AddProperty(const string& name, const string& glName, float defValue);
	void AddProperty(const string& name, const string& glName, const Color& color);
	void AddProperty(const string& name, const string& glName, const Vector3D& vec);
	void AddProperty(Property* prop);
	Property* GetProperty(const string& name);
	bool HaveProperty(const string& name) const;

protected:
	CROSS_FRIENDLY

	class LightUniforms{
	public:
		GLint position			= -1;
		GLint direction			= -1;
		GLint color				= -1;

		GLint intensity			= -1;
	
		GLint cut_off			= -1;
		GLint outer_cut_off		= -1;
	};

	GLuint program				= 0;
	Array<string> macrosies		= Array<string>();
	bool compiled				= false;
	//general attributes
	GLint aPosition				= -1;
	GLint aTexCoords			= -1;
	GLint aNormal				= -1;
	GLint aTangent				= -1;
	GLint aBitangent			= -1;
	//general uniforms
	GLint uMVP					= -1;
	GLint uModelMatrix			= -1;
	GLint uNormalMatrix			= -1;
	GLint uCameraPosition		= -1;
	GLint uAmbientLight			= -1;
	GLint uColor				= -1;
	//custom uniforms
	Array<Property*> properties	= Array<Property*>();

	GLuint GetProgram() const;

private:
	GLuint vertex_shader		= 0;
	GLuint fragment_shader		= 0;
	File* vertex_file			= NULL;
	File* fragment_file			= NULL;
	U32 makro_len				= 0;

	GLuint CompileShader(GLuint type, File* file);
	void CompileProgram();
};

}