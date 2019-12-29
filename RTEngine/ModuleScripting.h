#ifndef __MODULE_SCRIPTING_H__
#define __MODULE_SCRIPTING_H__
#include "Module.h"

extern "C"
{
#include "LUA/include/lua.h"
#include "LUA/include/lauxlib.h"
#include"LUA/include/lualib.h"
}

#include "LuaBridge/LuaBridge.h"

#pragma comment(lib,"LUA/liblua53.a")


using namespace luabridge;

class ModuleScripting :
	public Module
{
public:
	ModuleScripting(Application* app, bool start_enabled = true);
	~ModuleScripting();

	bool Init(JSON_Object* config)override;

	bool LoadScript(std::string path,uint* uuid);

	lua_State* v_machine = nullptr;

	//LUA C++ FUNCTIONS

	void SetBasicNamespace();

	void SetInputNamespace();

	void SetOutputNamespace();

	
};


//BASIC
void LUALog(const char* string);

//INPUT
float LUAGetKeyState(uint uuid, float key);
//OUPUT
void MoveX(uint uuid, float units);
void MoveY(uint uuid, float units);
void MoveZ(uint uuid, float units);
void TrackVar(std::string variable, std::string value);





#endif

