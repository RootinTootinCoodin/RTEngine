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

class ComponentScript;

class ModuleScripting :
	public Module
{
public:
	ModuleScripting(Application* app, bool start_enabled = true);
	~ModuleScripting();

	bool Init(JSON_Object* config)override;

	bool LoadScript(std::string path, ComponentScript* component);

	lua_State* v_machine = nullptr;

	//LUA C++ FUNCTIONS

	void SetBasicNamespace();

	void SetInputNamespace();

	void SetOutputNamespace();

	
};


//BASIC
void LUALog(const char* string);
float LUAGetDT();
void DeleteThis(uint uuid);

//INPUT
float LUAGetKeyState(uint uuid, float key);
float LUAGetButtonState(uint uuid, float key);
float LUAGetPositionX(uint uuid);
float LUAGetPositionY(uint uuid);
float LUAGetPositionZ(uint uuid);
float LUAGetMouseX();
float LUAGetMouseY();


//OUPUT
void MoveX(uint uuid, float units);
void MoveY(uint uuid, float units);
void MoveZ(uint uuid, float units);
void MoveForward(uint uuid, float units);
void MoveSideways(uint uuid, float units);
void RotateAlongY(uint uuid, float degrees);
void Instantiate(uint uuid, const char* name, bool parent_direction, float offset_x, float offset_y, float offset_z,bool spawning_from_child);

#endif

