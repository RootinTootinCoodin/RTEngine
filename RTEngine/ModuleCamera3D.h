#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/Geometry/AABB.h"

#define CAMERA_SPEED 10.0f

class Timer;
class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(JSON_Object* config) override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	void Save(JSON_Object* config);

	void Look(const vec3 &Position, const vec3 &Reference);
	void LookAt(const vec3 &Spot);

	mat4x4 At(const vec3 &right, const vec3 &up, const vec3 &direction);

	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void AdjustCameraToAABB(AABB& aabb);
	
private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z;
	mat4x4 CameraView;

	vec3 cameraRef;
	vec3 cameraPos;
	vec3 cameraTarget;
	vec3 cameraFront;

	vec3 cameraDirection;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 newPos;

	float sensitivity = 0.9f;
	float speed = 5.0f;
	float wheel = 100.0f;
	float speedmultiplier = 3.0f;

	ComponentCamera* editorCamera;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
	Timer timer;

};

#endif // !__MODULE_CAMERA_3D_H__
