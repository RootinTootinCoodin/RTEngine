#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* parent);
	~ComponentTransform();
};

#endif // ! __COMPONENT_TRANSFORM_H__


