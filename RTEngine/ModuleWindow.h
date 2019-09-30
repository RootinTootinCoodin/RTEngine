#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* config);
	bool CleanUp();

	void SetTitle(const char* title);
	void UpdateSize();
	void UpdateWindowMode();
	void UpdateBrightness();
	void UpdateRefreshRate();
	void UpdateTitle();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


	bool fullscreen = false;
	bool resizable = false; 
	bool borderless = false;
	bool full_desktop = false;
	int width = 1280;
	int height = 1024;
	float brightness = 1.0f;
	int refresh_rate = 60;
	char title[25] = "RTEngine";
};

#endif // __MODULE_WINDOW_H__