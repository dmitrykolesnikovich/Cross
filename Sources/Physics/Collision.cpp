/*	Copyright � 2015 Lukyanau Maksim

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
#include "Physics/Collision.h"
#include "RigidBody.h"

using namespace cross;

Collision::Contact::Contact() :
	Collision::Contact(Vector3D::Zero, 0)
{ }

Collision::Contact::Contact(Vector3D n, float d) :
	normal(n),
	depth(d),
	restitution(1.f)
{
	move[0] = Vector3D::Zero;
	move[1] = Vector3D::Zero;
}

Collision::Collision(RigidBody* first) :
	first(first),
	second(NULL),
	contact()
{ }

Collision::Collision(RigidBody* first, RigidBody* second) :
	first(first),
	second(second),
	contact()
{ }