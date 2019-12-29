#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__

#include <vector>
#include <array>
#include "Module.h"
#include "UIElement.h"
#include "ImGui/imgui.h"

class UIScene;
class UITests;
class UIConsole;
class UIConfig;
class UIAbout;
class UIConfiguration;
class UIInspector;
class UIHierarchy;
class UIAssets;
class UITime;
class UITextEditor;

enum UIFonts { REGULAR, REGULAR_BOLD, REGULAR_ITALIC, REGULAR_BOLDITALIC, TITLES, IMGUI_DEFAULT, LAST_UI_FONT };

class ModuleImGui :
	public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true );
	~ModuleImGui();

	bool Init(JSON_Object* config) override;
	update_status PreUpdate(float dt) override;

	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

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
	UIAssets* assets = nullptr;
	UITime* time = nullptr;
	UITextEditor* texted = nullptr;

	ImGuiIO* io;

	std::vector<UIElement*> UI;
	std::array<ImFont*, LAST_UI_FONT> uiFonts;
private:
	bool show_demo_window = false;
	bool dockspace_active = true;
};

#endif // __MODULE_IMGUI_H__

