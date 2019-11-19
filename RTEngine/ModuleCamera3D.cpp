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
	//SDL_Event event;

	//if (App->scene->selected_go != nullptr)
	//	cameraTarget = vec3(App->scene->selected_go->GetAABB().Centroid().x, App->scene->selected_go->GetAABB().Centroid().y, App->scene->selected_go->GetAABB().Centroid().z);

	//else
	//	cameraTarget = vec3(10.0f, 10.0f, 10.0f);

	//float speed_dt = speed * dt;
	//float wheel_dt = wheel * dt;

	//int mouse_x = +App->input->GetMouseXMotion();
	//int mouse_y = -App->input->GetMouseYMotion();

	//float DeltaX = (float)mouse_x * sensitivity;
	//float DeltaY = (float)mouse_y * sensitivity;;

	//if (App->ImGui->scene->IsMouseHovering())
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	{
	//		speed_dt /= speedmultiplier * dt;
	//	}

	//	if (App->input->GetMouseZ() != 0)
	//	{
	//		if (App->input->GetMouseZ() > 0)
	//			cameraPos -= Z * wheel_dt;

	//		else
	//			cameraPos += Z * wheel_dt;
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	//	{
	//		cameraPos.y = 6.0f;	cameraPos.z = 5.0f;
	//		Look(cameraPos, CameraView.translation());
	//	}

	//	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)	//Normal movement 
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	//		{
	//			if (mouse_x != 0)
	//			{
	//				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			}
	//			if (mouse_y != 0)
	//			{
	//				Y = rotate(Y, DeltaY, X);
	//				Z = rotate(Z, DeltaY, X);

	//				if (Y.y < 0.0f)
	//				{
	//					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//					Y = cross(Z, X);
	//				}
	//			}
	//			cameraPos = Z * length(cameraPos);
	//		}

	//		else
	//		{
	//			if (mouse_x != 0)
	//			{
	//				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			}

	//			if (mouse_y != 0)
	//			{
	//				Y = rotate(Y, DeltaY, X);
	//				Z = rotate(Z, DeltaY, X);

	//				if (Y.y < 0.0f)
	//				{
	//					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//					Y = cross(Z, X);
	//				}
	//			}

	//			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	//				cameraPos.y += speed_dt;
	//			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	//				cameraPos.y -= speed_dt;
	//			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//				cameraPos -= speed_dt * rotate(Z, DeltaX, vec3(0.0f, 0.0f, 1.0f));
	//			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//				cameraPos += speed_dt * rotate(Z, DeltaX, vec3(0.0f, 0.0f, 1.0f));
	//			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//				cameraPos += speed_dt * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//				cameraPos -= speed_dt * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//		}
	//	}

	//	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	//	{
	//		float mspeed = speed * dt;

	//		if (mouse_x < 0)
	//			cameraPos -= mspeed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//		if (mouse_x > 0)
	//			cameraPos += mspeed * rotate(X, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//		if (mouse_y < 0)
	//			cameraPos += mspeed * rotate(Y, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//		if (mouse_y > 0)
	//			cameraPos -= mspeed * rotate(Y, DeltaX, vec3(1.0f, 0.0f, 0.0f));
	//	}
	//}

	//Move(newPos);
	//CameraView = At(cameraPos, cameraPos + cameraFront, cameraUp);

	if (App->ImGui->scene->IsMouseHovering())
	{
		vec newPos(0.f, 0.f, 0.f);
		float speed_dt = speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed_dt *= speedmultiplier;

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				vec tmp = editorCamera->camera.front.Normalized() * speed;
				tmp.y = 0;
				newPos += tmp;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				vec tmp = editorCamera->camera.front.Normalized() * speed;
				tmp.y = 0;
				newPos -= tmp;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				vec tmp;
				tmp.x = editorCamera->camera.front.Normalized().z;
				tmp.z = -editorCamera->camera.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				vec tmp;
				tmp.x = -editorCamera->camera.front.Normalized().z;
				tmp.z = editorCamera->camera.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed;
			}
		}

		if (App->input->GetMouseZ() > 0)
		{
			newPos += editorCamera->camera.front.Normalized() * speed_dt * speedmultiplier;
		}
		else if (App->input->GetMouseZ() < 0)
		{
			newPos -= editorCamera->camera.front.Normalized() * speed_dt * speedmultiplier;
		}

		editorCamera->Move(newPos);

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			float dx = (float)App->input->GetMouseXMotion()*sensitivity*dt*-1;
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

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			editorCamera->position = vec(5.0f, 5.0f, 5.0f);
			editorCamera->reference = { 0,0,0 };
		}

	}
	
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