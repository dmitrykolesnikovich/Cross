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
#include "MemoryManager.h"
#include "Launcher.h"
#include <string>

using namespace cross;

const unsigned long CHECK_CODE = 0x12345678;

void* operator new(size_t size){
	return MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
}

void* operator new[](size_t size){
	return MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
}

void operator delete(void* p){
	MemoryManager::Instance()->Free(p);
}

void operator delete[](void* p){
	MemoryManager::Instance()->Free(p);
}

MemoryManager MemoryManager::instance;

MemoryManager* MemoryManager::Instance(){
	return &instance;
}

MemoryManager::MemoryManager():
	object_count(0)
{

}

void* MemoryManager::Alloc(unsigned int size, char* filename, unsigned int line){
	SanityCheck();

	alloc_objects[object_count].address = malloc(size + 4);
	alloc_objects[object_count].filename = filename;
	alloc_objects[object_count].line = line;
	alloc_objects[object_count].size = size;
	
	char* temp = (char*)alloc_objects[object_count].address;
	temp += size;
	memcpy(temp, &CHECK_CODE, 4);
	
	return alloc_objects[object_count].address;
}

void MemoryManager::Free(void* address){
	SanityCheck();
	if(address == NULL){
		launcher->LogIt("Null pointer deletion");
		return;
	}
	for(unsigned int i = 0; i < object_count; ++i){
		if(alloc_objects[i].address = address){
			free(address);
			if(i != object_count - 1){
				memcpy(alloc_objects + i, alloc_objects + object_count - 1, sizeof(MemoryObject));
			}
			object_count--;
			return;
		}
	}
	launcher->LogIt("Bad pointer deletion");
}

void MemoryManager::SanityCheck(){
	for(unsigned int i = 0; i < object_count; ++i){
		char* temp = (char*)alloc_objects[i].address;
		temp += alloc_objects[i].size;
		if(memcmp(temp, &CHECK_CODE, 4) != 0){
			launcher->LogIt("Memory corrupted at 0x%08X: %d bytes(%s: %d)\n",
				(unsigned long)alloc_objects[i].address,
				alloc_objects[i].size,
				alloc_objects[i].filename,
				alloc_objects[i].line);
		}
	}
}