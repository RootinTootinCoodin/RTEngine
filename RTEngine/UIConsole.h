#pragma once
#include "UIElement.h"
class UIConsole :
	public UIElement
{
public:

	UIConsole(Application* app);
	~UIConsole();

	void Draw();
};

