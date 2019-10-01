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

	std::vector<float> fps_arr;
	std::vector<float> ms_arr;
	int fps_limit_display = 0;
	std::string organization = "Akatsuki";

private:

	Timer	ms_timer;
	Timer	cap_timer;
	float	dt;
	float	current_fps;
	float	current_ms;
	float	avg_fps = 0;
	std::list<Module*> list_modules;
	int		arr_iterator = 59;
	int fps_limit = 60;

public:

	Application();
	~Application();

	void RequestBrowser(std::string url);

	float GetCurrentFPS();
	float GetCurrentMS();
	float GetAvgFPS();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SaveConfig();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif //__APPLICATION_H__