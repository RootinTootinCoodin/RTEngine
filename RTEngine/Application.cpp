#include "Application.h"

#include "p2Defs.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	ImGui = new ModuleImGui(this);
	camera = new ModuleCamera3D(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);

	AddModule(camera);
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
		LOG("Error opening config file");
	}
	else
	{
		LOG("Sucess opening config file");
	}
		
	config = json_value_get_object(config_value);

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(json_object_get_object(config,(*item)->name.c_str()));
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	current_ms = (float)ms_timer.Read();
	dt = current_ms / 1000.0f;
	current_fps = 1000.0f / current_ms;

	if (fps_limit_display == 0)
		fps_limit = window->refresh_rate;
	else
		fps_limit = fps_limit_display;

	float expected_ms = 1000 / (float)fps_limit;

	if (current_ms < expected_ms)
	{
		SDL_Delay(expected_ms - current_ms);
	}

	// Store current framedata
	fps_arr[arr_iterator] = current_fps;
	ms_arr[arr_iterator] = current_ms;

	if (arr_iterator <= 0)
		arr_iterator = 59;

	else
	arr_iterator--;
	// -------

	avg_fps = (avg_fps + current_fps) / 2;

	if (avg_fps > 2000000)
	{
		avg_fps = 0;
	}
	// Start ms timer
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}