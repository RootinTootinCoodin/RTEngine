#include "UIAbout.h"

UIAbout::UIAbout(Application * app) : UIElement(app)
{
	name = "About";
}

UIAbout::~UIAbout()
{
}

void UIAbout::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		ImGui::Text("Placeholder, fill this with text");

		ImGui::End();
	}
}