#pragma once
#include "UIElement.h"

struct model;
class UIInspector :
	public UIElement
{
public:
	UIInspector(Application* app);
	~UIInspector();

	void Draw();

	void DrawModelInfo(model* _model);
};

