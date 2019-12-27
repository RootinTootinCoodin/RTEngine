#include "ModuleScripting.h"
#include "Application.h"
#include "ModuleResource.h"
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
	//luaL_dofile(v_machine, "script.lua");
	//luaL_openlibs(v_machine);
	//lua_pcall(v_machine, 0, 0, 0);
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
		std::string name = path;
		FileSystem::getFileNameFromPath(name);
		res_script->scriptTable = getGlobal(v_machine, name.c_str());
	}
	else
		LOG("%s", lua_tostring(v_machine, -1));
	return false;
}
