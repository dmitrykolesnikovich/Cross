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
#include "System/MemoryManager.h"
#include "System/Exception.h"
#include "Math/All.h"
#include "Color.h"

#include <string>
#include <vector>
#include <list>
#include <map>
//TODO: remove form here
#if defined(__APPLE__) || defined(__APPLE_CC__)
#define IOS true
#endif

namespace cross{

typedef int8_t		S8;
typedef uint8_t		U8;
typedef int16_t		S16;
typedef uint16_t	U16;
typedef int32_t		S32;
typedef uint32_t	U32;
typedef int64_t		S64;
typedef uint64_t	U64;
typedef uint8_t		Byte;

template<typename T>
using Array = std::vector<T>;
template<typename T>
using List = std::list<T>;
template<typename K, typename V>
using Dictionary = std::map<K, V>;

class Game;
class Launcher;
class GraphicsGL;
class Graphics2D;
class Graphics3D;
class Input;
class Config;
class Sprite;
class Audio;
class Sound;
class Screen;
class File;
class Font;
class Camera;
class Camera2D;
class Material;
class Mesh;
class Model;
class Texture;
class Scene;
class Shader;
class MultiLightShader;
class Light;
class UI;
class Cubemap;

extern Game*		game;
extern Launcher*	launcher;
extern GraphicsGL*	gfxGL;
extern Graphics2D*	gfx2D;
extern Graphics3D*	gfx3D;
extern Audio*		audio;
extern Input*		input;
extern Config*		config;

}

cross::Game* CrossMain(cross::Launcher* launcher);

using namespace std;