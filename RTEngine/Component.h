#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

class GameObject;

class Component
{
public:
	Component();
	Component(componentType type, GameObject* parent);
	Component(componentType type, GameObject* parent, uint uuid);

	~Component();

	virtual bool UpdateComponent() { return true; };

	uint GetUUID() { return uuid; };
	componentType GetComponentType() { return type; };
private:

	virtual bool Enable() { return true; };
	virtual bool Disable() { return true; };
public:
private:
	bool active = true;
	uint uuid = 0;
	GameObject* parent = nullptr;
	componentType type = NO_TYPE;
};

#endif // !__COMPONENT_H__


