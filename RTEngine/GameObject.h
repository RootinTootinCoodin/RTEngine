#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__


#include "Globals.h"
#include "MathGeoLib/Geometry/AABB.h" //I dont like this, but i get compiler errors otherwise

#include <map>

class Component;
class ComponentTransform;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, GameObject* parent, ComponentTransform* transform = nullptr);
	GameObject(std::string name, GameObject* parent, uint uuid, ComponentTransform* transform = nullptr);

	~GameObject();


	Component* AddComponent(componentType type);
	GameObject* AddChildren(std::string name); //CANT BE REFERENCE
	uint GetUUID() const { return uuid; }; 
	uint GetParentUUID() const { return parent->GetUUID(); };
	std::string GetName() const { return name; }; 
	AABB GetAABB() const { return bounding_box; };
	GameObject* GetParent() const { return parent; };

	uint GetNumComponents()const { return components.size(); };
	uint GetNumChildren()const { return children.size(); };
	std::map<uint, Component*> GetComponentList() const { return components; };

	void RemoveComponents();
	void RecursiveGameObjectCleanUp();

	void RecursiveSetActive(bool active);
	void RecursiveSetStatic(bool _static);
	void RecursiveApplyTexture(texture* texture);
	void RecursiveGetChildren(std::vector<GameObject*>* buffer);
	void RecursiveHierarchyChildren();
	void RecursiveSetDirty();
	void RecursiveRemoveDirtyFlags();
	void RecalculateAABB();
	void ParentRecalculateAABB();

	Component* GetComponent(componentType type);

private:
	std::string name = "No name";
	uint uuid = 0;
	GameObject* parent;
	std::map<uint, Component*> components;
	std::map<uint, GameObject*> children;

	AABB bounding_box;

public:
	bool active = true;
	bool is_static = true;
	bool draw_aabb = false;


private:

	void Update();

	
};

#endif // !__GAME_OBJECT_H__


