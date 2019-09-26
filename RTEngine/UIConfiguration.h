#pragma once
#include "UIElement.h"
class UIConfiguration :
	public UIElement
{
public:
	UIConfiguration(Application* app);
	~UIConfiguration();

	void Draw();
};

