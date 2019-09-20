#include "UIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"



UIScene::UIScene(Application* App): UIElement(App)
{
	name = "Scene";
}


UIScene::~UIScene()
{
}

void UIScene::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin(name.c_str()))
	{
		

		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture,ImGui::GetWindowSize());

		ImGui::End();


	}
	ImGui::PopStyleVar();
}
