#include "ComponentTransform.h"
#include "GameObject.h"



ComponentTransform::ComponentTransform()
{
	local_transform.SetIdentity();
	global_transform.SetIdentity();

}

ComponentTransform::ComponentTransform(GameObject * parent) : Component(TRANSFORM,parent)
{
	local_transform.SetIdentity();
	global_transform.SetIdentity();
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::removeDirty()
{
	ComponentTransform* parent_transform = (ComponentTransform*)parent->GetComponent(TRANSFORM);
	global_transform = parent_transform->GetGlobalTransformMatrix()*local_transform;
	dirty = false;
}

void ComponentTransform::setLocalFromPSR()
{
	local_transform.SetIdentity();
	local_transform = float4x4::FromTRS(pos, rotation, scale);
}

void ComponentTransform::setPos(float3 & pos)
{
	this->pos = pos;
}

void ComponentTransform::setScale(float3 & scale)
{
	//local_transform.sca(pos);
	this->scale = scale;
}

void ComponentTransform::setRotation(Quat & rotation)
{
	this->rotation = rotation;
}

void ComponentTransform::setRotation(float3 & rotation)
{
	this->rotation = Quat::FromEulerXYZ(rotation.x,rotation.y,rotation.z);
}
