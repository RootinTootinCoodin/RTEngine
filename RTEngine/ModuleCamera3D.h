#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "p2Queue.h"
class btVector3;
class Timer;

enum CameraState
{
	NO_STATE,
	THIRD_PERSON,
	THIRD_PERSON_BACK,
	FIRST_PERSON,
	FROM_WHEEL,
	INITIAL_CAM,
	FRONT,
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void FollowCar(CameraState state);
	vec3 GetVec3From_btVec3(btVector3 vector);
	void ThirdPersonCamera();
	void ThirdPersonCameraFromBack();
	void FirstPersonCamera();
	void FirstPersonCameraFront();
	void WheelCamera();
	void ResetCamera();
	void FixedCamera();
	vec3 VehicleToWorld(vec3 localpos);
	void SetInitialSpeedAndPos(btVector3 position,btVector3 speed);
private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	mat4x4 Camera_view;
	vec3 Last_position;
private:
	p2Queue<vec3> pastDirections;
	mat4x4 ViewMatrix, ViewMatrixInverse;
	CameraState state = NO_STATE;
	vec3 InitialPosition;
	vec3 InitialSpeed;

};