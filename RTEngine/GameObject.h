#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__


#include "Globals.h"
#include "MathGeoLib/Geometry/AABB.h" //I dont like this, but i get compiler errors otherwise

#include <map>

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
	uint GetUUID() const { return uuid; }; 
	std::string GetName() const { return name; }; 

	void RecursiveGetChildren(std::vector<GameObject*>* buffer);
	void RecursiveHierarchyChildren();
	void RecalculateAABB();
	void ParentRecalculateAABB();

	Component* GetComponent(componentType type);

private:
	std::string name;
	uint uuid = 0;
	bool active = true;
	GameObject* parent;
	std::map<uint, Component*> components;
	std::map<uint, GameObject*> children;

	AABB bounding_box;

public:
private:

	void Update();

	
};

#endif // !__GAME_OBJECT_H__


