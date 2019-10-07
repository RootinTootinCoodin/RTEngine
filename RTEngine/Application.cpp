#include "Application.h"

#include "p2Defs.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"
#include "ModuleScene.h"

Application::Application()
{
	_app = this; //I get a linker error 2001 otherwise

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	ImGui = new ModuleImGui(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);

	AddModule(camera);
	AddModule(scene);
	// Renderer last!
	AddModule(ImGui);
	AddModule(renderer3D);
	//Fuck you i won't do what you tell me
	//Guess you should

}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end())
	{
		RELEASE(*item);
		item++;
	}
}

void Application::RequestBrowser(std::string url)
{
	ShellExecuteA(NULL, "open", url.data(), NULL, NULL, SW_SHOWNORMAL);
}

float Application::GetCurrentFPS()
{
	return current_fps;
}

float Application::GetCurrentMS()
{
	return current_ms;
}

float Application::GetAvgFPS()
{
	return avg_fps;
}

bool Application::Init()
{
	bool ret = true;

	JSON_Object* config;
	JSON_Value* config_value;

	config_value = json_parse_file_with_comments("config_file.json");
	if (config_value == NULL)
	{
		app_log("Error opening config file");
	}
	else
	{
		app_log("Sucess opening config file");
	}
		
	config = json_value_get_object(config_value);

	JSON_Object* app_config = json_object_get_object(config, "Application");
	organization = json_object_get_string(app_config, "organization_name");
	//json
	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(json_object_get_object(config,(*item)->name.c_str()));
		item++;
	}

	// After all Init calls we call Start() in all modules
	app_log("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	ms_timer.Start();
	cap_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	// Start timers
	ms_timer.Start();
	cap_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Read timer
	current_ms = ms_timer.Read();
	// -------

	dt = current_ms / 1000.0f;

	// Set limit to refresh rate if limit is 0
	if (fps_limit_display == 0)
	{
		window->UpdateRefreshRate();
		fps_limit = window->refresh_rate;
	}

	else
		fps_limit = fps_limit_display;
	// -------

	// Delay frame
	float expected_ms = 1000 / (float)fps_limit;

	if (current_ms < expected_ms)
	{
		SDL_Delay(expected_ms - current_ms);
	}
	// -------

	current_ms = cap_timer.Read();

	current_fps = 1000.0f / current_ms;
	
	// Store current framedata
	fps_arr.push_back(current_fps);
	if (fps_arr.size() > 400)
		fps_arr.erase(fps_arr.begin());

	ms_arr.push_back(current_ms);
	if (ms_arr.size() > 400)
		ms_arr.erase(ms_arr.begin());
	// -------

	// Calculate Average FPS
	avg_fps = (avg_fps + current_fps) / 2;

	if (avg_fps > 2000000)
	{
		avg_fps = 0;
	}
	// -------
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::SaveConfig()
{
	JSON_Value* config_value;

	config_value = json_value_init_object();
	if (config_value == NULL)
	{
		app_log("Error opening config file");
	}
	else
	{
		app_log("Sucess opening config file");
	}

	JSON_Value* config_module = json_value_init_object();
	JSON_Object* app_object = json_object(config_module);
	json_object_set_string(app_object, "organization_name", organization.c_str());
	json_object_set_number(app_object, "fps_limit", fps_limit_display);
	json_object_set_value(json_object(config_value), "Application", config_module);

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end()) 
	{
		config_module = json_value_init_object();
		(*item)->Save(json_object(config_module));
		json_object_set_value(json_object(config_value), (*item)->name.c_str(), config_module);
		item++;
	}

	json_serialize_to_file(config_value, "config_file.json");
}

void Application::app_log(const char * string,...)
{
	logs.push_back(string);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

Application* _app; //I get a linker error 2001 otherwise