#include "ModuleScripting.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
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
	//luaL_dofile(v_machine, "script.lua");


	//LuaRef s = getGlobal(v_machine, "testString");
	//LuaRef n = getGlobal(v_machine, "number");
	//std::string luaString = s.cast<std::string>();
	//int answer = n.cast<int>();
	//LOG(luaString.c_str());
	//LOG("%i", answer);
	return true;
}

bool ModuleScripting::LoadScript(std::string path, uint* res_uuid)
{
	if (luaL_dofile(v_machine, path.c_str()) == LUA_OK)
	{
		ResourceScript* res_script = (ResourceScript*)App->resource->createNewResource(RES_SCRIPT);
		*res_uuid = res_script->GetUUID();
		res_script->scriptTable = getGlobal(v_machine,"script");

	}
	else
		LOG("%s", lua_tostring(v_machine, -1));
	return false;
}

void ModuleScripting::SetBasicNamespace()
{
	getGlobalNamespace(v_machine)
		.beginNamespace("BASIC")
			.addFunction("LUALog", LUALog)
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

void LUALog(const char* string)
{
	LOG(string);
}