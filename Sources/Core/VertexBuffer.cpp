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
#include "VertexBuffer.h"

using namespace cross;

bool VertexBuffer::HasTextureCoordinates() const{
	return uv_enabled;
}

bool VertexBuffer::HasNormals() const{
	return normals_enabled;
}

bool VertexBuffer::HasTangents() const{
	return tangents_enabled;
}

bool VertexBuffer::HasBitangents() const{
	return bitangents_enabled;
}

U32 VertexBuffer::GetPossitionsOffset() const{
	return 0;
}

U32 VertexBuffer::GetTextureCoordinatesOffset() const{
	return GetPossitionsOffset() + (uv_enabled ? 3 : 0);
}

U32 VertexBuffer::GetNormalsOffset() const{
	return GetTextureCoordinatesOffset() + (normals_enabled ? 2 : 0);
}

U32 VertexBuffer::GetTangentsOffset() const{
	return GetNormalsOffset() + (tangents_enabled ? 3 : 0);
}

U32 VertexBuffer::GetBitangentsOffset() const{
	return GetTangentsOffset() + (bitangents_enabled ? 3 : 0);
}

U32 VertexBuffer::VertexSize() const{
	return (GetBitangentsOffset() + 3) * sizeof(float);
}

void VertexBuffer::UVEnabled(bool enabled){
	uv_enabled = enabled;
}

void VertexBuffer::NarmalsEnabled(bool enabled){
	normals_enabled = enabled;
}

void VertexBuffer::TangentsEnabled(bool enabled){
	tangents_enabled = enabled;
}

void VertexBuffer::BitangentsEnabled(bool enabled){
	bitangents_enabled = enabled;
}

void VertexBuffer::PushData(const Byte* bytes, U32 size){
	data.insert(data.end(), bytes, bytes+size);
}

Byte* VertexBuffer::GetData(){
	return data.data();
}

U32 VertexBuffer::GetDataSize() const{
    return (U32)data.size();
}

void VertexBuffer::Free(){
	data.clear();
}

VertexBuffer* VertexBuffer::Clone() const{
	return new VertexBuffer(*this);
}
