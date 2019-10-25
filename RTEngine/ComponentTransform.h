#pragma once
#include "Component.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* parent);
	~ComponentTransform();
};

