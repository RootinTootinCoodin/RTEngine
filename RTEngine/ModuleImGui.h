#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__

#include <vector>
#include "Module.h"
#include "UIElement.h"

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

	void CreateDockspace();

private:
public:
	std::vector<UIElement*> UI;
private:
	bool show_demo_window = false;
	bool dockspace_active = true;
};

#endif // __MODULE_IMGUI_H__

