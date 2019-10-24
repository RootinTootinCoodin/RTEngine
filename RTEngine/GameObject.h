#pragma once

#include <string>
#include <map>
#include "Globals.h"

class Component;
class GameObject
{
public:
	GameObject();
	GameObject(std::string name, GameObject* parent);
	GameObject(std::string name, GameObject* parent, uint uuid);

	~GameObject();
private:
	std::string name;
	uint uuid = 0;
	bool active = true;
	GameObject* parent;
	std::map<uint, Component*> components;

public:
private:

	void Update();
	Component* AddComponent(componentType type);
};

