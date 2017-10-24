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
#include "GraphicsGL.h"
#include "VertexBuffer.h"
#include "System.h"
#include "Game.h"
#include "File.h"
#include "Shaders/Shader.h"
#include "Shaders/LightsShader.h"
#include "Config.h"
#include "Scene.h"

#include <algorithm>

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, U32 line) {
	GLenum err = glGetError();
	while(err != GL_NO_ERROR) {
		char* error = new char[255];
		switch(err) {
		case GL_INVALID_OPERATION:    
			strcpy(error, "INVALID_OPERATION");      
			break;
		case GL_INVALID_ENUM:     
			strcpy(error, "INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:     
			strcpy(error, "INVALID_VALUE");         
			break;
		case GL_OUT_OF_MEMORY:     
			strcpy(error, "OUT_OF_MEMORY");          
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  
			strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");  
			break;
		default: 
			strcpy(error, "Unknown error");  
			break;
		}
		CROSS_ASSERT(false, "Rendering error number: %s in %s : %d", error, file, line);
		delete[] error;
		err = glGetError();
	}
}

void GraphicsGL::ClearGLErrorBuffer() {
    GLenum err = glGetError();
    while(err != GL_NO_ERROR) {
        err = glGetError();
    }
}

GraphicsGL::GraphicsGL() {
		system->LogIt("GraphicsGL::GraphicsGL()");

#if defined(OPENGL) || defined(EDITOR)
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		system->LogIt("\tUsed OpenGL %d.%d", magorV, minorV);
		CROSS_ASSERT(!glewInit(), "Unable to initialize GLEW");
#else
		system->LogIt("\tUsed OpenGL ES 2.0");
#endif
		const Byte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		system->LogIt("\tSupported shader version %s", shaderVersion);
		string strV((const char*)shaderVersion);
		strV.erase(remove(strV.begin(), strV.end(), '.'));
		shaders_version = atoi(strV.c_str());
		
		GLint value;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value);
		system->LogIt("\tMax Vetex Uniforms: %d", value);

		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
		system->LogIt("\tMax Fragment Uniforms: %d", value);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
		system->LogIt("\tMax Vertex Attributes: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		system->LogIt("\tMax Texture Size: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		system->LogIt("\tMax Texture Units: %d", value);

		system->WindowResized.Connect(this, &GraphicsGL::WindowResizeHandle);

		SAFE(glCullFace(GL_FRONT));
}

GraphicsGL::~GraphicsGL(){
	SAFE(glDeleteBuffers(1, &quadVBO));
	SAFE(glDeleteBuffers(1, &quadEBO));
	if(config->IsOffscreenRender()){
		delete offscreen_shader;
		SAFE(glDeleteBuffers(1, &framebuffer));
		SAFE(glDeleteBuffers(1, &colorbuffer));
		SAFE(glDeleteBuffers(1, &depthbuffer));
	}
}

void GraphicsGL::Start(){
	if(config->IsOffscreenRender()){
		offscreen_shader = game->GetCurrentScreen()->GetShader("Engine/Shaders/Texture.sha");
		offscreen_shader->Compile();

		SAFE(glGenFramebuffers(1, &framebuffer));

		GenerateFramebuffer();

		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		//quad setub
		static const float quadVertices[] = {
			-1.0f, -1.0f,		0.f, 0.f,
			 1.0f, -1.0f,		1.f, 0.f,
			 1.0f,  1.0f,		1.f, 1.f,
			-1.0f,  1.0f,		0.f, 1.f,
		};
		SAFE(glGenBuffers(1, &quadVBO));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
		SAFE(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

		static const GLushort quadIndices[] = { 0, 1, 2, 0, 2, 3 };
		SAFE(glGenBuffers(1, &quadEBO));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO));
		SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}

void GraphicsGL::Stop(){
	if(colorbuffer_texture){
		delete colorbuffer_texture;
	}
}

void GraphicsGL::PreProcessFrame(){
	if(config->IsOffscreenRender()){
		if(regenerate_framebuffer){
			GenerateFramebuffer();
		}
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		SAFE(glViewport(0, 0, system->GetWindowWidth() / 2, system->GetWindowHeight() / 2));
	}
}

void GraphicsGL::PostProcessFrame(){
	if(config->IsOffscreenRender()){
		//binding default frame buffer
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		SAFE(glViewport(0, 0, system->GetWindowWidth(), system->GetWindowHeight()));
		//drawing color buffer
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
		SAFE(glActiveTexture(GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));

		UseShader(offscreen_shader);
		CROSS_ASSERT(offscreen_shader->uColor != -1, "Textured shader doesn't have color uniform");
		SAFE(glUniform4fv(offscreen_shader->uColor, 1, Color::White.GetData()));

		CROSS_ASSERT(offscreen_shader->aPosition != -1, "Textured shader doesn't have verteces position coordinates");
		SAFE(glEnableVertexAttribArray(offscreen_shader->aPosition));
		SAFE(glVertexAttribPointer(offscreen_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
		

		CROSS_ASSERT(offscreen_shader->aTexCoords != -1, "Textured shader doesn't have texure coordinates");
		SAFE(glEnableVertexAttribArray(offscreen_shader->aTexCoords));
		SAFE(glVertexAttribPointer(offscreen_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));

		CROSS_ASSERT(offscreen_shader->uMVP != -1, "Textured shader doesn't have MVP matrix");
		Matrix mvp = Matrix::Identity;
		SAFE(glUniformMatrix4fv(offscreen_shader->uMVP, 1, GL_FALSE, mvp.GetData()));

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO));
		SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

U32 GraphicsGL::GetShaderVersion() const {
	return shaders_version;
}

Texture* GraphicsGL::GetColorBuffer(){
	CROSS_RETURN(false, NULL, "GetColorBuffer does not working properly!");
	CROSS_ASSERT(config->IsOffscreenRender(), "You obtain collorbuffer without postprocess using");
	if(!colorbuffer_texture){
		//colorbuffer_texture = new Texture();
		//colorbuffer_texture->Create(colorbuffer, bufferWidth, bufferHeight, 4, Texture::Filter::NEAREST);
		//colorbuffer_texture = new Texture(colorbuffer, bufferWidth, bufferHeight, 4, Texture::Filter::NEAREST);
	}
	return colorbuffer_texture;
}

void GraphicsGL::UseShader(Shader* shader){
	CROSS_FAIL(shader, "Attempt to draw with NULL shader");
	CROSS_FAIL(shader->IsCompiled(), "Attempt to draw with not compiled shader");
	SAFE(glUseProgram(shader->GetProgram()));
}

void GraphicsGL::GenerateFramebuffer(){
	SAFE(glDeleteBuffers(1, &framebuffer));
	SAFE(glDeleteBuffers(1, &colorbuffer));
	SAFE(glDeleteBuffers(1, &depthbuffer));
	if(colorbuffer_texture){
		delete colorbuffer_texture;
		colorbuffer_texture = NULL;
	}
	//generate color buffer
	SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
	//generate color buffer
	bufferWidth = system->GetWindowWidth() / 2;
	bufferHeight = system->GetWindowHeight() / 2;

	SAFE(glGenTextures(1, &colorbuffer));
	SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));
	SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	SAFE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0));
	//generate depth buffer
	SAFE(glGenRenderbuffers(1, &depthbuffer));
	SAFE(glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer));
	SAFE(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, bufferWidth, bufferHeight));
	SAFE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer));
	regenerate_framebuffer = false;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		CROSS_ASSERT(false, "Can not initialize second frame buffer");
	}
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height){
	SAFE(glViewport(0, 0, width, height));
	if(config->IsOffscreenRender()){
		//GenerateFramebuffer();
		regenerate_framebuffer = true;
	}
}
