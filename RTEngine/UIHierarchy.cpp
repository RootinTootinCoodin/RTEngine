#include "UIHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleTime.h"
#include "GameObject.h"



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
	if (ImGui::Button("Save Scene"))
		_app->scene->save = true;

	if (App->scene->selected_go != nullptr)
	{
		ImGui::SameLine();
		if (ImGui::Button("Add empty child"))
		{
			App->scene->selected_go->AddChildren("Token Child");
		}
	}
	if (App->time->GetState() == STATE_STOPPED)
	{
		static char scene_name[120];
		strcpy_s(scene_name, 120, App->scene->scene_name.c_str());
		if (ImGui::InputText("App Name", scene_name, 25, ImGuiInputTextFlags_EnterReturnsTrue))
			App->scene->scene_name = scene_name;
	}
	
	App->scene->root->RecursiveHierarchyChildren();

	ImGui::End();
}
