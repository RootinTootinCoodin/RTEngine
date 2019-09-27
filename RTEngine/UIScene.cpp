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
		ImGui::SetCursorPos((ImGui::GetWindowSize() - ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT)) * 0.5f);
		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	}
	ImGui::End();


	
}
