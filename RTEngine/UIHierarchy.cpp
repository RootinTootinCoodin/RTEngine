#include "UIHierarchy.h"



UIHierarchy::UIHierarchy(Application* app) : UIElement(app)
{
	name = "Hierarchy";
}


UIHierarchy::~UIHierarchy()
{
}

void UIHierarchy::Draw()
{
	ImGui::Begin(name.c_str(), &show_window);
	ImGui::End();
}
