#include "UIInspector.h"
#include "Application.h"
#include "ModuleScene.h"

#include "Globals.h"


UIInspector::UIInspector(Application* app) : UIElement(app)
{
	name = "Inspector";
}


UIInspector::~UIInspector()
{
}

void UIInspector::Draw()
{
	ImGui::Begin(name.c_str(), &show_window);
	if (ImGui::CollapsingHeader("Models"))
	{
		if (App->scene->models.size() > 0)
		{
			for (auto item = App->scene->models.begin(); item != App->scene->models.end(); item++)
			{
				if (ImGui::TreeNode((*item)->name.c_str()))
				{
					if (ImGui::IsItemClicked())
					{
						App->scene->current_model_index = std::distance(App->scene->models.begin(), item);
					}
					DrawModelInfo((*item));
					ImGui::TreePop();
				}
			}
			std::string model = "Current model: ";
			model += App->scene->models[App->scene->current_model_index]->name;
			ImGui::Text(model.c_str());
		}
		else
			ImGui::Text("No models loaded");
	}
	
	ImGui::End();
}

void UIInspector::DrawModelInfo(model * _model)
{
	
	for (auto item = _model->meshes.begin(); item != _model->meshes.end(); item++)
	{
		std::string name = "Mesh: ";
		name += (*item)->mesh_name.c_str();
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::BulletText("Index ID: %u", (*item)->id_index);
			ImGui::BulletText("Num of indices: %u", (*item)->num_indices);
			ImGui::Separator();
			ImGui::BulletText("Num of vertices: %u", (*item)->num_vertices);
			ImGui::Separator();
			if ((*item)->id_uvs != 0)
				ImGui::BulletText("UVs ID: %u", (*item)->id_uvs);
			else
				ImGui::BulletText("This mesh does not have coordinate textures");

			ImGui::TreePop();
		}
	}
}
