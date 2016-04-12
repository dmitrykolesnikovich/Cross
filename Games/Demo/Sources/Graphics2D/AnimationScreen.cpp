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
#include "Cross.h"
#include "Game.h"
#include "Graphics2D.h"
#include "Utils/Animation.h"
#include "AnimationScreen.h"
#include "MainScreen.h"
#include "Audio.h"
#include "Sprite.h"

void AnimationScreen::Start(){
	bcg_scale = 2;
	run_time = 2;
	thinking_time = 0;
	head_angle = 0;
	deltaY = 0;
	turn_left = true;
	gfx2D->SetClearColor(Color(0.25f, 0.25f, 0.25f));

	texture = gfx2D->LoadTexture("gfx2D/Spider.png");
	CRDictionary<string, Sprite*> sprites;
	gfx2D->LoadSprites(sprites, texture, "gfx2D/Spider.xml");


	spider_body = sprites["Body.png"];
	spider_head = sprites["Head.png"];
	Sprite* images[8];
	images[0] = sprites["00.png"];
	images[1] = sprites["01.png"];
	images[2] = sprites["02.png"];
	images[3] = sprites["03.png"];
	images[4] = sprites["04.png"];
	images[5] = sprites["05.png"];
	images[6] = sprites["06.png"];
	images[7] = sprites["07.png"];
	spider_run_anim = new Animation(0.08f, images, 8, true);
	spider_run_snd = new Audio("SpiderRun.wav", true, false);
}

void AnimationScreen::Stop(){
	delete texture;
	delete spider_run_anim;
	delete spider_run_snd;
	delete spider_body;
	delete spider_head;
}

void AnimationScreen::Update(float sec){
	spider_run_anim->Update(sec);

	if(run_time >= 0) {
		run_time -= sec;
		gfx2D->DrawSprite(Vector2D(GetWidth() / 2, GetHeight() / 2), spider_run_anim->GetImage());
		if(run_time < 0){
			thinking_time = 1.3f;
			spider_run_snd->Stop();
		}
	}

	if(thinking_time >= 0){
		thinking_time -= sec;
		if(turn_left) {
			head_angle += sec * 150.f;
			if(head_angle > 45)
				turn_left = false;
		} else {
			head_angle -= sec * 150.f;
			if(head_angle < -45)
				turn_left = true;
		}
		spider_head->SetRotate(head_angle);
		gfx2D->DrawSprite(Vector2D(GetWidth() / 2, GetHeight() / 2), spider_body);
		gfx2D->DrawSprite(Vector2D(GetWidth() / 2, GetHeight() / 2 - 65), spider_head);
		if(thinking_time < 0){
			run_time = 3.5f;
			head_angle = 0;
			spider_run_snd->Play();
		}
	}

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}
