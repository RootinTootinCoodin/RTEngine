#pragma once
#include "UIElement.h"
class UIHierarchy :
	public UIElement
{
public:
	UIHierarchy(Application* app);
	~UIHierarchy();

	void Draw();
};

