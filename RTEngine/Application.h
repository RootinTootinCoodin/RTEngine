#ifndef __APPLICATION_H__
#define __APPLICATION_H__


#include <list>


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"
#include "ModuleCamera3D.h"

#include "GL/glew.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_sdl.h"

#include "MathGeoLib/MathGeoLib.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleImGui* ImGui;
	ModuleCamera3D* camera;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	void RequestBrowser(std::string url);

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif //__APPLICATION_H__