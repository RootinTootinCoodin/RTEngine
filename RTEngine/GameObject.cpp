#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "MathGeoLib/Geometry/AABB.h"



GameObject::GameObject()
{
}

GameObject::GameObject(std::string name, GameObject * parent) : name(name), parent(parent)
{
	uuid = Generate_UUID();
	AddComponent(TRANSFORM);
	bounding_box.SetNegativeInfinity();
}

GameObject::GameObject(std::string name, GameObject * parent, uint uuid): name(name), parent(parent), uuid(uuid)
{
	AddComponent(TRANSFORM);
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
