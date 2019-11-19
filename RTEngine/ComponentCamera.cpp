#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

ComponentCamera::ComponentCamera()
{
}

ComponentCamera::ComponentCamera(GameObject * parent) : Component(CAMERA, parent)
{
	if (parent == _app->scene->root)
	{
		position = vec(-10.0f, 5.0f, 7.0f);
		reference = vec(0.0f, 0.0f, -1.0f);
	}

	else
	{
		ComponentTransform* parent_transf = (ComponentTransform*)parent->GetComponent(TRANSFORM);
		position = parent_transf->getPos();
		reference = position;
		reference.x += 1;
		reference.Normalize();
	}

	near_plane_distance = 0.1f;
	far_plane_distance = 1000;
	vertical_fov = 60;
	aspect_ratio = (float)_app->window->width / _app->window->height;

	camera.type = math::FrustumType::PerspectiveFrustum;
	camera.pos = position;
	camera.front = reference;
	camera.up = { 0.0f, 1.0f, 0.0f };

	UpdateFrustum();
	LookAt(reference);
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::UpdateFrustum()
{
	camera.nearPlaneDistance = near_plane_distance;
	camera.farPlaneDistance = far_plane_distance;

	camera.verticalFov = DEGTORAD * vertical_fov;
	camera.horizontalFov = 2.f * Atan(Tan(camera.verticalFov*0.5f)*(aspect_ratio));
}

void ComponentCamera::Move(const vec &increment)
{
	position += increment;
	reference += increment;
	camera.pos += increment;
}

void ComponentCamera::LookAt(const vec& spot)
{
	reference = spot;

	vec front = reference - camera.pos;

	float3x3 look = float3x3::LookAt(camera.front.Normalized(), front.Normalized(), camera.up.Normalized(), { 0,1,0 });

	camera.front = look.MulDir(camera.front).Normalized();
	camera.up = look.MulDir(camera.up).Normalized();
}

void ComponentCamera::UpdateTransform()
{
	//Update Position
	ComponentTransform* parent_transf = (ComponentTransform*)gameObject->GetComponent(TRANSFORM);
	position = parent_transf->getPos();
	camera.pos = position;

	//Update Rotation
	Quat rotation = parent_transf->getRotationQuat();
	camera.front = rotation.Mul(camera.front).Normalized();
	camera.up = rotation.Mul(camera.up).Normalized();
}

float * ComponentCamera::GetProjectionMatrix()
{
	static float4x4 matrix;
	matrix = camera.ProjectionMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}

float * ComponentCamera::GetViewMatrix()
{
	static float4x4 matrix;
	matrix = camera.ViewMatrix();
	matrix.Transpose();
	return  (float*)matrix.v;
}
