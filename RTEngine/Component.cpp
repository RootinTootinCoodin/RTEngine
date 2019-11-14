#include "Component.h"



Component::Component()
{
}

Component::Component(componentType type, GameObject * parent) : type(type),gameObject(parent)
{
	uuid = Generate_UUID();
}

Component::Component(componentType type, GameObject * parent, uint uuid) : type(type), gameObject(parent)
{
	this->uuid = uuid;
}


Component::~Component()
{
}
