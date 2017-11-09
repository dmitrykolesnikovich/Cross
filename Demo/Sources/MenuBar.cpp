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
#include "Demo.h"
#include "Screen.h"
#include "Texture.h"
#include "Utils/Debugger.h"

#include "Libs/ImGui/imgui.h"
#include "Libs/ImGui/imgui_freetype.h"
#include "Libs/ImGui/imgui_internal.h"

void MenuBar::Update(float sec) {
	if(show_style_editor) {
		ImGui::Begin("Style Editor", &show_style_editor, ImVec2(ImGui::GetWindowWidth() / 2.f, ImGui::GetWindowHeight() / 2.f));
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
	if(show_about) {
		ImGui::Begin("About", &show_about, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Cross++, v%s", cross::version);
		ImGui::Separator();
		ImGui::Text("By Maksim Lukyanov");
		ImGui::End();
	}
    if(show_stats) {
        ImGui::Begin("Stats", &show_stats);
        ImGui::Text("%.1f FPS(%.2f ms/frame)", Debugger::Instance()->GetFPS(), Debugger::Instance()->GetUpdateTime());
        ImGui::Text("CPU Time - %.2f ms", Debugger::Instance()->GetCPUTime());
        ImGui::End();
    }
}

void MenuBar::ShowMenu() {
	if(system->IsMobile()) {
		ImGui::PushFont(demo->big_font);
	}
	if(ImGui::BeginMainMenuBar())
	{
		bool mainScreen = false;
		if(game->GetCurrentScreen()->GetName() == "Main") {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
			mainScreen = true;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.52f, 0.24f, 0.0f));
		if(ImGui::Button("Back")) {
			game->SetScreen(game->GetStartScreen());
		}
		ImGui::PopStyleColor();
		if(mainScreen) {
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}
		
		ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SameLine(ImGui::GetWindowWidth() - cursor.x);
		if(ImGui::BeginMenu("Help")) {
            if(ImGui::MenuItem("Stats")) {
                show_stats = true;
            }
			if(ImGui::MenuItem("Style Editor")) {
				show_style_editor = true;
			}
			if(ImGui::MenuItem("About")) {
				show_about = true;
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

float MenuBar::GetHeight() const {
	return menu_height;
}