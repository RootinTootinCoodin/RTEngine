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
	if (ImGui::Begin(name.c_str(), &show_window,ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
	{
		DrawConsoleItems();
	}
	ImGui::End();
}

void UIConsole::DrawConsoleItems()
{
	LOG("yeet");
	for (auto item = App->logs.begin(); item != App->logs.end(); item++)
	{
		if (!ImGui::IsAnyMouseDown()) {
			ImGui::SetScrollHereY(1.0f);
			ImGui::SetScrollHereX(1.0f);
		}

		ImGui::Text((*item));
	}
}

