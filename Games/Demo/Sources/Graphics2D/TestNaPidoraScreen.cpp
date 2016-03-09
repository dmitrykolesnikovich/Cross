/*	Copyright � 2015 Vladas Zakrevskis

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
#include "TestNaPidoraScreen.h"
#include "Graphics2D.h"
#include "Game.h"
#include "AnimationScreen.h"
#include "MainScreen.h"
#include "Audio.h"
#include "Sprite.h"
#include "Launcher.h"

TestNaPidoraScreen::TestNaPidoraScreen()
{
	
	//fooButton3 = new Button(Vector2D(250.f, 250.f));
	//fooButton3 = new Button(Rect(50, 50, 100, 500));
	Sprite* button_sprite = gfx2D->LoadImage("DefaultButton.png");
	fooButton3 = new Button(button_sprite);
	//fooButton3 = new Button(Vector2D(150, 150), "Pidr");
	fooButton3->SetLocation(Vector2D(340.f, 250.f));
	fooButton3->SetText("SADAS");


	//fooButton3->SetLocation(Vector2D(250.f, 250.f));

	//fooButton3->SetText("long text button");
}

void TestNaPidoraScreen::Update(float sec)
{
	fooButton3->Update();

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)){
		game->SetScreen(new MainScreen());
	}
}

TestNaPidoraScreen::~TestNaPidoraScreen()
{
	
	delete fooButton3;
}

void TestNaPidoraScreen::OnKeyPressed(Key key)
{
	//if (key == ESCAPE)
		game->SetScreen(new MainScreen());
}
