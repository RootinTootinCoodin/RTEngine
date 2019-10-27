#include "UIInspector.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
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
	if (App->scene->selected_go)
	{
		DrawGameObjectInfo(App->scene->selected_go);
	}
	
	ImGui::End();
}

void UIInspector::DrawGameObjectInfo(GameObject* gameobject)
{
	ImGui::Checkbox("Active", &gameobject->active);
	ImGui::SameLine();
	ImGui::Checkbox("Draw AABB", &gameobject->draw_aabb);

	ImGui::Text(gameobject->GetName().c_str());
	ImGui::Separator();
	ImGui::Separator();
	if (ComponentTransform* transform = (ComponentTransform*)gameobject->GetComponent(TRANSFORM))
	{
		DrawTransformInfo(transform);
	}
	ImGui::Separator();
	ImGui::Separator();
	if (ComponentMesh* mesh = (ComponentMesh*)gameobject->GetComponent(MESH))
	{
		DrawMeshInfo(mesh);
	}
	ImGui::Separator();
	ImGui::Separator();
}

void UIInspector::DrawTransformInfo(ComponentTransform * transform)
{
	ImGui::Text("Component Transform");
	ImGui::Text("UUID: %u", transform->GetUUID());
	math::float4x4 local = transform->GetLocalTransformMatrix();
	float3 position = local.TranslatePart();
	float3 rotation = local.RotatePart().ToEulerXYZ();
	float3 scale = local.ExtractScale();

	ImGui::DragFloat3("Position", (float*)&position);
	ImGui::DragFloat3("Rotation", (float*)&rotation);
	ImGui::DragFloat3("Scale", (float*)&scale);
}

void UIInspector::DrawMeshInfo(ComponentMesh * mesh)
{
	ImGui::Text("Component Mesh");

	ImGui::Text("UUID: %u", mesh->GetUUID());

	ImGui::BulletText("Index ID: %u", mesh->id_index);
	ImGui::BulletText("Num of indices: %u", mesh->num_indices);
	ImGui::Separator();
	ImGui::BulletText("Num of vertices: %u", mesh->num_vertices);
	ImGui::Separator();

	if (mesh->id_uvs != 0)
		ImGui::BulletText("UVs ID: %u", mesh->id_uvs);
	else
		ImGui::BulletText("This mesh does not have coordinate textures");

	ImGui::Checkbox("Draw Normals", &mesh->draw_normals);


}


