#pragma once
#include "UIElement.h"
class UIInspector :
	public UIElement
{
public:
	UIInspector(Application* app);
	~UIInspector();

	void Draw();
};

