#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


GameObject::GameObject()
{
}

GameObject::GameObject(std::string name, GameObject * parent) : name(name), parent(parent)
{
	uuid = Generate_UUID();
	AddComponent(TRANSFORM);
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
	children.insert({ GetUUID(),ret });
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

Component * GameObject::GetComponent(componentType type)
{
	
	for (auto item = components.begin(); item != components.end(); item++)
	{
		if ((*item).second->GetComponentType() == type)
			return (*item).second;
	}
	return nullptr;
}
