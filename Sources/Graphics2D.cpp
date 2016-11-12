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
#include "Graphics2D.h"
#include "GraphicsGL.h"
#include "Launcher.h"
#include "System/Debugger.h"
#include "Sprite.h"
#include "Font.h"
#include "Camera2D.h"
#include "File.h"
#include "Texture.h"
#include "Shaders/Shader.h"
#include "Config.h"

#include <math.h>

#include "Libs/SOIL/SOIL.h"
#include "Libs/FreeType/ft2build.h"
#include "Libs/TinyXML/tinyxml.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H

using namespace cross;

struct PKM{
	Byte identifier[6];
	U16 dataType;
	U16 extendedWidth;
	U16 extendedHeight;
	U16 originalHeight;
	U16 originalWidth;
};

struct KTX{
	Byte identifier[12];
	U32 endianness;
	U32 glType;
	U32 glTypeSize;
	U32 glFormat;
	U32 glInternalFormat;
	U32 glBaseInternalFormat;
	U32 pixelWidth;
	U32 pixelHeight;
	U32 pixelDepth;
	U32 numberOfArrayElements;
	U32 numberOfFaces;
	U32 numberOfMipmapLevels;
	U32 bytesOfKeyValueData;
};

Graphics2D::Graphics2D() :
	camera(NULL),
	texture_shader(NULL),
	font_shader(NULL),
	simple_shader(NULL)
{
	launcher->LogIt("Graphics2D::Graphics2D()");
	texture_shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	texture_shader->Compile();
	font_shader = gfxGL->GetShader(DefaultShader::MONOCHROME);
	font_shader->Compile();
	simple_shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	simple_shader->Compile();
	this->default_font = new Font("Engine/Fonts/VeraMono.ttf", 50, Color::White);
	default_camera = new Camera2D();
}

Graphics2D::~Graphics2D(){
	launcher->LogIt("Graphics2D::~Graphics2D");
	delete default_font;
	delete default_camera;
	delete texture_shader;
	delete font_shader;
	delete simple_shader;
}

void Graphics2D::SetCamera(Camera2D* camera){
	this->camera = camera;
}

Camera2D* Graphics2D::GetCamera() {
	if(camera) {
		return camera;
	} else {
		return default_camera;
	}
}

Camera2D* Graphics2D::GetDefaultCamera(){
	return default_camera;
}

void Graphics2D::DrawPoint(Vector2D pos, Color color){
	gfxGL->UseShader(simple_shader);
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(simple_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, pos.GetData()));
	SAFE(glUniform3fv(simple_shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
	SAFE(glDrawArrays(GL_POINTS, 0, 1));
}

void Graphics2D::DrawLine(Vector2D p1, Vector2D p2, Color color){
	gfxGL->UseShader(simple_shader);
	float vertices[4] = { p1.x, p1.y, p2.x, p2.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(simple_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform3fv(simple_shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
	SAFE(glDrawArrays(GL_LINES, 0, 2));
}

void Graphics2D::DrawRect(Rect rect, Color color){
	DrawRect(rect, color, false);
}

void Graphics2D::DrawRect(Rect rect, Color color, bool filled){
	gfxGL->UseShader(simple_shader);
	float vertices[5 * 2] = {	rect.x, rect.y,
								rect.x + rect.width, rect.y,
								rect.x + rect.width, rect.y + rect.height,
								rect.x, rect.y + rect.height,
								rect.x, rect.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(simple_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform3fv(simple_shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
	if(filled){
		static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices));
	}else{
		SAFE(glDrawArrays(GL_LINE_STRIP, 0, 5));
	}
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color){
	DrawCircle(center, radius, color, false);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled){
	DrawCircle(center, radius, color, filled, 30);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled, U32 accuracy){
	gfxGL->UseShader(simple_shader);
	U32 vertexCount = accuracy;

	// Create a buffer for vertex data
	float* buffer = new float[vertexCount * 2]; 
	U32 idx = 0;

	// Center vertex for triangle fan
	buffer[idx++] = center.x;
	buffer[idx++] = center.y;

	// Outer vertices of the circle
	U32 outerVertexCount = vertexCount - 1;

	for(U32 i = 0; i < outerVertexCount; ++i){
		float percent = (i / (float)(outerVertexCount - 1));
		float rad = percent * 2 * PI;

		//Vertex position
		float outer_x = center.x + radius * cos(rad);
		float outer_y = center.y + radius * sin(rad);

		buffer[idx++] = outer_x;
		buffer[idx++] = outer_y;
	}
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(simple_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 0, buffer));
	//delete buffer;
	SAFE(glUniform3fv(simple_shader->uColor, 1, color.GetData()));
	SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
	if(filled){
		SAFE(glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount));
	}else{
		SAFE(glDrawArrays(GL_LINE_LOOP, 2, vertexCount - 2));
	}
	delete buffer;
}

void Graphics2D::DrawText(Vector2D pos, string textStr){
	DrawText(pos, textStr, this->default_font);
}

void Graphics2D::DrawText(Vector2D pos, const string &textStr, Font* font){
	const char* text = textStr.c_str();

	while(*text){
		Sprite* ch = font->GetChar(*text);
		float advance = font->GetCharAdvance(*text);
		ch->SetPosition(pos);
		DrawSprite(ch, font->GetColor(), true);
		pos.x += advance;
		text++;
	}
}

void Graphics2D::DrawSprite(Sprite* sprite){
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Vector2D pos, Sprite* sprite){
	sprite->SetPosition(pos);
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, bool monochrome){
	DrawSprite(sprite, color, GetCamera(), monochrome);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome){
	Shader* shader = nullptr;
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO));
	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->GetID()));
	if(monochrome){
		shader = font_shader;
	}else{
		shader = texture_shader;
	}

	gfxGL->UseShader(shader);

	SAFE(glUniform3fv(shader->uColor, 1, color.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));

	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * sprite->GetModelMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
	
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
}

Texture* Graphics2D::LoadTexture(const string& filename){
	return LoadTexture(filename, config->GetTextureFilter());
}

Texture* Graphics2D::LoadTexture(const string& filename, Texture::Filter filter){
	return LoadTexture(filename, filter, true);
}

Texture* Graphics2D::LoadTexture(const string& filename, Texture::Filter filter, bool compressed){
	auto it = loaded_textures.begin();
	for( ; it != loaded_textures.end(); it++){
		if( (*it).first->GetName() == filename ){
			break;
		}
	}

	if(it != loaded_textures.end()){
		(*it).second++;
		return (*it).first->Clone();
	}else{
		Texture* newTexture = NULL;
		Debugger::Instance()->SetTimeCheck();
		if(!compressed){
			newTexture = LoadRAWTexture(filename, filter);
		}else{
#if defined(WIN) || defined(IOS)
			newTexture = LoadRAWTexture(filename + ".png", filter);
#elif ANDROID
			newTexture = LoadKTXTexture(filename + ".ktx", filter);
#endif
		}	
		float loadTime = Debugger::Instance()->GetTimeCheck();
		launcher->LogIt("Texture(%s) loaded in %0.1fms", filename.c_str(), loadTime);

		pair<Texture*, S32> pair;
		pair.first = newTexture;
		pair.second = 1;
		loaded_textures.push_back(pair);
		return newTexture->Clone();
	}
}

void Graphics2D::SaveTexture(Texture* texture, const string& filename){
#ifdef OPENGL
	File file;
	file.name = filename;
	file.size = texture->GetWidth() * texture->GetHeight() * texture->GetChannels();
	file.data = new Byte[file.size * texture->GetChannels()];
	SAFE(glBindTexture(GL_TEXTURE_2D, texture->GetID()));
	SAFE(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, file.data));
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	launcher->SaveFile(&file);
#else
	throw CrossException("SaveTexture does not support by current graphics API");
#endif
}

void Graphics2D::ReleaseTexture(const string& filename, GLuint* id){
	auto it = loaded_textures.begin();
	for( ; it != loaded_textures.end(); it++){
		if( (*it).first->GetName() == filename ){
			break;
		}
	}

	if(it != loaded_textures.end()){
		(*it).second--;
		if((*it).second <= 0){
			Texture* texture = (*it).first;
			loaded_textures.erase(it);
			delete texture;
		}
	}else{
		SAFE(glDeleteTextures(1, id));
	}
}

Texture* Graphics2D::LoadRAWTexture(const string& filename, Texture::Filter filter){
	int width, height, channels;

	File* textureFile = launcher->LoadFile(filename);
	
	Byte* image = SOIL_load_image_from_memory(textureFile->data, textureFile->size, &width, &height, &channels, SOIL_LOAD_AUTO);
	delete textureFile;
	if(image == NULL){
		throw CrossException("SOL can't convert file:\n Pay attention on image color channels");
	}
	int newWidth = 1;
	int newHeight = 1;
	while(newWidth < width) {
		newWidth *= 2;
	}
	while(newHeight < height) {
		newHeight *= 2;
	}
	if(newWidth != width || newHeight != height){
		launcher->LogIt("Not power of 2 texture. Performance issue!");
		Byte* newImage = (Byte*)malloc(channels * newWidth * newHeight);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * newWidth * channels, image + i * width * channels, width * channels);
			//Clamp to edge effect
			if(newWidth > width){
				memcpy(newImage + i * newWidth * channels + width * channels, image + i * width * channels, channels);
			}
		}
		//Clamp to edge effect
		if(newHeight > height){
			memcpy(newImage + height * channels * newWidth, image + (height - 1) * width * channels, width * channels);
		}
		width = newWidth;
		height = newHeight;
		image = newImage;
	}
	bool generateMipmap = filter == Texture::Filter::BILINEAR || filter == Texture::Filter::TRILINEAR;
	Texture* texture = CreateTexture(image, channels, width, height, filter, Texture::Compression::NONE, Texture::TilingMode::CLAMP_TO_EDGE, generateMipmap);
	texture->SetName(filename);
	return texture;
}

Texture* Graphics2D::LoadPKMTexture(const string& filename, Texture::Filter filter){
	File* file = launcher->LoadFile(filename);

	PKM pkm;
	U32 offset = sizeof(PKM);
	memcpy(&pkm, file->data, offset);

	Texture* texture = CreateTexture(file->data + offset, 3, pkm.extendedWidth, pkm.extendedHeight, filter, Texture::Compression::ETC1, Texture::TilingMode::CLAMP_TO_EDGE, false);
	texture->SetName(filename);
	return texture;
}

Texture* Graphics2D::LoadKTXTexture(const string& filename, Texture::Filter filter){
	File* file = launcher->LoadFile(filename);

	KTX ktx;
	U32 offset = sizeof(KTX);
	memcpy(&ktx, file->data, offset);

	U32 imageSize;
	memcpy(&imageSize, file->data + offset, sizeof(U32));
	offset += sizeof(U32);

	Byte* imageData = new Byte[imageSize];
	memcpy(imageData, file->data + offset, imageSize);
	offset += imageSize;
	
	Texture* texture = CreateTexture(imageData, 3, ktx.pixelWidth, ktx.pixelHeight, filter, Texture::Compression::ETC1, Texture::TilingMode::CLAMP_TO_EDGE, false);
	texture->SetName(filename);

	U32 mipmapW = ktx.pixelWidth;
	U32 mipmapH = ktx.pixelHeight;
	for(U32 i = 1; i < ktx.numberOfMipmapLevels; i++){
		U32 padding = 3 - ((imageSize + 3) % 4);

		if(padding != 0){
			throw CrossException("Not zero padding");
		}

		mipmapW /= 2;
		mipmapH /= 2;

		memcpy(&imageSize, file->data + offset, sizeof(U32));
		offset += sizeof(U32);
		memcpy(imageData, file->data + offset, imageSize);
		offset += imageSize;
		texture->AddMipmapLelel(i, imageSize, imageData, mipmapW, mipmapH, Texture::Compression::ETC1);
	}
	delete imageData;
	return texture;
}

Texture* Graphics2D::CreateTexture(U32 channels, U32 width, U32 height, Texture::Filter filter){
	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(channels) {
	case 1:
		SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0));
		break;
	case 3:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));
		break;
	case 4:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
		break;
	default:
		throw CrossException("Wrong texture channel count");
	}
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	Texture* texture = new Texture(id, width, height, channels, filter);
	return texture;
}

Texture* Graphics2D::CreateTexture(	Byte* data, 
									U32 channels, 
									U32 width, 
									U32 height, 
									Texture::Filter filter,
									Texture::Compression compression,
									Texture::TilingMode tilingMode,
									bool generateMipmaps ){

	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	if(compression == Texture::Compression::NONE){
		switch(channels) {
		case 1:
			SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data));
			break;
		case 3:
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			break;
		case 4:
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			break;
		default:
			throw CrossException("Wrong texture channel count");
		}
	}else{
		switch(compression) {
		case cross::Texture::Compression::ETC1:
#ifdef ANDROID
		{
            uint32_t dataLength = ((width >> 2) * (height >> 2)) << 3;
            SAFE(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, width, height, 0, dataLength, data));
        }break;
#else
			throw CrossException("ETC1 compression not supported by current platform");
#endif
		default:
			break;
		}
	}

	switch(tilingMode) {
	case cross::Texture::TilingMode::CLAMP_TO_EDGE:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		break;
	case cross::Texture::TilingMode::REPEAT:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		break;
	default:
		break;
	}

	if(generateMipmaps) {
		SAFE(glGenerateMipmap(GL_TEXTURE_2D));
	}

	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	Texture* texture = new Texture(id, width, height, channels, filter);
	return texture;
}

void Graphics2D::LoadSprites(CRDictionary<string, Sprite*>& output, Texture* texture, string xmlFilename){
	File* xmlFile = launcher->LoadFile(xmlFilename);
	TiXmlDocument xml;
	Byte* source = new Byte[xmlFile->size + 1]; // +1 for null terminated string
	memcpy(source, xmlFile->data, xmlFile->size);
	source[xmlFile->size] = 0;
	delete xmlFile;
	xml.Parse((const char*)source, 0, TIXML_ENCODING_UTF8);
	delete source;

	TiXmlHandle xmlDoc(&xml);
	TiXmlElement* root;
	TiXmlElement* element;

	root = xmlDoc.FirstChildElement("TextureAtlas").Element();
	if(root){
		element = root->FirstChildElement("sprite");
		while(element){
			string name = element->Attribute("n");
			float xPos = stof(element->Attribute("x"));
			float yPos = stof(element->Attribute("y"));
			float width = stof(element->Attribute("w"));
			float height = stof(element->Attribute("h"));
			Rect rect(xPos, yPos, width, height);
			Sprite* sprite = new Sprite(texture, rect);
			output[name] = sprite;
			element = element->NextSiblingElement("sprite");
		}
	}else{
		throw CrossException("XML empty root element");
	}
}

void Graphics2D::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GetCamera()->Update(sec);
}

Font* Graphics2D::GetDefaultFont(){
	return default_font;
}
