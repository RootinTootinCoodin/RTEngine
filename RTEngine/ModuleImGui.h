#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__

#include "Module.h"
class ModuleImGui :
	public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true );
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	update_status PostUpdate(float dt);
	//bool CleanUp();
};

#endif

