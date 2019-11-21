#include "UITime.h"


UITime::UITime(Application * app) : UIElement(app)
{
	name = "Time";
}

void UITime::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		if (ImGui::Button("Play"))
		{

		}

		if (ImGui::Button("Pause"))
		{

		}

		if (ImGui::Button("Stop"))
		{

		}

		if (ImGui::Button("Tick"))
		{

		}
	}

	ImGui::End();
}
