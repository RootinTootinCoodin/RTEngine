#include "UIScene.h"



UIScene::UIScene()
{
	name = "Scene";
}


UIScene::~UIScene()
{
}

void UIScene::Draw()
{
	if (ImGui::Begin(name.c_str()))
	{
		ImGui::End();
	}
}
