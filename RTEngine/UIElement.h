#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include <string>
#include "ImGui/imgui.h"
#include "parson/parson.h"


class Application;

class UIElement
{
public:
	UIElement(Application* App, bool active = true):App(App), show_window(active){}
	~UIElement() {}

	virtual void Init() {}
	virtual void Draw() {}
	void SetActive(bool active) { this->show_window = active; }

public:
	std::string name = "No name";
	Application* App = nullptr;
	bool show_window = true;
	
};

#endif

