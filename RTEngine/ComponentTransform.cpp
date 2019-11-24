#include "ComponentTransform.h"
#include "GameObject.h"



ComponentTransform::ComponentTransform()
{
	local_transform.SetIdentity();
	global_transform.SetIdentity();
	pos = { 0,0,0 };
	rotation = Quat::identity;
	scale = { 1,1,1 };
}

ComponentTransform::ComponentTransform(GameObject * parent) : Component(TRANSFORM,parent)
{
	local_transform.SetIdentity();
	global_transform.SetIdentity();
	pos = { 0,0,0 };
	rotation = Quat::identity;
	scale = { 1,1,1 };
}


ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::UpdateComponent(float dt)
{
	float3 mov = { 1, 1, 1 };

	pos += mov * dt;

	return true;
}

void ComponentTransform::removeDirty()
{
	if (!gameObject->GetUUID() == 0)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)gameObject->GetParent()->GetComponent(TRANSFORM);
		global_transform = parent_transform->GetGlobalTransformMatrix()*local_transform;
	}
	else
		global_transform.SetIdentity();
	dirty = false;
}

void ComponentTransform::setLocalFromMatrix(math::float4x4 matrix)
{
	local_transform = matrix;
	pos = matrix.TranslatePart();
	scale = matrix.GetScale();
	rotation = matrix.RotatePart().ToQuat();
	gameObject->RecursiveSetDirty();

}

void ComponentTransform::setLocalFromPSR()
{
	local_transform.SetIdentity();
	local_transform = float4x4::FromTRS(pos, rotation, scale);
	gameObject->RecursiveSetDirty();
}

void ComponentTransform::setPos(float3 pos)
{
	this->pos = pos;
	setLocalFromPSR();
}

void ComponentTransform::setScale(float3 scale)
{
	//local_transform.sca(pos);
	this->scale = scale;
	setLocalFromPSR();
}

void ComponentTransform::setRotation(Quat rotation)
{
	this->rotation = rotation;
	setLocalFromPSR();
}

void ComponentTransform::setRotation(float3 rotation)
{
	this->rotation = Quat::FromEulerXYZ(rotation.x,rotation.y,rotation.z);
	setLocalFromPSR();
}
