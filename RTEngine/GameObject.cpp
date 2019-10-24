#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"


GameObject::GameObject()
{
}

GameObject::GameObject(std::string name, GameObject * parent) : name(name), parent(parent)
{
	uuid = 
		Generate_UUID();
	AddComponent(TRANSFORM);
}

GameObject::GameObject(std::string name, GameObject * parent, uint uuid): name(name), parent(parent), uuid(uuid)
{
	AddComponent(TRANSFORM);
}


GameObject::~GameObject()
{
}

Component * GameObject::AddComponent(componentType type)
{
	Component* ret;
	switch (type)
	{
	case NO_TYPE:
		break;
	case TRANSFORM:

		break;
	case MESH:
		break;
	case MATERIAL:
		break;
	default:
		break;
	}
	return nullptr;
}
