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
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth(),ImGui::GetWindowContentRegionHeight()-25), false, window_flags);
		DrawConsoleItems();

		ImGui::EndChild();
		

	}
	ImGui::End();
}

void UIConsole::DrawConsoleItems()
{
	ImGui::TextUnformatted(App->logs.begin());
	if (App->update_console) {
		ImGui::SetScrollHereY(1.0f);
		App->update_console = false;
	}

}

