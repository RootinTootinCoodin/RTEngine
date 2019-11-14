#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"

#include "MathGeoLib/Math/float4x4.h" //TODO find a way to not include this
#include "MathGeoLib/Math/Quat.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	math::float4x4 GetLocalTransformMatrix() const { return local_transform; };
	math::float4x4 GetGlobalTransformMatrix() const { return global_transform; };

	bool isDirty() const { return dirty; };
	void setDirty(bool _dirty) { dirty = _dirty; };
	void removeDirty();

	void setLocalFromPSR();
	void setPos(float3 & pos);
	void setScale(float3& scale);
	void setRotation(Quat& rotation);
	void setRotation(float3& rotation);

	float3 getPos() { return pos; };
	float3 getScale() { return scale; };
	float3 getRotation() { return rotation.ToEulerXYZ(); };


private:
	math::float4x4 local_transform;
	math::float4x4 global_transform;

	float3 pos;
	float3 scale;
	Quat rotation;

	bool dirty = true;
};

#endif // ! __COMPONENT_TRANSFORM_H__


