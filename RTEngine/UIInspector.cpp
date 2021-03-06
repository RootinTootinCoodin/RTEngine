#include "UIInspector.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleResource.h"
#include "ModuleLoader.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentScript.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceScript.h"
#include "FileSystem.h"
#include "Globals.h"
#include "UITextEditor.h"
#include "ModuleImGui.h"

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

	if (ImGui::Button("Save"))
	{
		_app->loader->ExportSceneOrModel(gameobject,true);
	}

	static char go_name[120];
	strcpy_s(go_name, 120, gameobject->GetName().c_str());
	if (ImGui::InputText("Name", go_name, 25, ImGuiInputTextFlags_EnterReturnsTrue))
		gameobject->SetName(go_name);
	
	ImGui::Text("UUID: %u", gameobject->GetUUID());
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
		gameobject->to_remove = true;
	ImGui::Separator();
	ImGui::Separator();
	if (ComponentTransform* transform = (ComponentTransform*)gameobject->GetComponent(TRANSFORM))
	{
		DrawTransformInfo(transform);
		ImGui::Separator();
		ImGui::Separator();
	}

	if (ComponentScript* script = (ComponentScript*)gameobject->GetComponent(SCRIPT))
	{
		DrawScriptInfo(script);
		ImGui::Separator();
		ImGui::Separator();
	}
	else
	{
		SelectScript(gameobject);
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
	rotation.x *= RADTODEG;
	rotation.y *= RADTODEG;
	rotation.z *= RADTODEG;


	float3 scale = transform->getScale();
	


	if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
		transform->setPos(position); 
	if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f))
	{
		rotation.x *= DEGTORAD;
		rotation.y *= DEGTORAD;
		rotation.z *= DEGTORAD;
		transform->setRotation(rotation);
	}
	if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f))
		transform->setScale(scale); 



}

void UIInspector::DrawMeshInfo(ComponentMesh * mesh_comp)
{
	if (ResourceMesh* mesh = (ResourceMesh*)App->resource->getResource(mesh_comp->getResourceUUID()))
	{
		ImGui::Text("Component Mesh");

		ImGui::Text("UUID: %u", mesh_comp->GetUUID());
		ImGui::Text("Resource UUID: %u", mesh->GetUUID());
		ImGui::Text("Times Referenced: %u", mesh->getAmountLoaded());

		ImGui::BulletText("Index ID: %u", mesh->id_index);
		ImGui::BulletText("Num of indices: %u", mesh->num_indices);
		ImGui::Separator();
		ImGui::BulletText("Num of vertices: %u", mesh->num_vertices);
		ImGui::Separator();

		if (mesh->uvs != nullptr)
			ImGui::BulletText("This mesh contains UVs");
		else
			ImGui::BulletText("This mesh does not have UVs");

		//ImGui::Checkbox("Draw Normals", &mesh->draw_normals);
	}
	else
		ImGui::Text("Mesh not loaded in memory");
}

void UIInspector::DrawMaterialInfo(ComponentMaterial * _material)
{
	ResourceMaterial* material = (ResourceMaterial*)App->resource->getResource(_material->getResourceUUID());
	ImGui::Text("Component Material");

	if (material)
	{
		ImGui::Text("Resource UUID: %u", material->GetUUID());
		ImGui::Text("Times referenced: %u", material->getAmountLoaded());
		ImGui::Text("Name: %s", material->name.c_str());
		ImGui::Text("Full Path: %s", material->path.c_str());
		ImGui::Text("ID: %u", material->id_texture);
		ImGui::Image((ImTextureID)material->id_texture, { 300,300 });
		ImGui::Text("Size: %u x %u", material->width, material->height);
		ImGui::Text("Bytes per pixel: %u", material->bpp);
		ImGui::Text("Image Depth: %u", material->depth);

	}
	else
		ImGui::Text("Resource not loaded: %u",_material->getResourceUUID());


	if (ImGui::Button("Use Checkered Texture"))
	{
		_material->AssignResourceUUID(1212121212);
	}
	if (ImGui::CollapsingHeader("Select another material"))
	{
		std::vector<ResourceMaterial*> textures = App->resource->getMaterials();
		for (int i = 0; i < textures.size(); i++)
		{
			ResourceMaterial* tmp= textures[i];
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
				tmp->decreaseAmountLoaded();
				_material->AssignResourceUUID(tmp->GetUUID());
			}
			ImGui::PopID();
		}
	}
}

void UIInspector::DrawCameraInfo(ComponentCamera * camera)
{
	ImGui::Text("Component Camera");

	ImGui::Text("UUID: %u", camera->GetUUID());

	if (ImGui::DragFloat("Near plane distance", &camera->near_plane_distance) || ImGui::DragFloat("Far plane distance", &camera->far_plane_distance))
	{
		camera->UpdateFrustum();
	}


}

void UIInspector::DrawScriptInfo(ComponentScript * _script)
{
	ImGui::Text("Component Script");

	ResourceScript* script = (ResourceScript*)App->resource->getResource(_script->getResourceUUID());

	if (script)
	{
		ImGui::Text("Script Name: %s", script->GetOriginalFile().c_str());
		if(ImGui::Button("Edit"))
		{
			App->ImGui->texted->SetNewScript(script->GetOriginalFile());
		}
		if (script->compiled)
		{
			ImGui::Text("Compiled: TRUE");
			ImGui::Text("Script pointing to: %u", (uint)script->scriptTable["UUID"]);
		}
		else
		{
			ImGui::Text("Compiled: FALSE");
			ImGui::Text("This script will be ignored until it compiles");
			if (ImGui::Button("Recompile"))
			{
				App->scripting->LoadScript(script->GetOriginalFile(), _script);
			}
		}
	}

	/*if (ImGui::Button("Edit script"))
	{
		App->ImGui->texted->scriptPath = script->GetOriginalFile();
		App->ImGui->texted->show_window = true;
	}*/

}

void UIInspector::SelectScript(GameObject* go)
{
	std::vector<std::string> files;
	FileSystem::DiscoverFiles(ASSETS_SCRIPTS_FOLDER, &files, nullptr);
	if(ImGui::CollapsingHeader("CurrentScripts"))
	{
		for (auto item = files.begin(); item != files.end(); item++)
		{
			std::string extension  = (*item);
			FileSystem::getExtension(extension);
			if (SCRIPT_EXTENSIONS(extension))
			{
				std::string name = (*item);
				FileSystem::getFileNameFromPath(name);
				if (ImGui::Button(name.c_str()))
				{
					ComponentScript* script = (ComponentScript*)go->AddComponent(SCRIPT);
					App->scripting->LoadScript((*item), script);
				}
			}
		}
		if (ImGui::CollapsingHeader("NewScript"))
		{
			static char script_name[120];
			bool doit = ImGui::InputText("Script_Name", script_name, 25, ImGuiInputTextFlags_EnterReturnsTrue);

			if (ImGui::Button("Create new script") || doit)
			{
				NewScript(script_name);
				ComponentScript* script = (ComponentScript*)go->AddComponent(SCRIPT);
				std::string path;
				FileSystem::FormFullPath(path, script_name, ASSETS_SCRIPTS, ".lua");
				App->scripting->LoadScript(path, script);
			}
		}
	}
}

void UIInspector::NewScript(char* script_name)
{
	char* buffer = FileSystem::ImportFile("Assets\\Scripts\\TEMPLATE.lua");
	std::string file = buffer;
	file.replace(file.find("TEMPLATE"), 8, script_name);
	//The open/close functions of the FileSystem add weird characters
	file.erase(file.find_last_of("d") + 1,100);
	std::vector<char> cstr(file.c_str(), file.c_str() + file.size() + 1);
	FileSystem::ExportBuffer(cstr.data(), cstr.size()-1, script_name, ASSETS_SCRIPTS, ".lua");
}



