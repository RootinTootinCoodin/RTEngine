#include "UIInspector.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
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
	if (ImGui::Checkbox("Active", &gameobject->active))
		gameobject->RecursiveSetActive(gameobject->active);
	ImGui::SameLine();

	if (ImGui::Checkbox("Static", &gameobject->is_static))
		gameobject->RecursiveSetStatic(gameobject->is_static);

	ImGui::SameLine();
	ImGui::Checkbox("Draw AABB", &gameobject->draw_aabb);

	ImGui::Text(gameobject->GetName().c_str());
	ImGui::Text("UUID: %u", gameobject->GetUUID());
	ImGui::Separator();
	ImGui::Separator();
	if (ComponentTransform* transform = (ComponentTransform*)gameobject->GetComponent(TRANSFORM))
	{
		DrawTransformInfo(transform);
		ImGui::Separator();
		ImGui::Separator();
	}

	if (ComponentMesh* mesh = (ComponentMesh*)gameobject->GetComponent(MESH))
	{
		DrawMeshInfo(mesh);
		ImGui::Separator();
		ImGui::Separator();
	}
	if (ComponentMaterial* material = (ComponentMaterial*)gameobject->GetComponent(MATERIAL))
	{
		DrawMaterialInfo(material);
		ImGui::Separator();
		ImGui::Separator();
	}

	if (ComponentCamera* camera = (ComponentCamera*)gameobject->GetComponent(CAMERA))
	{
		DrawCameraInfo(camera);
		ImGui::Separator();
		ImGui::Separator();
	}
	else
	{
		if (ImGui::Button("Add camera"))
			gameobject->AddComponent(CAMERA);
	}
}

void UIInspector::DrawTransformInfo(ComponentTransform * transform)
{
	ImGui::Text("Component Transform");
	ImGui::Text("UUID: %u", transform->GetUUID());
	float3 position = transform->getPos();
	float3 rotation = transform->getRotation();
	float3 scale = transform->getScale();

	if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
		transform->setPos(position); 
	
	if (ImGui::DragFloat3("Rotation", (float*)&rotation,0.1f));
		transform->setRotation(rotation); 
	if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f))
		transform->setScale(scale); 
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

	if (mesh->uvs != nullptr)
		ImGui::BulletText("This mesh contains UVs");
	else
		ImGui::BulletText("This mesh does not have UVs");

	ImGui::Checkbox("Draw Normals", &mesh->draw_normals);
}

void UIInspector::DrawMaterialInfo(ComponentMaterial * material)
{
	ImGui::Text("Component Material");

	ImGui::Text("UUID: %u", material->GetUUID());

	ImGui::Text("Name: %s", material->name.c_str());
	ImGui::Text("Full Path: %s", material->path.c_str());
	ImGui::Text("ID: %u", material->id_texture);
	ImGui::Image((ImTextureID)material->id_texture, { 300,300 });
	ImGui::Text("Size: %u x %u", material->width, material->height);
	ImGui::Text("Bytes per pixel: %u", material->bpp);
	ImGui::Text("Image Depth: %u", material->depth);

	if (ImGui::Button("Use Checkered Texture"))
	{
		material->CopyTextureToThis(App->scene->textures[1]);
	}
	if (ImGui::CollapsingHeader("Select another material"))
	{
		for (int i = 0; i < App->scene->textures.size(); i++)
		{
			texture* tmp = App->scene->textures[i];
			ImGui::Bullet();
			ImGui::PushID(i);
			bool button = ImGui::Button(tmp->name.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Image((ImTextureID)tmp->id_texture, { 50,50 });
				ImGui::EndTooltip();
			}

			if(button)
			{
				material->CopyTextureToThis(tmp);
			}
			ImGui::PopID();
		}
	}
}

void UIInspector::DrawCameraInfo(ComponentCamera * camera)
{
	ImGui::Text("Component Camera");

	ImGui::Text("UUID: %u", camera->GetUUID());


}



