#pragma once
#include "UIElement.h"
#include "SDL/include/SDL.h"

class UIConfiguration :
	public UIElement
{
public:
	UIConfiguration(Application* app);
	~UIConfiguration();

	void Draw();

	int display_fps = 60;
};

