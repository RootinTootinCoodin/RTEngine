#pragma once
#include "UIElement.h"
class UIConsole :
	public UIElement
{
public:
	UIConsole(Application* App);
	~UIConsole();

	void Draw();
};

