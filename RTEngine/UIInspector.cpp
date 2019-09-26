#include "UIInspector.h"



UIInspector::UIInspector(Application* app) : UIElement(app)
{
	name = "Inspector";
}


UIInspector::~UIInspector()
{
}

void UIInspector::Draw()
{
	ImGui::Begin(name.c_str(), &show_window);
	ImGui::End();
}
