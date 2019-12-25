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

	virtual void ComponentStart() { };
	virtual void ComponentCleanUp() {};

	virtual bool UpdateComponent(float dt) { return true; };

	uint GetUUID() { return uuid; };
	void setUUID(uint _uuid) { uuid = _uuid; };
	GameObject* getGameObject() { return gameObject; };
	componentType GetComponentType() { return type; };
private:

	virtual bool Enable() { return true; };
	virtual bool Disable() { return true; };
public:
protected:
	bool active = true;
	uint uuid = 0;
	GameObject* gameObject = nullptr;
	componentType type = NO_TYPE;
};

#endif // !__COMPONENT_H__


