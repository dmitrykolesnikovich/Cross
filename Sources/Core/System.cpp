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
#include "System.h"
#include "File.h"

#include <stdarg.h>
#include <fstream>

using namespace cross;

File* System::LoadFile(const string& filename){
	File* file = new File();
	file->name = filename;
	string filePath = AssetsPath() + filename;
	ifstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.seekg(0, fileStream.end);
		file->size = (size_t)fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		file->data = new Byte[file->size];
		memset(file->data, 0, file->size);
		fileStream.read((char*)file->data, file->size);
		fileStream.close();
		return file;
	} else {
		throw CrossException("Cannot open file %s", file->name.c_str());
	}
}

File* System::LoadDataFile(const string &filename){
    File* file = new File();
    file->name = filename;
    string filePath = DataPath() + filename;
    ifstream fileStream(filePath, istream::binary);
    if(fileStream.is_open()){
        fileStream.seekg(0, fileStream.end);
        file->size = (size_t)fileStream.tellg();
        fileStream.seekg(0, fileStream.beg);
        file->data = new Byte[file->size];
        memset(file->data, 0, file->size);
        fileStream.read((char*)file->data, file->size);
        fileStream.close();
        return file;
    } else {
        throw CrossException("Cannot open file %s", file->name.c_str());
    }
}

void System::SaveFile(File* file){/*
	string filePath = DataPath() + file->name;
	ofstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.write((char*)file->data, file->size);
		fileStream.close();
	}else{
		throw CrossException("Can't open file stream: %s", filePath.c_str());
	}*/
    string filePath = DataPath() + file->name;
    FILE* f = fopen(filePath.c_str(), "w");
    if(f){
        fwrite(file->data, 1, file->size, f);
        fclose(f);
    }else{
        throw CrossException("Can't open file stream: %s", filePath.c_str());
    }
}

void System::LogIt(const char* format, ...){
	va_list params;
	char buffer[4096];
	va_start(params, format);
	vsprintf(buffer, format, params);
	Log(buffer);
	va_end(params);
}

void System::LogIt(const Vector3D& vector){
	LogIt("X - %0.3f, Y - %0.3f, Z - %0.3f", vector.x, vector.y, vector.z);
}

S32 System::GetWindowWidth() const{
	return window_width;
}

S32 System::GetWindowHeight() const{
	return window_height;
}

System::Orientation System::GetDeviceOrientation() const{
    if(window_width > window_height){
        return Orientation::LANDSCAPE;
    }else{
        return Orientation::PORTRAIT;
    }
}

float System::GetAspectRatio() const{
	return GetWindowWidth() / (float)GetWindowHeight();
}

string System::PathFromFile(const string& filePath) const{
	const size_t last_slash_idx = filePath.rfind('/');
	if(std::string::npos != last_slash_idx){
		return filePath.substr(0, last_slash_idx);
	}else{
		throw CrossException("Wrong path format");
	}
}

string System::ExtensionFromFile(const string& file) const{
	return file.substr(file.find_last_of(".") +1);
}

string System::FileWithoutExtension(const string& file) const{
	size_t lastindex = file.find_last_of("."); 
	return file.substr(0, lastindex); 
}

void System::SetWindowSize(S32 width, S32 height){
	Orientation prevO = GetDeviceOrientation();
	window_width = width;
	window_height = height;
	WindowResized(width, height);
	if(prevO != GetDeviceOrientation()){
		OrientationChanged(GetDeviceOrientation());
	}
}
