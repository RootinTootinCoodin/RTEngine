#include "UIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"

#include "ModuleImGui.h"

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
		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, size, { 1,1 }, { 0,0 });
	}
	hovered = ImGui::IsWindowHovered();
	ImGui::End();
}

bool UIScene::IsMouseHovering()
{
	return hovered;
}
