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
#include "PrimitivesScreen.h"
#include "Launcher.h"

const GLuint NumVertices = 1;

GLfloat vertices[2] = { 0.0f, 0.0f };

void PrimitivesScreen::Start(){
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "primitives.vert" },
		{ GL_FRAGMENT_SHADER, "primitives.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = gfx3D->LoadShaders(shaders);
	glUseProgram(program);

	//glBindAttribLocation(program, 0, "vPosition");


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
}

void PrimitivesScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	//glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, NumVertices);
	glFlush();
}