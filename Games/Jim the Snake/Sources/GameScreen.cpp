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
	
#include "GameScreen.h"

GameScreen::GameScreen(Game* game):Screen(game){ }

void GameScreen::Start(){
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50, game->GetHeight() + 50);
}

void GameScreen::Update(float sec){
	graphics->Clear(1,0,0);
	graphics->DrawImage(game->GetWidth() /2, game->GetHeight()/2, background);
	graphics->DrawLine(PointX(50, 50), PointX(game->GetWidth() / 2, game->GetHeight() / 2), 0, 0, 1);
	RectX rect(100,100, 200, 100);
	graphics->DrawRect(rect, 1, 0, 0);
	graphics->DrawPixel(PointX(200, 200), 0, 1, 0);
	graphics->DrawCircle(PointX(500, 500), 50, 0, 1, 0);
}
