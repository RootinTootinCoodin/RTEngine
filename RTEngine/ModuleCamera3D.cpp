#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "Timer.h"
#include "ComponentCamera.h"
#include "ModuleImGui.h"
#include "UIScene.h"

#include "GameObject.h"

#include "MathGeoLib/Math/float2.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//CalculateViewMatrix();

	//X = vec3(1.0f, 0.0f, 0.0f);
	//Y = vec3(0.0f, 1.0f, 0.0f);
	//Z = vec3(0.0f, 0.0f, 1.0f);

	//newPos = vec3(0.0f, 0.0f, 0.0f);
	//cameraRef = vec3(0.0f, 0.0f, 0.0f);
	//cameraPos = vec3(0.0f, 4.0f, 10.0f);
	//cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	//cameraFront = vec3(0.0f, 0.0f, 1.0f);

	//cameraDirection = normalize(cameraPos - cameraTarget);
	//cameraRight = normalize(cross(Y, cameraDirection));
	//cameraUp = cross(cameraDirection, cameraRight);

	//CameraView = At(cameraRight, cameraUp, cameraDirection);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Init(JSON_Object* config)
{
	LOG("Setting up the camera");

	sensitivity = json_object_get_number(config, "sensitivity");
	if (sensitivity < 0.3)
		sensitivity = 0.3;

	editorCamera = (ComponentCamera*)App->scene->root->AddComponent(CAMERA);

	bool ret = true;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	editorCamera = nullptr;
	return true;
}

void ModuleCamera3D::Save(JSON_Object * config)
{
	json_object_set_number(config, "sensitivity", sensitivity);

	if (sensitivity <= 0)
		sensitivity = 0.4f;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->ImGui->scene->IsMouseHovering())
	{
		vec newPos(0.f, 0.f, 0.f);
		float speed_dt = speed * dt;
		float wheel_dt = wheel * dt;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed_dt *= speedmultiplier, wheel_dt *= speedmultiplier;

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos += editorCamera->camera.up.Normalized() * speed_dt;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos -= editorCamera->camera.up.Normalized() * speed_dt;
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed_dt;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed_dt;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				newPos += editorCamera->camera.front.Normalized() * speed_dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				newPos -= editorCamera->camera.front.Normalized() * speed_dt;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				vec tmp;
				tmp.x = editorCamera->camera.front.Normalized().z;
				tmp.z = -editorCamera->camera.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed_dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				vec tmp;
				tmp.x = -editorCamera->camera.front.Normalized().z;
				tmp.z = editorCamera->camera.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed_dt;
			}

			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT)
			{
				Quat rotationZ = Quat::Quat::RotateAxisAngle(editorCamera->camera.front, speed_dt/5);
				editorCamera->camera.front = rotationZ.Mul(editorCamera->camera.front).Normalized();
				editorCamera->camera.up = rotationZ.Mul(editorCamera->camera.up).Normalized();
			}

			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
			{
				Quat rotationZ = Quat::Quat::RotateAxisAngle(editorCamera->camera.front, -speed_dt/5);
				editorCamera->camera.front = rotationZ.Mul(editorCamera->camera.front).Normalized();
				editorCamera->camera.up = rotationZ.Mul(editorCamera->camera.up).Normalized();
			}
		}

		newPos += editorCamera->camera.front.Normalized() * wheel_dt * App->input->GetMouseZ();

		editorCamera->Move(newPos);

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			float dx = (float)App->input->GetMouseXMotion()*sensitivity*dt;
			float dy = (float)App->input->GetMouseYMotion()*sensitivity*dt*-1;

			if (dx != 0)
			{
				Quat rotationX = Quat::RotateY(dx);
				editorCamera->camera.front = rotationX.Mul(editorCamera->camera.front).Normalized();
				editorCamera->camera.up = rotationX.Mul(editorCamera->camera.up).Normalized();
			}

			if (dy != 0)
			{
				Quat rotationY = Quat::RotateAxisAngle(editorCamera->camera.WorldRight(), dy);

				float3 new_up = rotationY.Mul(editorCamera->camera.up).Normalized();

				if (new_up.y > 0.0f)
				{
					editorCamera->camera.up = new_up;
					editorCamera->camera.front = rotationY.Mul(editorCamera->camera.front).Normalized();
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			editorCamera->MoveTo(vec(5.0f, 5.0f, 5.0f));

			if (App->scene->selected_go != nullptr)
				editorCamera->reference = App->scene->selected_go->GetAABB().Centroid();
			else
				editorCamera->reference = { 0.0f, 0.0f, 0.0f };

			editorCamera->LookAt(editorCamera->reference);
		}

	}

	// Debug frustum
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		MousePicking();
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference)
{
	this->cameraPos = Position;
	this->cameraRef = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	cameraRef = Spot;

	Z = normalize(cameraPos - cameraRef);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
mat4x4 ModuleCamera3D::At(const vec3 &right, const vec3 &up, const vec3 &dir)
{
	mat4x4 matrix_1 = { right.x, right.y, right.z, 0, up.x, up.y, up.z, 0, dir.x, dir.y, dir.z, 0, 0, 0, 0, 1 };
	mat4x4 matrix_2 = { X.x, X.y, X.z, -cameraPos.x, Y.x, Y.y, Y.z, -cameraPos.y, Z.x, Z.y, Z.z, -cameraPos.z, 0, 0, 0, 1 };

	return matrix_1 * matrix_2;
}

void ModuleCamera3D::MousePicking()
{
	float2 mouse_pos;
	mouse_pos.x = ((App->input->GetMouseX() - scene_pos_global.x) / (scene_size.x / 2)) - 1;
	mouse_pos.y = ((App->input->GetMouseY() - scene_pos_global.y) / (scene_size.y / 2)) - 1;

	LineSegment raycast = editorCamera->camera.UnProjectLineSegment(mouse_pos.x, -mouse_pos.y);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	cameraPos += Movement;
	cameraRef += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::AdjustCameraToAABB(AABB& aabb )
{
	float3 distance = aabb.Size();
	//cameraPos -= Z * distance.Length();
	cameraPos.x = distance.Length();
	cameraPos.y = distance.Length();
	cameraPos.z = distance.Length();
	Look(cameraPos, CameraView.translation());
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, cameraPos), -dot(Y, cameraPos), -dot(Z, cameraPos), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}