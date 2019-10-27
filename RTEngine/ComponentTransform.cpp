#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	local_transform.SetIdentity();
}

ComponentTransform::ComponentTransform(GameObject * parent) : Component(TRANSFORM,parent)
{
	local_transform.SetIdentity();
}


ComponentTransform::~ComponentTransform()
{
}
