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
#include "TriangleScene.h"
#include "GraphicsGL.h"
#include "Input.h"
#include "Game.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Font.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Material.h"

void TriangleScene::Start(){
	CCScene::Start();
	
	triangle_shader = new Shader("Engine/Shaders/simple.vert", "Engine/Shaders/simple.frag");

	triangle_material = new Material(triangle_shader);
	triangle_material->SetDiffuseColor(Color::Red);

	VertexBuffer* vertexBuffer = new VertexBuffer();
	
	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 5.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = -5.f;  verticesData[1].y = -5.f;  verticesData[1].z = 0.0f;
	verticesData[2].x = 5.f;  verticesData[2].y = -5.f;  verticesData[2].z = 0.0f;

	vertexBuffer->PushData((CRByte*)&verticesData[0], 3 * sizeof(Vector3D));

	CRArray<unsigned int> indices = { 0, 1, 2 };

	triangle = new Mesh(vertexBuffer, indices, indices.size());
	triangle->SetMaterial(triangle_material);
}

void TriangleScene::Stop(){
	delete triangle_shader;
	delete triangle_material;
	delete triangle;
	CCScene::Stop();
}

void TriangleScene::Update(float sec){
	triangle->Draw();
	CCScene::Update(sec);
}