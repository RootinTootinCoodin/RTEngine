#include "UIConfiguration.h"

#include "Application.h"
#include "ModuleWindow.h"

UIConfiguration::UIConfiguration(Application* app) : UIElement(app)
{
	name = "Configuration";
}


UIConfiguration::~UIConfiguration()
{
}

void UIConfiguration::Draw()
{
	if (ImGui::Begin(name.c_str(), &show_window))
	{
		if (ImGui::CollapsingHeader("Window")) // 2 - Window
		{
			if (ImGui::DragInt("Width", &App->window->width))
				App->window->UpdateSize();
			if (ImGui::DragInt("Height", &App->window->height))
				App->window->UpdateSize();

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			{
				App->window->UpdateWindowMode();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable))
			{
				App->window->UpdateWindowMode();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Borderless", &App->window->borderless))
			{
				App->window->UpdateWindowMode();
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->full_desktop))
			{
				App->window->UpdateWindowMode();
			}
		}
		ImGui::End();
	}
}
