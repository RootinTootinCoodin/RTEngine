#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__

#include <vector>
#include "Module.h"
#include "UIElement.h"

class UIScene;
class UITests;
class UIConsole;
class UIConfig;
class UIAbout;
class UIConfiguration;
class UIInspector;
class UIHierarchy;

class ModuleImGui :
	public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true );
	~ModuleImGui();

	bool Init(JSON_Object* config);
	update_status PreUpdate(float dt);
	//update_status Update(float dt);
	update_status PostUpdate(float dt);
	//bool CleanUp();

	void CreateDockspace();

private:
public:
	UIScene* scene = nullptr;
	UITests* tests = nullptr;
	UIConsole* console = nullptr;
	UIAbout* about = nullptr;
	UIConfiguration* configuration = nullptr;
	UIInspector* inspector = nullptr;
	UIHierarchy* hierarchy = nullptr;

	std::vector<UIElement*> UI;
private:
	bool show_demo_window = false;
	bool dockspace_active = true;
};

#endif // __MODULE_IMGUI_H__

