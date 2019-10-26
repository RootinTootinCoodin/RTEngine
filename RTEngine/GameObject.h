#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__


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


	Component* AddComponent(componentType type);
	GameObject* AddChildren(std::string name);
	uint GetUUID() { return uuid; };
	void RecursiveGetChildren(std::vector<GameObject*>* buffer);

	Component* GetComponent(componentType type);

private:
	std::string name;
	uint uuid = 0;
	bool active = true;
	GameObject* parent;
	std::map<uint, Component*> components;
	std::map<uint, GameObject*> children;

public:
private:

	void Update();

	
};

#endif // !__GAME_OBJECT_H__


