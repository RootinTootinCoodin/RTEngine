#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__

#include "Module.h"
class ModuleImGui :
	public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true );
	~ModuleImGui();
};

#endif

