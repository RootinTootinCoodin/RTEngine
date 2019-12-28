#include "ModuleScripting.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentScript.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleLoader.h"
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
		res_script->compiled = true;
		res_script->scriptTable = getGlobal(v_machine,"script");
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
		.endNamespace();
}

void ModuleScripting::SetInputNamespace()
{
	getGlobalNamespace(v_machine)
		.beginNamespace("INPUT")
			.addFunction("GetKeyState", LUAGetKeyState)
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

void Instantiate(uint uuid, const char * name)
{
	std::string _name = name;
	_app->loader->ImportSceneOrModel(_name, false);
}

void LUALog(const char* string)
{
	LOG(string);
}

float LUAGetDT()
{
	return _app->time->game_dt;
}
