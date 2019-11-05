#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"

#include "MathGeoLib/Math/float4x4.h" //TODO find a way to not include this
class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	math::float4x4 GetLocalTransformMatrix() const { return local_transform; };

	void setLocalFromPSR(float3& pos, float3& scale, Quat& rotation);

private:
	math::float4x4 local_transform;
};

#endif // ! __COMPONENT_TRANSFORM_H__


