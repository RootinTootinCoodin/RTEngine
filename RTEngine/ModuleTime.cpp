#include "Globals.h"
#include "Application.h"
#include "ModuleTime.h"

ModuleTime::ModuleTime(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Time";
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init(JSON_Object * config)
{
	return true;
}

bool ModuleTime::CleanUp()
{
	return false;
}

void ModuleTime::Save(JSON_Object * config)
{
}

void ModuleTime::SetTitle(std::string & title)
{
}

