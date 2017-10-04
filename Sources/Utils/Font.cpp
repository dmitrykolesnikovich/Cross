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
#include "Utils/Font.h"
#include "System.h"
#include "File.h"
#include "Graphics2D.h"
#include "Sprite.h"
#include "Texture.h"

#include "Libs/FreeType/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#undef GetCharWidth

using namespace cross;

static FT_Library library = NULL;

Font::Font(const Font& font) :
	color(font.color),
	original(false),
	face(font.face),
	size(font.size),
	advances(128),
	textures(128),
	sprites(128),
	char_width(font.char_width),
	file(font.file),
	kill_textures(false)
{
	for(U8 i = 0; i < 128; ++i){
		this->advances[i] = font.advances[i];
		this->textures[i] = font.textures[i];
		this->sprites[i] = font.sprites[i]->Clone();
	}
}

Font::Font(string filename, float size, Color color) :
	color(color)
{
	file = system->LoadAssetFile(filename);
	FT_Error error;
	if(library == NULL){
		error = FT_Init_FreeType(&library);
		CROSS_FAIL(!error, "Error initializing FreeType library");
	}
	error = FT_New_Memory_Face(library, file->data, file->size, 0, &face);
	CROSS_FAIL(!error, "The font file could be opened and read, but it appears");
	SetSize(size);
}

Font::~Font(){
	for(Sprite* sprite : sprites) {
		delete sprite;
	}
	if(original) {
		KillTextures();
		delete file;
	} else {
		if(kill_textures) {
			KillTextures();
		}
	}
}

Color Font::GetColor() const{
	return color;
}

void Font::SetColor(Color color){
	this->color = color;
}

float Font::GetSize() const{
	return size;
}

void Font::SetSize(float size){
	this->size = size;
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, (FT_UInt)size);
	CROSS_FAIL(!error, "Error in set char size");
	if(IsFixedWidth()){
		FT_Error error = FT_Load_Char(face, 0x41, FT_LOAD_RENDER);
		CROSS_FAIL(!error, "Can't load glyph");
		char_width = (float)(face->glyph->advance.x >> 6);
	}
	Cache();
}

bool Font::IsFixedWidth() const{
	if(FT_IS_FIXED_WIDTH((FT_Long)face)){
		return true;
	}else{
		return false;
	}
}

float Font::GetCharWidth() const{
	CROSS_RETURN(IsFixedWidth(), 0.0f, "Char width can be obtained only for monospace fonts");
	return char_width;
}

Sprite* Font::GetChar(char c){
	return sprites[c - 29];
}

float Font::GetCharWidthAdvance(char c) const{
	return advances[c - 29];
}

void Font::Cache(){
	FT_Error error;
	if(kill_textures) {
		KillTextures();
	} else {
		kill_textures = true;
	}
	for(U8 i = 0; i < 128; i++){
		U32 glyphIndex = FT_Get_Char_Index(face, i + 29);
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
		if(error){
			system->LogIt("Can't load glyph %c", i);
		}
		FT_BitmapGlyph bitmapGlyhp;
		error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&bitmapGlyhp);
		if(error){
			system->LogIt("Can't abtain glyph %c", i);
		}
		advances[i] = (float)(face->glyph->advance.x >> 6);
		S32 bmpWidth = bitmapGlyhp->bitmap.width;
		S32 bmpHeight = bitmapGlyhp->bitmap.rows;
		float bearingX = (float)(face->glyph->metrics.horiBearingX >> 6);
		float bearingY = (float)(face->glyph->metrics.horiBearingY >> 6);
		Texture* texture = gfx2D->CreateTexture(	bitmapGlyhp->bitmap.buffer,
													1,
													bmpWidth,
													bmpHeight,
													Texture::Filter::LINEAR,
													Texture::Compression::NONE,
													Texture::TilingMode::CLAMP_TO_EDGE,
													false );
		texture->SetName("Char" + to_string(i) + "Texture");
		textures[i] = texture;
		Rect region(0, 0, (float)bmpWidth, (float)bmpHeight);
		Vector2D pivot(-bearingX, bmpHeight - bearingY);
		Sprite* sprite = new Sprite(texture, region, pivot);
		Sprite* delSprite = sprites[i];
		delete delSprite;
		sprites[i] = sprite;
	}
}

void Font::KillTextures(){
	for(Texture* texture : textures){
		delete texture;
	}
	textures.clear();
}

Font* Font::Clone() const{
	return new Font(*this);
}
