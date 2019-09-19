#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include <string>
#include "ImGui/imgui.h"

class UIElement
{
public:
	UIElement() {}
	~UIElement() {}

	virtual void Init() {}
	virtual void Draw() {}
private:
public:
	std::string name = "No name";
private:
	
};

#endif

