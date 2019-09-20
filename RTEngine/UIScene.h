#pragma once
#include "UIElement.h"
class UIScene :
	public UIElement
{
public:
	UIScene(Application* App);
	~UIScene();

	void Draw();
};

