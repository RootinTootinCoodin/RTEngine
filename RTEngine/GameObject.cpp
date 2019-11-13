#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"



GameObject::GameObject()
{
}

GameObject::GameObject(std::string _name, GameObject * parent, ComponentTransform* transform) : parent(parent)
{
	if (_name.size() > 0)
		name = _name;
	else
		name = "No name";
	uuid = Generate_UUID();

	if(!transform)
		AddComponent(TRANSFORM);
	else
		components.insert({ transform->GetUUID(), transform });

	bounding_box.SetNegativeInfinity();
}

GameObject::GameObject(std::string name, GameObject * parent, uint uuid, ComponentTransform* transform): name(name), parent(parent)
{
	this->uuid = uuid;

	if (!transform)
		AddComponent(TRANSFORM);
	else
		components.insert({ transform->GetUUID(), transform });

	bounding_box.SetNegativeInfinity();
}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (auto item = components.begin(); item != components.end(); item++)
	{
		(*item).second->UpdateComponent();
	}
}


Component* GameObject::AddComponent(componentType type)
{
	Component* ret;
	switch (type)
	{
	case NO_TYPE:


		break;
	case TRANSFORM:
		ret = new ComponentTransform(this);

		break;
	case MESH:

		ret = new ComponentMesh(this);
		break;
	case MATERIAL:

		ret = new ComponentMaterial(this);
		break;
	default:
		break;
	}

	components.insert({ ret->GetUUID(), ret });
	return ret;
}

GameObject * GameObject::AddChildren(std::string name)
{
	GameObject* ret = new GameObject(name, this);
	children.insert({ ret->GetUUID(),ret });
	return ret;
}

void GameObject::RecursiveSetActive(bool _active)
{
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveSetActive(_active);
	}
	active = _active;
}

void GameObject::RecursiveSetStatic(bool _static)
{
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveSetStatic(_static);
	}
	is_static = _static;
}

void GameObject::RecursiveApplyTexture(texture * texture)
{
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveApplyTexture(texture);
	}
	if (ComponentMaterial* material = (ComponentMaterial*)GetComponent(MATERIAL))
	{
		material->CopyTextureToThis(texture);
	}
}

void GameObject::RecursiveGetChildren(std::vector<GameObject*>* buffer)
{
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveGetChildren(buffer);
	}
	buffer->push_back(this);
}

void GameObject::RecursiveHierarchyChildren()
{
	for (auto item = children.begin(); item != children.end(); item++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (_app->scene->selected_go == (*item).second)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}
		ImGui::PushID((*item).second->GetUUID());
		bool open = ImGui::TreeNodeEx((*item).second->name.c_str(), node_flags);
		if (ImGui::IsItemClicked())
		{
			_app->scene->selected_go = (*item).second;
		}
		if (open)
		{
			(*item).second->RecursiveHierarchyChildren();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}


}

void GameObject::RecursiveSetDirty()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(TRANSFORM);
	transform->setDirty(true);
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveSetDirty();
	}
	
}

void GameObject::RecursiveRemoveDirtyFlags()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(TRANSFORM);
	if (transform->isDirty())
	{
		transform->removeDirty();
	}
	for (auto item = children.begin(); item != children.end(); item++)
	{
		(*item).second->RecursiveSetDirty();
	}
}

void GameObject::RecalculateAABB()
{
	if (ComponentMesh* mesh = (ComponentMesh*)GetComponent(MESH))
	{
		bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);
	}
	for (auto item = children.begin(); item != children.end(); item++)
	{
		bounding_box.Enclose((*item).second->bounding_box);
	}
}

void GameObject::ParentRecalculateAABB()
{
	RecalculateAABB();
	if (parent)
	{
		parent->ParentRecalculateAABB();
	}
}

Component * GameObject::GetComponent(componentType type)
{
	
	for (auto item = components.begin(); item != components.end(); item++)
	{
		if ((*item).second->GetComponentType() == type)
			return (*item).second;
	}
	return nullptr;
}
