#pragma once
#include "UIElement.h"
class UIAbout : public UIElement
{
public:
	UIAbout(Application* app);
	~UIAbout();

	void Draw();

	std::string text;
};

