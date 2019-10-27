#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include <vector>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* config);
	bool CleanUp();

	void Save(JSON_Object* config);

	void SetTitle(std::string& title);
	void UpdateSize();
	void UpdateWindowMode();
	void UpdateBrightness();
	void UpdateRefreshRate();
	void UpdateGamma();
	void UpdateTitle(char* name);

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
	int r = 255;
	int g = 255;
	int b = 255;
	std::string title = "RTEngine";
};

#endif // __MODULE_WINDOW_H__