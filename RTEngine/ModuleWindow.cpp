#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* config)
{
	title = json_object_get_string(config, "app_name");
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(json_object_get_boolean(config,"fullscreen"))
		{
			fullscreen = true;
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(json_object_get_boolean(config, "resizable"))
		{
			resizable = true;
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(json_object_get_boolean(config, "borderless"))
		{
			borderless = true;
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(json_object_get_boolean(config, "full_desktop"))
		{
			full_desktop = true;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		width = json_object_get_number(config, "width");
		height = json_object_get_number(config, "height");
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::Save(JSON_Object * config)
{
	json_object_set_boolean(config, "fullscreen", fullscreen);
	json_object_set_boolean(config, "resizable", resizable);
	json_object_set_boolean(config, "borderless", borderless);
	json_object_set_boolean(config, "full_desktop", full_desktop);
	json_object_set_number(config, "width", width);
	json_object_set_number(config, "height", height);
	json_object_set_number(config, "brightness", brightness);
	json_object_set_string(config, "app_name", title.c_str());
}

void ModuleWindow::SetTitle(std::string title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

void ModuleWindow::UpdateSize()
{
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::UpdateWindowMode()
{
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	else if (full_desktop)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	else if (!fullscreen && !full_desktop)
		SDL_SetWindowFullscreen(window, 0);
	
	if (borderless)
		SDL_SetWindowBordered(window, SDL_FALSE);

	if (!borderless)
		SDL_SetWindowBordered(window, SDL_TRUE);

	/*if (resizable)*/
		/*SDL_SetWindowResizable(window, SDL_TRUE);*/
}

void ModuleWindow::UpdateBrightness()
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::UpdateRefreshRate()
{
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(window), 0, &mode);
	refresh_rate = mode.refresh_rate;
}

void ModuleWindow::UpdateTitle(std::vector<char> _title)
{
	title = _title.data();
	SetTitle(title);
}
