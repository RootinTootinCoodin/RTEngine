#include "Component.h"



Component::Component()
{
}

Component::Component(componentType type, GameObject * parent) : type(type),parent(parent)
{
	uuid = Generate_UUID();
}

Component::Component(componentType type, GameObject * parent, uint uuid) : type(type), parent(parent)
{
	this->uuid = uuid;
}


Component::~Component()
{
}
