#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include <string>
#include "ImGui/imgui.h"

class Application;

class UIElement
{
public:
	UIElement(Application* App):App(App) {}
	~UIElement() {}

	virtual void Init() {}
	virtual void Draw() {}
private:
public:
	std::string name = "No name";
	Application* App = nullptr;
private:
	
};

#endif

