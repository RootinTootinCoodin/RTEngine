#include "UIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"



UIScene::UIScene(Application* App): UIElement(App)
{
	name = "Scene";
}


UIScene::~UIScene()
{
}

void UIScene::Draw()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
	if (ImGui::Begin(name.c_str(), &show_window, window_flags))
	{
		ImVec2 size = ImGui::GetWindowSize();
		if (size.x != previous_size.x || size.y != previous_size.y)
		{
			previous_size.x = size.x;
			previous_size.y = size.y;
			App->renderer3D->OnResize(previous_size.x, previous_size.y);
		}
		ImGui::SetCursorPos((ImGui::GetWindowSize() - ImVec2(App->window->width, App->window->height)) * 0.5f);
		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, ImVec2(App->window->width, App->window->height), { 1,1 }, { 0,0 });
	}
	ImGui::End();


	
}
