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

#include "Screen.h"

using namespace cross;

#define PNUM 500

class GameScreen : public Screen{
public:
	GameScreen(Game* game);
	void Start();
	void Update(float sec);

	void updateplayer(int pID);
	void drawplayer(int pID);
	void createplayer(int x,int y,int hp);
	void ComposeFrame();
	void nextturn();
	void restartgame();

	void DrawGrid( int x,int y);
	int PointDistance( int x1, int y1, int x2, int y2 );
	int random( int number);
	int thru(int nn1, int nn2);
private:
		Image* x_img;
	int pNum;
	int deathtimer;
	int turn;
	int turntimer;
	int spawntimer;
	bool pause;
	bool shiftpressed;
	bool enterpressed;
	int playernumber;
	int backr;
	int backg;
	int backb;
	int dangertimer;
	int realbackr;	int realbackg;	int realbackb;
	int playerhp[PNUM];
	int playerhpmax[PNUM];
	int playeratkn[PNUM];
	int playerx[PNUM];
	int playery[PNUM];
	int playerrange[PNUM];
	int playeratkspd[PNUM];
	int aipause[PNUM];
	int aiwarmup[PNUM];
	int aidisttoPN[PNUM];
	bool aggro[PNUM];
	int lastdir[PNUM];
	int kbdl[PNUM];	int kbdr[PNUM];	int kbdu[PNUM];	int kbdd[PNUM];
};