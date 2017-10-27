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
#include "MenuBar.h"
#include "System.h"
#include "Game.h"
#include "Texture.h"

#include "Libs/ImGui/imgui.h"

void MenuBar::Start() {
	Screen::Start();

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig fontConfig;
	fontConfig.SizePixels = io.Fonts->Fonts[0]->FontSize * 2.f;
	font_big = io.Fonts->AddFontDefault(&fontConfig);

	unsigned char* pixels;
	int width, height;
#ifdef USE_FREETYPE
	ImGuiFreeType::BuildFontAtlas(io.Fonts);
#endif
	// Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	big_font_texture = new Texture();
	system->LogIt("Creating second font texture(%dx%d)", width, height);
	big_font_texture->Create(pixels, 4, width, height,
		Texture::Filter::LINEAR,
		Texture::Compression::NONE,
		Texture::TilingMode::CLAMP_TO_EDGE, false);
	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)big_font_texture->GetID();
}

void MenuBar::Stop() {
	delete big_font_texture;
	Screen::Stop();
}

void MenuBar::Update(float sec) {
	Screen::Update(sec);
	if(show_style_editor){
		ImGui::Begin("Style Editor", &show_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
}

void MenuBar::ShowMenu() {
	if(system->IsMobile()) {
		ImGui::PushFont(font_big);
	}
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("Menu"))
		{
			if(ImGui::MenuItem("Style Editor")) {
				show_style_editor = true;
			}
			if(ImGui::MenuItem("Back")) {
				game->SetScreen(game->GetStartScreen());
			}
			ImGui::EndMenu();
		}
        ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 size = ImGui::GetItemRectSize();
		menu_height = size.y +style.FramePadding.y * 2.f - style.ItemSpacing.y * 2.f;// +style.ItemSpacing.y;
		ImGui::EndMainMenuBar();
	}
	if(system->IsMobile()) {
		ImGui::PopFont();
	}
}