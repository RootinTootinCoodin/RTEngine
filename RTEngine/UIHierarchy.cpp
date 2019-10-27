#include "UIHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
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

	ImGui::Checkbox("View as Hierachy", &App->scene->view_hierarchy);

	if(App->scene->view_hierarchy)
		App->scene->root->RecursiveHierarchyChildren();

	else
	{
		std::vector<GameObject*> gameObjects;
		App->scene->root->RecursiveGetChildren(&gameObjects);

		for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
		{
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (_app->scene->selected_go == (*item))
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}
			ImGui::PushID((*item)->GetUUID());
			bool open = ImGui::TreeNodeEx((*item)->GetName().c_str(), node_flags);
			if (ImGui::IsItemClicked())
			{
				_app->scene->selected_go = (*item);
			}
			if (open)
			{
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
}
