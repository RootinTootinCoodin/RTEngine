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
		if (ImGui::Button("Clear"))
			App->logs.clear();
		ImGui::SameLine();
		ImGui::Checkbox("Autoscroll", &autoscroll);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth(),ImGui::GetWindowContentRegionHeight()-25), false, window_flags);
		DrawConsoleItems();

		ImGui::EndChild();
		

	}
	ImGui::End();
}

void UIConsole::DrawConsoleItems()
{
	uint num_items = App->logs.size();
	for (auto item = App->logs.begin(); item != App->logs.end(); item++)
	{
		ImGui::Text((*item));
	}
	if (autoscroll)
	{
		if (num_items != prev_num_items)
		{
			ImGui::SetScrollHereY(1.0f);
			ImGui::SetScrollHereX(1.0f);
		}
	}
}

