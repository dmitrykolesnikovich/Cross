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
#include "Texture.h"

namespace cross{

/*	Class responsible 2D primitive drawing(ex. lines, dots),
	sprite drawing, texture loading and processing */
class Graphics2D{
public:
	Graphics2D();
	~Graphics2D();

	void Update(float sec);

	Texture* LoadTexture(const string& filename);
	Texture* LoadTexture(const string& filename, Texture::TilingMode tillingMode);
	Texture* LoadTexture(const string& filename, Texture::Filter filter);
	Texture* LoadTexture(const string& filename, Texture::TilingMode tillingMode, Texture::Filter filter, bool compressed);
	void SaveTexture(Texture* texture, const string& filename);
	Texture* CreateTexture(U32 channels, U32 width, U32 height, Texture::Filter filter);
	Texture* CreateTexture(	Byte* data,
							U32 channels,
							U32 width,
							U32 height,
							Texture::Filter filter,
							Texture::Compression compression,
							Texture::TilingMode tilingMode,
							bool generateMipmaps );
	void LoadSprites(Dictionary<string, Sprite*>& output, Texture* texture, string xml);

	Byte* LoadRawTextureData(const string& filename, int& width, int& height, int& channels);
	Texture* LoadRAWTexture(const string& filename, Texture::Filter filter);
	Texture* LoadPKMTexture(const string& filename, Texture::Filter filter);
	Texture* LoadKTXTexture(const string& filename, Texture::Filter filter);
};

}