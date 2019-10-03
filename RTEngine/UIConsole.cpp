#include "UIConsole.h"
#include "Application.h"


UIConsole::UIConsole(Application* app) : UIElement(app)

{
	name = "Console";
}


UIConsole::~UIConsole()
{
}

void UIConsole::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		DrawConsoleItems();
	}
	ImGui::End();
}

void UIConsole::DrawConsoleItems()
{
	for (auto item = App->logs.begin(); item != App->logs.end(); item++)
	{
		ImGui::SetScrollHereY(1.0f);
		ImGui::Text((*item));
	}
}

