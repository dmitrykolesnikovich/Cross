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
#include "TriangleScreen.h"
#include "GraphicsGL.h"

struct Vertex
{
	Vector3D pos;
};

void TriangleScreen::Start(){
	shader = new TriangleShaders();

	Vertex verticesData[3];

	verticesData[0].pos.x = 0.0f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
	verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;

	//buffer object
	SAFE(glGenBuffers(1, &vboId));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
	SAFE(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void TriangleScreen::Stop(){

}

void TriangleScreen::Update(float sec){
	SAFE(glUseProgram(shader->program));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
	if(shader->aPosition != -1)
	{
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if(shader->uMVP != -1)
	{
		Matrix mat = Matrix::CreateIdentity();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, (GLfloat*)&mat.m));
	}
	SAFE(glDrawArrays(GL_TRIANGLES, 0, 3));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}