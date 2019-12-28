#ifndef __UI_ABOUT_H__
#define __UI_ABOUT_H__

#include "UIElement.h"
class UIAbout : public UIElement
{
public:
	UIAbout(Application* app);
	~UIAbout();

	void Draw();

	std::string text;
};

#endif // !__UI_ABOUT_H__