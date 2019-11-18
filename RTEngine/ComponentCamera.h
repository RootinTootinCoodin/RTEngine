#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject* parent);

	~ComponentCamera();

	void UpdateFrustum();
	void Move(const vec &increment);
	void LookAt(const vec& spot);
	void UpdateTransform();

	float* GetProjectionMatrix();
	float* GetViewMatrix();

	Frustum camera;
	vec position, reference;

	float near_plane_distance, far_plane_distance, horizontal_fov, vertical_fov, aspect_ratio;
};

#endif // !__COMPONENT_CAMERA_H__