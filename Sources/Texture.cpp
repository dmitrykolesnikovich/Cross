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
#include "Texture.h"
#include "Graphics2D.h"

using namespace cross;

Texture::Texture(GLuint id, U32 width, U32 height) : 
	Texture(id, width, height, Filter::LINEAR)
{ }

Texture::Texture(GLuint id, U32 width, U32 height, Filter filter) :
	id(id),
	width(width),
	height(height)
{
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	ApplyFilter(filter);
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture(){
	gfx2D->ReleaseTexture(name, &id);
}

GLuint Texture::GetID() const{
	return id;
}

U32 Texture::GetWidth() const{
	return width;
}

U32 Texture::GetHeight() const{
	return height;
}

void Texture::SetName(const string& name){
	this->name = name;
}

string Texture::GetName(){
	return name;
}

void Texture::SetTilingMode(Texture::TilingMode mode){
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(mode) {
	case cross::Texture::TilingMode::CLAMP_TO_EDGE:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		break;
	case cross::Texture::TilingMode::REPEAT:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		break;
	default:
		throw CrossException("Unknown tiling mode");
	}
}

void Texture::AddMipmapLelel(U32 level, U32 dataLen, Byte* data, U32 w, U32 h, Texture::Compression comp){
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	if(comp == Texture::Compression::ETC1){
#ifdef ANDROID
		SAFE(glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_ETC1_RGB8_OES, w, h, 0, dataLen, data));
#endif // ANDROID
	}
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture* Texture::Clone() const{
	return new Texture(*this);
}

void Texture::ApplyFilter(Filter filter){
	switch(filter){
	case cross::Texture::Filter::NEAREST:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		break;
	case cross::Texture::Filter::LINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::Filter::BILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::Filter::TRILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	default:
		throw CrossException("Wrong texture filter type");
	}
}