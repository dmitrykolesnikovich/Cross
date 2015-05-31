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
#include "Apple.h"
#include "Spider.h"
#include "Snake.h"

GameScreen::GameScreen(Game* game):Screen(game){ }

void GameScreen::Start(){
	state = GameState::ONREADY;
	centerW = game->GetWidth() / 2;
	centerH = game->GetHeight() / 2;
	onready_time = 4.3f;
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50.f, game->GetHeight() + 50.f);
	ready_img = graphics->LoadImage("Game/ReadyTapLabel.png");
	score_img = graphics->LoadImage("Game/ScoreLabel.png");
	control_facepointer = graphics->LoadImage("Game/FacePointer.png");
	control_base = graphics->LoadImage("Game/ControlBase.png");
	pause_img = graphics->LoadImage("Game/PauseLabel.png");
	gameover_img = graphics->LoadImage("Game/GameOverLabel.png");
	Apple::Init(game);
	Spider::Init(game);
	snake = new Snake(game);
	spider = new Spider();
	music_enable = saver->LoadBool(KEY_MUSIC);
	sound_enable = saver->LoadBool(KEY_SOUND);
	if(music_enable){
		music = new Audio("Game/GameMusic.mp3", true, true);
		game_over = new Audio("Game/GameOver.wav", false, true);
		music->Play();
	}else{
		music = NULL;
		game_over = NULL;
	}
	if(sound_enable){
		punch = new Audio("Game/Punch.wav", false, false);
	}else{
		punch = NULL;
	}
	score_texter = ((SnakyGame*)game)->score_texter;
	snake = NULL;

	Image* resumeup = graphics->LoadImage("Game/ResumeUp.png");
	Image* resumedown = graphics->LoadImage("Game/ResumeDown.png");
	resume_btn = new Button(game, resumeup, resumedown);
	resume_btn->SetLocation(PointX(450, centerH - 40));
	resume_btn->RegisterCallback(bind(&GameScreen::OnResumeClick, this));
	Image* menuup = graphics->LoadImage("Game/MenuUp.png");
	Image* menudown = graphics->LoadImage("Game/MenuDown.png");
	menu_btn = new Button(game, menuup, menudown);
	menu_btn->SetLocation(PointX(450, centerH + 180));
	menu_btn->RegisterCallback(bind(&GameScreen::OnMenuClick, this));
	Image* restartup = graphics->LoadImage("Game/RestartUp.png");
	Image* restartdown = graphics->LoadImage("Game/RestartDown.png");
	restart_btn = new Button(game, restartup, restartdown);
	restart_btn->SetLocation(PointX(450, centerH - 40));
	restart_btn->RegisterCallback(bind(&GameScreen::OnRestartClick, this)); 

	Restart();
}

void GameScreen::Restart(){
	game_over->Stop();
	music->Play();
	for(Apple* apple : apples){
		delete apple;
	}
	apples.clear();
	state = GameState::ONREADY;
	score = 0;
	score_texter->SetScaleFactor(game->GetScaleFactor());
	graphics->ScaleImage(score_img, game->GetScaleFactor());
	delete snake;
	snake = new Snake(game);
	spider->Die();
}

void GameScreen::Update(float sec){
	graphics->Clear(0.25, 0.25, 0);
	graphics->DrawImage(game->GetWidth() /2, game->GetHeight()/2, background);
	DrawApples();
	spider->Draw();
	snake->DrawFace(sec);
	snake->DrawBody(sec);
	snake->EatableNear(spider);
	if(SpiderOnCollision()){
		bool dir = rand() % 2;
		if(dir){
			spider->Rotate(180.f);
		}else{
			spider->Rotate(-180.f);
		}
	}
	switch (state)
	{
	case GameState::RUNNING:{
			static bool down = false;
			spider->Update(sec, apples);
			CalcApples(sec);
			DrawScore();
			CalcInput(sec);
			snake->CalcHead(sec);
			if(snake->OnBorder() || snake->OnBiteYouself()){
				graphics->ScaleImage(score_img, game->GetScaleFactor() * 1.2f);
				score_texter->SetScaleFactor(game->GetScaleFactor() * 1.2f);
				if(score > saver->LoadInt(KEY_SCORE)){
					saver->SaveInt(KEY_SCORE, score);
				}
				state = GameState::DEAD0;
				time_dead01 = 1.f;
				music->Stop();
				punch->Play();
			}
			if(input->HaveKey() && input->GetKey() == Key::PAUSE){
				down = true;
			}
			if(down && !input->HaveKey()){
				state = GameState::PAUSED;
				music->Pause();
				down = false;
			}
		}break;
	case GameState::ONREADY:
		graphics->DrawImage(centerW, centerW, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			state = GameState::RUNNING;
			spider->Start();
		}
		break;
	case GameState::PAUSED:{
			static bool down = false;
			DrawScore();
			graphics->DrawImage(450, centerH - 250, pause_img);
			if(input->HaveKey() && input->GetKey() == Key::PAUSE){
				down = true;
			}
			if(down && !input->HaveKey()){
				state = GameState::RUNNING;
				music->Play();
				down = false;
			}
			resume_btn->Update();
			menu_btn->Update();
		}break;
	case GameState::DEAD0:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST0();
		time_dead01 -= sec;
		if(time_dead01 < 0) {
			state = GameState::DEAD1;
			time_dead02 = 1.5f;
			game_over->Play();
		}
		break;
	case GameState::DEAD1:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST1(sec);
		time_dead02 -= sec;
		if(time_dead02 < 0) {
			state = GameState::DEAD2;
		}
		break;
	case GameState::DEAD2:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST1(sec);
		graphics->DrawImage(450, centerH - 340, gameover_img);
		graphics->DrawImage(300, centerH - 210, score_img);
		score_texter->DrawText(520, centerH - 255, to_string(score));
		restart_btn->Update();
		menu_btn->Update();
		break;
	default:
		break;
	}
}

void GameScreen::CalcApples(float sec){
	if(next_apple <= 0) {
		next_apple = (float)(rand()%15);
		SetApple();
	} else {
		next_apple -= sec;
	}
	if(apples.size() == 0)
		SetApple();
	auto it = apples.begin();
	while(it != apples.end()){
		if((*it)->GetLifeTime() > 0){
			snake->EatableNear(*it);
			(*it)->Update(sec);
			it++;
		} else {
			delete *it;
			*it = NULL;
			it = apples.erase(it);
		}
	}
}

void GameScreen::SetApple(){
	Apple* apple = new Apple();;
	float top = score_img->GetHeight() + 50;
	float bottom = game->GetHeight() - 4 * apple->GetRadius();
	float left = 4 * apple->GetRadius();
	float right = game->GetWidth() - 4 * apple->GetRadius();
	PointX apple_pos;
	bool onSnake = true;
	while(onSnake) {
		onSnake = false;
		int randX = (int)(right - left);
		int randY = (int)(bottom - top);
		apple_pos.x = (float)(rand() % randX) + left;
		apple_pos.y = (float)(rand() % randY) + top;
		onSnake = snake->OnCollision(apple_pos, apple->GetRadius());
	}
	apple->SetPosition(apple_pos);
	apples.push_back(apple);
}

void GameScreen::DrawApples(){
	for(Apple* apple : apples) {
		apple->Draw();
	}
}

void GameScreen::DrawScore(){
	static const PointX pos(game->GetWidth() / 2 + 120, 50);
	graphics->DrawImage(pos, score_img);
	float offset = score_texter->GetWidth() / 2;
	if(score > 9)
		offset = score_texter->GetWidth();
	if(score > 99)
		offset = score_texter->GetWidth() * 2;
	score_texter->DrawText(game->GetWidth() / 2 + 380 - offset, 20, to_string(score));
}

void GameScreen::AddScore(int gain){
	score += gain;
}

void GameScreen::StartSpider(){
	spider->Start();
}

void GameScreen::CalcInput(float sec){
	if(input->HaveInput()){
		if(control_pos.x == 0 && control_pos.y == 0){
			control_pos.x = input->GetInput().x;
			control_pos.y = input->GetInput().y;
			return;
		}
		float fingerAngle = Angle(control_pos, input->GetInput());
		//determine in witch direction we need to move
		float clockwise;
		if( fingerAngle > snake->face_angle )
			clockwise = 360 + snake->face_angle - fingerAngle;
		else 
			clockwise = snake->face_angle - fingerAngle;
		float counterclockwise = 360 - clockwise;
		//rotate snake head
		if(clockwise < snake->GetSpeedW() * sec || counterclockwise < snake->GetSpeedW() * sec) {
			snake->face_angle = fingerAngle;
		} else {
			if(counterclockwise < clockwise) 
				snake->face_angle += snake->GetSpeedW() * sec;
			else
				snake->face_angle -= snake->GetSpeedW() * sec;
			if(snake->face_angle >= 180.f)
				snake->face_angle -= 360.f;
			if(snake->face_angle <= -180.f)
				snake->face_angle += 360.f;
		}
		//draw control elements
		graphics->Rotate(control_facepointer, snake->face_angle);
		graphics->DrawImage(control_pos, control_base);
		graphics->DrawImage(control_pos.x, control_pos.y - 2, control_facepointer);
	}else{
		control_pos.x = 0;
		control_pos.y = 0;
	}
}

bool GameScreen::SpiderOnCollision(){
	PointX spiderAhead(spider->GetPosition().x, spider->GetPosition().y);
	spiderAhead.y += -spider->GetSpeedV() * sin(spider->GetAngle() / 180.0 * PI) * 0.1;
	spiderAhead.x += spider->GetSpeedV() * cos(spider->GetAngle() / 180.0 * PI) * 0.1;
	return snake->OnCollision(spiderAhead, spider->GetRadius());
}

GameScreen::~GameScreen(){
	delete snake;
	delete music;
	for(Apple* apple : apples){
		delete apple;
	}
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(ready_img);
	Spider::Release();
}

void GameScreen::OnMenuClick(){
	game->SetScreen(new MenuScreen(game));
}

void GameScreen::OnResumeClick(){
	state = GameState::RUNNING;
}

void GameScreen::OnRestartClick(){
	Restart();
}
