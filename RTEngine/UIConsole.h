#pragma once
#include "UIElement.h"

class UIConsole :
	public UIElement
{
public:

	UIConsole(Application* app);
	~UIConsole();

	void Draw();

private:
	void DrawConsoleItems();
public:
private:
	bool has_change = false;
	uint prev_num_items = 0;

};

