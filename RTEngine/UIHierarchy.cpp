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
	if (ImGui::Button("Save Scene"))
		_app->scene->save = true;
	App->scene->root->RecursiveHierarchyChildren();

	ImGui::End();
}
