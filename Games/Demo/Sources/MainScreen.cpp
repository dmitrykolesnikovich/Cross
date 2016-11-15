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
#include "MainScreen.h"
#include "Graphics2D.h"
#include "Launcher.h"
#include "Sprite.h"
#include "AudioScreen.h"
#include "Camera2D.h"
#include "Graphics2D/PrimitivesScreen.h"
#include "Graphics2D/AnimationScreen.h"
#include "Graphics2D/SpritesScreen.h"
#include "Graphics2D/TextScreen.h"
#include "Graphics3D/Simple/TriangleScene.h"
#include "Graphics3D/Simple/SolidModelScene.h"
#include "Graphics3D/Simple/TexturedModelScene.h"
#include "Graphics3D/Simple/ComplexModelScene.h"
#include "Graphics3D/Light/SpecularScene.h"
#include "Graphics3D/Light/DiffuseScene.h"
#include "Graphics3D/Light/SpecularMapScene.h"
#include "Graphics3D/Light/DirectionalLightScene.h"
#include "Graphics3D/Light/PointLightScene.h"
#include "Graphics3D/Light/SpotLightScene.h"
#include "Graphics3D/Light/MultiLightScene.h"
#include "Graphics3D/Light/CamaroScene.h"
#include "Graphics3D/Advanced/DepthScene.h"
#include "Graphics3D/Advanced/StencilScene.h"
#include "Demo.h"

void MainScreen::Start(){
	ScrollScreen::Start();
	SetBackground(Color::Black);
	next_screen = NULL;
	button_sprite = demo->GetCommonSprite("DefaultButton.png");
	button_sprite->SetScale(1.35f);

	key_released_delegate = MakeDelegate(this, &MainScreen::KeyReleasedHandle);
	input->KeyPressed += key_released_delegate;

	window_resized_delegate = MakeDelegate(this, &MainScreen::WindowResizedHandle);
	game->WindowResized += window_resized_delegate;
	//main menu
	main_menu = new Menu(true);
	current_menu = main_menu;
	Button* graphics2Dbtn = new Button("Graphics 2D");
	Button* graphics3Dbtn = new Button("Graphics 3D");
	Button* audioBtn = new Button("Audio");
	graphics2Dbtn->SetImages(button_sprite->Clone(), nullptr);
	graphics3Dbtn->SetImages(button_sprite->Clone(), nullptr);
	audioBtn->SetImages(button_sprite->Clone(), nullptr);
	graphics2Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics2DClick);
	graphics3Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics3DClick);
	audioBtn->Clicked += MakeDelegate(this, &MainScreen::OnAudioClick);
	main_menu->AddButton(graphics2Dbtn);
	main_menu->AddButton(graphics3Dbtn);
	main_menu->AddButton(audioBtn);
	//graphics 2D menu
	graphics2D_menu = new Menu(false);
	Button* primitivesBtn = new Button("Primitives");
	Button* spritesBtn = new Button("Sprites");
	Button* textBtn = new Button("Text Drawing");
	Button* animationBtn = new Button("Animation");
	primitivesBtn->SetImages(button_sprite->Clone());
	spritesBtn->SetImages(button_sprite->Clone());
	textBtn->SetImages(button_sprite->Clone());
	animationBtn->SetImages(button_sprite->Clone());
	primitivesBtn->Clicked += MakeDelegate(this, &MainScreen::OnPrimitivesClick);
	spritesBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpritesClick);
	animationBtn->Clicked += MakeDelegate(this, &MainScreen::OnAnimationClick);
	textBtn->Clicked += MakeDelegate(this, &MainScreen::OnTextClick);
	graphics2D_menu->AddButton(primitivesBtn);
	graphics2D_menu->AddButton(spritesBtn);
	graphics2D_menu->AddButton(textBtn);
	graphics2D_menu->AddButton(animationBtn);
	//graphics 3D menu
	graphics3D_menu = new Menu(true);
	Button* simpleBtn				= new Button("Simple");
	Button* lightBtn				= new Button("Light");
	Button* advancedBtn				= new Button("Advanced");
	simpleBtn->SetImages(button_sprite->Clone());
	lightBtn->SetImages(button_sprite->Clone());
	advancedBtn->SetImages(button_sprite->Clone());
	simpleBtn->Clicked += MakeDelegate(this, &MainScreen::OnSimpleClick);
	lightBtn->Clicked += MakeDelegate(this, &MainScreen::OnLightClick);
	advancedBtn->Clicked += MakeDelegate(this, &MainScreen::OnAdvancedClick);
	graphics3D_menu->AddButton(simpleBtn);
	graphics3D_menu->AddButton(lightBtn);
	graphics3D_menu->AddButton(advancedBtn);

	graphics3D_simple = new Menu(false);
	Button* triangleBtn				= new Button("Triangle");
	Button* solidModelBtn			= new Button("Solid Model");
	Button* texturedModelBtn		= new Button("Textured Model");
	Button* complexBtn				= new Button("Complex Model");
	triangleBtn->SetImages(button_sprite->Clone());
	solidModelBtn->SetImages(button_sprite->Clone());
	texturedModelBtn->SetImages(button_sprite->Clone());
	complexBtn->SetImages(button_sprite->Clone());
	triangleBtn->Clicked += MakeDelegate(this, &MainScreen::OnTriangleClick);
	solidModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnSolidModelClick);
	texturedModelBtn->Clicked += MakeDelegate(this, &MainScreen::OnTexturedModelClick);
	complexBtn->Clicked += MakeDelegate(this, &MainScreen::OnComplexModelClick);
	graphics3D_simple->AddButton(triangleBtn);
	graphics3D_simple->AddButton(solidModelBtn);
	graphics3D_simple->AddButton(texturedModelBtn);
	graphics3D_simple->AddButton(complexBtn);

	graphics3D_light = new Menu(false);
	Button* specularBtn 			= new Button("Specular");
	Button* diffuseMap				= new Button("Diffuse");
	Button* specularMapBtn			= new Button("Specular Map");
	Button* directionalLigthBtn		= new Button("Directional Light");
	Button* pointLightBtn			= new Button("Point Light");
	Button* spotLightBtn			= new Button("Spot Light");
	Button* multiLightBtn			= new Button("Multi-Ligth");
	Button* camaroBtn				= new Button("Camaro");
	specularBtn->SetImages(button_sprite->Clone());
	diffuseMap->SetImages(button_sprite->Clone());
	specularMapBtn->SetImages(button_sprite->Clone());
	directionalLigthBtn->SetImages(button_sprite->Clone());
	pointLightBtn->SetImages(button_sprite->Clone());
	spotLightBtn->SetImages(button_sprite->Clone());
	multiLightBtn->SetImages(button_sprite->Clone());
	camaroBtn->SetImages(button_sprite->Clone());
	specularBtn->Clicked += MakeDelegate(this, &MainScreen::OnMaterialClick);
	diffuseMap->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseMapClick);
	specularMapBtn->Clicked += MakeDelegate(this, &MainScreen::OnDiffuseSpecularMapClick);
	directionalLigthBtn->Clicked += MakeDelegate(this, &MainScreen::OnDirectionalLight);
	pointLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnPointLightClick);
	spotLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpotLightClick);
	multiLightBtn->Clicked += MakeDelegate(this, &MainScreen::OnMultiLightClick);
	camaroBtn->Clicked += MakeDelegate(this, &MainScreen::OnCamaroClick);
	graphics3D_light->AddButton(specularBtn);
	graphics3D_light->AddButton(diffuseMap);
	graphics3D_light->AddButton(specularMapBtn);
	graphics3D_light->AddButton(directionalLigthBtn);
	graphics3D_light->AddButton(pointLightBtn);
	graphics3D_light->AddButton(spotLightBtn);
	graphics3D_light->AddButton(multiLightBtn);
	graphics3D_light->AddButton(camaroBtn);

	graphics3D_advanced = new Menu(true);
	Button* depthTestBtn			= new Button("Depth Test");
	Button* stencilTestBtn			= new Button("Stencil Test");
	depthTestBtn->SetImages(button_sprite->Clone());
	stencilTestBtn->SetImages(button_sprite->Clone());
	depthTestBtn->Clicked += MakeDelegate(this, &MainScreen::OnDepthTestClick);
	stencilTestBtn->Clicked += MakeDelegate(this, &MainScreen::OnStencilTestClick);
	graphics3D_advanced->AddButton(depthTestBtn);
	graphics3D_advanced->AddButton(stencilTestBtn);

	graphics2D_menu->Active(false);
	graphics3D_menu->Active(false);
	graphics3D_simple->Active(false);
	graphics3D_light->Active(false);
	graphics3D_advanced->Active(false);

	delete button_sprite;
}

void MainScreen::Stop(){
	ScrollScreen::Stop();
	delete main_menu;
	delete graphics2D_menu;
	delete graphics3D_menu;
	delete graphics3D_simple;
	delete graphics3D_light;
	delete graphics3D_advanced;
	input->KeyPressed -= key_released_delegate;
	game->WindowResized -= window_resized_delegate;
}

void MainScreen::Update(float sec){
	current_menu->Update(sec);

	if(next_screen){
		gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
		game->SetScreen(next_screen);
		return;
	}
}

void MainScreen::AdjustScreenHeight(Menu* menu){
	SetHeight(menu->GetHeight());
	Vector2D camPos(0.f, 0.f);
	camPos.y = GetHeight() - gfx2D->GetCamera()->GetViewHeight();
	gfx2D->GetCamera()->SetPosition(camPos);
}

void MainScreen::KeyReleasedHandle(Key key){
	if(key == Key::ESCAPE || key == Key::BACK){
		if(current_menu == main_menu){
			launcher->PromtToExit();
		}else{
			current_menu->Active(false);
			main_menu->Active(true);
			current_menu = main_menu;
			float scrWidth = 1600.f;
			float scrHeight = scrWidth / launcher->GetAspectRatio();
			SetWidth(scrWidth);
			SetHeight(scrHeight);
			gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
		}
	}
}

void MainScreen::WindowResizedHandle(S32 width, S32 height){
	ResetScreenSize();
}

void MainScreen::OnGraphics2DClick(){
	AdjustScreenHeight(graphics2D_menu);
	current_menu->Active(false);
	graphics2D_menu->Active(true);
	current_menu = graphics2D_menu;
}

void MainScreen::OnGraphics3DClick(){
	AdjustScreenHeight(graphics3D_menu);
	current_menu->Active(false);
	graphics3D_menu->Active(true);
	current_menu = graphics3D_menu;
}

void MainScreen::OnAnimationClick(){
	next_screen = new AnimationScreen();
}

void MainScreen::OnSimpleClick(){
	AdjustScreenHeight(graphics3D_simple);
	current_menu->Active(false);
	graphics3D_simple->Active(true);
	current_menu = graphics3D_simple;
}

void MainScreen::OnLightClick(){
	AdjustScreenHeight(graphics3D_light);
	current_menu->Active(false);
	graphics3D_light->Active(true);
	current_menu = graphics3D_light;
}

void MainScreen::OnAdvancedClick(){
	AdjustScreenHeight(graphics3D_advanced);
	current_menu->Active(false);
	graphics3D_advanced->Active(true);
	current_menu = graphics3D_advanced;
}

void MainScreen::OnPrimitivesClick(){
	next_screen = new PrimitivesScreen();
}

void MainScreen::OnSpritesClick(){
	next_screen = new SpritesScreen();
}

void MainScreen::OnTextClick(){
	next_screen = new TextScreen();
}

void MainScreen::OnTriangleClick(){
	next_screen = new TriangleScene();
}

void MainScreen::OnSolidModelClick(){
	next_screen = new SolidModelScene();
}

void MainScreen::OnTexturedModelClick(){
	next_screen = new TexturedModelScene();
}

void MainScreen::OnComplexModelClick(){
	next_screen = new ComplexModelScene();
}

void MainScreen::OnAudioClick() {
	next_screen = new AudioScreen();
}

void MainScreen::OnMaterialClick(){
	next_screen = new SpecularScene();
}

void MainScreen::OnDiffuseMapClick(){
	next_screen = new DiffuseScene();
}

void MainScreen::OnDiffuseSpecularMapClick(){
	next_screen = new SpecularMapScene();
}

void MainScreen::OnDirectionalLight(){
	next_screen = new DirectionalLightScene();
}

void MainScreen::OnPointLightClick(){
	next_screen = new PointLightScene();
}

void MainScreen::OnSpotLightClick(){
	next_screen = new SpotLightScene();
}

void MainScreen::OnMultiLightClick(){
	next_screen = new MultiLightScene();
}

void MainScreen::OnCamaroClick(){
	next_screen = new CamaroScene();
}

void MainScreen::OnDepthTestClick(){
	next_screen = new DepthScene();
}

void MainScreen::OnStencilTestClick(){
	next_screen = new StencilScene();
}