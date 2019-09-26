#include "UIConfiguration.h"



UIConfiguration::UIConfiguration(Application* app) : UIElement(app)
{
	name = "Configuration";
}


UIConfiguration::~UIConfiguration()
{
}

void UIConfiguration::Draw()
{
	ImGui::Begin(name.c_str(), &show_window);
	ImGui::End();
}
