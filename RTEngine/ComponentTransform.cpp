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

void ComponentTransform::setLocalFromPSR(float3 & pos, float3 & scale, Quat & rotation)
{
	local_transform.SetIdentity();
	local_transform = local_transform.FromTRS(pos, rotation, scale);
}
