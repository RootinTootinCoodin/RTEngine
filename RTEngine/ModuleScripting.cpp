#include "ModuleScripting.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentScript.h"
#include "ComponentCamera.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleLoader.h"
#include "ModuleCamera3D.h"
#include "ModuleDebug.h"
#include "ResourceScript.h"
#include "FileSystem.h"



ModuleScripting::ModuleScripting(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scripting";
}


ModuleScripting::~ModuleScripting()
{
}

bool ModuleScripting::Init(JSON_Object * config)
{
	v_machine = luaL_newstate();
	luaL_openlibs(v_machine);
	lua_pcall(v_machine, 0, 0, 0);
	SetBasicNamespace();
	SetInputNamespace();
	SetOutputNamespace();

	return true;
}

bool ModuleScripting::LoadScript(std::string path, ComponentScript* component)
{
	bool ret = false;
	int compiled = luaL_dofile(v_machine, path.c_str());
	ResourceScript* res_script = (ResourceScript*)App->resource->createNewResource(RES_SCRIPT);
	res_script->SetOriginalFile(path);
	component->AssignResourceUUID(res_script->GetUUID());
	if (compiled == LUA_OK)
	{
		std::string name = path;
		FileSystem::getFileNameFromPath(name);
		res_script->compiled = true;

		//I'm gonna be honest, the other team told me how to do this (in order to have various script instances). -Lorién
		LuaRef getTable = getGlobal(v_machine, name.c_str());
		LuaRef table(getTable());
		res_script->scriptTable = table;
		res_script->scriptTable["UUID"] = component->getGameObject()->GetUUID();
		ret = true;
	}
	else
	{
		LOG("Could not compile script %s, check for compilation errors",path.c_str());
		LOG("%s", lua_tostring(v_machine, -1));
	}
	return ret;
}

void ModuleScripting::SetBasicNamespace()
{
	getGlobalNamespace(v_machine)
		.beginNamespace("BASIC")
			.addFunction("LUALog", LUALog)
			.addFunction("LUAGetDT",LUAGetDT)
			.addFunction("Instantiate",Instantiate)
			.addFunction("DeleteThis",DeleteThis)
		.endNamespace();
}

void ModuleScripting::SetInputNamespace()
{
	getGlobalNamespace(v_machine)
		.beginNamespace("INPUT")
			.addFunction("GetKeyState", LUAGetKeyState)
			.addFunction("GetButtonState", LUAGetButtonState)
			.addFunction("GetPositionX",LUAGetPositionX)
			.addFunction("GetPositionY", LUAGetPositionY)
			.addFunction("GetPositionZ", LUAGetPositionZ)
			.addFunction("GetMouseX",LUAGetMouseX)
			.addFunction("GetMouxeY",LUAGetMouseY)
		.endNamespace();
}


void ModuleScripting::SetOutputNamespace()
{
	getGlobalNamespace(v_machine)
		.beginNamespace("OUTPUT")
			.addFunction("MoveX", MoveX)
			.addFunction("MoveY",MoveY)
			.addFunction("MoveZ",MoveZ)
			.addFunction("MoveForward",MoveForward)
			.addFunction("MoveSideways",MoveSideways)
			.addFunction("RotateAlongY",RotateAlongY)
		.endNamespace();
}

void ModuleScripting::RecompileAllScriptsWithPath(std::string path)
{
	std::vector<GameObject*> gameObjects;
	App->scene->root->RecursiveGetChildren(&gameObjects);
	for (auto item = gameObjects.begin(); item != gameObjects.end(); item++)
	{
		if (ComponentScript* script = (ComponentScript*)(*item)->GetComponent(SCRIPT))
		{
			ResourceScript* res_script = (ResourceScript*)App->resource->getResource(script->getResourceUUID());
			if (res_script->GetOriginalFile() == path)
			{
				App->scripting->LoadScript(path, script);
			}
		}
	}
}

float LUAGetKeyState(uint uuid, float key)
{
	float ret = -1;
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ret = _app->input->GetKey(key);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
	return ret;
}

float LUAGetButtonState(uint uuid, float key)
{
	float ret = -1;
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ret = _app->input->GetMouseButton(key);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
	return ret;
}

float LUAGetPositionX(uint uuid)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		return pos.x;
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
	return 0;
}

float LUAGetPositionY(uint uuid)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		return pos.y;
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
	return 0;
}

float LUAGetPositionZ(uint uuid)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		return pos.z;
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
	return 0;
}

float LUAGetMouseX()
{
	return _app->input->GetMouseX();
}

float LUAGetMouseY()
{
	return _app->input->GetMouseY();
}

void MoveX(uint uuid, float units)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		pos.x += units;
		transform->setPos(pos);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}

void MoveY(uint uuid, float units)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		pos.y += units;
		transform->setPos(pos);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}

void MoveZ(uint uuid, float units)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->getPos();
		pos.z += units;
		transform->setPos(pos);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}
void MoveForward(uint uuid, float units)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->GetLocalTransformMatrix().WorldZ();
		pos *= units;
		transform->setPos(transform->getPos() + pos);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}

void MoveSideways(uint uuid, float units)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 pos = transform->GetLocalTransformMatrix().WorldX();
		pos *= units;
		transform->setPos(transform->getPos() + pos);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}

void RotateAlongY(uint uuid, float degrees)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		float3 rotation = transform->getRotation();
		rotation.y += DEGTORAD * degrees;
		transform->setRotation(rotation);
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}

void Instantiate(uint uuid, const char * name,bool parent_direction,float offset_x,float offset_y,float offset_z, bool spawning_from_child)
{
	GameObject* parent = _app->scene->root->RecursiveFindChild(uuid);
	std::string _name = name;
	GameObject* go = _app->loader->ImportSceneOrModel(_name, false,true);
	if (parent_direction)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)parent->GetComponent(TRANSFORM);
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(TRANSFORM);
		transform->setLocalFromMatrix(parent_transform->GetLocalTransformMatrix());
		float3 pos;
		if (!spawning_from_child)
			pos = transform->getPos();
		else
		{
			parent_transform = (ComponentTransform*)parent->GetParent()->GetComponent(TRANSFORM);
			pos = parent_transform->getPos();
			float3 rot = transform->getRotation();
			rot -= parent_transform->getRotation();
			transform->setRotation(rot);
		}
		pos.x += offset_x;
		pos.y += offset_y;
		pos.z += offset_z;
		transform->setPos(pos);
		go->RecursiveRemoveDirtyFlags();
	}
}

void LUALog(const char* string)
{
	LOG(string);
}

float LUAGetDT()
{
	return _app->time->game_dt;
}

void DeleteThis(uint uuid)
{
	if (GameObject* go = _app->scene->root->RecursiveFindChild(uuid))
	{
		go->to_remove = true;
	}
	else
		LOG("Script accessing an unexisting gameobject with uuid %u", uuid);
}
